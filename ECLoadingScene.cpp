#include "ECLoadingScene.h"
#include "ECGameScene.h"
#include "ECMainMenuScene.h"
#include "ECLevelSelectLayer.h"
#include "ECSettingsScene.h"


CCScene* ECLoadingScene::loadingWithTargetSceneWithArgs(TARGET_SCENE targetScene, std::string& args)
{
	CCScene* scene = NULL;
	do
	{
		scene = CCScene::create();
		CC_BREAK_IF(!scene);

		ECLoadingScene* layer = ECLoadingScene::createLoadingWithTargetSceneWithArgs(targetScene, args);
		CC_BREAK_IF(!layer);

		scene->addChild(layer);

	}while(0);
	return scene;
}
ECLoadingScene* ECLoadingScene::createLoadingWithTargetSceneWithArgs(TARGET_SCENE targetScene, std::string& args)
{
	ECLoadingScene* loadingLayer = new ECLoadingScene();
	if (loadingLayer && loadingLayer->initLoadingWithTargetSceneWithArgs(targetScene, args)) {
		loadingLayer->autorelease();
		return loadingLayer;
	}
	else {
		delete loadingLayer;
		loadingLayer = NULL;
		return NULL;
	}
}

bool ECLoadingScene::initLoadingWithTargetSceneWithArgs(TARGET_SCENE targetScene, std::string& args)
{
	bool isSuccess = false;
	do
	{
		CC_BREAK_IF(!CCLayer::init());

		_targetScene = targetScene;
		
		_args.append(args);

		CCSize winSize = CCDirector::sharedDirector()->getWinSize();

		CCLabelTTF* loadingLabel = CCLabelTTF::create("LOADING...", "ARIAL", 14);
		loadingLabel->setPosition(ccp(winSize.width * 0.5f, winSize.height * 0.5f));
		this->addChild(loadingLabel);

		this->scheduleUpdate();

		isSuccess = true;
	}while(0);
	return isSuccess;
}
void ECLoadingScene::update(float delta)
{
	this->unscheduleAllSelectors();

	switch (_targetScene)
	{
	case TARGET_SCENE_GAME_SCENE:
		CCDirector::sharedDirector()->replaceScene(ECGameScene::sceneWithGameLayerToLevel(_args));
		break;
	case TARGET_SCENE_MAIN_MENU_SCENE:
		CCDirector::sharedDirector()->replaceScene(ECMainMenuLayer::scene());
		break;
	case TARGET_SCENE_LEVEL_SELECT_SCENE:
		CCDirector::sharedDirector()->replaceScene(ECLevelSelectLayer::scene());
		break;
	case TARGET_SCENE_SETTINGS_SCENE:
		CCDirector::sharedDirector()->replaceScene(ECSettingsScene::scene());
		break;
	default:
		//
		//CCAssert(NULL, "%@ : unsupported target scene %i", this, _targetScene);
		CCLOG("UNSUPPORTED TARGET SCENE");
		break;
	}
}