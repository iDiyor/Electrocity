//  Copyright (c) 2015 Diyor Islomov. All rights reserved.
//	diyor.islomov@gmail.com || @iDiyor 
#include "ECTower.h"
#include "ECGameScene.h"
#include "ECBuilding.h"

ECTower::ECTower() : 
	is_collision_with_building(false)
{
	
}
ECTower::~ECTower()
{
	
}
void ECTower::onEnter()
{
	CCDirector* pDirector = CCDirector::sharedDirector();
	pDirector->getTouchDispatcher()->addTargetedDelegate(this,0,true);
	CCSprite::onEnter();
}
void ECTower::onExit()
{
	CCDirector* pDirector = CCDirector::sharedDirector();
	pDirector->getTouchDispatcher()->removeDelegate(this);
	CCSprite::onExit();
}

ECTower* ECTower::CreateTowerWithFileName(const char* filename)
{
	ECTower* tower = new ECTower();
	if (tower && tower->InitTowerWithFileName(filename)) {
		tower->autorelease();
		return tower;
	} else {
		delete tower;
		tower = NULL;
		return NULL;
	}
}
bool ECTower::InitTowerWithFileName(const char* filename)
{
	bool is_success = false;
	do
	{
		CC_BREAK_IF(!CCSprite::initWithSpriteFrameName(filename));

		tower_state_ = ON_TOWER_TOUCH_ENDED;

		is_success = true;
	}while (0);
	return is_success;
}
void ECTower::setPosition(const CCPoint& position)
{
	CCSize screen_size = CCDirector::sharedDirector()->getWinSize();
	CCPoint updated_position = position;

	if (position.x > screen_size.width) 
	{
		updated_position.x = screen_size.width;
	} 
	else if (position.x < 0) 
	{
		updated_position.x = 0;
	} 
	else if (position.y > screen_size.height) 	
	{
		updated_position.y = screen_size.height;
	}
	else if (position.y < 0) 
	{
		updated_position.y = 0;
	}
	CCSprite::setPosition(updated_position);
}
bool ECTower::IsTouchOnMe(const CCTouch* touch)
{
	CCPoint touch_point = touch->getLocationInView();
	touch_point = CCDirector::sharedDirector()->convertToGL(touch_point);
	if (this->boundingBox().containsPoint(touch_point))
		return true;

	return false;
}
void ECTower::PlayDustAnimation()
{
	CCLOG("DUST_ANIM");
}
int ECTower::GetTowerState() const
{
	return tower_state_;
}
CCPoint ECTower::GetTouchStartPoint() {
	return touch_start_point_;
}
bool ECTower::CheckCollisionWithBuilding(ECBuilding* building) {
	
	// tower`s collision boundary
	CCPoint tower_top_point = ccp(this->getPosition().x - (this->getContentSize().width * 0.08f), 
								  this->getPosition().y + (this->getContentSize().height * 0.40f));
	CCPoint tower_bottom_point = ccp(tower_top_point.x, tower_top_point.y - (this->getContentSize().height * 0.80f));

	// building`s collision boundary
	CCPoint building_position = building->getPosition();
	CCSize building_sprite_size = building->getContentSize();
	float building_sprite_scale = building->getScale();

	CCPoint left_bottom_point	= ccp(building_position.x - (building_sprite_size.width * (0.40f * building_sprite_scale)),
									  building_position.y + (building_sprite_size.height * (0.10f * building_sprite_scale)));
	CCPoint left_top_point		= ccp(left_bottom_point.x, left_bottom_point.y + (building_sprite_size.height * (0.10f * building_sprite_scale)));
	CCPoint right_top_point		= ccp(left_top_point.x + (building_sprite_size.width * (0.77f * building_sprite_scale)), left_top_point.y);
	CCPoint right_bottom_point	= ccp(right_top_point.x, left_bottom_point.y);
	

	float s = 0 , t = 0;

	if ((ccpLineIntersect(tower_top_point, tower_bottom_point, left_bottom_point, left_top_point, &s, &t) && s >= 0 && s <= 1 && t >= 0 && t <= 1) ||
		(ccpLineIntersect(tower_top_point, tower_bottom_point, left_top_point, right_top_point, &s, &t) && s >= 0 && s <= 1 && t >= 0 && t <= 1) ||
		(ccpLineIntersect(tower_top_point, tower_bottom_point, right_top_point, right_bottom_point, &s, &t) && s >= 0 && s <= 1 && t >= 0 && t <= 1) || 
		(ccpLineIntersect(tower_top_point, tower_bottom_point, left_bottom_point, right_bottom_point, &s, &t) && s >= 0 && s <= 1 && t >= 0 && t <= 1)) 
	{
		is_collision_with_building = true;
		return true;
	}
	is_collision_with_building = false;
	return false;
}
void ECTower::MoveAnimationIfCollision() {

}

bool ECTower::ccTouchBegan(CCTouch* pTouch, CCEvent* pEvent)
{
	CCPoint touch_point = CCDirector::sharedDirector()->convertToGL(pTouch->getLocationInView());
	bool is_contains_point = this->boundingBox().containsPoint(touch_point);

	if (is_contains_point) 
	{
		tower_state_ = ON_TOWER_TOUCH_BEGIN;
		prev_position_ = touch_point;
		if (!is_collision_with_building)
			touch_start_point_ = touch_point;
	}
	return is_contains_point;
}
void ECTower::ccTouchMoved(CCTouch* pTouch, CCEvent* pEvent)
{
	tower_state_ = ON_TOWER_TOUCH_MOVED;

	CCPoint touch_point = CCDirector::sharedDirector()->convertToGL(pTouch->getLocationInView());
	CCPoint move_to = ccpSub(touch_point, prev_position_);

	this->setPosition(ccpAdd(this->getPosition(), move_to));
	prev_position_ = touch_point;

}
void ECTower::ccTouchEnded(CCTouch* pTouch, CCEvent* pEvent)
{
	tower_state_ = ON_TOWER_TOUCH_ENDED;

	if (is_collision_with_building) {
		CCMoveTo* move_to_old_point = CCMoveTo::create(0.2f, touch_start_point_);
		this->runAction(move_to_old_point);
	}

}
void ECTower::touchDelegateRetain()
{
	this->retain();
}
void ECTower::touchDelegateRelease()
{
	this->release();
}





