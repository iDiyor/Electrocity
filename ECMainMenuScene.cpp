#include "ECMainMenuScene.h"
#include "ECSceneManager.h"

ECMainMenuLayer::ECMainMenuLayer()
{

}
ECMainMenuLayer::~ECMainMenuLayer()
{
	/** 
	* removing cached sprite files
	* All scenes uses the same file name for background sprite - "background"
	* while switching from one scene to another, this name collision causes
	* sprite frame cache a problem not knowing which one to use.
	* Therefore, in destructor we removing previous spritesheet before loading new one
	* to prevent name collision.
	*/
	CCSpriteFrameCache::sharedSpriteFrameCache()->removeSpriteFramesFromFile("main_menu_spritesheet.plist");
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

		screen_size_ = CCDirector::sharedDirector()->getWinSize();

		CCTexture2D::PVRImagesHavePremultipliedAlpha(true);
		CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("main_menu_spritesheet.plist");
		CCSpriteBatchNode* main_menu_spritesheet = CCSpriteBatchNode::create("main_menu_spritesheet.pvr.ccz");
		this->addChild(main_menu_spritesheet);

		//background
		CCSprite* background = CCSprite::createWithSpriteFrameName("background.png");
		background->setAnchorPoint(ccp(0,0));
		main_menu_spritesheet->addChild(background);

		// play button
		CCSprite* play_button_sprite = CCSprite::createWithSpriteFrameName("play.png");
		CCSprite* play_button_selected_sprite = CCSprite::createWithSpriteFrameName("play_selected.png");
		CCMenuItemSprite* play_button = CCMenuItemSprite::create(play_button_sprite,
																 play_button_selected_sprite,
																 NULL,
																 this,
																 menu_selector(ECMainMenuLayer::onButtonClicked));
		play_button->setPosition(ccp(screen_size_.width * 0.5f, screen_size_.height * 0.5f));
		play_button->setTag(T_PLAY);

		
		// settings button
		CCSprite* settings_button_sprite = CCSprite::createWithSpriteFrameName("option.png");
		CCSprite* settings_button_selected_sprite = CCSprite::createWithSpriteFrameName("option_selected.png");
		CCMenuItemSprite* settings_button = CCMenuItemSprite::create(settings_button_sprite,
																	 settings_button_selected_sprite,
																	 NULL,
																     this,
																	 menu_selector(ECMainMenuLayer::onButtonClicked));
		settings_button->setPosition(ccp(screen_size_.width * 0.9f, screen_size_.height * 0.15f));
		settings_button->setTag(T_SETTINGS);

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
	case T_PLAY:
		ECSceneManager::GoLevelSelectScene();
		break;
	case T_SETTINGS:
		ECSceneManager::GoSettingsScene();
		break;
	default:
		ECSceneManager::GoLevelSelectScene();
		break;
	}
}

