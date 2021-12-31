// Fill out your copyright notice in the Description page of Project Settings.


#include "TruckBodyPawn.h"

#include "DrawDebugHelpers.h" // gisil
#include "Engine/World.h"
#include "UObject/ConstructorHelpers.h"
#include "FOVSensor.h"
#include "Trailer.h"
#include "NPC.h"

#include "Components/InputComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
// #include "Blueprint/UserWidget.h"
#include "MyUserWidget.h"

#include <string>
#include <fstream>
#include <math.h>

// SLATE
#include "Runtime/Engine/Public/Slate/SceneViewport.h"
#include "Runtime/SlateCore/Public/Widgets/SWindow.h"
#include "Runtime/Slate/Public/Widgets/SViewport.h"
#include "Runtime/SlateCore/Public/Widgets/SOverlay.h"
#include "Runtime/Slate/Public/Framework/Application/SlateApplication.h"
#include "Runtime/Slate/Public/Widgets/Layout/SConstraintCanvas.h"

// Sets default values
ATruckBodyPawn::ATruckBodyPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Setup mesh
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	// MeshComponent->SetupAttachment(GetRootComponent());
	SetRootComponent(MeshComponent);

	// Assigning a specific mesh programmatically to the MeshComponent
	static ConstructorHelpers::FObjectFinder<UStaticMesh> 
		MeshComponentAsset(TEXT("StaticMesh'/Game/Static/FH/FH_blender_Body'"));

	if(MeshComponentAsset.Succeeded()) {
		MeshComponent->SetStaticMesh(MeshComponentAsset.Object);
		MeshComponent->SetRelativeLocation(FVector(0.f, 0.f, 0.f));
		MeshComponent->SetWorldScale3D(FVector(1.f, 1.f, 1.f));
	}

	// To enable receiving inputs from keyboard controller inside the PAWN
	AutoPossessPlayer = EAutoReceiveInput::Player0;

	// adding possible ray color options
	RayColorOptions = {FColor::Green, FColor::Yellow, FColor::Purple, FColor::Orange, FColor::Magenta, 
						FColor::Blue, FColor::Cyan, FColor::Red};

	// Load assets to be changed in runtime
	static ConstructorHelpers::FObjectFinder<UStaticMesh> VehicleMeshAsset(TEXT("StaticMesh'/Game/Static/Mercedes/mercedes'"));	
	VehicleAsset = VehicleMeshAsset.Object;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> PedestrianMeshAsset(TEXT("StaticMesh'/Game/Static/Pedestrian/PedestrianStatic'"));	
	PedestrianAsset = PedestrianMeshAsset.Object;	

	static ConstructorHelpers::FObjectFinder<UStaticMesh> TrafficLightMeshAsset(TEXT("StaticMesh'/Game/Static/TrafficLight/traffic_light'"));	
	TrafficLightAsset = TrafficLightMeshAsset.Object;	

	// HUD
    // static ConstructorHelpers::FClassFinder<UUserWidget> UIObj(TEXT("/Game/UI/BP_MyUserWidget"));
	static ConstructorHelpers::FClassFinder<UMyUserWidget> UIObj(TEXT("/Game/UI/BP_MyUserWidget"));
    HUDWidgetClass = UIObj.Class;		
	
}

void ATruckBodyPawn::ParseJson() {
	// Parse the json for settings and spawn the sensors

	// std::ifstream jason_stream("/home/goksan/Desktop/test.json");
	// std::ifstream jason_stream("/home/simuser/Work/TruckFOV/test.json");
	

    FString RelativePath = FPaths::GameDir();
    FString AbsolutePath(FPaths::ConvertRelativePathToFull(RelativePath));
	std::string json_path = std::string(TCHAR_TO_UTF8(*AbsolutePath));

	std::ifstream jason_stream((json_path + "config.json").c_str());
    json_obj = nlohmann::json::parse(jason_stream);

}

