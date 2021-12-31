// Fill out your copyright notice in the Description page of Project Settings.


#include "Trailer.h"

// gisil
#include "Engine/World.h"
#include "UObject/ConstructorHelpers.h"


// Sets default values
ATrailer::ATrailer()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Setup mesh
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	MeshComponent->SetupAttachment(GetRootComponent());

	// Assigning a specific mesh programmatically to the MeshComponent
	static ConstructorHelpers::FObjectFinder<UStaticMesh> 
		MeshComponentAsset(TEXT("StaticMesh'/Game/Static/FH/FH_blender_Trailer'"));

	if(MeshComponentAsset.Succeeded()) {
		MeshComponent->SetStaticMesh(MeshComponentAsset.Object);
		MeshComponent->SetRelativeLocation(FVector(0.f, 0.f, 0.f));
		MeshComponent->SetWorldScale3D(FVector(1.f, 1.f, 1.f));
	}	

}

// Called when the game starts or when spawned
void ATrailer::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATrailer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

