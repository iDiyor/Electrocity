//  Copyright (c) 2015 Diyor Islomov. All rights reserved.
//	diyor.islomov@gmail.com || @iDiyor 
#ifndef __EC_LOADING_SCENE_H__
#define __EC_LOADING_SCENE_H__

#include "cocos2d.h"
#include <string>

using namespace cocos2d;

typedef enum
{
	TARGET_SCENE_INVALID = 1001,
	TARGET_SCENE_GAME_SCENE,
	TARGET_SCENE_MAIN_MENU_SCENE,
	TARGET_SCENE_LEVEL_SELECT_SCENE,
	TARGET_SCENE_SETTINGS_SCENE,

}TargetScene;

class ECLoadingScene : public CCLayer
{
public:

	static CCScene* LoadingWithTargetSceneWithArgs(TargetScene target_scene, std::string& args);
	static ECLoadingScene* CreateLoadingWithTargetSceneWithArgs(TargetScene target_scene, std::string& args);
	bool InitLoadingWithTargetSceneWithArgs(TargetScene target_scene, std::string& args);

	//static CCScene* loadingWithTargetScene(TARGET_SCENE targetScene);
	//static ECLoadingScene* createLoadingWithTargetScene(TARGET_SCENE targetScene);
	//bool initLoadingWithTargetScene(TARGET_SCENE targetScene);

	virtual void update(float delta);

private:
	TargetScene target_scene_;
	std::string args_;
};
#endif //__EC_LOADING_SCENE_H__