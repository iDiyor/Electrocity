#ifndef __TOWER_H__
#define __TOWER_H__

#include "cocos2d.h"

using namespace cocos2d;

class ECTower : public CCSprite
{
public:

	bool isTouchOnMe(const CCTouch* touch);
	void playDustAnimation();
	
	void setPosition(const CCPoint& position);
	const CCPoint& getPosition();
};
#endif //__TOWER_H__