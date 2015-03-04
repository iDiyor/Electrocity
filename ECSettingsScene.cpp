#include "ECSettingsScene.h"
#include "ECSceneManager.h"

ECSettingsScene::ECSettingsScene(){

}

ECSettingsScene::~ECSettingsScene(){

}

CCScene* ECSettingsScene::scene() {
  CCScene* scene = NULL;
  do {
		scene = CCScene::create();
		CC_BREAK_IF(!scene);

		ECSettingsScene* layer = ECSettingsScene::create();
		CC_BREAK_IF(!layer);

		scene->addChild(layer);
    
  }while(0);
  return scene;
}

bool ECSettingsScene::init() {
  bool is_success = false;
  do {
		CC_BREAK_IF(!CCLayer::init());
    
		CCSize screen_size = CCDirector::sharedDirector()->getWinSize();
    

		CCTexture2D::PVRImagesHavePremultipliedAlpha(true);
		CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("settings_spritesheet.plist");
		CCSpriteBatchNode* settings_spritesheet = CCSpriteBatchNode::create("settings_spritesheet.pvr.ccz");
		this->addChild(settings_spritesheet);

		// background
		CCSprite* background_sprite = CCSprite::createWithSpriteFrameName("background.png");
		background_sprite->setAnchorPoint(ccp(0,0));
		settings_spritesheet->addChild(background_sprite);
		
		// about/credits board
		// moving down from top and going up
		about_board = CCSprite::createWithSpriteFrameName("credits.png");
		about_board->setAnchorPoint(ccp(0.5f, 1.0f));
		about_board->setPosition(ccp(screen_size.width * 0.5f, screen_size.height + about_board->getContentSize().height));
		settings_spritesheet->addChild(about_board);

		// settings name boards
		CCSprite* settings_name_board = CCSprite::createWithSpriteFrameName("settings_name.png");
		settings_name_board->setPosition(ccp(screen_size.width * 0.547f, screen_size.height * 0.277f));
		settings_spritesheet->addChild(settings_name_board);

		// music house
		CCSprite* music_button_sprite = CCSprite::createWithSpriteFrameName("music.png");
		CCSprite* music_button_selected_sprite = CCSprite::createWithSpriteFrameName("music_selected.png");
		CCMenuItemSprite* music_button_on = CCMenuItemSprite::create(music_button_sprite, NULL, NULL, NULL, NULL);
		CCMenuItemSprite* music_button_off = CCMenuItemSprite::create(music_button_selected_sprite, NULL, NULL, NULL, NULL);
		CCMenuItemToggle* music_button = CCMenuItemToggle::createWithTarget(this,
																			menu_selector(ECSettingsScene::OnMASSettingsChanged),
																			music_button_on,
																			music_button_off,
																			NULL);
		music_button->setPosition(ccp(screen_size.width * 0.32f, screen_size.height * 0.22f));
		music_button->setTag(T_MUSIC);

		// about house
		CCSprite* about_button_sprite = CCSprite::createWithSpriteFrameName("about.png");
		CCSprite* about_button_selected_sprite = CCSprite::createWithSpriteFrameName("about_selected.png");
		CCMenuItemSprite* about_button_on = CCMenuItemSprite::create(about_button_sprite, NULL, NULL, NULL, NULL);
		CCMenuItemSprite* about_button_off = CCMenuItemSprite::create(about_button_selected_sprite, NULL, NULL, NULL, NULL);
		CCMenuItemToggle* about_button = CCMenuItemToggle::createWithTarget(this,
																			menu_selector(ECSettingsScene::OnMASSettingsChanged),
																			about_button_on,
																			about_button_off,
																			NULL);
		about_button->setPosition(ccp(screen_size.width * 0.533f, screen_size.height * 0.3f));
		about_button->setTag(T_ABOUT);

		// sound house
		CCSprite* sound_button_sprite = CCSprite::createWithSpriteFrameName("sound.png");
		CCSprite* sound_button_selected_sprite = CCSprite::createWithSpriteFrameName("sound_selected.png");
		CCMenuItemSprite* sound_button_on = CCMenuItemSprite::create(sound_button_sprite, NULL, NULL, NULL, NULL);
		CCMenuItemSprite* sound_button_off = CCMenuItemSprite::create(sound_button_selected_sprite, NULL, NULL, NULL, NULL);
		CCMenuItemToggle* sound_button = CCMenuItemToggle::createWithTarget(this,
																			menu_selector(ECSettingsScene::OnMASSettingsChanged),
																			sound_button_on,
																			sound_button_off,
																			NULL);
		sound_button->setPosition(ccp(screen_size.width * 0.735f, screen_size.height * 0.268f));
		sound_button->setTag(T_SOUND);

		// main menu back button
		CCSprite* back_button_sprite = CCSprite::createWithSpriteFrameName("back.png");
		CCSprite* back_button_sprite_selected = CCSprite::createWithSpriteFrameName("back_selected.png");
		CCMenuItemSprite* back_button = CCMenuItemSprite::create(back_button_sprite, 
																 back_button_sprite_selected,	
																 NULL,
																 this,
																 menu_selector(ECSettingsScene::GoMainMenu));
		back_button->setPosition(ccp(back_button->getContentSize().width * 0.7f, back_button->getContentSize().height * 0.7f));
		CCMenu* menu = CCMenu::create(back_button, music_button, about_button, sound_button,NULL);
		menu->setPosition(ccp(0,0));
		this->addChild(menu);


		is_success = true;   

  }while(0);
  return is_success;
}
void ECSettingsScene::GoMainMenu(CCObject* sender) {
	ECSceneManager::GoMainMenuScene();	
}
void ECSettingsScene::OnMASSettingsChanged(CCObject* sender) {
	CCMenuItem* toggle_button = (CCMenuItemToggle*)sender;
	switch (toggle_button->getTag())
	{
	case T_MUSIC:
		break;
	case T_ABOUT:
		this->OnAboutClicked();
		break;
	case T_SOUND:
		break;
	}
}
void ECSettingsScene::OnAboutClicked() {
	CCMoveBy* move_down = CCMoveBy::create(0.3f, ccp(0, -about_board->getContentSize().height));
	about_board->runAction(move_down);
}
