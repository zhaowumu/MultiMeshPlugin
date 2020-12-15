// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DynamicMesh3.h"
#include "DynamicMeshAABBTree3.h"
#include "Components/StaticMeshComponent.h"
#include "MultiMeshComponent.generated.h"

//
// /**
//  * ֧�ֵĲ�����������
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
// * ʹ�õķ��߼�������
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
// 	/** Mesh�ķ������� */
// 	UPROPERTY(EditAnywhere, Category = MeshOptions)
// 	EMultiMeshNormalsMode NormalsMode = EMultiMeshNormalsMode::SplitNormals;
//
// protected:
// 	// ���bEnableSpatialQueries=true��bEnableInsideQueries=true����ÿ���޸�SourceMeshʱ������´�AABBTree
// 	//FDynamicMeshAABBTree3 MeshAABBTree;
//
// 	/** ���ڳ�ʼ�����������е�mesh�����SourceMesh */
// 	FDynamicMesh3 SourceMesh;
//
// 	TArray<class UMaterialInterface*> AllMaterials;
//
// public:
//
// 	/*
//      * ����EditMesh�����԰�ȫ���޸Ĵ˲�����ӵ�е�SourceMesh
//      * ��ʹ�õ�ǰSourceMesh��Ϊ��������EditFunc��
//      * ��Ӧ�ð��µ�/�޸Ĺ��İ汾��������
//      * (���Ҫ����һ��ȫ�µ����������ʹ��MoveTemp��ִ�д˲����������踴��)
//      */
// 	//virtual void EditMesh(TFunctionRef<void(FDynamicMesh3&)> EditFunc, bool bCopySouceMesh = false);
//
// 	/**
// 	* ÿ���ڲ�SourceMesh����ʱ������㲥��ί��
// 	*/
// 	void OnMeshModified(FDynamicMesh3& Mesh);
//
// 	/** �������ϵ��ô�ѡ��ɸ��ݡ�����ģʽ�����ü��㷨�� */
// 	virtual void RecomputeNormals(FDynamicMesh3& MeshOut);
//
// 	/** ʹ��OtherMesh����ָ���Ĳ������㣨ת��Ϊ����ռ䣩�����洢�����ǵ�SourceMesh�� */
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
// 	* ���޸�SourceMeshʱ���á�������д�˺�����ʹ���µ�SourceMesh��������Ե������
// 	*/
// 	//virtual void OnMeshEditedInternal(FDynamicMesh3& MeshResult);
};
