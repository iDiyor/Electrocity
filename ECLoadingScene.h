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

}TARGET_SCENE;

class ECLoadingScene : public CCLayer
{
public:

	static CCScene* loadingWithTargetSceneWithArgs(TARGET_SCENE targetScene, std::string& args);
	static ECLoadingScene* createLoadingWithTargetSceneWithArgs(TARGET_SCENE targetScene, std::string& args);
	bool initLoadingWithTargetSceneWithArgs(TARGET_SCENE targetScene, std::string& args);

	//static CCScene* loadingWithTargetScene(TARGET_SCENE targetScene);
	//static ECLoadingScene* createLoadingWithTargetScene(TARGET_SCENE targetScene);
	//bool initLoadingWithTargetScene(TARGET_SCENE targetScene);

	virtual void update(float delta);

private:
	TARGET_SCENE _targetScene;
	std::string _args;
};
#endif //__EC_LOADING_SCENE_H__