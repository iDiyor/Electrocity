//  Copyright (c) 2015 Diyor Islomov. All rights reserved.
//	diyor.islomov@gmail.com || @iDiyor 
#include "ECLevelChapterLayer.h"
#include "ECSceneManager.h"
#include "ECAudioManager.h"

#define NUMBER_OF_LEVELS_ON_EACH_CHAPTER 15

ECLevelChapterLayer::ECLevelChapterLayer()
{

}
ECLevelChapterLayer::~ECLevelChapterLayer()
{

}
ECLevelChapterLayer* ECLevelChapterLayer::CreateWithBackgroundImage(const char* background_img_file_name, const int page_number)
{
	ECLevelChapterLayer* layer = new ECLevelChapterLayer();
	if (layer && layer->InitWithBackgroundImage(background_img_file_name, page_number)) {
		layer->autorelease();
		return layer;
	}
	else {
		delete layer;
		layer = NULL;
		return NULL;
	}
}

bool ECLevelChapterLayer::InitWithBackgroundImage(const char* background_img_filename, const int page_number)
{
	bool is_success = false;
	do
	{
		CC_BREAK_IF(!CCLayer::init());
		CCSize visible_size = CCDirector::sharedDirector()->getVisibleSize();
		CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();

		screen_size_ = CCSize(origin.x + visible_size.width, origin.y + visible_size.height);

		CCSprite* background = CCSprite::createWithSpriteFrameName(background_img_filename);

		if (page_number == 1) {
			background->setAnchorPoint(ccp(0.09433962264151f, 0.0f));
		} else {
			background->setAnchorPoint(ccp(0.0f, 0.0f));
		}

		this->addChild(background);

		is_success = true;
	}while(0);
	return is_success;
}
void ECLevelChapterLayer::CreateLevelSelectButtons(int row,											// rows
												   int col,											// columns
								                   int number_order_from,							// counter start number 
												   std::vector<int>& v_open_levels,					// nonblocked levels 
												   std::map<int,int>& mp_played_with_stars_buttons) // level number and number of stars after completion
{
	float x_offset = 50;
	float y_offset = 50;

	float x_distance = 90;
	float y_distance = 59;

	CCMenu* menu = CCMenu::create();
	menu->setPosition(ccp(0,0));
	this->addChild(menu);

	CCSprite* button_normal = NULL;
	CCSprite* button_selected = NULL;
	CCSprite* button_blocked = NULL;
	CCMenuItemSprite* button = NULL;

	CCLabelTTF* button_number_label = NULL;
	// align button sprites
	for (int i = 0; i < row; i++)
	{
		for (int j = 0; j < col; j++)
		{
			button_normal	= CCSprite::createWithSpriteFrameName("level_select_button_normal.png");
			button_selected = CCSprite::createWithSpriteFrameName("level_select_button_selected.png");
			button_blocked	= CCSprite::createWithSpriteFrameName("level_select_button_blocked.png");

			button = CCMenuItemSprite::create(button_normal, button_selected, button_blocked, this,menu_selector(ECLevelChapterLayer::SelectLevel));
			button->setPosition(ccp(x_offset + x_distance * j, screen_size_.height - y_offset - y_distance * i));
			button->setEnabled(false);
			menu->addChild(button);

			button_number_label = CCLabelTTF::create();
			//buttonNumber->setPosition(ccp(xOffset + xDistance * j, screen_size_.height - yOffset - yDistance * i));
			button_number_label->setVisible(false);
			button_number_label->setAnchorPoint(ccp(0.5f, 0.95f));
			button_number_label->setPosition(ccp(button->getPosition().x, button->getPosition().y));
			this->addChild(button_number_label);
		}
	}

	// create numbers label
	int current_number = number_order_from;
	CCObject* obj = NULL;
	CCARRAY_FOREACH(this->getChildren(), obj)
	{
		if (CCLabelTTF* buttonNumber = dynamic_cast<CCLabelTTF*>(obj)) {
			buttonNumber->setString(CCString::createWithFormat("%i", current_number)->getCString());
			current_number++;

			std::vector<int>::iterator iterator;
			for (iterator = v_open_levels.begin(); iterator != v_open_levels.end(); ++iterator) {
				int val = (int)*iterator;
				if (val == current_number) {
					buttonNumber->setVisible(true);
				}
			}
		}
	}
	// setup button tags
	int current_tag = number_order_from;
	CCObject* tag_obj = NULL;
	CCARRAY_FOREACH(menu->getChildren(), tag_obj)
	{
		if (CCMenuItemSprite* button = dynamic_cast<CCMenuItemSprite*>(tag_obj)) {
			button->setTag(current_tag);
			current_tag++;

			// iterate throught open levels, if find then sets button property -> enabled.
			std::vector<int>::iterator iterator_open_levels;
			for (iterator_open_levels = v_open_levels.begin(); iterator_open_levels != v_open_levels.end(); ++iterator_open_levels) {
				int level_number = (int)*iterator_open_levels;
				if (level_number == current_tag) {
					button->setEnabled(true);
				}
			}

			// stars on level button icon 
			CCSprite* stars_board_sprite = NULL;
			// iterate through played levels and mark level icon playd with attaching small board on it
			std::map<int,int>::iterator map_iterator;
			for (map_iterator = mp_played_with_stars_buttons.begin(); map_iterator != mp_played_with_stars_buttons.end(); ++map_iterator) {
				// level number and number of stars of that level when completed
				int level_number			  = map_iterator->first;
				int number_of_stars_for_level = map_iterator->second;
				if (level_number == current_tag) {
					switch (number_of_stars_for_level)
					{
						case 3:
							stars_board_sprite = CCSprite::createWithSpriteFrameName("three_star.png");
						break;
						case 2:
							stars_board_sprite = CCSprite::createWithSpriteFrameName("two_star.png");
						break;
						case 1:
							stars_board_sprite = CCSprite::createWithSpriteFrameName("one_star.png");
						break;
						case 0:
						continue; // if zero no stars
						break;
					} // switch statement
					stars_board_sprite->setPosition(ccp(button->getContentSize().width * 0.85f, button->getContentSize().height * 0.25f));
					button->addChild(stars_board_sprite);
				} // if statement
			} // for statement
		}
	}
}
void ECLevelChapterLayer::SelectLevel(CCObject* sender)
{
	ECAudioManager::PlayButtonClickSound(LEVEL_SELECT_SCENE_AUDIO);

	if (CCMenuItemSprite* button = dynamic_cast<CCMenuItemSprite*>(sender)) {
		CCLOG("level%i", button->getTag());
		CCString* selected_level = CCString::createWithFormat("level%i", button->getTag());
		std::string selected_level_string;
		selected_level_string.assign(selected_level->getCString());
		ECSceneManager::GoGameSceneWithLevel(selected_level_string);
	}
}
