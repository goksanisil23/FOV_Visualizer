// Fill out your copyright notice in the Description page of Project Settings.


#include "MyUserWidget.h"
#include "UMG/Public/Components/Button.h"
#include "UMG/Public/Components/CheckBox.h"
#include "UMG/Public/Components/SpinBox.h"
#include "UMG/Public/Components/TextBlock.h"
#include "UMG/Public/Components/ComboBoxString.h"

#include "TruckBodyPawn.h"

#include <fstream>
#include <iomanip> // for setw

void UMyUserWidget::NativeConstruct() {
    Super::NativeConstruct();

    // set initially active sensor and traffic object from json file
    active_item = "sensors"; 
    active_idx = 0;    

    Save_config->OnClicked.AddDynamic(this, &UMyUserWidget::OnClick);
    CheckBox_rays->OnCheckStateChanged.AddDynamic(this, &UMyUserWidget::OnCheckRay);
    CheckBox_cone->OnCheckStateChanged.AddDynamic(this, &UMyUserWidget::OnCheckCone);


    Sensor_location_x->OnValueChanged.AddDynamic(this, &UMyUserWidget::OnLocXChanged);
    Sensor_location_y->OnValueChanged.AddDynamic(this, &UMyUserWidget::OnLocYChanged);
    Sensor_location_z->OnValueChanged.AddDynamic(this, &UMyUserWidget::OnLocZChanged);
    Sensor_rotation_yaw->OnValueChanged.AddDynamic(this, &UMyUserWidget::OnRotYawChanged);
    Sensor_rotation_pitch->OnValueChanged.AddDynamic(this, &UMyUserWidget::OnRotPitchChanged);
    Sensor_rotation_roll->OnValueChanged.AddDynamic(this, &UMyUserWidget::OnRotRollChanged);    
    Sensor_range->OnValueChanged.AddDynamic(this, &UMyUserWidget::OnRangeChanged);
    Sensor_fov_hor->OnValueChanged.AddDynamic(this, &UMyUserWidget::OnFOVHorChanged);
    Sensor_fov_vert_low->OnValueChanged.AddDynamic(this, &UMyUserWidget::OnFOVVertLowChanged);
    Sensor_fov_vert_high->OnValueChanged.AddDynamic(this, &UMyUserWidget::OnFOVVertHighChanged);
    Sensor_res_hor->OnValueChanged.AddDynamic(this, &UMyUserWidget::OnResHorChanged);
    Sensor_res_vert->OnValueChanged.AddDynamic(this, &UMyUserWidget::OnResVertChanged);

    Config_option->OnSelectionChanged.AddDynamic(this, &UMyUserWidget::OnConfigOptChanged);

    Sensor_fov_vert_high->SetMinValue(1.0);
    Sensor_fov_vert_high->SetMaxValue(180);
    
    Sensor_fov_vert_low->SetMinValue(-180.0);
    Sensor_fov_vert_low->SetMaxValue(-1.0);  

    Sensor_res_hor->SetMinValue(0.05);
    Sensor_res_hor->SetMaxValue(5.0); 
    
    Sensor_res_vert->SetMinValue(0.05);
    Sensor_res_vert->SetMaxValue(5.0);    

}

void UMyUserWidget::OnClick() {
    GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, "Saved new config file.");

    FString RelativePath = FPaths::GameDir();
    FString AbsolutePath(FPaths::ConvertRelativePathToFull(RelativePath));
	std::string json_path = std::string(TCHAR_TO_UTF8(*AbsolutePath));

    std::ofstream output_config_file((json_path + "new_config.json").c_str());
    output_config_file << std::setw(4) << truck_ref->json_obj << std::endl;     
}

