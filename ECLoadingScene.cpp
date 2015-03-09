#include "ECLoadingScene.h"
#include "ECGameScene.h"
#include "ECMainMenuScene.h"
#include "ECLevelSelectLayer.h"
#include "ECSettingsScene.h"


CCScene* ECLoadingScene::LoadingWithTargetSceneWithArgs(TargetScene target_scene, std::string& args)
{
	CCScene* scene = NULL;
	do
	{
		scene = CCScene::create();
		CC_BREAK_IF(!scene);

		ECLoadingScene* layer = ECLoadingScene::CreateLoadingWithTargetSceneWithArgs(target_scene, args);
		CC_BREAK_IF(!layer);

		scene->addChild(layer);

	}while(0);
	return scene;
}
ECLoadingScene* ECLoadingScene::CreateLoadingWithTargetSceneWithArgs(TargetScene target_scene, std::string& args)
{
	ECLoadingScene* loadingLayer = new ECLoadingScene();
	if (loadingLayer && loadingLayer->InitLoadingWithTargetSceneWithArgs(target_scene, args)) {
		loadingLayer->autorelease();
		return loadingLayer;
	}
	else {
		delete loadingLayer;
		loadingLayer = NULL;
		return NULL;
	}
}

bool ECLoadingScene::InitLoadingWithTargetSceneWithArgs(TargetScene target_scene, std::string& args)
{
	bool isSuccess = false;
	do
	{
		CC_BREAK_IF(!CCLayer::init());

		target_scene_ = target_scene;
		
		args_.append(args);

		CCSize screen_size = CCDirector::sharedDirector()->getWinSize();

		CCLabelTTF* loading_label = CCLabelTTF::create("LOADING...", "ARIAL", 14);
		loading_label->setPosition(ccp(screen_size.width * 0.5f, screen_size.height * 0.5f));
		this->addChild(loading_label);

		this->scheduleUpdate();

		isSuccess = true;
	}while(0);
	return isSuccess;
}
void ECLoadingScene::update(float delta)
{
	this->unscheduleAllSelectors();

	switch (target_scene_)
	{
	case TARGET_SCENE_GAME_SCENE:
		CCDirector::sharedDirector()->replaceScene(ECGameScene::SceneWithGameLayerToLevel(args_));
		break;
	case TARGET_SCENE_MAIN_MENU_SCENE:
		CCDirector::sharedDirector()->replaceScene(ECMainMenuLayer::scene());
		break;
	case TARGET_SCENE_LEVEL_SELECT_SCENE:
		CCDirector::sharedDirector()->replaceScene(ECLevelSelectLayer::scene());
		break;
	case TARGET_SCENE_SETTINGS_SCENE: 
		{
			CCTransitionSlideInR* transition = CCTransitionSlideInR::create(0.3f, ECSettingsScene::scene());
			CCDirector::sharedDirector()->replaceScene(transition);

		}
		
		break;
	default:
		//
		//CCAssert(NULL, "%@ : unsupported target scene %i", this, _targetScene);
		CCLOG("UNSUPPORTED TARGET SCENE");
		break;
	}
}