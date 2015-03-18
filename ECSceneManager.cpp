//  Copyright (c) 2015 Diyor Islomov. All rights reserved.
//	diyor.islomov@gmail.com || @iDiyor 
#include "ECSceneManager.h"
//#include "ECGameScene.h"
//#include "ECMainMenuScene.h"
//#include "ECLevelSelectLayer.h"
#include "ECLoadingScene.h"
#include "ECMainMenuScene.h"
#include "ECSettingsScene.h"

void ECSceneManager::GoGameSceneWithLevel(std::string& level)
{
	if (CCDirector::sharedDirector()->getRunningScene()) {
		//CCDirector::sharedDirector()->replaceScene(ECGameScene::sceneWithGameLayerToLevel(level));
		CCDirector::sharedDirector()->replaceScene(ECLoadingScene::LoadingWithTargetSceneWithArgs(TARGET_SCENE_GAME_SCENE, level));
	}
	else {
		//CCDirector::sharedDirector()->runWithScene(ECGameScene::sceneWithGameLayerToLevel(level));
		CCDirector::sharedDirector()->runWithScene(ECLoadingScene::LoadingWithTargetSceneWithArgs(TARGET_SCENE_GAME_SCENE, level));
	}
}
void ECSceneManager::GoMainMenuScene()
{
	std::string nullStr = "0";
	if (CCDirector::sharedDirector()->getRunningScene()) {
		//CCDirector::sharedDirector()->replaceScene(ECMainMenuLayer::scene());
		CCDirector::sharedDirector()->replaceScene(ECLoadingScene::LoadingWithTargetSceneWithArgs(TARGET_SCENE_MAIN_MENU_SCENE,	nullStr));
	}
	else {
		//CCDirector::sharedDirector()->runWithScene(ECMainMenuLayer::scene());
		CCDirector::sharedDirector()->runWithScene(ECLoadingScene::LoadingWithTargetSceneWithArgs(TARGET_SCENE_MAIN_MENU_SCENE, nullStr));
	}
}
void ECSceneManager::GoMainMenuSceneWithoutLoadingScene() {
	if (CCDirector::sharedDirector()->getRunningScene()) {
		//CCDirector::sharedDirector()->replaceScene(ECLevelSelectLayer::scene());
		CCTransitionSlideInL* transition = CCTransitionSlideInL::create(0.3f, ECMainMenuLayer::scene());
		CCDirector::sharedDirector()->replaceScene(transition);
	}
	else {
	//	//CCDirector::sharedDirector()->runWithScene(ECLevelSelectLayer::scene());
	//	//CCDirector::sharedDirector()->runWithScene(ECLoadingScene::loadingWithTargetSceneWithArgs(TARGET_SCENE_SETTINGS_SCENE, nullStr));
	//	
		CCTransitionSlideInL* transition = CCTransitionSlideInL::create(0.3f, ECMainMenuLayer::scene());
		CCDirector::sharedDirector()->runWithScene(transition);
	}
}

void ECSceneManager::GoLevelSelectScene()
{
	std::string nullStr = "0";
	if (CCDirector::sharedDirector()->getRunningScene()) {
		//CCDirector::sharedDirector()->replaceScene(ECLevelSelectLayer::scene());
		CCDirector::sharedDirector()->replaceScene(ECLoadingScene::LoadingWithTargetSceneWithArgs(TARGET_SCENE_LEVEL_SELECT_SCENE, nullStr));
	}
	else {
		//CCDirector::sharedDirector()->runWithScene(ECLevelSelectLayer::scene());
		CCDirector::sharedDirector()->runWithScene(ECLoadingScene::LoadingWithTargetSceneWithArgs(TARGET_SCENE_LEVEL_SELECT_SCENE, nullStr));
	}
}
void ECSceneManager::GoSettingsScene() {
	std::string nullStr = "0";
	if (CCDirector::sharedDirector()->getRunningScene()) {
		//CCDirector::sharedDirector()->replaceScene(ECLevelSelectLayer::scene());
		CCDirector::sharedDirector()->replaceScene(ECLoadingScene::LoadingWithTargetSceneWithArgs(TARGET_SCENE_SETTINGS_SCENE, nullStr));
	}
	else {
		//CCDirector::sharedDirector()->runWithScene(ECLevelSelectLayer::scene());
		CCDirector::sharedDirector()->runWithScene(ECLoadingScene::LoadingWithTargetSceneWithArgs(TARGET_SCENE_SETTINGS_SCENE, nullStr));
	}
}
void ECSceneManager::GoSettingsSceneWithoutLoadingScene() {
	if (CCDirector::sharedDirector()->getRunningScene()) {
		//CCDirector::sharedDirector()->replaceScene(ECLevelSelectLayer::scene());
		CCTransitionSlideInR* transition = CCTransitionSlideInR::create(0.3f, ECSettingsScene::scene());
		CCDirector::sharedDirector()->replaceScene(transition);
	}
	else {
	//	//CCDirector::sharedDirector()->runWithScene(ECLevelSelectLayer::scene());
	//	//CCDirector::sharedDirector()->runWithScene(ECLoadingScene::loadingWithTargetSceneWithArgs(TARGET_SCENE_SETTINGS_SCENE, nullStr));
	//	
		CCTransitionSlideInR* transition = CCTransitionSlideInR::create(0.3f, ECSettingsScene::scene());
		CCDirector::sharedDirector()->runWithScene(transition);
	}
}