void UMyUserWidget::BindTruckPawn(ATruckBodyPawn* truck_ref_input) {
    
    // take the main truck reference
    truck_ref = truck_ref_input;

    // set initial GUI values from the json
    UpdateGUI();

    // set GUI limits
    Sensor_range->SetMinSliderValue(1.0);
    Sensor_range->SetMaxSliderValue(10000.0);
    
    Sensor_fov_hor->SetMinSliderValue(1.0);
    Sensor_fov_hor->SetMaxSliderValue(360.0);
    
    Sensor_fov_vert_low->SetMinSliderValue(-180.0);
    Sensor_fov_vert_low->SetMaxSliderValue(-1.0);
    
    Sensor_fov_vert_high->SetMinSliderValue(1.0);
    Sensor_fov_vert_high->SetMaxSliderValue(180.0);

    Sensor_res_hor->SetMinSliderValue(0.05);
    Sensor_res_hor->SetMaxSliderValue(5.0);
    Sensor_res_hor->Delta = 0.001;
    
    Sensor_res_vert->SetMinSliderValue(0.05);
    Sensor_res_vert->SetMaxSliderValue(5.0);
    Sensor_res_vert->Delta = 0.001;

    // Adding options to selection box in the GUI, based on json data
    for(auto& sensor : truck_ref->json_obj.at("sensors")) {
        std::string temp = sensor.at("id");
        Config_option->AddOption(FString(temp.c_str()));
    }

    for(auto& npc : truck_ref->json_obj.at("traffic_objects")) {
        std::string temp = npc.at("id");
        Config_option->AddOption(FString(temp.c_str()));
    }

    std::string temp = truck_ref->json_obj.at("trailer").at(0).at("id");
    Config_option->AddOption(FString(temp.c_str()));


    // start the app with the selected option as the first sensor on the json
    Config_option->SetSelectedOption(std::string(truck_ref->json_obj.at(active_item).at(active_idx).at("id")).c_str());

}	

void UMyUserWidget::UpdateTruck() {
    truck_ref->UpdateSensors();
    truck_ref->UpdateNPCs();
    truck_ref->UpdateTrailer();
}

void UMyUserWidget::OnCheckCone(bool check_state) {
    truck_ref->json_obj.at(active_item).at(active_idx).at("cone") = check_state;
    UpdateTruck();
}

void UMyUserWidget::OnCheckRay(bool check_state) {
    truck_ref->json_obj.at(active_item).at(active_idx).at("raycast") = check_state;
    UpdateTruck();
}		

void UMyUserWidget::OnLocXChanged(float val) {    
    truck_ref->json_obj.at(active_item).at(active_idx).at("location").at("x") = val;
    UpdateTruck();
}

void UMyUserWidget::OnLocYChanged(float val) {
    truck_ref->json_obj.at(active_item).at(active_idx).at("location").at("y") = val;
    UpdateTruck();
}	

void UMyUserWidget::OnLocZChanged(float val) {
    truck_ref->json_obj.at(active_item).at(active_idx).at("location").at("z") = val;
    UpdateTruck();
}

void UMyUserWidget::OnRotYawChanged(float val) {
    truck_ref->json_obj.at(active_item).at(active_idx).at("rotation").at("yaw") = val;
    UpdateTruck();
}

void UMyUserWidget::OnRotPitchChanged(float val) {
    truck_ref->json_obj.at(active_item).at(active_idx).at("rotation").at("pitch") = val;
    UpdateTruck();
}

void UMyUserWidget::OnRotRollChanged(float val) {
    truck_ref->json_obj.at(active_item).at(active_idx).at("rotation").at("roll") = val;
    UpdateTruck();
}

void UMyUserWidget::OnRangeChanged(float val) {
    truck_ref->json_obj.at(active_item).at(active_idx).at("range").at("max") = val;
    UpdateTruck();
}

void UMyUserWidget::OnFOVHorChanged(float val) {
    truck_ref->json_obj.at(active_item).at(active_idx).at("field_of_view").at("horizontal") = val;
    UpdateTruck();
}		

void UMyUserWidget::OnFOVVertLowChanged(float val) {
    truck_ref->json_obj.at(active_item).at(active_idx).at("field_of_view").at("lower_vertical") = val;
    UpdateTruck();
}		

