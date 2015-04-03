//  Copyright (c) 2015 Diyor Islomov. All rights reserved.
//	diyor.islomov@gmail.com || @iDiyor 
#include "ECBuilding.h"
#include "ECLine.h"
#include "ECTower.h"

#include <stdio.h>

ECBuilding::ECBuilding()
{
	light_state_	= LIGHT_OFF;
	contact_state_	= NO_CONTACT;
	contacted_line_index_ = -1;
}
ECBuilding::~ECBuilding()
{

}
ECBuilding* ECBuilding::CreateBuildingWithFileName(const std::string& filename)
{
	ECBuilding* building = new ECBuilding();
	if (building && building->InitBuildingWithFileName(filename)) {
		building->autorelease();
		return building;
	} else {
		delete building;
		building = NULL;
		return NULL;
	}
}
bool ECBuilding::InitBuildingWithFileName(const std::string& filename)
{
	bool is_success = false;
	do
	{
		CC_BREAK_IF(!CCSprite::initWithSpriteFrameName(filename.c_str()));

		building_image_filename_for_off_state_ = filename;

		std::string house_name_in_string = building_image_filename_for_off_state_.substr(0,5);							// "house"
		std::size_t image_number_in_string = building_image_filename_for_off_state_.find_last_of("_");					// position of last "_"
		std::string image_file_number_and_format = building_image_filename_for_off_state_.substr(image_number_in_string);// get from last "_" to the end => "_number.png"
		house_name_in_string.append("_on");																				// "house_on
		house_name_in_string.append(image_file_number_and_format);														// "house_on_number.png"

		building_image_filename_for_on_state_ = house_name_in_string;

		is_success = true;
	}while(0);
	return is_success;
}
void ECBuilding::SetBuildingLight(LightState state)
{
	// create on an image file name
	CCSpriteFrame* building_sprite_frame = NULL;
	
	switch (state)
	{
	case LIGHT_OFF:
		{		
			if (GetLightState() == LIGHT_ON) {
				building_sprite_frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(building_image_filename_for_off_state_.c_str());
				this->setDisplayFrame(building_sprite_frame);
				light_state_ = LIGHT_OFF;
			}
		}	
		break;
	case LIGHT_ON:
		{	
			if (GetLightState() == LIGHT_OFF) {
				building_sprite_frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(building_image_filename_for_on_state_.c_str());
				this->setDisplayFrame(building_sprite_frame);
				light_state_ = LIGHT_ON;
			}
		}
		break;
	}
}
int ECBuilding::GetLightState() {
	return light_state_;
}
void ECBuilding::SetContactState(ContactState state) {
	contact_state_ = state;
}
int ECBuilding::GetContactState() const{
	return contact_state_;
}
void ECBuilding::SetContactedLineIndex(int index) {
	contacted_line_index_ = index;
}
int ECBuilding::GetContactedLineIndex() const {
	return contacted_line_index_;
}


