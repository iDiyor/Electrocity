#include "ECBuilding.h"
#include "ECLine.h"
#include "ECTower.h"

#include <stdio.h>

ECBuilding::ECBuilding()
{
	light_state_ = LIGHT_OFF;
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
bool ECBuilding::CheckCollisionWithLine(ECLine* line)
{
	CCPoint line_begin_point = line->getPosition();
	CCPoint line_end_point = line->GetLineEndPoint();

	float scale = this->getScale();

	CCPoint pLeftBottom = ccp(this->getPosition().x - (this->getContentSize().width * (0.09375f * scale)),
										this->getPosition().y + (this->getContentSize().height * (0.269231f * scale)));
	CCPoint pLeftTop = ccp(pLeftBottom.x, pLeftBottom.y + (this->getContentSize().height * (0.15385f * scale)));
	CCPoint pRightTop = ccp(pLeftTop.x + (this->getContentSize().width * (0.125f * scale)), pLeftTop.y);
	CCPoint pRightBottom = ccp(pRightTop.x, pLeftBottom.y);	

	float s;
	float t;

	if ((ccpLineIntersect(line_begin_point, line_end_point, pLeftBottom, pLeftTop, &s, &t) && s >= 0 && s <= 1 && t >= 0 && t <= 1) ||
		(ccpLineIntersect(line_begin_point, line_end_point, pLeftTop, pRightTop, &s, &t) && s >= 0 && s <= 1 && t >= 0 && t <= 1) ||
		(ccpLineIntersect(line_begin_point, line_end_point, pRightTop, pRightBottom, &s, &t) && s >= 0 && s <= 1 && t >= 0 && t <= 1) || 
		(ccpLineIntersect(line_begin_point, line_end_point, pLeftBottom, pRightBottom, &s, &t) && s >= 0 && s <= 1 && t >= 0 && t <= 1)) 
	{
		return true;
	}
	return false;
}
bool ECBuilding::CheckCollisionWithLine(ECTower* tower_a, ECTower* tower_b)
{
	// points that represents the distance between to towers  TOWER_A <--------> TOWER_B
	CCPoint tower_a_point = ccp(tower_a->getPosition().x - (tower_a->getContentSize().width * 0.5f * 0.116f),
							    tower_a->getPosition().y + (tower_a->getContentSize().height * 0.37f));
	CCPoint tower_b_point = ccp(tower_b->getPosition().x - (tower_b->getContentSize().width * 0.5f * 0.116f),
							    tower_b->getPosition().y + (tower_b->getContentSize().height * 0.37f));

	float scale = this->getScale();

	CCPoint left_bottom_point	= ccp(this->getPosition().x - (this->getContentSize().width * (0.09375f * scale)),
									  this->getPosition().y + (this->getContentSize().height * (0.269231f * scale)));
	CCPoint left_top_point		= ccp(left_bottom_point.x, left_bottom_point.y + (this->getContentSize().height * (0.15385f * scale)));
	CCPoint right_top_point		= ccp(left_top_point.x + (this->getContentSize().width * (0.125f * scale)), left_top_point.y);
	CCPoint right_bottom_point	= ccp(right_top_point.x, left_bottom_point.y);	

	float s;
	float t;

	
	if ((ccpLineIntersect(tower_a_point, tower_b_point, left_bottom_point, left_top_point, &s, &t) && s >= 0 && s <= 1 && t >= 0 && t <= 1) ||
		(ccpLineIntersect(tower_a_point, tower_b_point, left_top_point, right_top_point, &s, &t) && s >= 0 && s <= 1 && t >= 0 && t <= 1) ||
		(ccpLineIntersect(tower_a_point, tower_b_point, right_top_point, right_bottom_point, &s, &t) && s >= 0 && s <= 1 && t >= 0 && t <= 1) || 
		(ccpLineIntersect(tower_a_point, tower_b_point, left_bottom_point, right_bottom_point, &s, &t) && s >= 0 && s <= 1 && t >= 0 && t <= 1)) 
	{
		return true;
	}
	return false;
}
void ECBuilding::SetBuildingLight(LightState state)
{
	// create on an image file name
	CCSpriteFrame* building_sprite_frame = NULL;
	
	switch (state)
	{
	case LIGHT_OFF:
		{		
			building_sprite_frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(building_image_filename_for_off_state_.c_str());
			this->setDisplayFrame(building_sprite_frame);
			light_state_ = LIGHT_OFF;
		}	
		break;
	case LIGHT_ON:
		{	
			building_sprite_frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(building_image_filename_for_on_state_.c_str());
			this->setDisplayFrame(building_sprite_frame);
			light_state_ = LIGHT_ON;
		}
		break;
	}
}
int ECBuilding::GetLightState()
{
	return light_state_;
}