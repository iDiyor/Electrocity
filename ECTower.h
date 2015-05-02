//  Copyright (c) 2015 Diyor Islomov. All rights reserved.
//	diyor.islomov@gmail.com || @iDiyor 
#ifndef __EC_TOWER_H__
#define __EC_TOWER_H__

#include "cocos2d.h"

using namespace cocos2d;

class ECBuilding;

typedef enum  {
	ON_TOWER_TOUCH_BEGIN = 101,
	ON_TOWER_TOUCH_MOVED,
	ON_TOWER_TOUCH_ENDED
}TowerState;

class ECTower : public CCSprite, public CCTargetedTouchDelegate
{
public:

	static ECTower* CreateTowerWithFileName(const char* filename);
	bool InitTowerWithFileName(const char* filename);


	ECTower();
	~ECTower();

	virtual void onEnter();
	virtual void onExit();

	bool IsTouchOnMe(const CCTouch* touch);
	void PlayDustAnimation();

	int GetTowerState() const;
	CCPoint GetTouchStartPoint();
	bool CheckCollisionWithBuilding(ECBuilding* building);
	void MoveAnimationIfCollision();

	void setPosition(const CCPoint& position);

	virtual bool ccTouchBegan(CCTouch* pTouch, CCEvent* pEvent);
	virtual void ccTouchMoved(CCTouch* pTouch, CCEvent* pEvent);
	virtual void ccTouchEnded(CCTouch* pTouch, CCEvent* pEvent);

	virtual void touchDelegateRetain();
	virtual void touchDelegateRelease();

private:
	CCPoint prev_position_;
	CCPoint touch_start_point_;
	TowerState tower_state_;
	bool is_collision_with_building;
};
#endif //__EC_TOWER_H__