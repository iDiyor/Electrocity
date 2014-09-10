#include "Tower.h"

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
const CCPoint& ECTower::getPosition()
{
	return CCPointZero;
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
