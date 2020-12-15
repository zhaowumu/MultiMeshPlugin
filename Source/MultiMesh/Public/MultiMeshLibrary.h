// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "DynamicMesh3.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "ProceduralMeshComponent.h"
#include "Components/SplineMeshComponent.h"

#include "MultiMeshLibrary.generated.h"


/**
* 支持的布尔运算类型
*/
UENUM(BlueprintType)
enum class EMultiMeshBooleanOperation : uint8
{
	Union,
	Subtraction,
	Intersection
};

/**
* 使用的法线计算类型
*/
UENUM(BlueprintType)
enum class EMultiMeshNormalsMode : uint8
{
	SplitNormals = 0,
	PerVertexNormals = 1,
	FaceNormals = 2
};


USTRUCT(BlueprintType)
struct FMultiMeshStaticSection
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	UStaticMesh* Mesh;

	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	FTransform Transform;

	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	EMultiMeshBooleanOperation Operation;

	FMultiMeshStaticSection()
		: Mesh(nullptr)
		  , Transform(FTransform::Identity)
		  , Operation(EMultiMeshBooleanOperation::Union)
	{
	}

	FMultiMeshStaticSection(UStaticMesh* InMesh, FTransform InTransform, EMultiMeshBooleanOperation InOp): Mesh(InMesh),
		Transform(InTransform),
		Operation(InOp)
	{
	}
};

USTRUCT(BlueprintType)
struct FMultiMeshSplineSection
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	USplineMeshComponent* SpMeshComponent;

	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	EMultiMeshBooleanOperation Operation;

	FMultiMeshSplineSection()
		: SpMeshComponent(nullptr)
		  , Operation(EMultiMeshBooleanOperation::Union)
	{
	}

	FMultiMeshSplineSection(USplineMeshComponent* InMesh, EMultiMeshBooleanOperation InOp):
		SpMeshComponent(InMesh),
		Operation(InOp)
	{
	}
};


/**
 * 
 */
UCLASS(meta=(ScriptName="MultiMeshLibrary"))
class MULTIMESH_API UMultiMeshLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	/*
	 * 多静态mesh混合
	 */
	UFUNCTION(BlueprintCallable, Category = "Components|MultiMeshLibrary")
	static void MultiMeshFromStaticMeshes(UProceduralMeshComponent* ProcMeshComponent,
	                                      TArray<FMultiMeshStaticSection> MultiMeshSection);

	// UFUNCTION(BlueprintCallable, Category = "Components|MultiMeshLibrary")
	// static void MultiMeshFromSplineMeshes(UProceduralMeshComponent* ProcMeshComponent,
	//                                       TArray<FMultiMeshSplineSection> MultiMeshSection,
	//                                       UMaterialInterface* defaultMaterial);


	static bool StaticMesh_To_DynamicMesh3(
		UStaticMesh* StaticMesh,
		FDynamicMesh3& MeshOut);

	static bool BoolCalculteDynamicMesh3(FDynamicMesh3 AMesh, const FTransform3d& AMeshTransfrom, FDynamicMesh3 BMesh,
	                                     const FTransform3d& BMeshTransfrom, FDynamicMesh3& ResultMesh
	                                     , EMultiMeshBooleanOperation Operation);

	static void RecomputeNormals(FDynamicMesh3& MeshOut, EMultiMeshNormalsMode mode);

	static void CopyMultiMeshFromStaticMesh(UProceduralMeshComponent* ProcMeshComponent,
	                                        UStaticMesh* StaticMesh, int32 LODIndex,
	                                        bool bCreateCollision);

	static void UpdateProceduralMeshFromDDynamicMesh3(UProceduralMeshComponent* Component,
	                                                  const FDynamicMesh3* Mesh,
	                                                  TMap<int, FStaticMaterial> mats,
	                                                  bool bUseFaceNormals,
	                                                  bool bInitializeUV0,
	                                                  bool bInitializePerVertexColors,
	                                                  bool bCollison);
};
