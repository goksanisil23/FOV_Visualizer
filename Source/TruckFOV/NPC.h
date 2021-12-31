// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include <string>
#include "NPC.generated.h"

// Forward declaring classes that are used for creating pointers (we dont need access to their info within header)
class UStaticMeshComponent;

UCLASS()
class TRUCKFOV_API ANPC : public AActor
{
	GENERATED_BODY()
	
public:	

	std::string mesh_path;
	
	// Sets default values for this actor's properties
	ANPC();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// gisil
	UPROPERTY(VisibleAnywhere, Category = "Mesh")
	UStaticMeshComponent* MeshComponent;
	
};
