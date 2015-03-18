//  Copyright (c) 2015 Diyor Islomov. All rights reserved.
//	diyor.islomov@gmail.com || @iDiyor 
#ifndef __EC_SCENE_MANAGER_H__
#define __EC_SCENE_MANAGER_H__

#include "cocos2d.h"

using namespace cocos2d;

class ECSceneManager 
{
public:

	static void GoGameSceneWithLevel(std::string& level);
	static void GoMainMenuScene();
	static void GoMainMenuSceneWithoutLoadingScene();
	static void GoLevelSelectScene();
	static void GoSettingsScene();
	static void GoSettingsSceneWithoutLoadingScene();
};

#endif //__EC_SCENE_MANAGER_H__