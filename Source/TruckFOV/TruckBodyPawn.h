// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"

#include "json.hpp"
#include <vector>

#include "TruckBodyPawn.generated.h"

// Forward declaring classes that are used for creating pointers (we dont need access to their info within header)
class UStaticMeshComponent;
class UCameraComponent;
class USpringArmComponent;
class AFOVSensor;
class ATrailer;
class ANPC;

// class UUserWidget;
class UMyUserWidget;


UCLASS()
class TRUCKFOV_API ATruckBodyPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ATruckBodyPawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// gisil
	UFUNCTION()
	void SpawnSensor();

	UFUNCTION()
	void SpawnTrailer();	

	UFUNCTION()
	void SpawnNPCs();		

	UFUNCTION()
	void ParseJson();		

	UFUNCTION()
	void UpdateSensors();

	UFUNCTION()
	void UpdateNPCs();

	UFUNCTION()
	void UpdateTrailer();

private:
	void ConfigUpdate();
	void MoveForward(float val);
	void MoveRight(float val);
	void TurnYaw(float val);
	void TurnPitch(float val);
	void ElevateUp(float val);
	void ElevateDown(float val);	

	std::vector<FColor> RayColorOptions;

	UStaticMesh* VehicleAsset;
	UStaticMesh* PedestrianAsset;
	UStaticMesh* TrafficLightAsset;		

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// containers to be used in json file parsing
	nlohmann::json json_obj;

	ATrailer* SpawnedTrailerRef;

	std::vector<AFOVSensor*> SpawnedSensorRefs;
	std::vector<ANPC*> SpawnedNPCRefs;

	// gisil
	UPROPERTY(VisibleAnywhere, Category = "Mesh")
	UStaticMeshComponent* MeshComponent;

	FVector ViewLoc;
	FRotator ViewRot;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// gisil
	virtual void GetActorEyesViewPoint( FVector& Location, FRotator& Rotation ) const override;

	void CreateNewUIWindow();

private:
	UPROPERTY(EditAnywhere, Category="HUD")
	// TSubclassOf<UUserWidget> HUDWidgetClass;
	TSubclassOf<UMyUserWidget> HUDWidgetClass;

	UPROPERTY(EditAnywhere, Category="HUD")
	// UUserWidget* CurrentWidget;
	UMyUserWidget* CurrentWidget;
	
};