void UMyUserWidget::OnFOVVertHighChanged(float val) {
    truck_ref->json_obj.at(active_item).at(active_idx).at("field_of_view").at("upper_vertical") = val;
    UpdateTruck();
}		

void UMyUserWidget::OnResHorChanged(float val) {
    truck_ref->json_obj.at(active_item).at(active_idx).at("resolution").at("horizontal") = val;
    UpdateTruck();
}

void UMyUserWidget::OnResVertChanged(float val) {
    truck_ref->json_obj.at(active_item).at(active_idx).at("resolution").at("vertical") = val;
    UpdateTruck();
}


// We hide/show the possible configurable actor options based on the selection, as well as updating the UI with the currently active values
void UMyUserWidget::OnConfigOptChanged(FString selection, ESelectInfo::Type type_input) {
    
    std::string selected_option = std::string(TCHAR_TO_UTF8(*selection));

    for(nlohmann::json::iterator itr = truck_ref->json_obj.begin(); itr != truck_ref->json_obj.end(); itr++) {
        int ent_ctr = 0;
        for(auto& entry : itr.value()) {
            std::string id(entry.at("id"));
            if(!selected_option.compare(id)) {
                
                // set the correct type of item and it's index
                active_idx = ent_ctr;
                active_item = itr.key();

                // GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString(active_item.c_str()) + FString(" ") + FString::FromInt(active_idx));

                UpdateGUI();
            }
            ent_ctr++;
        }
    }

}

