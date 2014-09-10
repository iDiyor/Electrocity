#ifndef __SCENE_MANAGER_H__
#define __SCENE_MANAGER_H__

#include "cocos2d.h"

using namespace cocos2d;

class ECSceneManager 
{
public:

	static void goGameSceneWithLevelLayer(const char* level);
	static void goMainMenuScene();

};

#endif //__SCENE_MANAGER_H__