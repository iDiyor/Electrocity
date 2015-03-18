//  Copyright (c) 2015 Diyor Islomov. All rights reserved.
//	diyor.islomov@gmail.com || @iDiyor 
#include "ECLine.h"
#include "ECTower.h"

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

	//_beginLinePoint = this->getPosition();
	line_end_point_ = tower_b_point;//ccpAdd(this->getPosition(), ccpMult(ccp(cos(angle_in_rad), sin(angle_in_rad)), length));
}
CCPoint ECLine::GetLineEndPoint()
{
	return line_end_point_;
}


