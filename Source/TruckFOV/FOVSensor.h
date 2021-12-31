// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "ProceduralMeshComponent.h"
#include <string>

#include "FOVSensor.generated.h"

// Forward declaring classes that are used for creating pointers (we dont need access to their info within header)
class UStaticMeshComponent;
class UTextRenderComponent;
class UMaterial;
class UMaterialInstanceDynamic;

UCLASS()
class TRUCKFOV_API AFOVSensor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFOVSensor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// gisil
	UPROPERTY(VisibleAnywhere, Category = "Mesh")
	UStaticMeshComponent* MeshComponent;

	UFUNCTION()
	void RayCastFunc();

	UFUNCTION()
	void CreateFOVMesh();	

	UPROPERTY(EditAnywhere, Category = "RayCastOptions")
	float RaycastMaxRange;

	UPROPERTY(EditAnywhere, Category = "RayCastOptions")
	float HitBoxSize;

	UPROPERTY(EditAnywhere, Category = "RayCastOptions")
	FVector RayOffset;

	UPROPERTY(EditAnywhere, Category = "RayCastOptions")
	FRotator SensorRotator;	

	UPROPERTY(EditAnywhere, Category = "RayCastOptions")
	float UpperVertFOV;
	UPROPERTY(EditAnywhere, Category = "RayCastOptions")
	float LowerVertFOV;
	UPROPERTY(EditAnywhere, Category = "RayCastOptions")
	float HorFOV;	
	UPROPERTY(EditAnywhere, Category = "RayCastOptions")
	float HorizontalRes;	
	UPROPERTY(EditAnywhere, Category = "RayCastOptions")
	float VerticalRes;					
	UPROPERTY(EditAnywhere, Category = "RayCastOptions")
	FColor RayColor;

	// UPROPERTY(EditAnywhere, Category = "RayCastOptions")
	// UTextRenderComponent* TextComp;	
	// std::string text_str;

	bool RayCastEnabled;
	bool ProcMeshEnabled;

	UPROPERTY(VisibleAnywhere)
	UProceduralMeshComponent* proc_mesh;
	UPROPERTY(VisibleAnywhere)
	UMaterial* proc_mesh_material;
	UPROPERTY(VisibleAnywhere)
	UMaterialInstanceDynamic* proc_mesh_dyn_material;

	// end of gisil

	
	
};
