//  Copyright (c) 2015 Diyor Islomov. All rights reserved.
//	diyor.islomov@gmail.com || @iDiyor 
#include "ECLine.h"
#include "ECTower.h"
#include "ECBuilding.h"

ECLine::ECLine() {

}

ECLine::~ECLine() {

}

bool ECLine::init() {
	bool is_success = false;
	do {
		CC_BREAK_IF(!CCSprite::initWithSpriteFrameName("line.png"));

		this->setAnchorPoint(ccp(0.0f, 0.5f));

		is_success = true;
	}while(0);
	return is_success;
}

void ECLine::SetLineType(LineType line_type) {
	line_type_ = line_type;
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

	if ((ccpLineIntersect(line_head_, line_tail_, left_bottom_point, left_top_point, &s, &t) && s >= 0 && s <= 1 && t >= 0 && t <= 1) ||
		(ccpLineIntersect(line_head_, line_tail_, left_top_point, right_top_point, &s, &t) && s >= 0 && s <= 1 && t >= 0 && t <= 1) ||
		(ccpLineIntersect(line_head_, line_tail_, right_top_point, right_bottom_point, &s, &t) && s >= 0 && s <= 1 && t >= 0 && t <= 1) || 
		(ccpLineIntersect(line_head_, line_tail_, left_bottom_point, right_bottom_point, &s, &t) && s >= 0 && s <= 1 && t >= 0 && t <= 1)) 
	{
		return true;
	}
	return false;
}

void ECLine::AttachLineToTowers(ECTower* tower_a, ECTower* tower_b) {
	tower_a_ = tower_a;
	tower_b_ = tower_b;

	UpdatePosition();
}

void ECLine::UpdatePosition() {
	if (tower_a_ != NULL && tower_b_ != NULL) {
		if (line_type_ == LINE_REAL) {
			line_head_ = ccp(tower_a_->getPosition().x - (tower_a_->getContentSize().width * 0.5f * 0.116f),
						     tower_a_->getPosition().y + (tower_a_->getContentSize().height * 0.37f));
			line_tail_ = ccp(tower_b_->getPosition().x - (tower_b_->getContentSize().width * 0.5f * 0.116f),
							 tower_b_->getPosition().y + (tower_b_->getContentSize().height * 0.37f));
		} 
		else if (line_type_ == LINE_SHADOW) {
			line_head_ = ccp(tower_a_->getPosition().x + (tower_a_->getContentSize().width * 0.5f * 0.62f),
							 tower_a_->getPosition().y - (tower_a_->getContentSize().height * 0.24f));
			line_tail_ = ccp(tower_b_->getPosition().x + (tower_b_->getContentSize().width * 0.5f * 0.62f),
							 tower_b_->getPosition().y - (tower_b_->getContentSize().height * 0.24f));
		}


		float diff_x = line_tail_.x - line_head_.x; // x difference
		float diff_y = line_tail_.y - line_head_.y; // y difference

		//float diff = ccpDistance(towerPointB, towerPointA);

		float length = sqrtf((diff_x*diff_x) + (diff_y*diff_y)); // distance between two points
		float angle_in_rad = atan2f(diff_y, diff_x); 
		float angle_in_deg = CC_RADIANS_TO_DEGREES(angle_in_rad) * -1;

		this->setPosition(line_head_);
		this->setRotation(angle_in_deg);
		this->setScaleX(length / this->getContentSize().width);
	}
}


