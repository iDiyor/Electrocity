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

ECTower* ECTower::createTowerWithFileName(const char* fileName)
{
	ECTower* _tower = new ECTower();
	if (_tower && _tower->initTowerWithFileName(fileName)) {
		_tower->autorelease();
		return _tower;
	} else {
		delete _tower;
		_tower = NULL;
		return NULL;
	}
}
bool ECTower::initTowerWithFileName(const char* fileName)
{
	bool _isSuccess = false;
	do
	{
		CC_BREAK_IF(!CCSprite::initWithSpriteFrameName(fileName));


		_isSuccess = true;
	}while (0);
	return _isSuccess;
}
void ECTower::setPosition(const CCPoint& position)
{
	CCSize screenSize = CCDirector::sharedDirector()->getWinSize();
	CCPoint updatedPosition = position;

	if (position.x > screenSize.width) 
	{
		updatedPosition.x = screenSize.width;
	} 
	else if (position.x < 0) 
	{
		updatedPosition.x = 0;
	} 
	else if (position.y > screenSize.height) 	
	{
		updatedPosition.y = screenSize.height;
	}
	else if (position.y < 0) 
	{
		updatedPosition.y = 0;
	}
	CCSprite::setPosition(updatedPosition);
}
bool ECTower::isTouchOnMe(const CCTouch* touch)
{
	CCPoint touchPoint = touch->getLocationInView();
	touchPoint = CCDirector::sharedDirector()->convertToGL(touchPoint);
	if (this->boundingBox().containsPoint(touchPoint)) {

		return true;
	}
	return false;
}
void ECTower::playDustAnimation()
{
	CCLOG("DUST_ANIM");
}
int ECTower::getTowerState()
{
	return towerState;
}
bool ECTower::ccTouchBegan(CCTouch* pTouch, CCEvent* pEvent)
{
	CCPoint touchPoint = CCDirector::sharedDirector()->convertToGL(pTouch->getLocationInView());
	bool isContainsPoint = this->boundingBox().containsPoint(touchPoint);

	if (isContainsPoint) 
	{
		_prevPosition = touchPoint;
		towerState = ON_TOWER_BEGIN;
	}
	return isContainsPoint;
}
void ECTower::ccTouchMoved(CCTouch* pTouch, CCEvent* pEvent)
{
	CCPoint touchPoint = CCDirector::sharedDirector()->convertToGL(pTouch->getLocationInView());
	CCPoint moveTo = ccpSub(touchPoint, _prevPosition);

	this->setPosition(ccpAdd(this->getPosition(), moveTo));
	_prevPosition = touchPoint;

	towerState = ON_TOWER_MOVED;
}
void ECTower::ccTouchEnded(CCTouch* pTouch, CCEvent* pEvent)
{
	towerState = ON_TOWER_ENDED;
}
void ECTower::touchDelegateRetain()
{
	this->retain();
}
void ECTower::touchDelegateRelease()
{
	this->release();
}





