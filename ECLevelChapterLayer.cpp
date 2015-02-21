#include "ECLevelChapterLayer.h"
#include "ECSceneManager.h"

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

bool ECLevelChapterLayer::InitWithBackgroundImage(const char* background_img_file_name, const int page_number)
{
	bool isSuccess = false;
	do
	{
		CC_BREAK_IF(!CCLayer::init());

		screen_size_ = CCDirector::sharedDirector()->getWinSize();

		CCSprite* background = CCSprite::createWithSpriteFrameName(background_img_file_name);

		if (page_number == 1) {
			background->setAnchorPoint(ccp(0.09433962264151f, 0.0f));
		} else {
			background->setAnchorPoint(ccp(0.0f, 0.0f));
		}

		this->addChild(background);

		isSuccess = true;
	}while(0);
	return isSuccess;
}
void ECLevelChapterLayer::CreateLevelSelectButtons(int row,											// rows
												   int col,											// columns
								                   int numberOrderFrom,								// counter start number 
												   std::vector<int>& v_open_levels,					// nonblocked levels 
												   std::map<int,int>& mp_played_with_stars_buttons) // level number and number of stars after completed
{
	float xOffset = 50;
	float yOffset = 50;

	float xDistance = 90;
	float yDistance = 59;

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
			button_normal = CCSprite::createWithSpriteFrameName("level_select_button_normal.png");
			button_selected = CCSprite::createWithSpriteFrameName("level_select_button_selected.png");
			button_blocked = CCSprite::createWithSpriteFrameName("level_select_button_blocked.png");

			button = CCMenuItemSprite::create(button_normal, button_selected, button_blocked,
																this,menu_selector(ECLevelChapterLayer::SelectLevel));
			button->setPosition(ccp(xOffset + xDistance * j, screen_size_.height - yOffset - yDistance * i));
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
	int currentNumber = numberOrderFrom;
	CCObject* obj = NULL;
	CCARRAY_FOREACH(this->getChildren(), obj)
	{
		if (CCLabelTTF* buttonNumber = dynamic_cast<CCLabelTTF*>(obj)) {
			buttonNumber->setString(CCString::createWithFormat("%i", currentNumber)->getCString());
			currentNumber++;

			std::vector<int>::iterator iterator;
			for (iterator = v_open_levels.begin(); iterator != v_open_levels.end(); ++iterator) {
				int val = (int)*iterator;
				if (val == currentNumber) {
					buttonNumber->setVisible(true);
				}
			}
		}
	}
	// setup button tags
	int currentTag = numberOrderFrom;
	CCObject* tagObj = NULL;
	CCARRAY_FOREACH(menu->getChildren(), tagObj)
	{
		if (CCMenuItemSprite* button = dynamic_cast<CCMenuItemSprite*>(tagObj)) {
			button->setTag(currentTag);
			currentTag++;

			// iterate throught open levels, if find then sets button property -> enabled.
			std::vector<int>::iterator iterator_open_levels;
			for (iterator_open_levels = v_open_levels.begin(); iterator_open_levels != v_open_levels.end(); ++iterator_open_levels) {
				int level_number = (int)*iterator_open_levels;
				if (level_number == currentTag) {
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
				if (level_number == currentTag) {
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
	if (CCMenuItemSprite* button = dynamic_cast<CCMenuItemSprite*>(sender)) {
		CCLOG("level%i", button->getTag());
		CCString* selectedLevel = CCString::createWithFormat("level%i", button->getTag());
		std::string selectedLevelStr;
		selectedLevelStr.assign(selectedLevel->getCString());
		ECSceneManager::GoGameSceneWithLevel(selectedLevelStr);
	}
}
