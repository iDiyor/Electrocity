//  Copyright (c) 2015 Diyor Islomov. All rights reserved.
//	diyor.islomov@gmail.com || @iDiyor 
#include "ECMainMenuScene.h"
#include "ECSceneManager.h"
#include "ECDataProvider.h"
#include "ECDataProviderExt.h"
#include "ECAudioManager.h"
#include "NativeUtils.h"

ECMainMenuLayer::ECMainMenuLayer() :
	is_exit_dialog_shown(false)
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
		
		this->setKeypadEnabled(true);

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
		menu->setTag(T_PSL_MENU);
		this->addChild(menu);

		// audio
		//audio_manager_ = ECAudioManager::CreateAudioManagerForScene(MAIN_MENU_SCENE_AUDIO);
		//audio_manager_->PlayBackgroundMusic();
		//ECAudioManager::PlayBackgroundMusic(GAME_SCENE_AUDIO);
		if (ECDataProvider::GetGameLaunchCounter() == 0) {
			ECAudioManager::MusicSetting(true, GAME_SCENE_AUDIO);
			ECAudioManager::SoundSetting(true);
			ECDataProvider::SetGameLaunchCounter(1);
			ECDataProvider::SetChapterPage(0);

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
			NativeUtils::showLeaderboard(LEADERBOARD_HIGH_SCORE);
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
void ECMainMenuLayer::ShowExitDialog() {
	// audio
	ECAudioManager::PlayButtonClickSound(MAIN_MENU_SCENE_AUDIO);
	
	/*pthread_t thread_1;
	pthread_create(&thread_1, NULL, &ECSettingsScene::CallFromThread, NULL);
	pthread_join(thread_1, NULL);*/

	CCMenu* psl_menu = (CCMenu*)this->getChildByTag(T_PSL_MENU);
	psl_menu->setEnabled(false);

	// black shaded transparent background layer
	CCLayerColor* dialog_transparent_background_layer = CCLayerColor::create(ccc4(0,0,0,100));
	dialog_transparent_background_layer->setTag(T_TRANSPARENT_BLACK_LAYER);
	this->addChild(dialog_transparent_background_layer);
	
	CCSprite* game_exit_dialog = CCSprite::createWithSpriteFrameName("exit_game_dialog.png");
	game_exit_dialog->setPosition(ccp(screen_size_.width * 0.5f, screen_size_.height * 0.5f));
	dialog_transparent_background_layer->addChild(game_exit_dialog);

	CCSprite* dialog_yes_button = CCSprite::createWithSpriteFrameName("exit_game_dialog_yes.png");
	CCSprite* dialog_no_button = CCSprite::createWithSpriteFrameName("exit_game_dialog_no.png");

	CCMenuItemSprite* dialog_yes_button_item = CCMenuItemSprite::create(dialog_yes_button, dialog_yes_button, NULL, this, menu_selector(ECMainMenuLayer::OnExitDialogButtonClicked));
	CCMenuItemSprite* dialog_no_button_item = CCMenuItemSprite::create(dialog_no_button, dialog_no_button, NULL, this, menu_selector(ECMainMenuLayer::OnExitDialogButtonClicked));
	dialog_yes_button_item->setPosition(ccp(game_exit_dialog->getContentSize().width * 0.22f, game_exit_dialog->getContentSize().height * 0.10f * -1));
	dialog_no_button_item->setPosition(ccp(game_exit_dialog->getContentSize().width * 0.79f, game_exit_dialog->getContentSize().height * 0.10f * -1));
	dialog_yes_button_item->setTag(T_EXIT_DIALOG_YES);
	dialog_no_button_item->setTag(T_EXIT_DIALOG_NO);

	CCMenu* menu = CCMenu::create(dialog_yes_button_item, dialog_no_button_item, NULL);
	menu->setPosition(ccp(0,0));
	game_exit_dialog->addChild(menu);

	game_exit_dialog->setScale(0.1f);

	CCScaleTo* scale_up_2X = CCScaleTo::create(0.2f, 1.5f);
	CCMoveTo* move_up = CCMoveTo::create(0.2f, ccp(screen_size_.width * 0.5f, screen_size_.height * 0.6f));
	CCScaleTo* scale_up_1X = CCScaleTo::create(0.2f, 1.0f);
	CCMoveTo* move_down = CCMoveTo::create(0.2f, ccp(screen_size_.width * 0.5f, screen_size_.height * 0.5f));
	CCSpawn* spawn_1 = CCSpawn::create(move_up, scale_up_2X, NULL);
	CCSpawn* spawn_2 = CCSpawn::create(move_down, scale_up_1X, NULL);
	CCSequence* sequence = CCSequence::create(spawn_1, spawn_2, NULL);
	game_exit_dialog->runAction(sequence);
}

void ECMainMenuLayer::OnExitDialogButtonClicked(CCObject* sender) {
	// audio
	ECAudioManager::PlayButtonClickSound(SETTINGS_SCENE_AUDIO);

	CCMenuItem* item = (CCMenuItem*)sender;
	switch (item->getTag())
	{
	case T_EXIT_DIALOG_YES:
		{
			CCDirector::sharedDirector()->end();
		}
		break;
	case T_EXIT_DIALOG_NO:
		{
			this->removeChildByTag(T_TRANSPARENT_BLACK_LAYER);
			CCMenu* mas_menu = (CCMenu*)this->getChildByTag(T_PSL_MENU);
			mas_menu->setEnabled(true);
			is_exit_dialog_shown = false;
		}
		break;
	}
}

void ECMainMenuLayer::keyBackClicked() {
	if (!is_exit_dialog_shown) {
		ShowExitDialog();
		is_exit_dialog_shown = true;
	}

		
}


