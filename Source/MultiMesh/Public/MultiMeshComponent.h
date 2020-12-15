// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DynamicMesh3.h"
#include "DynamicMeshAABBTree3.h"
#include "Components/StaticMeshComponent.h"
#include "MultiMeshComponent.generated.h"

//
// /**
//  * 支持的布尔运算类型
//  */
// UENUM(BlueprintType)
// enum class EMultiMeshBooleanOperation : uint8
// {
// 	Union,
// 	Subtraction,
// 	Intersection
// };
//
// /**
// * 使用的法线计算类型
// */
// UENUM(BlueprintType)
// enum class EMultiMeshNormalsMode : uint8
// {
// 	SplitNormals = 0,
// 	PerVertexNormals = 1,
// 	FaceNormals = 2
// };
//
//
// USTRUCT(BlueprintType)
// struct FMultiMeshSection
// {
// 	GENERATED_BODY()
// public:
// 	UPROPERTY(BlueprintReadWrite,EditAnywhere)
// 	UStaticMesh* Mesh;
//
// 	UPROPERTY(BlueprintReadWrite,EditAnywhere)
// 	FTransform Transform;
//
// 	UPROPERTY(BlueprintReadWrite,EditAnywhere)
// 	EMultiMeshBooleanOperation Operation;
//
// 	FMultiMeshSection()
// 		: Mesh(nullptr)
// 		  , Transform(FTransform::Identity)
// 		  , Operation(EMultiMeshBooleanOperation::Union)
// 	{
// 	}
//
// 	FMultiMeshSection(UStaticMesh* InMesh, FTransform InTransform,EMultiMeshBooleanOperation InOp): Mesh(InMesh), Transform(InTransform),
// 	                                                                Operation(InOp)
// 	{
// 	}
// };

/**
 * 
 */
//UCLASS(meta = (BlueprintSpawnableComponent), ClassGroup = Rendering)
//UCLASS(Blueprintable, ClassGroup=(Rendering, Common), hidecategories=(Object,Activation,"Components|Activation"), ShowCategories=(Mobility), editinlinenew, meta=(BlueprintSpawnableComponent))
UCLASS(ClassGroup=Rendering, hidecategories=(Physics), meta=(BlueprintSpawnableComponent))
class MULTIMESH_API UMultiMeshComponent : public UStaticMeshComponent, public IInterface_CollisionDataProvider
{
	GENERATED_BODY()

public:

// 	//UMultiMeshComponent(class FObjectInitializer const& ObjectInitializer);
//
// 	virtual void OnComponentCreated() override;
//
// 	UFUNCTION(BlueprintCallable, Category = "Components|MultiMesh")
// 	void AddMeshes(UStaticMesh* mesh, FTransform tran, EMultiMeshBooleanOperation op);
//
// 	UFUNCTION(BlueprintCallable, Category = "Components|MultiMesh")
// 	void RemoveMeshes();
//
// 	UFUNCTION(BlueprintCallable, Category = "Components|MultiMesh")
// 	void ClearMeshes();
//
// 	UFUNCTION(BlueprintCallable, Category = "Components|MultiMesh")
// 	void UpdateMeshes();
//
//
// public:
//
//
// 	//virtual void InitializeComponent() override;
//
// #if WITH_EDITOR
//
// 	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
//
//
// #endif
//
//
// 	//virtual void PostInitProperties() override;
//
// 	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Components|MultiMesh")
// 	TArray<FMultiMeshSection> MultiMeshes;
//
//
// 	bool IsInUpdate = false;
//
//
// 	UPROPERTY(BlueprintReadWrite,EditAnywhere)
// 	int32 MultiNum = 0;
//
// 	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components|MultiMesh")
// 	UMaterialInterface* MultiMaterial = nullptr;
//
// 	/** Mesh的发现类型 */
// 	UPROPERTY(EditAnywhere, Category = MeshOptions)
// 	EMultiMeshNormalsMode NormalsMode = EMultiMeshNormalsMode::SplitNormals;
//
// protected:
// 	// 如果bEnableSpatialQueries=true或bEnableInsideQueries=true，则每次修改SourceMesh时都会更新此AABBTree
// 	//FDynamicMeshAABBTree3 MeshAABBTree;
//
// 	/** 用于初始化各种子类中的mesh组件的SourceMesh */
// 	FDynamicMesh3 SourceMesh;
//
// 	TArray<class UMaterialInterface*> AllMaterials;
//
// public:
//
// 	/*
//      * 调用EditMesh（）以安全地修改此参与者拥有的SourceMesh
//      * 将使用当前SourceMesh作为参数调用EditFunc，
//      * 你应该把新的/修改过的版本传回来。
//      * (如果要生成一个全新的网格，则可以使用MoveTemp来执行此操作，而无需复制)
//      */
// 	//virtual void EditMesh(TFunctionRef<void(FDynamicMesh3&)> EditFunc, bool bCopySouceMesh = false);
//
// 	/**
// 	* 每当内部SourceMesh更新时，都会广播此委托
// 	*/
// 	void OnMeshModified(FDynamicMesh3& Mesh);
//
// 	/** 在网格上调用此选项可根据“法线模式”设置计算法线 */
// 	virtual void RecomputeNormals(FDynamicMesh3& MeshOut);
//
// 	/** 使用OtherMesh计算指定的布尔运算（转换为世界空间），并存储在我们的SourceMesh中 */
// 	//UFUNCTION(BlueprintCallable)
// 	void BooleanWithMesh(FDynamicMesh3 AMesh, const FTransform3d& AMeshTransfrom, FDynamicMesh3 BMesh,
// 	                     const FTransform3d& BMeshTransfrom, FDynamicMesh3& OutMesh
// 	                     , EMultiMeshBooleanOperation Operation);
//
// 	void DynamicMesh3_To_StaticMesh(
// 		UStaticMesh* OutStaticMesh,
// 		const FDynamicMesh3* Mesh);
//
// 	bool StaticMesh_To_DynamicMesh3(UStaticMesh* InMesh, FDynamicMesh3& MeshOut);
//
//
// protected:
// 	/**
// 	* 在修改SourceMesh时调用。子类重写此函数以使用新的SourceMesh更新其各自的组件。
// 	*/
// 	//virtual void OnMeshEditedInternal(FDynamicMesh3& MeshResult);
};
