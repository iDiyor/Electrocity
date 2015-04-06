//  Copyright (c) 2015 Diyor Islomov. All rights reserved.
//	diyor.islomov@gmail.com || @iDiyor 
#include "ECLevelSelectLayer.h"
#include "ECLevelChapterLayer.h"
#include "ECScrollView.h"
#include "ECDataProviderExt.h"
#include "ECDataProvider.h"
#include "ECSceneManager.h"
#include "ECAudioManager.h"

ECLevelSelectLayer::ECLevelSelectLayer() {

}

ECLevelSelectLayer::~ECLevelSelectLayer() {
	CCSpriteFrameCache::sharedSpriteFrameCache()->removeSpriteFramesFromFile("level_select_layer_spritesheet.plist");

	//delete audio_manager_;
	//audio_manager_ = NULL;
}

CCScene* ECLevelSelectLayer::scene()
{
	CCScene* scene = NULL;
	do
	{
		scene = CCScene::create();
		CC_BREAK_IF(!scene);

		ECLevelSelectLayer* layer = ECLevelSelectLayer::create();
		CC_BREAK_IF(!layer);

		scene->addChild(layer);

	}while(0);
	return scene;
}
bool ECLevelSelectLayer::init()
{
	bool is_success = false;
	do
	{
		CC_BREAK_IF(!CCLayer::init());

		CCSize visible_size = CCDirector::sharedDirector()->getVisibleSize();
		CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();

		screen_size_ = CCSize(origin.x + visible_size.width, origin.y + visible_size.height);

		CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("level_select_layer_spritesheet.plist");
		CCSpriteBatchNode* level_select_layer_spritesheet = CCSpriteBatchNode::create("level_select_layer_spritesheet.pvr.ccz");
		this->addChild(level_select_layer_spritesheet);

		// scroll view
		ECLevelChapterLayer* chapter_page_one   = ECLevelChapterLayer::CreateWithBackgroundImage("chapter_1.png", 1);
		ECLevelChapterLayer* chapter_page_two   = ECLevelChapterLayer::CreateWithBackgroundImage("chapter_2.png", 2);
		ECLevelChapterLayer* chapter_page_three = ECLevelChapterLayer::CreateWithBackgroundImage("chapter_3.png", 3);
		ECLevelChapterLayer* chapter_page_four  = ECLevelChapterLayer::CreateWithBackgroundImage("chapter_4.png", 4);

		ECDataProviderExt* data_provider = new ECDataProviderExt("level_state.xml", "levels");
		// containers
		std::vector<int> v_open_levels;
		std::map<int, int> mp_played_with_stars_buttons;
		// loading data and putting them in containers
		data_provider->LoadDataForLevelSelectButtons(v_open_levels, mp_played_with_stars_buttons);

		chapter_page_one->CreateLevelSelectButtons(3,5,1,v_open_levels, mp_played_with_stars_buttons);
		chapter_page_two->CreateLevelSelectButtons(3,5,16, v_open_levels, mp_played_with_stars_buttons);
		chapter_page_three->CreateLevelSelectButtons(3,5,31, v_open_levels, mp_played_with_stars_buttons);
		chapter_page_four->CreateLevelSelectButtons(3,5,46, v_open_levels,mp_played_with_stars_buttons);

		delete data_provider;
		data_provider = NULL;

		CCArray* level_chapters_array = new CCArray();
		level_chapters_array->addObject(chapter_page_one);
		level_chapters_array->addObject(chapter_page_two);
		level_chapters_array->addObject(chapter_page_three);
		level_chapters_array->addObject(chapter_page_four);

		ECScrollLayer* scroller = ECScrollLayer::createWithLayersWithOffset(level_chapters_array, 1);
		scroller->_pages_indicator_normal_color = ccc4(14,134,93,255);
		scroller->_pages_indicator_selected_color = ccc4(227,164,45,255);
		scroller->_pages_indicator_position = ccp(screen_size_.width * 0.5f, origin.y + 5);
		scroller->_margin_off_set = 60.0f;
		scroller->selectPage(0);
		this->addChild(scroller);

		//this->scheduleUpdate();
		// total score label
		CCString* total_score_string = CCString::createWithFormat("Score: %i", ECDataProvider::GetGeneralScore());
		CCLabelBMFont* total_score_label = CCLabelBMFont::create(total_score_string->getCString(), "general_font.fnt");
		total_score_label->setPosition(ccp(screen_size_.width - total_score_label->getContentSize().width * 0.6f, 
										   total_score_label->getContentSize().height * 1.2f));
		this->addChild(total_score_label);

		// main menu back button
		CCSprite* back_button_sprite		  = CCSprite::createWithSpriteFrameName("back.png");
		CCSprite* back_button_selected_sprite = CCSprite::createWithSpriteFrameName("back_selected.png");
		CCMenuItemSprite* back_button = CCMenuItemSprite::create(back_button_sprite, 
																 back_button_selected_sprite,	
																 NULL,
																 this,
																 menu_selector(ECLevelSelectLayer::GoMainMenu));
		back_button->setPosition(ccp(back_button->getContentSize().width * 0.7f, back_button->getContentSize().height * 1.2f));
		CCMenu* menu = CCMenu::create(back_button, NULL);
		menu->setPosition(ccp(0,0));
		this->addChild(menu);

		//audio
		//audio_manager_ = ECAudioManager::CreateAudioManagerForScene(LEVEL_SELECT_SCENE_AUDIO);

		is_success = true;
	}while(0);
	return is_success;
}
void ECLevelSelectLayer::update(float delta)
{

}

void ECLevelSelectLayer::scrollViewDidScroll(CCScrollView* view)
{

}
void ECLevelSelectLayer::scrollViewDidZoom(CCScrollView* view)
{

}
void ECLevelSelectLayer::GoMainMenu(CCObject* sender) {
	// audio
	ECAudioManager::PlayButtonClickSound(LEVEL_SELECT_SCENE_AUDIO);
	ECSceneManager::GoMainMenuScene();
}
