//  Copyright (c) 2015 Diyor Islomov. All rights reserved.
//	diyor.islomov@gmail.com || @iDiyor 
#include "ECMainMenuScene.h"
#include "ECSceneManager.h"
#include "ECDataProvider.h"
#include "ECDataProviderExt.h"
#include "ECAudioManager.h"

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

	//delete audio_manager_;
	//audio_manager_ = NULL;
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

		CCSize visible_size = CCDirector::sharedDirector()->getVisibleSize();
		CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();

		screen_size_ = CCSize(origin.x + visible_size.width, origin.y + visible_size.height);

		CCTexture2D::PVRImagesHavePremultipliedAlpha(true);
		// main menu spritesheet
		CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("main_menu_spritesheet.plist");
		CCSpriteBatchNode* main_menu_spritesheet = CCSpriteBatchNode::create("main_menu_spritesheet.pvr.ccz");
		this->addChild(main_menu_spritesheet);

		//background
		CCSprite* background = CCSprite::createWithSpriteFrameName("main_menu_background.png");
		background->setAnchorPoint(ccp(0,0));
		main_menu_spritesheet->addChild(background);

		// game label
		CCSprite* electrocity_title = CCSprite::createWithSpriteFrameName("game_title.png");
		electrocity_title->setPosition(ccp(screen_size_.width * 0.5f, screen_size_.height * 0.8f));
		main_menu_spritesheet->addChild(electrocity_title);
		CCMoveBy* move_by_down_electrocity_title = CCMoveBy::create(1.35f, ccp(0, electrocity_title->getContentSize().height * 0.075f * -1));
		CCMoveBy* move_by_up_electrocity_title = CCMoveBy::create(1.35f, ccp(0, electrocity_title->getContentSize().height * 0.075f));
		CCScaleTo* scale_to_up_electrocity_title = CCScaleTo::create(1.35f, 1.05f);
		CCScaleTo* scale_to_down_electrocity_title = CCScaleTo::create(1.35f, 1.0f);
		CCSpawn* spawn_move_down_and_scale_up = CCSpawn::create(move_by_down_electrocity_title, scale_to_up_electrocity_title, NULL);
		CCSpawn* spawn_move_up_and_scale_down = CCSpawn::create(move_by_up_electrocity_title, scale_to_down_electrocity_title, NULL);
		CCSequence* sequence_electrocity_titile = CCSequence::create(spawn_move_down_and_scale_up, spawn_move_up_and_scale_down, NULL);
		electrocity_title->runAction(CCRepeatForever::create(sequence_electrocity_titile));

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

		// leaderboards button
		CCSprite* leaderboards_button_sprite = CCSprite::createWithSpriteFrameName("leaderboards.png");
		CCSprite* leaderboards_button_selected_sprite = CCSprite::createWithSpriteFrameName("leaderboards.png");
		CCMenuItemSprite* leaderboards_button = CCMenuItemSprite::create(leaderboards_button_sprite,
																		 leaderboards_button_selected_sprite,
																		 NULL,
																		 this,
																		 menu_selector(ECMainMenuLayer::onButtonClicked));
		leaderboards_button->setPosition(ccp(screen_size_.width * 0.75f, screen_size_.height * 0.12f));
		leaderboards_button->setTag(T_LEADERBOARDS);

		// parent menu
		CCMenu* menu = CCMenu::create(play_button, settings_button, leaderboards_button, NULL);
		menu->setPosition(ccp(0,0));
		this->addChild(menu);

		// audio
		//audio_manager_ = ECAudioManager::CreateAudioManagerForScene(MAIN_MENU_SCENE_AUDIO);
		//audio_manager_->PlayBackgroundMusic();
		//ECAudioManager::PlayBackgroundMusic(GAME_SCENE_AUDIO);
		if (ECDataProvider::GetGameLaunchCounter() == 0) {
			ECAudioManager::MusicSetting(true, GAME_SCENE_AUDIO);
			ECAudioManager::SoundSetting(true);
			ECDataProvider::SetGameLaunchCounter(1);

			ECDataProviderExt::MoveXMLFileFromAssetsToWritabalePath("level_state.xml");
		}

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
		{
			ECAudioManager::PlayButtonClickSound(MAIN_MENU_SCENE_AUDIO);
			ECSceneManager::GoLevelSelectScene();
		}
		break;
	case T_SETTINGS:
		{
			ECAudioManager::PlayButtonClickSound(MAIN_MENU_SCENE_AUDIO);
			ECSceneManager::GoSettingsSceneWithoutLoadingScene();
		}
		break;
	case T_LEADERBOARDS:
		{
			ECAudioManager::PlayButtonClickSound(MAIN_MENU_SCENE_AUDIO);
			ECSceneManager::GoSettingsScene();
		}
		break;
	default:
		{
			ECAudioManager::PlayButtonClickSound(MAIN_MENU_SCENE_AUDIO);
			ECSceneManager::GoLevelSelectScene();
		}
		break;
	}
}

