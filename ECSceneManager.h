#ifndef __EC_SCENE_MANAGER_H__
#define __EC_SCENE_MANAGER_H__

#include "cocos2d.h"

using namespace cocos2d;

class ECSceneManager 
{
public:

	static void GoGameSceneWithLevel(std::string& level);
	static void GoMainMenuScene();
	static void GoLevelSelectScene();
	static void GoSettingsScene();

};

#endif //__EC_SCENE_MANAGER_H__