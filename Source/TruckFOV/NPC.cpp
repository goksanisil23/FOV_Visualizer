// Fill out your copyright notice in the Description page of Project Settings.


#include "NPC.h"

// gisil
#include "Engine/World.h"
#include "UObject/ConstructorHelpers.h"

// Sets default values
ANPC::ANPC()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Setup mesh
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	MeshComponent->SetupAttachment(GetRootComponent());

	// default mesh
	mesh_path = "StaticMesh'/Game/StarterContent/Shapes/Shape_Cylinder.Shape_Cylinder'";	

	// Assigning a specific mesh programmatically to the MeshComponent

	static ConstructorHelpers::FObjectFinder<UStaticMesh> 
		MeshComponentAsset(*FString( mesh_path.c_str()));

	if(MeshComponentAsset.Succeeded()) {
		MeshComponent->SetStaticMesh(MeshComponentAsset.Object);
		MeshComponent->SetRelativeLocation(FVector(0.f, 0.f, 0.f));
		MeshComponent->SetWorldScale3D(FVector(1.f, 1.f, 1.f));
	}	


}

// Called when the game starts or when spawned
void ANPC::BeginPlay()
{
	Super::BeginPlay();
	
	// GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, *FString( mesh_path.c_str() ) );

}

// Called every frame
void ANPC::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

