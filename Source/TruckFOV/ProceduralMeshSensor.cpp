// Fill out your copyright notice in the Description page of Project Settings.


#include "ProceduralMeshSensor.h"

#include "UObject/ConstructorHelpers.h"


// Sets default values
AProceduralMeshSensor::AProceduralMeshSensor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	proc_mesh = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("GeneratedMesh"));
	RootComponent = proc_mesh;
	proc_mesh->bUseAsyncCooking = true;

}

// Called when the game starts or when spawned
void AProceduralMeshSensor::BeginPlay()
{
	Super::BeginPlay();
	
	CreateTriangle();

}

// Called every frame
void AProceduralMeshSensor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AProceduralMeshSensor::CreateTriangle() {
	TArray<FVector> vertices;

	vertices.Add(FVector(0,0,0));
	vertices.Add(FVector(0,1000,0));
	vertices.Add(FVector(0,0,1000));

	TArray<int32> triangles;
	triangles.Add(0);
	triangles.Add(1);
	triangles.Add(2);

	TArray<FVector> normals;
	// normals.Add(FVector(1,0,0));
	// normals.Add(FVector(1,0,0));
	// normals.Add(FVector(1,0,0));

	TArray<FVector2D> UV0;
	// UV0.Add(FVector2D(0,0));
	// UV0.Add(FVector2D(10,0));
	// UV0.Add(FVector2D(0,10));

	TArray<FProcMeshTangent> tangents;
	// tangents.Add(FProcMeshTangent(0,1,0));
	// tangents.Add(FProcMeshTangent(0,1,0));
	// tangents.Add(FProcMeshTangent(0,1,0));

	TArray<FLinearColor> vertexColors;
	vertexColors.Add(FLinearColor(1.0, 0.0, 0.0, 1.0));
	vertexColors.Add(FLinearColor(1.0, 0.0, 0.0, 1.0));
	vertexColors.Add(FLinearColor(1.0, 0.0, 0.0, 1.0));

	proc_mesh->CreateMeshSection_LinearColor(0, vertices, triangles, normals, UV0, vertexColors, tangents, true);
	// proc_mesh->CreateMeshSection_LinearColor(0, vertices, triangles, normals, UV0, vertexColors, FProcMeshTangent(), true);


}

