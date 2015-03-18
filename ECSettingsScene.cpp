//  Copyright (c) 2015 Diyor Islomov. All rights reserved.
//	diyor.islomov@gmail.com || @iDiyor 
#include "ECSettingsScene.h"
#include "ECSceneManager.h"
#include "ECDataProvider.h"
#include "ECDataProviderExt.h"
#include "ECAudioManager.h"

ECSettingsScene::ECSettingsScene(){

}

ECSettingsScene::~ECSettingsScene(){
	
	/** 
	* removing cached sprite files
	* All scenes uses the same file name for background sprite - "background"
	* while switching from one scene to another, this name collision causes
	* sprite frame cache a problem not knowing which one to use.
	* Therefore, in destructor we removing previous spritesheet before loading new one
	* to prevent name collision.
	*/
	CCSpriteFrameCache::sharedSpriteFrameCache()->removeSpriteFramesFromFile("settings_spritesheet.plist");

	delete audio_manager_;
	audio_manager_ = NULL;
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
    
		screen_size_ = CCDirector::sharedDirector()->getWinSize();
    

		CCTexture2D::PVRImagesHavePremultipliedAlpha(true);
		CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("settings_spritesheet.plist");
		CCSpriteBatchNode* settings_spritesheet = CCSpriteBatchNode::create("settings_spritesheet.pvr.ccz");
		this->addChild(settings_spritesheet);

		// background
		CCSprite* background_sprite = CCSprite::createWithSpriteFrameName("settings_background.png");
		background_sprite->setAnchorPoint(ccp(0,0));
		settings_spritesheet->addChild(background_sprite);
		
		// about/credits board
		// moving down from top and going up
		about_board_ = CCSprite::createWithSpriteFrameName("credits.png");
		about_board_->setAnchorPoint(ccp(0.5f, 1.0f));
		about_board_->setPosition(ccp(screen_size_.width * 0.5f, screen_size_.height + about_board_->getContentSize().height));
		settings_spritesheet->addChild(about_board_);

		// settings name boards
		CCSprite* settings_name_board = CCSprite::createWithSpriteFrameName("settings_name.png");
		settings_name_board->setPosition(ccp(screen_size_.width * 0.547f, screen_size_.height * 0.277f));
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
		music_button->setPosition(ccp(screen_size_.width * 0.32f, screen_size_.height * 0.22f));
		music_button->setTag(T_MUSIC);
		ECDataProvider::GetSettingsParameter(MUSIC_SETTING) ? music_button->setSelectedIndex(1) : music_button->setSelectedIndex(0);

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
		about_button->setPosition(ccp(screen_size_.width * 0.533f, screen_size_.height * 0.3f));
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
		sound_button->setPosition(ccp(screen_size_.width * 0.735f, screen_size_.height * 0.268f));
		sound_button->setTag(T_SOUND);
		ECDataProvider::GetSettingsParameter(SOUND_SETTING) ? sound_button->setSelectedIndex(1) : sound_button->setSelectedIndex(0);

		// trash can
		CCSprite* trashcan_button_sprite = CCSprite::createWithSpriteFrameName("data_reset.png");
		CCSprite* trashcan_button_selected_sprite = CCSprite::createWithSpriteFrameName("data_reset_selected.png");
		CCMenuItemSprite* trashcan_button = CCMenuItemSprite::create(trashcan_button_sprite,
																	 trashcan_button_selected_sprite,
																	 NULL,
																	 this,
																	 menu_selector(ECSettingsScene::OnTrashcanClicked));
		trashcan_button->setPosition(ccp(screen_size_.width * 0.188f, screen_size_.height * 0.13f));

		// main menu back button
		CCSprite* back_button_sprite = CCSprite::createWithSpriteFrameName("back.png");
		CCSprite* back_button_sprite_selected = CCSprite::createWithSpriteFrameName("back_selected.png");
		CCMenuItemSprite* back_button = CCMenuItemSprite::create(back_button_sprite, 
																 back_button_sprite_selected,	
																 NULL,
																 this,
																 menu_selector(ECSettingsScene::GoMainMenu));
		back_button->setPosition(ccp(back_button->getContentSize().width * 0.7f, back_button->getContentSize().height * 0.7f));
		CCMenu* menu = CCMenu::create(back_button, music_button, about_button, sound_button, trashcan_button,NULL);
		menu->setPosition(ccp(0,0));
		this->addChild(menu);

		// audio manager
		audio_manager_ = ECAudioManager::CreateAudioManagerForScene(SETTINGS_SCENE_AUDIO);

		is_success = true;   

  }while(0);
  return is_success;
}
void ECSettingsScene::GoMainMenu(CCObject* sender) {
	// audio
	audio_manager_->PlayButtonClickSound();
	ECSceneManager::GoMainMenuSceneWithoutLoadingScene();	
}
void ECSettingsScene::OnMASSettingsChanged(CCObject* sender) {
	CCMenuItemToggle* toggle_button = (CCMenuItemToggle*)sender;

	switch (toggle_button->getTag())
	{
	case T_MUSIC:
		{
			// audio
			audio_manager_->PlayButtonClickSound();
			if (toggle_button->getSelectedIndex() == 0) {
				audio_manager_->MusicSetting(false);
			} else {
				audio_manager_->MusicSetting(true);
			}
		}
		break;
	case T_ABOUT:
		this->OnAboutClicked();
		break;
	case T_SOUND:
		{
			// audio
			audio_manager_->PlayButtonClickSound();
			if (toggle_button->getSelectedIndex() == 0) {
				audio_manager_->SoundSetting(false);
			} else {
				audio_manager_->SoundSetting(true);
			}
		}
		break;
	}
}
void ECSettingsScene::OnAboutClicked() {

	//CCLOG("Y_POS: %.1f", about_board_->getPosition().y);

	// audio
	audio_manager_->PlayButtonClickSound();

	CCMoveTo* move_animaton = NULL;
	CCPoint current_position = about_board_->getPosition();
	
	if (current_position.y > screen_size_.height) {
		move_animaton = CCMoveTo::create(0.3f, ccp(current_position.x, screen_size_.height));	
	} else {
		move_animaton = CCMoveTo::create(0.3f, ccp(current_position.x, screen_size_.height + about_board_->getContentSize().height));
	} 

	about_board_->runAction(move_animaton);	
}
void ECSettingsScene::OnTrashcanClicked(CCObject* sender) {

	// audio
	audio_manager_->PlayButtonClickSound();

	ECDataProviderExt* data_provider = new ECDataProviderExt("level_state.xml", "levels");
	// first level
	data_provider->SetPlayedAndStarsOnLevelButton("level1", false, 0);
	data_provider->SetBlockForLevel("level1", false);

	// levels : 2 - 60 : blocked and 0 stars
	for (int i = 2; i <= 60; i++) {
		CCString* level_name = CCString::createWithFormat("level%i", i);
		data_provider->SetPlayedAndStarsOnLevelButton(level_name->getCString(), false, 0);
		data_provider->SetBlockForLevel(level_name->getCString(), true);
		CCLOG("LEVEL: %s", level_name->getCString());
	}

	ECDataProvider::SetGeneralScore(0);

	// saves data in xml
	data_provider->SaveFile();
	delete data_provider;
	data_provider = NULL;
}