// Called when the game starts or when spawned
void ATruckBodyPawn::BeginPlay()
{
	Super::BeginPlay();
	
	// gisil
	ParseJson();

	SpawnSensor();
	SpawnTrailer();
	SpawnNPCs();

	UpdateSensors();
	UpdateTrailer();

	FVector init_loc = GetActorLocation();
	FRotator init_rot = GetActorRotation();
	ViewLoc = init_loc + FVector(2000,0,1000);
	ViewRot = init_rot + FRotator(-15,-180,0);	

	// Create the UI Widget
     if (HUDWidgetClass != nullptr)
    {
        //  CurrentWidget = CreateWidget<UUserWidget>(GetWorld(), HUDWidgetClass);
		CurrentWidget = CreateWidget<UMyUserWidget>(GetWorld(), HUDWidgetClass);
 
        if (CurrentWidget)
        {
			// Creates UI in the existing Game Window
            CurrentWidget->AddToViewport();
			
			// Create UI Window
			// CreateNewUIWindow();

			// sending the reference of truck object to the widget, to call update functions
			CurrentWidget->BindTruckPawn(this);	
        }
    }	
}

// Called every frame
void ATruckBodyPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ATruckBodyPawn::GetActorEyesViewPoint( FVector& Location, FRotator& Rotation ) const {
	Location = ViewLoc;
	Rotation = ViewRot;
}

// Called to bind functionality to input
void ATruckBodyPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction(TEXT("UpdateConfigPress"), IE_Pressed, this, &ATruckBodyPawn::ConfigUpdate);

	PlayerInputComponent->BindAxis("MoveForward", this, &ATruckBodyPawn::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ATruckBodyPawn::MoveRight);
	PlayerInputComponent->BindAxis("TurnYaw", this, &ATruckBodyPawn::TurnYaw);
	PlayerInputComponent->BindAxis("TurnPitch", this, &ATruckBodyPawn::TurnPitch);
	PlayerInputComponent->BindAxis("Up", this, &ATruckBodyPawn::ElevateUp);
	PlayerInputComponent->BindAxis("Down", this, &ATruckBodyPawn::ElevateDown);	
}

void ATruckBodyPawn::MoveForward(float val) {
	ViewLoc.X += cos(ViewRot.Yaw*PI/180)*val*10;
	ViewLoc.Y += sin(ViewRot.Yaw*PI/180)*val*10;

	ViewLoc.Z += sin(ViewRot.Pitch*PI/180)*val*10;

	// GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, "FORWARD");
}

void ATruckBodyPawn::MoveRight(float val) {
	ViewLoc.X += cos((90+ViewRot.Yaw)*PI/180)*val*10;
	ViewLoc.Y += sin((90+ViewRot.Yaw)*PI/180)*val*10;

	// GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, "RIGHT");
}

void ATruckBodyPawn::TurnYaw(float val) {
	ViewRot.Yaw += val;
	// GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, "YAW");
}

void ATruckBodyPawn::TurnPitch(float val) {
	ViewRot.Pitch += val;
	// GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, "PITCH");
}

void ATruckBodyPawn::ElevateUp(float val) {
	ViewLoc.Z += val*10; 
	// GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, "UP");
}

void ATruckBodyPawn::ElevateDown(float val) {
	ViewLoc.Z -= val*10; 
	// GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, "DOWN");
}

void ATruckBodyPawn::ConfigUpdate() {
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, "Config Update");

	ParseJson();

	UpdateSensors();
	UpdateTrailer();
	UpdateNPCs();

}

