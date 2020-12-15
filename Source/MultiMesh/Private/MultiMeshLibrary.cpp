// Fill out your copyright notice in the Description page of Project Settings.


#include "MultiMeshLibrary.h"
#include "DynamicMesh3.h"
#include "MeshTransforms.h"
#include "Components/SplineMeshComponent.h"
#include "MeshNormals.h"
#include "KismetProceduralMeshLibrary.h"
#include "Operations/MeshBoolean.h"
#include "DynamicMeshAttributeSet.h"
#include "Runtime/Engine/Private/SplineMeshSceneProxy.h"
void UMultiMeshLibrary::MultiMeshFromStaticMeshes(UProceduralMeshComponent* ProcMeshComponent,
                                                  TArray<FMultiMeshStaticSection> MultiMeshSection)
{
	const int MeshCount = MultiMeshSection.Num();

	if (ProcMeshComponent != nullptr &&
		MeshCount > 0)
	{
		if (MeshCount == 1)
		{
			//只有一个设置为当前显示
			CopyMultiMeshFromStaticMesh(ProcMeshComponent, MultiMeshSection[0].Mesh, 0, true);
			return;
		}

		//多个bool运算


		FDynamicMesh3 SourceDynamicMesh3;
		FTransform3d SourceTransform;

		StaticMesh_To_DynamicMesh3(MultiMeshSection[0].Mesh, SourceDynamicMesh3);
		SourceTransform = FTransform3d(MultiMeshSection[0].Transform);

		FDynamicMesh3 TempDynamicMesh3;
		FTransform3d TempTransform;

		FDynamicMesh3 ResultDynamicMesh3;


		//材质
		TMap<int, FStaticMaterial> materialList;

		for (auto Element : MultiMeshSection[0].Mesh->StaticMaterials)
		{
			materialList.Add(materialList.Num(), Element);
		}


		for (int i = 1; i < MeshCount; i++)
		{
			StaticMesh_To_DynamicMesh3(MultiMeshSection[i].Mesh, TempDynamicMesh3);
			TempTransform = FTransform3d(MultiMeshSection[i].Transform);
			BoolCalculteDynamicMesh3(SourceDynamicMesh3, SourceTransform, TempDynamicMesh3, TempTransform,
			                         ResultDynamicMesh3, MultiMeshSection[i].Operation);

			SourceDynamicMesh3 = ResultDynamicMesh3;

			// for (auto e : MultiMeshSection[i].Mesh->StaticMaterials)
			// {
			// 	materialList.Add(materialList.Num(), e);
			// }
		}

		//所有的合并完成后重新计算法线
		RecomputeNormals(SourceDynamicMesh3, EMultiMeshNormalsMode::PerVertexNormals);

		UpdateProceduralMeshFromDDynamicMesh3(ProcMeshComponent, &SourceDynamicMesh3, materialList,
		                                      true, true, true, true);
	}
}

// void UMultiMeshLibrary::MultiMeshFromSplineMeshes(UProceduralMeshComponent* ProcMeshComponent,
//                                                   TArray<FMultiMeshSplineSection> MultiMeshSection,
//                                                   UMaterialInterface* defaultMaterial)
// {
// 	//TODO
// 	// FSplineMeshSceneProxy* SplineProxy = static_cast<FSplineMeshSceneProxy*>(MultiMeshSection[0].SpMeshComponent->SceneProxy);
// 	//
// 	// SplineProxy->RenderData
//
// 	//BoolCalculteDynamicMesh3
// }

