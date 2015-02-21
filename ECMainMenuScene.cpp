#include "ECMainMenuScene.h"
#include "ECSceneManager.h"

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
	bool is_success = false;
	do
	{
		CC_BREAK_IF(!CCLayer::init());

		CCSize screen_size = CCDirector::sharedDirector()->getWinSize();

		// play button
		CCMenuItemFont* play_button = CCMenuItemFont::create("PLAY", this, menu_selector(ECMainMenuLayer::onButtonClicked));
		play_button->setPosition(ccp(screen_size.width * 0.5f, screen_size.height * 0.5f));
		play_button->setTag(kPlayButton);

		// settings button
		CCMenuItemFont* settings_button = CCMenuItemFont::create("Settings", this, menu_selector(ECMainMenuLayer::onButtonClicked));
		settings_button->setPosition(ccp(100,100));
		settings_button->setTag(kSettingsButton);

		// parent menu
		CCMenu* menu = CCMenu::create(play_button, settings_button, NULL);
		menu->setPosition(ccp(0,0));
		this->addChild(menu);

		is_success = true;
	}while(0);
	return is_success;
}
void ECMainMenuLayer::onButtonClicked(CCObject* pSender)
{
	CCMenuItem* button = (CCMenuItem*)(pSender);
	switch (button->getTag())
	{
	case kPlayButton:
		ECSceneManager::GoLevelSelectScene();
		break;
	case kSettingsButton:
		ECSceneManager::GoSettingsScene();
		break;
	default:
		ECSceneManager::GoLevelSelectScene();
		break;
	}
}