void ATruckBodyPawn::SpawnNPCs() {

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	int ctr = 0;
	for(auto& npc : json_obj.at("traffic_objects")) {
		FVector Loc(npc.at("location").at("x"),
					npc.at("location").at("y"),
					npc.at("location").at("z")); 

		FRotator Rot(npc.at("rotation").at("pitch"),
					npc.at("rotation").at("yaw"),
					npc.at("rotation").at("roll"));
		

		SpawnedNPCRefs.emplace_back(GetWorld()->SpawnActor<ANPC>(ANPC::StaticClass(), Loc, Rot, SpawnParams));
		SpawnedNPCRefs.at(ctr)->AttachToActor(this, FAttachmentTransformRules::KeepRelativeTransform);

		// choose with type of NPC 
		if(npc.at("type") == "vehicle") {
			SpawnedNPCRefs.at(ctr)->MeshComponent->SetStaticMesh(VehicleAsset);
		}
		else if(npc.at("type") == "pedestrian") {
			SpawnedNPCRefs.at(ctr)->MeshComponent->SetStaticMesh(PedestrianAsset);
		}
		else if(npc.at("type") == "traffic_light") {
			SpawnedNPCRefs.at(ctr)->MeshComponent->SetStaticMesh(TrafficLightAsset);
		}
		else {
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, "UNRECOGNIZED OBJECT TYPE IN JSON FILE");
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, "QUITTING APPLICATION");
			FPlatformProcess::Sleep(5.0);
			FGenericPlatformMisc::RequestExit(false);
		}

		SpawnedNPCRefs.at(ctr)->MeshComponent->SetRelativeLocation(FVector(0.f, 0.f, 0.f));
		SpawnedNPCRefs.at(ctr)->MeshComponent->SetWorldScale3D(FVector(1.f, 1.f, 1.f));		

		// reloading mesh causes position reset
		SpawnedNPCRefs.at(ctr)->SetActorRelativeLocation(Loc);
		SpawnedNPCRefs.at(ctr)->SetActorRelativeRotation(Rot);	

		ctr++;		
	}

}

void ATruckBodyPawn::SpawnSensor() {

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	int ctr = 0;
	for(auto& sensor : json_obj.at("sensors")) {

		FVector Loc(sensor.at("location").at("x"),
					sensor.at("location").at("y"),
					sensor.at("location").at("z")); 

		FRotator Rot(sensor.at("rotation").at("pitch"),
					sensor.at("rotation").at("yaw"),
					sensor.at("rotation").at("roll"));

		SpawnedSensorRefs.emplace_back(GetWorld()->SpawnActor<AFOVSensor>(AFOVSensor::StaticClass(), Loc, Rot, SpawnParams));

		// FAttachmentTransformRules AttachmentRules;
		SpawnedSensorRefs.at(ctr)->AttachToActor(this, FAttachmentTransformRules::KeepRelativeTransform);

		// set Ray color
		SpawnedSensorRefs.at(ctr)->RayColor = RayColorOptions.at(ctr);

		ctr++;
	}

}

void ATruckBodyPawn::SpawnTrailer() {

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	FVector Loc(0,0,0); 
	FRotator Rot(json_obj.at("trailer").at(0).at("rotation").at("pitch"),
				 json_obj.at("trailer").at(0).at("rotation").at("yaw"),
				 json_obj.at("trailer").at(0).at("rotation").at("roll"));	
	
	SpawnedTrailerRef = GetWorld()->SpawnActor<ATrailer>(ATrailer::StaticClass(), Loc, Rot, SpawnParams);

	// FAttachmentTransformRules AttachmentRules;
	SpawnedTrailerRef->AttachToActor(this, FAttachmentTransformRules::KeepRelativeTransform);

}

