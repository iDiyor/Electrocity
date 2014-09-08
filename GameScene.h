#ifndef __GAME_SCENE_H__
#define __GAME_SCENE_H__

#include "cocos2d.h"

using namespace	cocos2d;

class ECGameScene : public CCLayer
{
public:

	CCScene* scene();
	virtual bool init();

	CREATE_FUNC(ECGameScene);

	ECGameScene();
	~ECGameScene();

};	


#endif __GAME_SCENE_H__