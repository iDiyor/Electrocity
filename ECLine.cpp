//  Copyright (c) 2015 Diyor Islomov. All rights reserved.
//	diyor.islomov@gmail.com || @iDiyor 
#include "ECLine.h"
#include "ECTower.h"
#include "ECBuilding.h"

ECLine::ECLine()
{

}
ECLine::~ECLine()
{

}
bool ECLine::init()
{
	bool is_success = false;
	do
	{
		CC_BREAK_IF(!CCSprite::initWithSpriteFrameName("line.png"));

		this->setAnchorPoint(ccp(0.0f, 0.5f));

		is_success = true;
	}while(0);
	return is_success;
}
void ECLine::ResetLineBetweenTowers(ECTower* tower_a, ECTower* tower_b) 
{
	CCPoint tower_a_point = ccp(tower_a->getPosition().x - (tower_a->getContentSize().width * 0.5f * 0.116f),
							  tower_a->getPosition().y + (tower_a->getContentSize().height * 0.37f));
	CCPoint tower_b_point = ccp(tower_b->getPosition().x - (tower_b->getContentSize().width * 0.5f * 0.116f),
							  tower_b->getPosition().y + (tower_b->getContentSize().height * 0.37f));
 
	float diff_x = tower_b_point.x - tower_a_point.x; // x difference
	float diff_y = tower_b_point.y - tower_a_point.y; // y difference

	//float diff = ccpDistance(towerPointB, towerPointA);

	float length = sqrtf((diff_x*diff_x) + (diff_y*diff_y)); // distance between two points
	float angle_in_rad = atan2f(diff_y, diff_x); 
	float angle_in_deg = CC_RADIANS_TO_DEGREES(angle_in_rad) * -1;

	this->setPosition(tower_a_point);
	this->setRotation(angle_in_deg);
	this->setScaleX(length / this->getContentSize().width);

	line_start_point_ = tower_a_point;
	line_end_point_   = tower_b_point;

	//_beginLinePoint = this->getPosition();
	//line_end_point_ = tower_b_point;//ccpAdd(this->getPosition(), ccpMult(ccp(cos(angle_in_rad), sin(angle_in_rad)), length));
}
bool ECLine::CheckCollisionWithBuilding(ECBuilding* building) {

	CCPoint building_position = building->getPosition();
	CCSize building_sprite_size = building->getContentSize();
	float building_sprite_scale = building->getScale();

	CCPoint left_bottom_point	= ccp(building_position.x - (building_sprite_size.width * (0.09375f * building_sprite_scale)),
									  building_position.y + (building_sprite_size.height * (0.269231f * building_sprite_scale)));
	CCPoint left_top_point		= ccp(left_bottom_point.x, left_bottom_point.y + (building_sprite_size.height * (0.15385f * building_sprite_scale)));
	CCPoint right_top_point		= ccp(left_top_point.x + (building_sprite_size.width * (0.125f * building_sprite_scale)), left_top_point.y);
	CCPoint right_bottom_point	= ccp(right_top_point.x, left_bottom_point.y);

	float s = 0 , t = 0;

	if ((ccpLineIntersect(line_start_point_, line_end_point_, left_bottom_point, left_top_point, &s, &t) && s >= 0 && s <= 1 && t >= 0 && t <= 1) ||
		(ccpLineIntersect(line_start_point_, line_end_point_, left_top_point, right_top_point, &s, &t) && s >= 0 && s <= 1 && t >= 0 && t <= 1) ||
		(ccpLineIntersect(line_start_point_, line_end_point_, right_top_point, right_bottom_point, &s, &t) && s >= 0 && s <= 1 && t >= 0 && t <= 1) || 
		(ccpLineIntersect(line_start_point_, line_end_point_, left_bottom_point, right_bottom_point, &s, &t) && s >= 0 && s <= 1 && t >= 0 && t <= 1)) 
	{
		//building->SetContactState(YES_CONTACT);
		return true;
	}

	//building->SetContactState(NO_CONTACT);
	return false;
}
CCPoint ECLine::GetLineEndPoint()
{
	return line_end_point_;
}