void ATruckBodyPawn::UpdateSensors()  {

	int ctr = 0;

	for(auto& sensor_ref : SpawnedSensorRefs) {
		sensor_ref->RaycastMaxRange = json_obj.at("sensors")[ctr].at("range").at("max");
		sensor_ref->UpperVertFOV = json_obj.at("sensors")[ctr].at("field_of_view").at("upper_vertical");
		sensor_ref->LowerVertFOV = json_obj.at("sensors")[ctr].at("field_of_view").at("lower_vertical");
		sensor_ref->HorFOV = json_obj.at("sensors")[ctr].at("field_of_view").at("horizontal");
		sensor_ref->HorizontalRes = json_obj.at("sensors")[ctr].at("resolution").at("horizontal");
		sensor_ref->VerticalRes = json_obj.at("sensors")[ctr].at("resolution").at("vertical");

		sensor_ref->RayCastEnabled = json_obj.at("sensors")[ctr].at("raycast");
		sensor_ref->ProcMeshEnabled = json_obj.at("sensors")[ctr].at("cone");

		FVector Loc(json_obj.at("sensors")[ctr].at("location").at("x"),
					json_obj.at("sensors")[ctr].at("location").at("y"),
					json_obj.at("sensors")[ctr].at("location").at("z")); 

		FRotator Rot(json_obj.at("sensors")[ctr].at("rotation").at("pitch"),
					json_obj.at("sensors")[ctr].at("rotation").at("yaw"),
					json_obj.at("sensors")[ctr].at("rotation").at("roll"));				

		sensor_ref->SetActorRelativeLocation(Loc);
		sensor_ref->SetActorRelativeRotation(Rot);

		// execute
		sensor_ref->CreateFOVMesh();	

		ctr++;
	}
}

void ATruckBodyPawn::UpdateTrailer() {

	FRotator Rot(json_obj.at("trailer").at(0).at("rotation").at("pitch"),
				 json_obj.at("trailer").at(0).at("rotation").at("yaw"),
				 json_obj.at("trailer").at(0).at("rotation").at("roll"));	

	SpawnedTrailerRef->SetActorRelativeRotation(Rot);

}

void ATruckBodyPawn::UpdateNPCs() {

	int ctr = 0;

	for(auto& npc_ref : SpawnedNPCRefs) {

		FVector Loc(json_obj.at("traffic_objects")[ctr].at("location").at("x"),
					json_obj.at("traffic_objects")[ctr].at("location").at("y"),
					json_obj.at("traffic_objects")[ctr].at("location").at("z")); 

		FRotator Rot(json_obj.at("traffic_objects")[ctr].at("rotation").at("pitch"),
					json_obj.at("traffic_objects")[ctr].at("rotation").at("yaw"),
					json_obj.at("traffic_objects")[ctr].at("rotation").at("roll"));				

		npc_ref->SetActorRelativeLocation(Loc);
		npc_ref->SetActorRelativeRotation(Rot);

		ctr++;
	}

}


void ATruckBodyPawn::CreateNewUIWindow() {

	TSharedPtr<SWindow> UIWindow = nullptr;
	UIWindow = SNew(SWindow)
		.ScreenPosition(FVector2D(100, 100))
		.ClientSize(FVector2D(500, 800))
		.SizingRule(ESizingRule::UserSized)
		.Title(FText::FromString(TEXT("Configuration Window")))
		.AutoCenter(EAutoCenter::PrimaryWorkArea)
		.FocusWhenFirstShown(false)
		.UseOSWindowBorder(true)
		.UserResizeBorder(true)
		.CreateTitleBar(true)
		.SupportsTransparency(EWindowTransparency::PerWindow)
		.InitialOpacity(1.f)
		.SupportsMaximize(true);

	FSlateApplication & SlateApp = FSlateApplication::Get();

	UIWindow->SetViewportSizeDrivenByWindow(false);
	
	SlateApp.AddWindow(UIWindow.ToSharedRef(), true);

	TSharedPtr<SWidget> SlateWidget = CurrentWidget->TakeWidget();
	TSharedRef<SConstraintCanvas> ViewportWidget = SNew(SConstraintCanvas);

	ViewportWidget->AddSlot()
		[
			SlateWidget.ToSharedRef()
		];

	ViewportWidget->SetVisibility(EVisibility::Visible);

	UIWindow->SetContent(ViewportWidget);
	UIWindow->ShowWindow();
	UIWindow->SetWindowMode(EWindowMode::Windowed);
	
}