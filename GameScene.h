#ifndef __GAME_SCENE_H__
#define __GAME_SCENE_H__

#include "cocos2d.h"

using namespace	cocos2d;

class ECGameScene : public CCLayer
{
public:

	static CCScene* sceneWithGameLayerToLevel(const char* level);
	static ECGameScene* createGameLayerToLevel(const char* level);
	static ECGameScene* sharedGameLayer();
	bool initGameLayerToLayer(const char* level);

	CREATE_FUNC(ECGameScene);

	ECGameScene();
	~ECGameScene();

	virtual void ccTouchesBegan(CCSet* pTouches, CCEvent* pEvent);
	virtual void ccTouchesMoved(CCSet* pTouches, CCEvent* pEvent);
	virtual void ccTouchesEnded(CCSet* pTouches, CCEvent* pEvent);

private:

	static ECGameScene* _instanceOfGameLayer;

	CCSize _screenSize;

};	


#endif __GAME_SCENE_H__