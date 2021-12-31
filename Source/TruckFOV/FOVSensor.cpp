// Fill out your copyright notice in the Description page of Project Settings.


#include "FOVSensor.h"

#include "DrawDebugHelpers.h" // gisil
#include "Engine/World.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/TextRenderComponent.h"
#include "Materials/Material.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Materials/MaterialInterface.h"

// Sets default values
AFOVSensor::AFOVSensor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Setup mesh
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	MeshComponent->SetupAttachment(GetRootComponent());

	// Assigning a specific mesh programmatically to the MeshComponent
	static ConstructorHelpers::FObjectFinder<UStaticMesh> 
		MeshComponentAsset(TEXT("StaticMesh'/Game/Static/Lidar/hesai'"));

	if(MeshComponentAsset.Succeeded()) {
		MeshComponent->SetStaticMesh(MeshComponentAsset.Object);
		MeshComponent->SetRelativeLocation(FVector(0.f, 0.f, 0.f));
		MeshComponent->SetWorldScale3D(FVector(1.0f, 1.0f, 1.0f));
	}	

	// Setting default values for Lidar
	RayCastEnabled = true;
	ProcMeshEnabled = true;

	// Sensor Position
	RayOffset.X = 0.0f;
	RayOffset.Y = 0.0f;
	RayOffset.Z = 0.0f;

	// Sensor Orientation w.r.t vehicle axis
	SensorRotator.Yaw = 0.0;
	SensorRotator.Pitch = 0.0;
	SensorRotator.Roll = 0.0;	

	// Initialize raycast params
	RaycastMaxRange = 2000.0;
	HitBoxSize = 15.0;

	UpperVertFOV = 25.0;
	LowerVertFOV = -25.0;
	HorFOV = 45.0;
	HorizontalRes = 5.0;
	VerticalRes = 5.0;	

	//////// Procedural Mesh Part ///////////
	proc_mesh = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("ProceduralMesh"));
	// RootComponent = proc_mesh;
	proc_mesh->bUseAsyncCooking = true;

	ConstructorHelpers::FObjectFinder<UMaterial> MaterialObj(TEXT("Material'/Game/Static/Material/ProcMeshMaterial'"));
	proc_mesh_material = MaterialObj.Object;

	proc_mesh_dyn_material = UMaterialInstanceDynamic::Create(proc_mesh_material, NULL);

	// // Text component
	// text_str = "SENSOR_NAME";
	// TextComp = GetWorld()->SpawnActor<

}

// Called when the game starts or when spawned
void AFOVSensor::BeginPlay()
{
	Super::BeginPlay();
	
}