bool UMultiMeshLibrary::StaticMesh_To_DynamicMesh3(UStaticMesh* StaticMesh, FDynamicMesh3& MeshOut)
{
	FDynamicMesh3 DynmicMesh;
#ifdef WITH_EDITOR
	if (StaticMesh)
	{
		StaticMesh->bAllowCPUAccess = true;
		/*FMeshDescription* MeshDescription = StaticMesh->GetMeshDescription(0);
		FMeshDescriptionToDynamicMesh Converter;
		Converter.Convert(MeshDescription, DynmicMesh);

		MeshOut = MoveTemp(DynmicMesh);
		return true;*/
	}
	else
	{
		return false;
	}

#endif

	if (StaticMesh && StaticMesh->bAllowCPUAccess && StaticMesh->RenderData != nullptr)
	{
		if (StaticMesh->RenderData->LODResources.IsValidIndex(0))
		{
			int32 NumSections = StaticMesh->GetNumSections(0);
			const FStaticMeshLODResources& LOD = StaticMesh->RenderData->LODResources[0];
			const FStaticMeshVertexBuffers& VertexBuffers = LOD.VertexBuffers;


			DynmicMesh.EnableAttributes();
			FDynamicMeshNormalOverlay* Normals = DynmicMesh.Attributes()->PrimaryNormals();
			FDynamicMeshUVOverlay* UVs = DynmicMesh.Attributes()->PrimaryUV();

			TArray<int32> Triangles;
			for (int32 SectionIndex = 0; SectionIndex < NumSections; SectionIndex++)
			{
				if (LOD.Sections.IsValidIndex(SectionIndex))
				{
					// Map from vert buffer for whole mesh to vert buffer for section of interest
					TArray<int32> MeshToSectionVertMap;

					const FStaticMeshSection& Section = LOD.Sections[SectionIndex];
					const uint32 OnePastLastIndex = Section.FirstIndex + Section.NumTriangles * 3;
					FIndexArrayView Indices = LOD.IndexBuffer.GetArrayView();
					// Iterate over section index buffer, copying verts as needed
					for (uint32 i = Section.FirstIndex; i < OnePastLastIndex; i++)
					{
						uint32 MeshVertIndex = Indices[i];

						// See if we have this vert already in our section vert buffer, and copy vert in if not 
						int32 NewIndex = MeshToSectionVertMap.Find(MeshVertIndex);
						if (NewIndex == INDEX_NONE)
						{
							DynmicMesh.AppendVertex(VertexBuffers.PositionVertexBuffer.VertexPosition(MeshVertIndex));
							MeshToSectionVertMap.Add(MeshVertIndex);
						}

						Normals->AppendElement(
							FVector(VertexBuffers.StaticMeshVertexBuffer.VertexTangentZ(MeshVertIndex)));
						UVs->AppendElement(VertexBuffers.StaticMeshVertexBuffer.GetVertexUV(MeshVertIndex, 0));

						// Add to index buffer
						Triangles.Add(MeshVertIndex);
					}
				}
			}


			for (int Index = 0; Index < Triangles.Num(); Index += 3)
			{
				auto tid = DynmicMesh.AppendTriangle(Triangles[Index + 0], Triangles[Index + 1], Triangles[Index + 2]);

				if (Normals && Normals->IsElement(Index + 0) && Normals->IsElement(Index + 1) && Normals->IsElement(
					Index + 2))
				{
					Normals->SetTriangle(tid, FIndex3i(Index + 0, Index + 1, Index + 2));
				}
				if (UVs && UVs->IsElement(Index + 0) && UVs->IsElement(Index + 1) && UVs->IsElement(Index + 2))
				{
					UVs->SetTriangle(tid, FIndex3i(Index + 0, Index + 1, Index + 2));
				}
			}


			MeshOut = MoveTemp(DynmicMesh);
			return true;
		}
	}
	return false;
}

bool UMultiMeshLibrary::BoolCalculteDynamicMesh3(FDynamicMesh3 AMesh, const FTransform3d& AMeshTransfrom,
                                                 FDynamicMesh3 BMesh, const FTransform3d& BMeshTransfrom,
                                                 FDynamicMesh3& ResultMesh,
                                                 EMultiMeshBooleanOperation Operation)
{
	//FTransform3d ActorToWorld(GetActorTransform());
	//FTransform3d OtherToWorld(OtherMeshActor->GetActorTransform());

	//OtherMeshActor->GetMeshCopy(BMesh);
	MeshTransforms::ApplyTransform(BMesh, BMeshTransfrom);
	MeshTransforms::ApplyTransformInverse(BMesh, AMeshTransfrom);

	FMeshBoolean::EBooleanOp ApplyOp = FMeshBoolean::EBooleanOp::Union;

	switch (Operation)
	{
	default:
		break;
	case EMultiMeshBooleanOperation::Subtraction:
		ApplyOp = FMeshBoolean::EBooleanOp::Difference;
		break;
	case EMultiMeshBooleanOperation::Intersection:
		ApplyOp = FMeshBoolean::EBooleanOp::Intersect;
		break;
	}

	FMeshBoolean Boolean(
		&AMesh, FTransform3d::Identity(),
		&BMesh, FTransform3d::Identity(),
		&ResultMesh,
		ApplyOp);
	Boolean.bPutResultInInputSpace = true;
	bool BoolOperationResult = Boolean.Compute();

	// if (!BoolOperationResult)
	// {
	// 	//TODO fill holes
	// }
	return BoolOperationResult;
}

