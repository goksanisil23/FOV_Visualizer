// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "ProceduralMeshComponent.h"

#include "ProceduralMeshSensor.generated.h"

UCLASS()
class TRUCKFOV_API AProceduralMeshSensor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AProceduralMeshSensor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	void CreateTriangle();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(VisibleAnywhere)
	UProceduralMeshComponent* proc_mesh;

	
	
};
