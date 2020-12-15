// Fill out your copyright notice in the Description page of Project Settings.


#include "MultiMeshComponent.h"

#include <mftransform.h>

#include "DynamicMesh3.h"
#include "MeshTransforms.h"
#include "MeshNormals.h"
#include "Operations/MeshBoolean.h"
#include "DynamicMeshAttributeSet.h"
#include "DynamicMeshToMeshDescription.h"
#include "MeshDescription.h"
#include "StaticMeshAttributes.h"

#if WITH_EDITOR
#include "IHierarchicalLODUtilities.h"
#include "HierarchicalLODUtilitiesModule.h"
#endif // WITH_EDITOR

//
// void UMultiMeshComponent::OnComponentCreated()
// {
// 	Super::OnComponentCreated();
// 	UpdateMeshes();
// }
//
// void UMultiMeshComponent::AddMeshes(UStaticMesh* mesh, FTransform tran, EMultiMeshBooleanOperation op)
// {
// 	// UStaticMesh* mesh, FTransform tran, EMultiMeshBooleanOperation op
// 	MultiMeshes.Add(FMultiMeshSection(mesh, tran, op));
// }
//
// void UMultiMeshComponent::RemoveMeshes()
// {
// }
//
// void UMultiMeshComponent::ClearMeshes()
// {
// }
//
// void UMultiMeshComponent::UpdateMeshes()
// {
//
// #if WITH_EDITOR
// 	if (IsInUpdate)
// 	{
// 		return;
// 	}
//
//
// 	IsInUpdate = true;
//
// 	auto& TempMeshList = MultiMeshes;
//
//
// 	if (TempMeshList.Num() == 0)
// 	{
// 		IsInUpdate = false;
// 		return;
// 	}
//
// 	if (TempMeshList.Num() == 1)
// 	{
// 		this->SetStaticMesh(TempMeshList[0].Mesh);
// 		IsInUpdate = false;
// 		return;
// 	}
//
// 	if (TempMeshList.Num() > 1)
// 	{
// 		auto msg = FString::Printf(TEXT("%s coooo   %d"), *TempMeshList[0].Mesh->GetName(), MultiNum);
// 		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, msg, false);
//
// 		int count = TempMeshList.Num();
//
// 		FDynamicMesh3 TempOriginMesh;
//
// 		FDynamicMesh3 TempAddMesh;
//
// 		FDynamicMesh3 TempOutMesh;
//
// 		StaticMesh_To_DynamicMesh3(TempMeshList[0].Mesh, TempOriginMesh);
//
// 		FTransform3d TempTrans;
// 		for (int i = 1; i < count; i++)
// 		{
// 			StaticMesh_To_DynamicMesh3(TempMeshList[i].Mesh, TempAddMesh);
//
//
// 			TempTrans = FTransform3d(TQuaternion<double>(TempMeshList[i].Transform.GetRotation()),
// 			                         TempMeshList[i].Transform.GetLocation() + GetAttachmentRootActor()->
// 			                         GetActorLocation(),
// 			                         TempMeshList[i].Transform.GetScale3D());
//
// 			BooleanWithMesh(TempOriginMesh, FTransform3d(GetAttachmentRootActor()->GetActorTransform()), TempAddMesh,
// 			                FTransform3d(TempTrans),
// 			                TempOutMesh, TempMeshList[i].Operation);
// 			//GetRelativeTransform()
// 			//GetWorldTransform()
// 			//GetComponentToWorld()
//
// 			//GetAttachmentRootActor()->Get
// 			TempOriginMesh = TempOutMesh;
// 		}
// 		OnMeshModified(TempOutMesh);
//
// 		auto MyMesh = GetStaticMesh();
//
// 		if (MyMesh == nullptr)
// 		{
// 			MyMesh = NewObject<UStaticMesh>();
// 			this->SetStaticMesh(MyMesh);
// 			// add one material slot
// 			MyMesh->StaticMaterials.Add(FStaticMaterial());
// 		}
//
//
// 		DynamicMesh3_To_StaticMesh(MyMesh, &SourceMesh);
//
//
// 		// update material on new section
// 		//UMaterialInterface* UseMaterial = (this->GetMaterial(0) != nullptr)
// 		//	? this->GetMaterial(0)
// 		//	: UMaterial::GetDefaultMaterial(MD_Surface);
// 		SetMaterial(0, MultiMaterial);
// 	}
//
// 	IsInUpdate = false;
// #endif
// }
//
// #if WITH_EDITOR
//
// void UMultiMeshComponent::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
// {
// 	Super::PostEditChangeProperty(PropertyChangedEvent);
// 	UpdateMeshes();
// }
//
// #endif
//
// // void UMultiMeshComponent::PostInitProperties()
// // {
// // 	Super::PostInitProperties();
// // 	UpdateMeshes();
// // }
// //
//
// void UMultiMeshComponent::DynamicMesh3_To_StaticMesh(UStaticMesh* OutStaticMesh, const FDynamicMesh3* Mesh)
// {
// 	FMeshDescription MeshDescription;
// 	FStaticMeshAttributes StaticMeshAttributes(MeshDescription);
// 	StaticMeshAttributes.Register();
//
// 	FDynamicMeshToMeshDescription Converter;
// 	Converter.Convert(Mesh, MeshDescription);
//
// 	// todo: vertex color support
//
// 	//UStaticMesh* StaticMesh = NewObject<UStaticMesh>(Component);
// 	//FName MaterialSlotName = StaticMesh->AddMaterial(MyMaterial);
//
// 	// Build the static mesh render data, one FMeshDescription* per LOD.
// 	TArray<const FMeshDescription*> MeshDescriptionPtrs;
// 	MeshDescriptionPtrs.Emplace(&MeshDescription);
// 	OutStaticMesh->BuildFromMeshDescriptions(MeshDescriptionPtrs);
// }
//
// bool UMultiMeshComponent::StaticMesh_To_DynamicMesh3(UStaticMesh* InMesh, FDynamicMesh3& MeshOut)
// {
// 	FDynamicMesh3 DynmicMesh;
// #ifdef WITH_EDITOR
// 	if (InMesh)
// 	{
// 		InMesh->bAllowCPUAccess = true;
// 		/*FMeshDescription* MeshDescription = StaticMesh->GetMeshDescription(0);
// 		FMeshDescriptionToDynamicMesh Converter;
// 		Converter.Convert(MeshDescription, DynmicMesh);
//
// 		MeshOut = MoveTemp(DynmicMesh);
// 		return true;*/
// 	}
// 	else
// 	{
// 		return false;
// 	}
//
// #endif
//
// 	if (InMesh && InMesh->bAllowCPUAccess && InMesh->RenderData != nullptr)
// 	{
// 		if (InMesh->RenderData->LODResources.IsValidIndex(0))
// 		{
// 			int32 NumSections = InMesh->GetNumSections(0);
// 			const FStaticMeshLODResources& LOD = InMesh->RenderData->LODResources[0];
// 			const FStaticMeshVertexBuffers& VertexBuffers = LOD.VertexBuffers;
//
//
// 			DynmicMesh.EnableAttributes();
// 			FDynamicMeshNormalOverlay* Normals = DynmicMesh.Attributes()->PrimaryNormals();
// 			FDynamicMeshUVOverlay* UVs = DynmicMesh.Attributes()->PrimaryUV();
//
// 			TArray<int32> Triangles;
// 			for (int32 SectionIndex = 0; SectionIndex < NumSections; SectionIndex++)
// 			{
// 				if (LOD.Sections.IsValidIndex(SectionIndex))
// 				{
// 					// Map from vert buffer for whole mesh to vert buffer for section of interest
// 					TArray<int32> MeshToSectionVertMap;
//
// 					const FStaticMeshSection& Section = LOD.Sections[SectionIndex];
// 					const uint32 OnePastLastIndex = Section.FirstIndex + Section.NumTriangles * 3;
// 					FIndexArrayView Indices = LOD.IndexBuffer.GetArrayView();
// 					// Iterate over section index buffer, copying verts as needed
// 					for (uint32 i = Section.FirstIndex; i < OnePastLastIndex; i++)
// 					{
// 						uint32 MeshVertIndex = Indices[i];
//
// 						// See if we have this vert already in our section vert buffer, and copy vert in if not 
// 						int32 NewIndex = MeshToSectionVertMap.Find(MeshVertIndex);
// 						if (NewIndex == INDEX_NONE)
// 						{
// 							DynmicMesh.AppendVertex(VertexBuffers.PositionVertexBuffer.VertexPosition(MeshVertIndex));
// 							MeshToSectionVertMap.Add(MeshVertIndex);
// 						}
//
// 						Normals->AppendElement(
// 							FVector(VertexBuffers.StaticMeshVertexBuffer.VertexTangentZ(MeshVertIndex)));
// 						UVs->AppendElement(VertexBuffers.StaticMeshVertexBuffer.GetVertexUV(MeshVertIndex, 0));
//
// 						// Add to index buffer
// 						Triangles.Add(MeshVertIndex);
// 					}
// 				}
// 			}
//
//
// 			for (int Index = 0; Index < Triangles.Num(); Index += 3)
// 			{
// 				auto tid = DynmicMesh.AppendTriangle(Triangles[Index + 0], Triangles[Index + 1], Triangles[Index + 2]);
//
// 				if (Normals && Normals->IsElement(Index + 0) && Normals->IsElement(Index + 1) && Normals->IsElement(
// 					Index + 2))
// 				{
// 					Normals->SetTriangle(tid, FIndex3i(Index + 0, Index + 1, Index + 2));
// 				}
// 				if (UVs && UVs->IsElement(Index + 0) && UVs->IsElement(Index + 1) && UVs->IsElement(Index + 2))
// 				{
// 					UVs->SetTriangle(tid, FIndex3i(Index + 0, Index + 1, Index + 2));
// 				}
// 			}
//
//
// 			MeshOut = MoveTemp(DynmicMesh);
// 			return true;
// 		}
// 	}
// 	return false;
// }
//
//
// void UMultiMeshComponent::RecomputeNormals(FDynamicMesh3& MeshOut)
// {
// 	if (this->NormalsMode == EMultiMeshNormalsMode::PerVertexNormals)
// 	{
// 		MeshOut.EnableAttributes();
// 		FMeshNormals::InitializeOverlayToPerVertexNormals(MeshOut.Attributes()->PrimaryNormals(), false);
// 	}
// 	else if (this->NormalsMode == EMultiMeshNormalsMode::FaceNormals)
// 	{
// 		MeshOut.EnableAttributes();
// 		FMeshNormals::InitializeOverlayToPerTriangleNormals(MeshOut.Attributes()->PrimaryNormals());
// 	}
// }
//
// void UMultiMeshComponent::BooleanWithMesh(FDynamicMesh3 AMesh, const FTransform3d& AMeshTransfrom, FDynamicMesh3 BMesh,
//                                           const FTransform3d& BMeshTransfrom, FDynamicMesh3& OutMesh,
//                                           EMultiMeshBooleanOperation Operation)
// {
// 	//FTransform3d ActorToWorld(GetActorTransform());
// 	//FTransform3d OtherToWorld(OtherMeshActor->GetActorTransform());
//
// 	//OtherMeshActor->GetMeshCopy(BMesh);
// 	MeshTransforms::ApplyTransform(BMesh, BMeshTransfrom);
// 	MeshTransforms::ApplyTransformInverse(BMesh, AMeshTransfrom);
//
// 	FMeshBoolean::EBooleanOp ApplyOp = FMeshBoolean::EBooleanOp::Union;
//
// 	switch (Operation)
// 	{
// 	default:
// 		break;
// 	case EMultiMeshBooleanOperation::Subtraction:
// 		ApplyOp = FMeshBoolean::EBooleanOp::Difference;
// 		break;
// 	case EMultiMeshBooleanOperation::Intersection:
// 		ApplyOp = FMeshBoolean::EBooleanOp::Intersect;
// 		break;
// 	}
//
// 	FMeshBoolean Boolean(
// 		&AMesh, FTransform3d::Identity(),
// 		&BMesh, FTransform3d::Identity(),
// 		&OutMesh,
// 		ApplyOp);
// 	Boolean.bPutResultInInputSpace = true;
// 	bool bOK = Boolean.Compute();
//
// 	if (!bOK)
// 	{
// 		// fill holes
// 	}
//
// 	RecomputeNormals(OutMesh);
//
// 	//OnMeshModified(OutMesh);
// }
//
// void UMultiMeshComponent::OnMeshModified(FDynamicMesh3& Mesh)
// {
// 	SourceMesh = Mesh;
// }
