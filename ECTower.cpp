//  Copyright (c) 2015 Diyor Islomov. All rights reserved.
//	diyor.islomov@gmail.com || @iDiyor 
#include "ECTower.h"
#include "ECGameScene.h"

ECTower::ECTower()
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
bool ECTower::ccTouchBegan(CCTouch* pTouch, CCEvent* pEvent)
{
	CCPoint touch_point = CCDirector::sharedDirector()->convertToGL(pTouch->getLocationInView());
	bool is_contains_point = this->boundingBox().containsPoint(touch_point);

	if (is_contains_point) 
	{
		prev_position_ = touch_point;
		tower_state_ = ON_TOWER_TOUCH_BEGIN;
	}
	return is_contains_point;
}
void ECTower::ccTouchMoved(CCTouch* pTouch, CCEvent* pEvent)
{
	CCPoint touch_point = CCDirector::sharedDirector()->convertToGL(pTouch->getLocationInView());
	CCPoint move_to = ccpSub(touch_point, prev_position_);

	this->setPosition(ccpAdd(this->getPosition(), move_to));
	prev_position_ = touch_point;

	tower_state_ = ON_TOWER_TOUCH_MOVED;
}
void ECTower::ccTouchEnded(CCTouch* pTouch, CCEvent* pEvent)
{

	tower_state_ = ON_TOWER_TOUCH_ENDED;
}
void ECTower::touchDelegateRetain()
{
	this->retain();
}
void ECTower::touchDelegateRelease()
{
	this->release();
}





