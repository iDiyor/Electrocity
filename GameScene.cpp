#include "GameScene.h"

#include "cocos-ext.h"

using namespace cocos2d::extension;

ECGameScene::ECGameScene()
{

}
ECGameScene::~ECGameScene()
{

}
CCScene* ECGameScene::sceneWithGameLayerToLevel(const char* level)
{
	CCScene* _scene = NULL;
	do
	{
		_scene = CCScene::create();
		CC_BREAK_IF(!_scene);

		ECGameScene* _layer = ECGameScene::createGameLayerToLevel(level);
		CC_BREAK_IF(!_layer);

		_scene->addChild(_layer);
	}while (0);
	return _scene;
}
ECGameScene* ECGameScene::createGameLayerToLevel(const char* level)
{
	ECGameScene* gameLayer = new ECGameScene();
	if (gameLayer && gameLayer->initGameLayerToLayer(level))
	{
		gameLayer->autorelease();
		return gameLayer;
	}
	else
	{
		delete gameLayer;
		gameLayer = NULL;
		return NULL;
	}
}
ECGameScene* ECGameScene::_instanceOfGameLayer = NULL;
ECGameScene* ECGameScene::sharedGameLayer()
{
	CC_ASSERT(_instanceOfGameLayer);
	return _instanceOfGameLayer;
}
bool ECGameScene::initGameLayerToLayer(const char* level)
{
	bool _isSuccess = false;
	do
	{
		CC_BREAK_IF(!CCLayer::init());

		_screenSize = CCDirector::sharedDirector()->getWinSize();

		CCNode* level = SceneReader::sharedSceneReader()->createNodeWithSceneFile("level1.json");

		this->addChild(level);

		_isSuccess = true;
	}while(0);
	return _isSuccess;
}

void ECGameScene::ccTouchesBegan(CCSet* pTouches, CCEvent* pEvent)
{

}
void ECGameScene::ccTouchesMoved(CCSet* pTouches, CCEvent* pEvent)
{

}
void ECGameScene::ccTouchesEnded(CCSet* pTouches, CCEvent* pEvent)
{

}




