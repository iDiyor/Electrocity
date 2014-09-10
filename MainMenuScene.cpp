#include "MainMenuScene.h"
#include "SceneManager.h"

ECMainMenuLayer::ECMainMenuLayer()
{

}
ECMainMenuLayer::~ECMainMenuLayer()
{

}
CCScene* ECMainMenuLayer::scene()
{
	CCScene* scene = NULL;
	do
	{
		scene = CCScene::create();
		CC_BREAK_IF(!scene);

		ECMainMenuLayer* layer = ECMainMenuLayer::create();
		CC_BREAK_IF(!layer);

		scene->addChild(layer);
	}while(0);
	return scene;

}
bool ECMainMenuLayer::init()
{
	bool _isSuccess = false;
	do
	{
		CC_BREAK_IF(!CCLayer::init());

		CCSize screenSize = CCDirector::sharedDirector()->getWinSize();

		CCMenuItemFont* playButton = CCMenuItemFont::create("PLAY", this, menu_selector(ECMainMenuLayer::onButtonClicked));
		playButton->setPosition(ccp(screenSize.width * 0.5f, screenSize.height * 0.5f));
		playButton->setTag(kPlayButton);

		CCMenu* menu = CCMenu::create(playButton, NULL);
		menu->setPosition(ccp(0,0));
		this->addChild(menu);

		_isSuccess = true;
	}while(0);
	return _isSuccess;
}
void ECMainMenuLayer::onButtonClicked(CCObject* pSender)
{
	CCMenuItem* button = (CCMenuItem*)(pSender);
	if (button->getTag() == kPlayButton) 
	{
		ECSceneManager::goGameSceneWithLevelLayer(NULL);
	}
}

