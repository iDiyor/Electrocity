#ifndef __EC_TOWER_H__
#define __EC_TOWER_H__

#include "cocos2d.h"

using namespace cocos2d;

enum TOWER_STATE {
	ON_TOWER_BEGIN = 101,
	ON_TOWER_MOVED,
	ON_TOWER_ENDED
};

class ECTower : public CCSprite, public CCTargetedTouchDelegate
{
public:

	static ECTower* createTowerWithFileName(const char* fileName);
	bool initTowerWithFileName(const char* fileName);


	ECTower();
	~ECTower();

	virtual void onEnter();
	virtual void onExit();

	bool isTouchOnMe(const CCTouch* touch);
	void playDustAnimation();

	int getTowerState();


	virtual void setPosition(const CCPoint& position);

	virtual bool ccTouchBegan(CCTouch* pTouch, CCEvent* pEvent);
	virtual void ccTouchMoved(CCTouch* pTouch, CCEvent* pEvent);
	virtual void ccTouchEnded(CCTouch* pTouch, CCEvent* pEvent);

	virtual void touchDelegateRetain();
	virtual void touchDelegateRelease();

private:
	CCPoint _prevPosition;
	TOWER_STATE towerState;
};
#endif //__EC_TOWER_H__