void UMultiMeshLibrary::RecomputeNormals(FDynamicMesh3& MeshOut, EMultiMeshNormalsMode mode)
{
	if (mode == EMultiMeshNormalsMode::PerVertexNormals)
	{
		MeshOut.EnableAttributes();
		FMeshNormals::InitializeOverlayToPerVertexNormals(MeshOut.Attributes()->PrimaryNormals(), false);
	}
	else if (mode == EMultiMeshNormalsMode::FaceNormals)
	{
		MeshOut.EnableAttributes();
		FMeshNormals::InitializeOverlayToPerTriangleNormals(MeshOut.Attributes()->PrimaryNormals());
	}
}

void UMultiMeshLibrary::CopyMultiMeshFromStaticMesh(UProceduralMeshComponent* ProcMeshComponent,
                                                    UStaticMesh* StaticMesh, int32 LODIndex, bool bCreateCollision)
{
	if (
		StaticMesh != nullptr &&
		ProcMeshComponent != nullptr)
	{
		//// MESH DATA

		int32 NumSections = StaticMesh->GetNumSections(LODIndex);
		for (int32 SectionIndex = 0; SectionIndex < NumSections; SectionIndex++)
		{
			// Buffers for copying geom data
			TArray<FVector> Vertices;
			TArray<int32> Triangles;
			TArray<FVector> Normals;
			TArray<FVector2D> UVs;
			TArray<FVector2D> UVs1;
			TArray<FVector2D> UVs2;
			TArray<FVector2D> UVs3;
			TArray<FProcMeshTangent> Tangents;

			// Get geom data from static mesh
			UKismetProceduralMeshLibrary::GetSectionFromStaticMesh(StaticMesh, LODIndex, SectionIndex, Vertices,
			                                                       Triangles, Normals, UVs, Tangents);

			// Create section using data
			TArray<FLinearColor> DummyColors;
			ProcMeshComponent->CreateMeshSection_LinearColor(SectionIndex, Vertices, Triangles, Normals, UVs, UVs1,
			                                                 UVs2, UVs3, DummyColors, Tangents, bCreateCollision);
		}

		//// SIMPLE COLLISION

		// Clear any existing collision hulls
		ProcMeshComponent->ClearCollisionConvexMeshes();

		if (StaticMesh->BodySetup != nullptr)
		{
			// Iterate over all convex hulls on static mesh..
			const int32 NumConvex = StaticMesh->BodySetup->AggGeom.ConvexElems.Num();
			for (int ConvexIndex = 0; ConvexIndex < NumConvex; ConvexIndex++)
			{
				// Copy convex verts to ProcMesh
				FKConvexElem& MeshConvex = StaticMesh->BodySetup->AggGeom.ConvexElems[ConvexIndex];
				ProcMeshComponent->AddCollisionConvexMesh(MeshConvex.VertexData);
			}
		}

		//// MATERIALS

		for (int32 MatIndex = 0; MatIndex < StaticMesh->StaticMaterials.Num(); MatIndex++)
		{
			ProcMeshComponent->SetMaterial(MatIndex, StaticMesh->GetMaterial(MatIndex));
		}
	}
}

