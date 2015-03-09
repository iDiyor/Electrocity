#ifndef __EC_TOWER_H__
#define __EC_TOWER_H__

#include "cocos2d.h"

using namespace cocos2d;

typedef enum  {
	ON_TOWER_BEGIN = 101,
	ON_TOWER_MOVED,
	ON_TOWER_ENDED
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

	void setPosition(const CCPoint& position);

	virtual bool ccTouchBegan(CCTouch* pTouch, CCEvent* pEvent);
	virtual void ccTouchMoved(CCTouch* pTouch, CCEvent* pEvent);
	virtual void ccTouchEnded(CCTouch* pTouch, CCEvent* pEvent);

	virtual void touchDelegateRetain();
	virtual void touchDelegateRelease();

private:
	CCPoint prev_position_;
	TowerState tower_state_;
};
#endif //__EC_TOWER_H__