void UMyUserWidget::UpdateGUI() {

    // rotation is common for all configurable objects
    Sensor_rotation_yaw->SetValue(truck_ref->json_obj.at(active_item).at(active_idx).at("rotation").at("yaw"));
    Sensor_rotation_pitch->SetValue(truck_ref->json_obj.at(active_item).at(active_idx).at("rotation").at("pitch"));
    Sensor_rotation_roll->SetValue(truck_ref->json_obj.at(active_item).at(active_idx).at("rotation").at("roll"));     

    // trailer has no rotation
    if(active_item.compare("trailer")) {

        Sensor_location_x->SetValue(truck_ref->json_obj.at(active_item).at(active_idx).at("location").at("x"));
        Sensor_location_y->SetValue(truck_ref->json_obj.at(active_item).at(active_idx).at("location").at("y"));
        Sensor_location_z->SetValue(truck_ref->json_obj.at(active_item).at(active_idx).at("location").at("z"));           

        Sensor_location_x->SetVisibility(ESlateVisibility::Visible);
        Sensor_location_y->SetVisibility(ESlateVisibility::Visible);
        Sensor_location_z->SetVisibility(ESlateVisibility::Visible);
        loc_x_text->SetVisibility(ESlateVisibility::Visible);
        loc_y_text->SetVisibility(ESlateVisibility::Visible);
        loc_z_text->SetVisibility(ESlateVisibility::Visible);
        loc_text->SetVisibility(ESlateVisibility::Visible);

    }
    else {
        Sensor_location_x->SetVisibility(ESlateVisibility::Hidden);
        Sensor_location_y->SetVisibility(ESlateVisibility::Hidden);
        Sensor_location_z->SetVisibility(ESlateVisibility::Hidden);
        loc_x_text->SetVisibility(ESlateVisibility::Hidden);
        loc_y_text->SetVisibility(ESlateVisibility::Hidden);
        loc_z_text->SetVisibility(ESlateVisibility::Hidden);  
        loc_text->SetVisibility(ESlateVisibility::Hidden);      
    }

    // only configurable for sensors
    if(!active_item.compare("sensors")) {
        Sensor_range->SetValue(truck_ref->json_obj.at(active_item).at(active_idx).at("range").at("max"));
        Sensor_fov_hor->SetValue(truck_ref->json_obj.at(active_item).at(active_idx).at("field_of_view").at("horizontal"));
        Sensor_fov_vert_low->SetValue(truck_ref->json_obj.at(active_item).at(active_idx).at("field_of_view").at("lower_vertical"));
        Sensor_fov_vert_high->SetValue(truck_ref->json_obj.at(active_item).at(active_idx).at("field_of_view").at("upper_vertical"));
        Sensor_res_hor->SetValue(truck_ref->json_obj.at(active_item).at(active_idx).at("resolution").at("horizontal"));
        Sensor_res_vert->SetValue(truck_ref->json_obj.at(active_item).at(active_idx).at("resolution").at("vertical"));

        // set the GUI values to the values obtained from the json file
        if(truck_ref->json_obj.at(active_item).at(active_idx).at("cone")) {
            CheckBox_cone->SetCheckedState(ECheckBoxState::Checked);
        }
        else {
            CheckBox_cone->SetCheckedState(ECheckBoxState::Unchecked);
        }

        if(truck_ref->json_obj.at(active_item).at(active_idx).at("raycast")) {
            CheckBox_rays->SetCheckedState(ECheckBoxState::Checked);
        }
        else {
            CheckBox_rays->SetCheckedState(ECheckBoxState::Unchecked);
        }
        CheckBox_rays->SetVisibility(ESlateVisibility::Visible);
        CheckBox_cone->SetVisibility(ESlateVisibility::Visible);       

        Sensor_range->SetVisibility(ESlateVisibility::Visible);
        Sensor_fov_hor->SetVisibility(ESlateVisibility::Visible);
        Sensor_fov_vert_low->SetVisibility(ESlateVisibility::Visible);
        Sensor_fov_vert_high->SetVisibility(ESlateVisibility::Visible);
        Sensor_res_hor->SetVisibility(ESlateVisibility::Visible);
        Sensor_res_vert->SetVisibility(ESlateVisibility::Visible);

        range_text->SetVisibility(ESlateVisibility::Visible);
        fov_text->SetVisibility(ESlateVisibility::Visible);	
        fov_hor_text->SetVisibility(ESlateVisibility::Visible);
        fov_vert_plus_text->SetVisibility(ESlateVisibility::Visible);
        fov_vert_minus_text->SetVisibility(ESlateVisibility::Visible);
        resolution_text->SetVisibility(ESlateVisibility::Visible);
        resolution_hor_text->SetVisibility(ESlateVisibility::Visible);
        resolution_vert_text->SetVisibility(ESlateVisibility::Visible);
        rays_text->SetVisibility(ESlateVisibility::Visible);
        cone_text->SetVisibility(ESlateVisibility::Visible);

    }
    else { //hiding sensor specific options
        CheckBox_rays->SetVisibility(ESlateVisibility::Hidden);
        CheckBox_cone->SetVisibility(ESlateVisibility::Hidden);

        CheckBox_rays->SetVisibility(ESlateVisibility::Hidden);
        CheckBox_cone->SetVisibility(ESlateVisibility::Hidden);       

        Sensor_range->SetVisibility(ESlateVisibility::Hidden);
        Sensor_fov_hor->SetVisibility(ESlateVisibility::Hidden);
        Sensor_fov_vert_low->SetVisibility(ESlateVisibility::Hidden);
        Sensor_fov_vert_high->SetVisibility(ESlateVisibility::Hidden);
        Sensor_res_hor->SetVisibility(ESlateVisibility::Hidden);
        Sensor_res_vert->SetVisibility(ESlateVisibility::Hidden);        

        range_text->SetVisibility(ESlateVisibility::Hidden);
        fov_text->SetVisibility(ESlateVisibility::Hidden);	
        fov_hor_text->SetVisibility(ESlateVisibility::Hidden);
        fov_vert_plus_text->SetVisibility(ESlateVisibility::Hidden);
        fov_vert_minus_text->SetVisibility(ESlateVisibility::Hidden);
        resolution_text->SetVisibility(ESlateVisibility::Hidden);
        resolution_hor_text->SetVisibility(ESlateVisibility::Hidden);
        resolution_vert_text->SetVisibility(ESlateVisibility::Hidden);
        rays_text->SetVisibility(ESlateVisibility::Hidden);
        cone_text->SetVisibility(ESlateVisibility::Hidden);        
    }

}