void AFOVSensor::CreateFOVMesh() {

		TArray<FVector> vertices;
		TArray<int32> triangles_front;
		TArray<int32> triangles_left;
		TArray<FVector> normals;
		TArray<FVector2D> UV0;
		TArray<FProcMeshTangent> tangents;
		TArray<FLinearColor> vertexColors;

		FVector SensorCenterLocation = GetActorLocation();
		FVector ForwardVector = GetActorForwardVector();
		FVector RightVector = GetActorRightVector();
		FVector UpVector = GetActorUpVector();
		FVector Start = SensorCenterLocation  + RayOffset.X * ForwardVector + RayOffset.Y * RightVector + RayOffset.Z * UpVector;

		float hor_fov_lower = (-HorFOV/2.0);
		float hor_fov_upper = (HorFOV/2.0);


		for(float i = LowerVertFOV; i <= UpperVertFOV; i+=VerticalRes) {
			for(float j = hor_fov_lower; j <= hor_fov_upper; j+=HorizontalRes) {
				auto RotatedRay = ForwardVector.RotateAngleAxis(j, UpVector);
				RotatedRay = RotatedRay.RotateAngleAxis(i, -RightVector);
				FVector End = Start + (RotatedRay*RaycastMaxRange);
				
				vertices.Add(End);
				vertexColors.Add(FColor::Cyan);

			}
		}

		vertices.Add(Start);

		// drawing rectangles from lower left corner to the upper right corner
		int hor_length = (hor_fov_upper - hor_fov_lower)/HorizontalRes + 1;
		int vert_length = (UpperVertFOV - LowerVertFOV)/VerticalRes;

		for(int i = 0; i < vert_length; i++) {
			for(int j = 0; j < hor_length-1; j++) {

				// front face of the cone
				triangles_front.Add(j + hor_length*i);
				triangles_front.Add(j + hor_length*i + 1);
				triangles_front.Add(j + hor_length*i + 1 + hor_length);

				triangles_front.Add(j + hor_length*i + 1 + hor_length);
				triangles_front.Add(j + hor_length*i + hor_length);
				triangles_front.Add(j + hor_length*i);

				// lower side of the cone
				if(i==0){
					triangles_front.Add(vertices.Num()-1); // adding "Start" point
					triangles_front.Add(j + hor_length*i);
					triangles_front.Add(j + hor_length*i + 1);
				}
				// upper side of the cone
				else if(i==(vert_length-1)){
					triangles_front.Add(vertices.Num()-1); // adding "Start" point
					triangles_front.Add(j + hor_length*(i+1));
					triangles_front.Add(j + hor_length*(i+1) + 1);
				}
			}

			// left side of the cone
			triangles_front.Add(vertices.Num()-1); // adding "Start" point
			triangles_front.Add(hor_length*i);
			triangles_front.Add(hor_length*(i+1));
			// right side of the cone
			triangles_front.Add(vertices.Num()-1); // adding "Start" point
			triangles_front.Add(hor_length*i+(hor_length-1));
			triangles_front.Add(hor_length*(i+1)+(hor_length-1));
			
		}

		proc_mesh->CreateMeshSection_LinearColor(0, vertices, triangles_front, normals, UV0, vertexColors, tangents, true); 

		proc_mesh_dyn_material->SetVectorParameterValue(TEXT("ColorParam"), RayColor);
		proc_mesh->SetMaterial(0, proc_mesh_dyn_material);


		if(!ProcMeshEnabled) {
			proc_mesh->SetVisibility(false);
		}
		else {
			proc_mesh->SetVisibility(true);
		}
		


}

// Called every frame
void AFOVSensor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// gisil
	if(RayCastEnabled) {
		RayCastFunc();
	}	
	
}

// gisil
void AFOVSensor::RayCastFunc() {
	
	FCollisionQueryParams CollisionParams;
	FHitResult HitResVec;

	FVector SensorCenterLocation = GetActorLocation();
	FVector ForwardVector = GetActorForwardVector();
	FVector RightVector = GetActorRightVector();
	FVector UpVector = GetActorUpVector();
	FVector Start = SensorCenterLocation  + RayOffset.X * ForwardVector + RayOffset.Y * RightVector + RayOffset.Z * UpVector;

	// GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, ForwardVector.ToString());
	// GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, RotatedRay.ToString());
	// GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, GetActorRotation().ToString());

	for(float i = LowerVertFOV; i <= UpperVertFOV; i+=VerticalRes) {
		for(float j = -HorFOV/2.0; j <= HorFOV/2.0; j+=HorizontalRes) {
			auto RotatedRay = ForwardVector.RotateAngleAxis(j, UpVector);
			RotatedRay = RotatedRay.RotateAngleAxis(i, -RightVector);
			FVector End = Start + (RotatedRay*RaycastMaxRange);

			// bool bHit = GetWorld()->LineTraceSingleByChannel(HitResVec, Start, End, ECC_Camera, CollisionParams);
			DrawDebugLine(GetWorld(), Start, End, RayColor);

			// if(bHit) {
			// 	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, "HIT");
			// 	DrawDebugSolidBox(GetWorld(), HitResVec.ImpactPoint, FVector(HitBoxSize, HitBoxSize, HitBoxSize), FColor::Red);
			// }
		}
	}
	
	// draw the local axis of the sensor
	DrawDebugLine(GetWorld(), Start, Start + ForwardVector*100, FColor::Green, false, -1, 0, 5.0);
	DrawDebugLine(GetWorld(), Start, Start + RightVector*100, FColor::Blue, false, -1, 0, 5.0);
	DrawDebugLine(GetWorld(), Start, Start + UpVector*100, FColor::Red, false, -1, 0, 5.0);

	

}
