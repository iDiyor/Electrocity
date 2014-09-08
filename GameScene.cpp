#include "GameScene.h"

ECGameScene::ECGameScene()
{

}
ECGameScene::~ECGameScene()
{

}
CCScene* ECGameScene::scene()
{
	CCScene* _scene = NULL;
	do
	{
		_scene = CCScene::create();
		CC_BREAK_IF(!_scene);

		ECGameScene* _layer = ECGameScene::create();
		CC_BREAK_IF(!_layer);

		_scene->addChild(_layer);
	}while (0);
	return _scene;
}
bool ECGameScene::init()
{
	bool _isSuccess = false;
	do
	{
		CC_BREAK_IF(!CCLayer::init());



	}while(0);
	return _isSuccess;
}



