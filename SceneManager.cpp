#include "SceneManager.h"
#include "GameScene.h"
#include "MainMenuScene.h"

void ECSceneManager::goGameSceneWithLevelLayer(const char* level)
{
	if (CCDirector::sharedDirector()->getRunningScene())
	{
		CCDirector::sharedDirector()->replaceScene(ECGameScene::sceneWithGameLayerToLevel(level));
	}
	else
	{
		CCDirector::sharedDirector()->runWithScene(ECGameScene::sceneWithGameLayerToLevel(level));
	}
}
void ECSceneManager::goMainMenuScene()
{
	if (CCDirector::sharedDirector()->getRunningScene())
	{
		CCDirector::sharedDirector()->replaceScene(ECMainMenuLayer::scene());
	}
	else
	{
		CCDirector::sharedDirector()->runWithScene(ECMainMenuLayer::scene());
	}
}