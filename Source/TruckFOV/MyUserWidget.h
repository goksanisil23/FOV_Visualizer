// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "json.hpp"
#include <string>

#include "MyUserWidget.generated.h"

// forward decleration
class UButton;
class UCheckBox;
class USpinBox;
class UTextBlock;
class UComboBoxString;

class ATruckBodyPawn; 

/**
 * 
 */
UCLASS()
class TRUCKFOV_API UMyUserWidget : public UUserWidget
{
	GENERATED_BODY()

// gisil
public:
	virtual void NativeConstruct() override;

	UPROPERTY(meta = (BindWidget))
	UButton* Save_config;

	UPROPERTY(meta = (BindWidget))
	UCheckBox* CheckBox_rays;	
	UPROPERTY(meta = (BindWidget))
	UCheckBox* CheckBox_cone;		

	UPROPERTY(meta = (BindWidget))
	USpinBox* Sensor_location_x;	
	UPROPERTY(meta = (BindWidget))
	USpinBox* Sensor_location_y;
	UPROPERTY(meta = (BindWidget))
	USpinBox* Sensor_location_z;	
	UPROPERTY(meta = (BindWidget))
	USpinBox* Sensor_rotation_yaw;	
	UPROPERTY(meta = (BindWidget))
	USpinBox* Sensor_rotation_pitch;	
	UPROPERTY(meta = (BindWidget))
	USpinBox* Sensor_rotation_roll;	
	UPROPERTY(meta = (BindWidget))
	USpinBox* Sensor_range;			
	UPROPERTY(meta = (BindWidget))
	USpinBox* Sensor_fov_hor;	
	UPROPERTY(meta = (BindWidget))
	USpinBox* Sensor_fov_vert_low;	
	UPROPERTY(meta = (BindWidget))
	USpinBox* Sensor_fov_vert_high;	
	UPROPERTY(meta = (BindWidget))
	USpinBox* Sensor_res_hor;
	UPROPERTY(meta = (BindWidget))
	USpinBox* Sensor_res_vert;			

	UPROPERTY(meta = (BindWidget))
	UTextBlock* range_text;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* fov_text;	
	UPROPERTY(meta = (BindWidget))
	UTextBlock* fov_hor_text;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* fov_vert_plus_text;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* fov_vert_minus_text;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* resolution_text;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* resolution_hor_text;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* resolution_vert_text;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* rays_text;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* cone_text;	
	UPROPERTY(meta = (BindWidget))
	UTextBlock* loc_x_text;	
	UPROPERTY(meta = (BindWidget))
	UTextBlock* loc_y_text;	
	UPROPERTY(meta = (BindWidget))
	UTextBlock* loc_z_text;																		
	UPROPERTY(meta = (BindWidget))
	UTextBlock* loc_text;																		

	UPROPERTY(meta = (BindWidget))
	UComboBoxString* Config_option;
	
	UFUNCTION()
	void OnClick();

	UFUNCTION()
	void OnCheckCone(bool check_state);
	UFUNCTION()
	void OnCheckRay(bool check_state);		

	UFUNCTION()
	void OnLocXChanged(float val);	
	UFUNCTION()
	void OnLocYChanged(float val);		
	UFUNCTION()
	void OnLocZChanged(float val);	
	UFUNCTION()
	void OnRotYawChanged(float val);	
	UFUNCTION()
	void OnRotPitchChanged(float val);	
	UFUNCTION()
	void OnRotRollChanged(float val);	
	UFUNCTION()
	void OnRangeChanged(float val);	
	UFUNCTION()
	void OnFOVHorChanged(float val);					
	UFUNCTION()
	void OnFOVVertLowChanged(float val);
	UFUNCTION()
	void OnFOVVertHighChanged(float val);
	UFUNCTION()
	void OnResHorChanged(float val);
	UFUNCTION()
	void OnResVertChanged(float val);

	UFUNCTION()
	void OnConfigOptChanged(FString selection, ESelectInfo::Type type_input);				


	void BindTruckPawn(ATruckBodyPawn* truck_ref_input);

	ATruckBodyPawn* truck_ref;	

private:
	std::string active_item;
	int active_idx;
	void UpdateGUI();
	void UpdateTruck();

};