void UMultiMeshLibrary::UpdateProceduralMeshFromDDynamicMesh3(UProceduralMeshComponent* Component,
                                                              const FDynamicMesh3* Mesh,
                                                              TMap<int, FStaticMaterial> mats,
                                                              bool bUseFaceNormals,
                                                              bool bInitializeUV0, bool bInitializePerVertexColors,
                                                              bool bCollison)
{
	Component->ClearAllMeshSections();

	int32 NumTriangles = Mesh->TriangleCount();
	int32 NumVertices = NumTriangles * 3;

	TArray<FVector> Vertices, Normals;
	Vertices.SetNumUninitialized(NumVertices);
	Normals.SetNumUninitialized(NumVertices);

	FMeshNormals PerVertexNormals(Mesh);
	bool bUsePerVertexNormals = false;
	const FDynamicMeshNormalOverlay* NormalOverlay = nullptr;
	if (Mesh->HasAttributes() == false && bUseFaceNormals == false)
	{
		PerVertexNormals.ComputeVertexNormals();
		bUsePerVertexNormals = true;
	}
	else if (Mesh->HasAttributes())
	{
		NormalOverlay = Mesh->Attributes()->PrimaryNormals();
	}

	const FDynamicMeshUVOverlay* UVOverlay = (Mesh->HasAttributes()) ? Mesh->Attributes()->PrimaryUV() : nullptr;
	TArray<FVector2D> UV0;
	if (UVOverlay && bInitializeUV0)
	{
		UV0.SetNum(NumVertices);
	}

	TArray<FLinearColor> VtxColors;
	bool bUsePerVertexColors = false;
	if (bInitializePerVertexColors && Mesh->HasVertexColors())
	{
		VtxColors.SetNum(NumVertices);
		bUsePerVertexColors = true;
	}

	TArray<FProcMeshTangent> Tangents; // not supporting this for now

	TArray<int32> Triangles;
	Triangles.SetNumUninitialized(NumTriangles * 3);

	FVector3d Position[3];
	FVector3f Normal[3];
	FVector2f UV[3];
	int32 BufferIndex = 0;
	for (int32 tid : Mesh->TriangleIndicesItr())
	{
		int32 k = 3 * (BufferIndex++);

		FIndex3i TriVerts = Mesh->GetTriangle(tid);

		Mesh->GetTriVertices(tid, Position[0], Position[1], Position[2]);
		Vertices[k] = (FVector)Position[0];
		Vertices[k + 1] = (FVector)Position[1];
		Vertices[k + 2] = (FVector)Position[2];


		if (bUsePerVertexNormals)
		{
			Normals[k] = (FVector)PerVertexNormals[TriVerts.A];
			Normals[k + 1] = (FVector)PerVertexNormals[TriVerts.B];
			Normals[k + 2] = (FVector)PerVertexNormals[TriVerts.C];
		}
		else if (NormalOverlay != nullptr && bUseFaceNormals == false && NormalOverlay->IsSetTriangle(tid))
		{
			NormalOverlay->GetTriElements(tid, Normal[0], Normal[1], Normal[2]);
			Normals[k] = (FVector)Normal[0];
			Normals[k + 1] = (FVector)Normal[1];
			Normals[k + 2] = (FVector)Normal[2];
		}
		else
		{
			FVector3d TriNormal = Mesh->GetTriNormal(tid);
			Normals[k] = (FVector)TriNormal;
			Normals[k + 1] = (FVector)TriNormal;
			Normals[k + 2] = (FVector)TriNormal;
		}

		if (UVOverlay != nullptr && UVOverlay->IsSetTriangle(tid))
		{
			UVOverlay->GetTriElements(tid, UV[0], UV[1], UV[2]);
			UV0[k] = (FVector2D)UV[0];
			UV0[k + 1] = (FVector2D)UV[1];
			UV0[k + 2] = (FVector2D)UV[2];
		}

		if (bUsePerVertexColors)
		{
			VtxColors[k] = (FLinearColor)Mesh->GetVertexColor(TriVerts.A);
			VtxColors[k + 1] = (FLinearColor)Mesh->GetVertexColor(TriVerts.B);
			VtxColors[k + 2] = (FLinearColor)Mesh->GetVertexColor(TriVerts.C);
		}

		Triangles[k] = k;
		Triangles[k + 1] = k + 1;
		Triangles[k + 2] = k + 2;
	}

	Component->CreateMeshSection_LinearColor(0, Vertices, Triangles, Normals, UV0, VtxColors, Tangents, bCollison);

	//Component->SetMaterial(0, material.GetMaterialInterface());

	for (auto m : mats)
	{
		//Component->CreateAndSetMaterialInstanceDynamicFromMaterial(m.Key, m.Value.MaterialInterface);
		Component->SetMaterial(m.Key, m.Value.MaterialInterface);
	}
}
