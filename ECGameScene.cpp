//  Copyright (c) 2015 Diyor Islomov. All rights reserved.
//	diyor.islomov@gmail.com || @iDiyor 
#include "ECGameScene.h"
#include "ECTower.h"
#include "ECBuilding.h"
#include "ECLine.h"
#include "ECDataProviderExt.h"
#include "ECSceneManager.h"
#include "ECDataProvider.h"
#include "ECAudioManager.h"

#include <cstdlib>

#define MAX_NUMBER_OF_LEVELS_IN_GAME 60
#define MIN_SCORE_FOR_EACH_LEVEL 2000
#define MAX_SCORE_FOR_EACH_LEVEL 4000
#define NUMBER_OF_LEVELS_IN_EACH_CHAPTER 15

#define BEST_TIME_KEY "BEST_TIME"
#define BEST_SCORE_KEY "BEST_SCORE"

ECGameScene::ECGameScene() :
	game_layer_spritesheet_(NULL),
	game_level_data_(NULL),
	current_level_(""),
	game_timer_(0),
	temp_label_(NULL),
	temp_final_game_score_(0),
	on_light_building_counter_(0)
{

}
ECGameScene::~ECGameScene()
{
	delete game_level_data_;
	game_level_data_ = NULL;

	// removing cached sprite files
	CCSpriteFrameCache::sharedSpriteFrameCache()->removeSpriteFramesFromFile("game_layer_spritesheet.plist");

	// audio
	//delete audio_manager_;
	//audio_manager_ = NULL;
}
CCScene* ECGameScene::SceneWithGameLayerToLevel(std::string& level)
{
	CCScene* _scene = NULL;
	do
	{
		_scene = CCScene::create();
		CC_BREAK_IF(!_scene);

		ECGameScene* _layer = ECGameScene::CreateGameLayerToLevel(level);
		CC_BREAK_IF(!_layer);

		_scene->addChild(_layer);
	}while (0);
	return _scene;
}
ECGameScene* ECGameScene::CreateGameLayerToLevel(std::string& level)
{
	ECGameScene* gameLayer = new ECGameScene();
	if (gameLayer && gameLayer->InitGameLayerToLayer(level))
	{
		gameLayer->autorelease();
		return gameLayer;
	}
	else
	{
		delete gameLayer;
		gameLayer = NULL;
		return NULL;
	}
}
ECGameScene* ECGameScene::instance_of_gamelayer_ = NULL;
ECGameScene* ECGameScene::SharedGameLayer()
{
	CC_ASSERT(instance_of_gamelayer_);
	return instance_of_gamelayer_;
}
bool ECGameScene::InitGameLayerToLayer(std::string& level)
{
	bool is_success = false;
	do
	{
		CC_BREAK_IF(!CCLayer::init());
		this->setTouchEnabled(true);
		this->setKeypadEnabled(true);

		instance_of_gamelayer_ = this;

		CCSize visible_size = CCDirector::sharedDirector()->getVisibleSize();
		CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();

		screen_size_ = CCSize(origin.x + visible_size.width, origin.y + visible_size.height);

		CCTexture2D::PVRImagesHavePremultipliedAlpha(true);
		// spritesheet
		CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("game_layer_spritesheet.plist");
		game_layer_spritesheet_ = CCSpriteBatchNode::create("game_layer_spritesheet.pvr.ccz");
		this->addChild(game_layer_spritesheet_);

		// background
		CCSprite* background = CCSprite::createWithSpriteFrameName("bg.png");
		background->setPosition(ccp(screen_size_.width * 0.5f, screen_size_.height * 0.5f));
		game_layer_spritesheet_->addChild(background, Z_BACKGROUND);
		
		// ui elements in game - pause, restart
		// pause
		CCSprite* game_pause_button_sprite = CCSprite::createWithSpriteFrameName("pause_button.png");
		CCSprite* game_pause_button_sprite_selected = CCSprite::createWithSpriteFrameName("pause_button_selected.png");
		CCMenuItemSprite* game_pause_button = CCMenuItemSprite::create(game_pause_button_sprite,
																	game_pause_button_sprite_selected,
																	 NULL,
																	 this,
																	 menu_selector(ECGameScene::PauseGame));
		game_pause_button->setPosition(ccp(screen_size_.width - game_pause_button->getContentSize().width * 0.5f - 2,
										   screen_size_.height - game_pause_button->getContentSize().height * 0.5f - 2));

		// restart
		CCSprite* game_restart_button_sprite = CCSprite::createWithSpriteFrameName("restart_button.png");
		CCSprite* game_restart_button_sprite_selected = CCSprite::createWithSpriteFrameName("restart_button_selected.png");
		CCMenuItemSprite* game_restart_button = CCMenuItemSprite::create(game_restart_button_sprite,
																	  game_restart_button_sprite_selected,
																	   NULL,
																	   this,
																	   menu_selector(ECGameScene::RestartGame));
		game_restart_button->setPosition(ccp(game_restart_button->getPosition().x - game_restart_button->getContentSize().width - 5,
											game_restart_button->getPosition().y));

		CCMenu* game_ui_control_menu = CCMenu::create(game_pause_button, game_restart_button, NULL);
		game_ui_control_menu->setPosition(0, 0);
		this->addChild(game_ui_control_menu, Z_UI_LABELS, T_PAUSE_RESTART_MENU);


		// loading level data
		game_level_data_ = new ECDataProviderExt("data4.xml", "levels", level.c_str());
		// set up level
		this->LoadGameDataForLevel(level.c_str());
		// seting current level
		current_level_.assign(level);

		// labels - fonts
		// current level
		int current_level_number = GetCurrentLevelNumber();
		CCString* current_level_string = CCString::createWithFormat("Level %i", current_level_number);
		CCLabelBMFont* current_level_Label = CCLabelBMFont::create(current_level_string->getCString(), "general_font.fnt");
		current_level_Label->setPosition(ccp(screen_size_.width - current_level_Label->getContentSize().width * 0.6f ,
											screen_size_.height - visible_size.height + current_level_Label->getContentSize().height * 0.5f));
		this->addChild(current_level_Label);

		// timer label
		CCLabelBMFont* level_time_label = CCLabelBMFont::create("Time: 0", "general_font.fnt");
		level_time_label->setAnchorPoint(ccp(0.0f, 0.5f));
		level_time_label->setPosition(ccp(level_time_label->getContentSize().width * 0.05f,
										screen_size_.height - level_time_label->getContentSize().height * 0.6f));
		this->addChild(level_time_label, Z_UI_LABELS, T_LEVEL_TIME_LABEL);

		// best time label
		float best_time = ECDataProvider::GetBestTimeForLevel(current_level_.c_str());
		CCString* best_time_string = CCString::createWithFormat("Best time: %.1f", best_time);
		CCLabelBMFont* level_best_time_label = CCLabelBMFont::create(best_time_string->getCString(), "general_font.fnt");
		level_best_time_label->setAnchorPoint(ccp(0.0f, 0.5f));
		level_best_time_label->setPosition(ccpSub(level_time_label->getPosition(), ccp(0, level_best_time_label->getContentSize().height)));
		this->addChild(level_best_time_label, Z_UI_LABELS, T_LEVEL_BEST_TIME_LABEL);

		// building lights on counter
		CCLabelBMFont* lights_counter_label = CCLabelBMFont::create(CCString::createWithFormat("0 | %i", buildings_.size())->getCString(), "general_font.fnt");
		lights_counter_label->setPosition(ccp(screen_size_.width * 0.5f, level_time_label->getPosition().y));
		this->addChild(lights_counter_label, Z_UI_LABELS, T_LEVEL_LIGHTS_COUNTER);


		// audio
		//audio_manager_ = ECAudioManager::CreateAudioManagerForScene(GAME_SCENE_AUDIO);
		//ECAudioManager::PlayBackgroundMusic(GAME_SCENE_AUDIO);

		this->scheduleUpdate();

		is_success = true;
	}while(0);
	return is_success;
}
void ECGameScene::update(float delta)
{
	// loop through towers and find moving ones then update line
	for (unsigned int i = 0; i < towers_.size(); i++)
	{
		ECTower* tower = static_cast<ECTower*>(towers_.at(i));
		if (tower->GetTowerState() == ON_TOWER_TOUCH_MOVED || tower->GetTowerState() == ON_TOWER_TOUCH_ENDED)
		{
			this->ResetLines();

			if (this->CheckForLineAndBuildingCollision())
				break;

			//this->CheckForTowerAndBuildingCollision(tower);
		}
	}
	
	//this->ResetLines();
	//this->CheckForCollision();

	// check for win state
	//this->CheckForWinState();

	// timer, updating game timer label
	game_timer_ += delta;
	CCLabelBMFont* gameTimerLabel = (CCLabelBMFont*)this->getChildByTag(T_LEVEL_TIME_LABEL);
	gameTimerLabel->setString(CCString::createWithFormat("Time: %.1f", game_timer_)->getCString());

}
void ECGameScene::LoadGameDataForLevel(const char* level)
{
	//load specific node(building,tower) attributes -> call methods with args
	std::map<std::string, int> mp_node_attributes;

	//variables for each attributes
	char image_filename[20];
	int node_attribute_image_number = 0;
	int node_attribute_quantity		= 0;
	float node_attribute_scale		= 0;

	// loading building attributes | assigning attributes
	game_level_data_->LoadNodeAttributesForNodeType(TYPE_BUILDING, &node_attribute_image_number, 
																   &node_attribute_quantity, 
																   &node_attribute_scale);
	// building full image file name with sprite number
	std::sprintf(image_filename, "house_off_%i.png", node_attribute_image_number);
	//creating buildings according to attributes
	this->CreateBuildingInQuant(node_attribute_quantity, image_filename, node_attribute_scale);

	//clear for next use
	mp_node_attributes.clear();

	// loading towers attributes | assigning attributes
	game_level_data_->LoadNodeAttributesForNodeType(TYPE_TOWER, &node_attribute_image_number, 
																&node_attribute_quantity, 
																&node_attribute_scale);
	// building full image file name with sprite number
	std::sprintf(image_filename, "tower_%i.png", node_attribute_image_number);
	//creating buildings according to attributes
	this->CreateTowerInQuant(node_attribute_quantity, image_filename, node_attribute_scale);

	//creating lines
	bool is_lines_loop = game_level_data_->LoadLinesLoopAttribute();
	this->CreateLines(is_lines_loop);

	//clear for next use
	mp_node_attributes.clear();
}
void ECGameScene::CreateTowerInQuant(const int quantity, const char* image_filename, const float scale)
{
	for (unsigned int i = 0; i < quantity; i++)
	{
		ECTower* tower = ECTower::CreateTowerWithFileName(image_filename);
		tower->setScale(scale);
		game_layer_spritesheet_->addChild(tower, Z_TOWERS);
		towers_.push_back(tower);
	}
	this->ResetGameObjectsOfType(TYPE_TOWER);
}
void ECGameScene::CreateBuildingInQuant(const int quantity, const char* image_filename, const float scale)
{
	for (unsigned int i = 0; i < quantity; i++)
	{
		ECBuilding* building = ECBuilding::CreateBuildingWithFileName(image_filename);
		building->setScale(scale);
		game_layer_spritesheet_->addChild(building, Z_BUILDING);
		buildings_.push_back(building);
	}
	this->ResetGameObjectsOfType(TYPE_BUILDING);
}
void ECGameScene::ResetGameObjectsOfType(NodeType node)
{
	std::vector<CCPoint>	v_node_positions;
	std::vector<CCSprite*>	v_nodes;

	switch (node)
	{
	case TYPE_BUILDING:
		{
			game_level_data_->LoadPositionXYOfBuildings(v_node_positions);
			v_nodes = buildings_;
		}
		break;
	case TYPE_TOWER:
		{
			game_level_data_->LoadPositionXYOfTowers(v_node_positions);
			v_nodes = towers_;
		}
		break;
	}

	// loop pos and node
	for (unsigned int i = 0; i < v_nodes.size(); i++)
	{
		CCSprite* node = dynamic_cast<CCSprite*>(v_nodes.at(i));
		CCPoint position_to_set = (CCPoint)v_node_positions.at(i);
		node->setPosition(position_to_set);
	}

	v_node_positions.clear();
	v_nodes.clear();
}
void ECGameScene::CreateLines(bool is_lines_loop)
{
	int lines_quantity = 0;
	if (!is_lines_loop) {
		lines_quantity = towers_.size() - 1;
	} else {
		lines_quantity = towers_.size();
	}

	for (unsigned int i = 0; i < lines_quantity; i++)
	{
		ECLine* line = ECLine::create();
		game_layer_spritesheet_->addChild(line, Z_LINES);
		lines_.push_back(line);
	}
	this->ResetLines();
}
void ECGameScene::ResetLines()
{
	int counter_a = 0;
	int counter_b = 0;

	for (unsigned int i = 0; i < towers_.size(); i++)
	{
		if (i == towers_.size() - 1) //if i points to the last tower in the array
		{
			counter_a = 0; //first tower in array
			counter_b = i; //last tower in array
		}
		else
		{
			counter_a = i;		//couple first
			counter_b = 1 + i;	// couple second
		}

		ECTower* tower_a = dynamic_cast<ECTower*>(towers_.at(counter_a));
		ECTower* tower_b = dynamic_cast<ECTower*>(towers_.at(counter_b));
		ECLine* line = NULL;

		// i must not point to a greater number than lines` number in the array
		if (i < lines_.size())
		{
			line = dynamic_cast<ECLine*>(lines_.at(i));
		}

		if (tower_a && tower_b && line)
		{
			// updating line between two towers | position & scaleX
			line->ResetLineBetweenTowers(tower_a, tower_b);
		}
	}
}
bool ECGameScene::CheckForLineAndBuildingCollision()
{
	ECLine* line = NULL;
	ECBuilding* building = NULL;
	for (int i = 0; i < lines_.size(); i++) {
		line = dynamic_cast<ECLine*>(lines_.at(i));
		for (int j = 0; j < buildings_.size(); j++) {
			building = dynamic_cast<ECBuilding*>(buildings_.at(j));

			if (line->CheckCollisionWithBuilding(building) && building->GetContactState() == NO_CONTACT) {
				building->SetContactedLineIndex(i);
				building->SetContactState(YES_CONTACT);
				building->SetBuildingLight(LIGHT_ON);
				on_light_building_counter_++;
			}
			else if (!line->CheckCollisionWithBuilding(building) && building->GetContactedLineIndex() == i) {
				building->SetContactedLineIndex(-1);
				building->SetContactState(NO_CONTACT);
				building->SetBuildingLight(LIGHT_OFF);
				on_light_building_counter_--;
			}
		}
	}

	// check for win state inside collision
	//CheckForWinState();

	// update on lights counter
	CCLabelBMFont* on_light_counter_label = (CCLabelBMFont*)this->getChildByTag(T_LEVEL_LIGHTS_COUNTER);
	on_light_counter_label->setString(CCString::createWithFormat("%i | %i", on_light_building_counter_, buildings_.size())->getCString());
	
	return CheckForWinState();
}
bool ECGameScene::CheckForTowerAndBuildingCollision(ECTower* tower) {
	ECBuilding* building = NULL;
	for (int i = 0; i < buildings_.size(); i++) {
		building = dynamic_cast<ECBuilding*>(buildings_.at(i));
		if (tower->CheckCollisionWithBuilding(building)) {
			building->setColor(ccc3(255,0,0));
			return true;
		} else {
			building->setColor(ccc3(255,255,255));
		}
	}
	return false;
}

bool ECGameScene::CheckForWinState() {
	
	// win situation
	if (on_light_building_counter_ == buildings_.size()) {
		CCLOG("WIN");
		// unschedule update, disable touch on towers
		this->PauseGameLoop(true);
		CCCallFunc* call_pause_game_function = CCCallFunc::create(this, callfunc_selector(ECGameScene::WinGame));
		// delay to show win sprites, just for effect
		CCDelayTime* delay_before_pause_game_function_call = CCDelayTime::create(0.5f);
		CCSequence* sequence = CCSequence::create(delay_before_pause_game_function_call, call_pause_game_function, NULL);
		this->runAction(sequence);
		return true;
	}
	return false;
		//CCLOG("COUNTER: %i", _gameLightsCounter);
}
void ECGameScene::PauseGame(CCObject* pSender) {
	CCLOG("PAUSE BUTTON");

	// click
	ECAudioManager::PlayButtonClickSound(GAME_SCENE_AUDIO);

	// pause the loop | update method.
	this->PauseGameLoop(true);

	// black shaded transparent background layer
	CCLayerColor* pauseShadeTransparentBackgroundLayer = CCLayerColor::create(ccc4(0,0,0,100));
	this->addChild(pauseShadeTransparentBackgroundLayer, Z_PAUSE_BLACK_LAYER, T_SHADE_PAUSE_BACKGROUND_LAYER);

	// stick
	CCSprite* stick = CCSprite::createWithSpriteFrameName("stick.png");
	stick->setAnchorPoint(ccp(0.5f, 0.0f));
	stick->setPosition(ccp(screen_size_.width * 0.5f, -stick->getContentSize().height));
	this->addChild(stick, Z_PAUSE_MENU, T_STICK); // cannot add to spriteSheet, because shaded background sit on top of stick
	
	/***************************
	***********MENU************
	*		|CONTINUE|
	*		|SKIP LVL|
	*		|LVL SELE|
	*		|MAIN MEN|
	*****************************/
	
	// continue
	CCSprite* continueButtonSprite = CCSprite::createWithSpriteFrameName("continue_button.png");
	CCSprite* continueButtonSpriteSelected = CCSprite::createWithSpriteFrameName("continue_button_selected.png");
	CCMenuItemSprite* continueButton = CCMenuItemSprite::create(continueButtonSprite,
																continueButtonSpriteSelected,
																NULL,
																this,
																menu_selector(ECGameScene::PauseMenuItemClicked));
	continueButton->setTag(T_CONTINUE);
	continueButton->setPosition(ccp(stick->getPosition().x, stick->getPosition().y + stick->getContentSize().height - continueButton->getContentSize().height * 0.6f));
	// skip level
	CCSprite* skipLevelButtonSprite = CCSprite::createWithSpriteFrameName("skip_level_button.png");
	CCSprite* skipLevelButtonSpriteSelected = CCSprite::createWithSpriteFrameName("skip_level_button_selected.png");
	CCMenuItemSprite* skipLevelButton = CCMenuItemSprite::create(skipLevelButtonSprite,
																 skipLevelButtonSpriteSelected,
																 NULL,
																 this,
																 menu_selector(ECGameScene::PauseMenuItemClicked));
	skipLevelButton->setTag(T_SKIP_LEVEL);
	skipLevelButton->setPosition(ccpSub(continueButton->getPosition(), ccp(0, skipLevelButton->getContentSize().height * 0.8f)));
	// select level
	CCSprite* selectLevelButtonSprite = CCSprite::createWithSpriteFrameName("level_select_button.png");
	CCSprite* selectLevelButtonSpriteSelected = CCSprite::createWithSpriteFrameName("level_select_button_selected.png");
	CCMenuItemSprite* selectLevelButton = CCMenuItemSprite::create(selectLevelButtonSprite,
																   selectLevelButtonSpriteSelected,
																   NULL,
																   this,
																   menu_selector(ECGameScene::PauseMenuItemClicked));
	selectLevelButton->setTag(T_SELECT_LEVEL);
	selectLevelButton->setPosition(ccpSub(skipLevelButton->getPosition(), ccp(0, selectLevelButton->getContentSize().height * 0.8f)));
	// main menu
	CCSprite* mainMenuButtonSprite = CCSprite::createWithSpriteFrameName("main_menu_button.png");
	CCSprite* mainMenuButtonSpriteSelected = CCSprite::createWithSpriteFrameName("main_menu_button_selected.png");
	CCMenuItemSprite* mainMenuButton = CCMenuItemSprite::create(mainMenuButtonSprite,
																   mainMenuButtonSpriteSelected,
																   NULL,
																   this,
																   menu_selector(ECGameScene::PauseMenuItemClicked));
	mainMenuButton->setTag(T_MAIN_MENU);
	mainMenuButton->setPosition(ccpSub(selectLevelButton->getPosition(), ccp(0, mainMenuButton->getContentSize().height * 0.8f)));

	// menu
	CCMenu* afterPauseOptionMenu = CCMenu::create(continueButton, skipLevelButton, selectLevelButton, mainMenuButton, NULL);
	afterPauseOptionMenu->setPosition(ccp(0,0));
	this->addChild(afterPauseOptionMenu, Z_PAUSE_MENU, T_AFTER_PAUSE_MENU);

	// move up stick
	CCMoveBy* move_stick_up = CCMoveBy::create(0.2f, ccp(0, stick->getContentSize().height));
	stick->runAction(move_stick_up);

	// move up menu items | buttons
	CCMoveBy* move_menu_up = CCMoveBy::create(0.2f, ccp(0, stick->getContentSize().height));
	afterPauseOptionMenu->runAction(move_menu_up);

	// move restart - pause buttons up
	this->MoveUIElementsInDirection(A_MOVE_UP);
}
void ECGameScene::PauseGame() {
	this->PauseGame(NULL);
}
void ECGameScene::PauseGameLoop(bool is_paused) {
	if (is_paused) {
		// disable touch in towers
		this->SetTowersTouchMode(false);
		// unschedule game update
		this->unscheduleUpdate();

	} else {
		// schedule update of game loop
		this->scheduleUpdate();
		// enable touch in towers
		this->SetTowersTouchMode(true);
	}
}
void ECGameScene::RestartGame(CCObject* pSender) {
	ECAudioManager::PlayButtonClickSound(GAME_SCENE_AUDIO);
	// just restart scene with a current level
	ECSceneManager::GoGameSceneWithLevel(current_level_);
}
void ECGameScene::NextLevel(CCObject* pSender) {
	ECAudioManager::PlayButtonClickSound(GAME_SCENE_AUDIO);

	int nextLevelNumber = GetNextLevelNumber();
	CCString* nextLevel = CCString::createWithFormat("level%i",nextLevelNumber);
	std::string nextLevelString (nextLevel->getCString());
	ECSceneManager::GoGameSceneWithLevel(nextLevelString);
}
void ECGameScene::LevelSelectLayer(CCObject* pSender) {
	ECAudioManager::PlayButtonClickSound(GAME_SCENE_AUDIO);
	ECSceneManager::GoLevelSelectScene();
}
void ECGameScene::PauseMenuItemClicked(CCObject* pSender) {
	CCMenuItem* item = (CCMenuItem*)pSender;
	switch (item->getTag())
	{
	case T_CONTINUE:
		{
			//shaded transparent background
			CCLayerColor* pause_shade_transparent_background_layer = (CCLayerColor*)this->getChildByTag(T_SHADE_PAUSE_BACKGROUND_LAYER);
			this->removeChild(pause_shade_transparent_background_layer, true);

			// stick
			CCSprite* stick = (CCSprite*)this->getChildByTag(T_STICK);
			CCMoveBy* move_stick_down = CCMoveBy::create(0.3f, ccp(0, -stick->getContentSize().height));
			CCCallFuncND* remove_function_for_stick = CCCallFuncND::create(this, callfuncND_selector(ECGameScene::RemoveObjectFromParent), (void*)true);
			CCSequence* stick_sequence = CCSequence::create(move_stick_down, remove_function_for_stick, NULL);
			stick->runAction(stick_sequence);

			// menu
			CCMenu* afterPauseMenu = (CCMenu*)this->getChildByTag(T_AFTER_PAUSE_MENU);
			CCMoveBy* moveMenuDown = CCMoveBy::create(0.3f, ccp(0, -stick->getContentSize().height));
			CCCallFuncND* removeFunctionForMenu = CCCallFuncND::create(this, callfuncND_selector(ECGameScene::RemoveObjectFromParent), (void*)true);
			CCSequence* menuSequence = CCSequence::create(moveMenuDown, removeFunctionForMenu, NULL);
			afterPauseMenu->runAction(menuSequence);

			// move restart - pause buttons down
			this->MoveUIElementsInDirection(A_MOVE_DOWN);

			// continue the loop
			this->PauseGameLoop(false);
		}
		break;
	case T_SKIP_LEVEL:
		{
			this->NextLevel(NULL);
		}
		break;

	case T_SELECT_LEVEL:
		ECSceneManager::GoLevelSelectScene();
		break;

	case T_MAIN_MENU:
		ECSceneManager::GoMainMenuScene();
		break;
	}

	// sound
	ECAudioManager::PlayButtonClickSound(GAME_SCENE_AUDIO);
}
void ECGameScene::WinGame() {

	// audio
	ECAudioManager::PlayActionWinSound();

	// pause game loop
	//this->pauseGameLoop(true);
	// move restart - pause buttons down
	this->MoveUIElementsInDirection(A_MOVE_UP);

	// saves achievements
	int score = CalculateFinalScoreForTime(game_timer_);
	// a variable used in label update/timer
	final_game_score_ = score;
	// saving achievment in database
	this->SaveAchievements(game_timer_, score);
	this->SaveLevelData();

	float best_time = ECDataProvider::GetBestTimeForLevel(current_level_.c_str());
	int best_score = ECDataProvider::GetBestScoreForLevel(current_level_.c_str());


	/**************************************************
	/					 LAYERS
	/__________________________________________________
	/|					LEVEL #						  |
	/|												  |
	/|	TIME: 00									  |
	/|	BEST TIME: 00								  |
	/|												  |
	/|	SCORE: 00						*STAMP*		  |
	/|	BEST SCORE: 00								  |
	/|												  |
	/__________________________________________________
	/	____|____		____|____			____|____
	/  | RESTART |	   | NEXT LVL|         | LVL SEL |
	/  ***********	   ***********         ***********
	**************************************************/

	// black shaded transparent background layer
	CCLayerColor* pauseShadeTransparentBackgroundLayer = CCLayerColor::create(ccc4(0,0,0,100));
	this->addChild(pauseShadeTransparentBackgroundLayer, Z_PAUSE_BLACK_LAYER, T_WIN_SHADE_BACKGROUND_LAYER);

	// win info board
	CCSprite* winInfoBoard = CCSprite::createWithSpriteFrameName("level_complete_info_board.png");
	winInfoBoard->setPosition(ccp(screen_size_.width * 0.5f, -winInfoBoard->getContentSize().height));
	// adding to game layer not spritesheet because of z order of black transparent bg layer and winInfoBoard sprite
	this->addChild(winInfoBoard, Z_WIN_INFO_BOARD, T_WIN_INFO_BOARD);
	/**************************************************
	*					  LABELS
	**************************************************/
	// header : current level
	CCString* level_name_string = CCString::createWithFormat("=LEVEL %i=", GetCurrentLevelNumber());
	CCLabelBMFont* level_name_label = CCLabelBMFont::create(level_name_string->getCString(), "win_font_header.fnt");
	level_name_label->setAnchorPoint(ccp(0.5f, 1.0f));
	level_name_label->setScale(0.7f);
	level_name_label->setPosition(ccp(winInfoBoard->getContentSize().width * 0.5f,
									  winInfoBoard->getContentSize().height - level_name_label->getContentSize().height * 0.3f));
	winInfoBoard->addChild(level_name_label);

	// current played time
	CCString* played_time_string = CCString::createWithFormat("Time: %.1f", game_timer_);
	CCLabelBMFont* played_time_label = CCLabelBMFont::create(played_time_string->getCString(), "win_font_text.fnt");
	played_time_label->setAnchorPoint(ccp(0.0f, 0.5f));
	played_time_label->setScale(1.3f);
	played_time_label->setPosition(ccp(winInfoBoard->getContentSize().width * 0.1f,
									   level_name_label->getPosition().y - level_name_label->getContentSize().height * 1.1f));
	winInfoBoard->addChild(played_time_label);
	// best time
	CCString* best_time_string = CCString::createWithFormat("Best time: %.1f", best_time);
	CCLabelBMFont* best_time_label = CCLabelBMFont::create(best_time_string->getCString(), "win_font_text.fnt");
	best_time_label->setAnchorPoint(ccp(0.0f, 0.5f));
	best_time_label->setScale(0.8f);
	best_time_label->setPosition(ccp(winInfoBoard->getContentSize().width * 0.1f,
									 played_time_label->getPosition().y - best_time_label->getContentSize().height));
	winInfoBoard->addChild(best_time_label);

	// score
	CCString* score_string = CCString::createWithFormat("Score: %i", score);
	CCLabelBMFont* score_label = CCLabelBMFont::create(score_string->getCString(), "win_font_text.fnt");
	score_label->setAnchorPoint(ccp(0.0f, 0.5f));
	score_label->setScale(1.3f);
	score_label->setPosition(ccpSub(best_time_label->getPosition(), ccp(0, score_label->getContentSize().height * 1.5f)));
	winInfoBoard->addChild(score_label);
	// best score
	CCString* best_score_string = CCString::createWithFormat("Best score: %i", best_score);
	CCLabelBMFont* best_score_label = CCLabelBMFont::create(best_score_string->getCString(), "win_font_text.fnt");
	best_score_label->setAnchorPoint(ccp(0.0f, 0.5f));
	best_score_label->setScale(0.8f);
	best_score_label->setPosition(ccpSub(score_label->getPosition(), ccp(0, best_score_label->getContentSize().height )));
	winInfoBoard->addChild(best_score_label);

	/*********************************************
	*					STAMPS
	*********************************************/
	CCSprite* level_completed_stamp_sprite = CCSprite::createWithSpriteFrameName("level_completed_stamp.png");
	level_completed_stamp_sprite->setPosition(ccp(winInfoBoard->getContentSize().width * 0.72f,
												  winInfoBoard->getContentSize().height * 0.6f));
	level_completed_stamp_sprite->setScale(10.0f);
	winInfoBoard->addChild(level_completed_stamp_sprite);

	// level complete stamp sprite animation
	CCDelayTime* level_completed_stamp_sprite_delay_time = CCDelayTime::create(0.2f);
	CCCallFunc* call_completed_sound_effect_play_function = CCCallFunc::create(this, callfunc_selector(ECGameScene::PlayCompletedSoundEffect));
	CCScaleTo* level_completed_stamp_sprite_animation = CCScaleTo::create(0.2f, 1.0f);
	CCSequence* level_completed_stamp_sprite_sequence = CCSequence::create(level_completed_stamp_sprite_delay_time, // delay time and give some time for the win info board to complete it's animation
																		   call_completed_sound_effect_play_function,
																		   level_completed_stamp_sprite_animation,	// scale down animation of the stamp
																		   NULL);
	level_completed_stamp_sprite->runAction(level_completed_stamp_sprite_sequence);


	/**********************************************
	********************MENU***********************
	*****________ _____________ ______________*****
	***** RESTART | NEXT LEVEL | LEVEL SELECT *****
	*****________ _____________ ______________*****
	***********************************************/
	// Replay
	CCSprite* replayButtonSprite = CCSprite::createWithSpriteFrameName("go_replay_level_button.png");
	CCSprite* replayButtonSpriteSelected = CCSprite::createWithSpriteFrameName("go_replay_level_button_selected.png");

	CCMenuItemSprite* replayButton = CCMenuItemSprite::create(replayButtonSprite, replayButtonSpriteSelected, NULL, this,
															  menu_selector(ECGameScene::RestartGame));
	//replayButton->setPosition(ccpSub(winInfoBoard->getPosition(), ccp(winInfoBoard->getContentSize().width * 1.5f, 0)));
	replayButton->setPosition(winInfoBoard->getPosition().x - winInfoBoard->getContentSize().width * 0.35f,
							  screen_size_.height * 0.5f - winInfoBoard->getContentSize().height * 0.55f);
	// Next level
	CCSprite* nextLevelButtonSprite = CCSprite::createWithSpriteFrameName("go_next_level_button.png");
	CCSprite* nextLevelButtonSpriteSelected = CCSprite::createWithSpriteFrameName("go_next_level_button_selected.png");

	CCMenuItemSprite* nextLevelButton = CCMenuItemSprite::create(nextLevelButtonSprite, nextLevelButtonSpriteSelected, NULL, this,
																 menu_selector(ECGameScene::NextLevel));
	nextLevelButton->setPosition(ccp(winInfoBoard->getPosition().x + winInfoBoard->getContentSize().width * 0.01f,
									 replayButton->getPosition().y));

	// Level select page
	CCSprite* levelSelectButtonSprite = CCSprite::createWithSpriteFrameName("go_level_menu_button.png");
	CCSprite* levelSelectButtonSpriteSelected = CCSprite::createWithSpriteFrameName("go_level_menu_button_selected.png");

	CCMenuItemSprite* levelSelectButton = CCMenuItemSprite::create(levelSelectButtonSprite, levelSelectButtonSpriteSelected, NULL, this,
																 menu_selector(ECGameScene::LevelSelectLayer));
	levelSelectButton->setPosition(ccp(winInfoBoard->getPosition().x + winInfoBoard->getContentSize().width * 0.35f,
									   replayButton->getPosition().y));

	// menu
	CCMenu* menu = CCMenu::create(replayButton, nextLevelButton, levelSelectButton, NULL);
	menu->setPosition(ccp(0, winInfoBoard->getPosition().y));
	this->addChild(menu, 1000);

	/**************************************************
	*					 ANIMATIONS
	**************************************************/
	// win info board
	CCMoveTo* infoBoardMoveUp = CCMoveTo::create(0.2f, ccp(screen_size_.width * 0.5f, screen_size_.height * 0.5f));
	winInfoBoard->runAction(infoBoardMoveUp);
	// buttons under win info board
	CCMoveTo* menuMoveUp = CCMoveTo::create(0.2f, ccp(0, 0));
	menu->runAction(menuMoveUp);

	// score label timer animation
	//final_game_score_ = score;
	temp_label_ = score_label;
	this->schedule(schedule_selector(ECGameScene::LabelUpdateTimer));
}
void ECGameScene::LabelUpdateTimer(float delta) {
	temp_final_game_score_ += delta * 4000;
	temp_label_->setString(CCString::createWithFormat("Score: %.0f", temp_final_game_score_)->getCString());
	if (temp_final_game_score_ >= final_game_score_) {					// when temp score timer eqauls actual score value
		this->unschedule(schedule_selector(ECGameScene::LabelUpdateTimer));
		temp_label_->setString(CCString::createWithFormat("Score: %.0f", final_game_score_)->getCString());

		temp_final_game_score_ = 0;

		//
		int percentage_of_score = CalcualtePercentageForScore(final_game_score_);
		this->AddStarsBasedOnPercentage(percentage_of_score);
	}
}
void ECGameScene::AddStarsBasedOnPercentage(int percentage) {
	// *** - 75 < x < 100
	// **  - 50 < x < 75
	// *   - 0 < x < 50
	if (percentage > 75 && percentage < 100) {
		this->AddStarsOnWinBoard(3);
	}
	else if (percentage > 50 && percentage < 75) {
		this->AddStarsOnWinBoard(2);
	}
	else if (percentage <= 50) {
		this->AddStarsOnWinBoard(1);
	}
}
int ECGameScene::GetNumberOfStarstFromPercentage(int percentage) {
	int number_of_starts = 0;
	if (percentage > 75 && percentage < 100) {
		number_of_starts = 3;
	}
	else if (percentage > 50 && percentage < 75) {
		number_of_starts = 2;
	}
	else if (percentage <= 50) {
		number_of_starts = 1;
	}
	return number_of_starts;
}
void ECGameScene::AddStarsOnWinBoard(const int star_number) {
	// audio
	ECAudioManager::PlayActionCompletedSound();
	
	// win info board sprite node
	CCNode* win_info_board = this->getChildByTag(T_WIN_INFO_BOARD);

	for (int i = 0; i < star_number; i++) {
		CCSprite* start_sprite = CCSprite::createWithSpriteFrameName("star.png");
		start_sprite->setPosition(ccp(win_info_board->getContentSize().width * 0.68f + start_sprite->getContentSize().width * 0.8f * i,
									  win_info_board->getContentSize().height * 0.3f));
		start_sprite->setScale(5.0f);
		win_info_board->addChild(start_sprite);

		/*******************************************
		*				Animation
		*******************************************/
		CCScaleTo* start_scale_animation = CCScaleTo::create(0.2f, 1.0f);
		start_sprite->runAction(start_scale_animation);
	}
}
void ECGameScene::PlayCompletedSoundEffect() {
	// audio
	ECAudioManager::PlayActionCompletedSound();
}
int ECGameScene::GetCurrentLevelNumber() const {
	// extracted int value from a String object representing current level (i.e. "level13" -> returns 13)
	return atoi(current_level_.substr(5,current_level_.length()).c_str());
}
int ECGameScene::GetNextLevelNumber() const {
	int current_level_number = GetCurrentLevelNumber();
	int next_level_number = 0;
	if (current_level_number >= MAX_NUMBER_OF_LEVELS_IN_GAME) {
		next_level_number = 1;
	} else {
		next_level_number = ++current_level_number;
	}
	return next_level_number;
}
void ECGameScene::MoveUIElementsInDirection(UIElementsMoveDirection direction) {
	// pause_restart_menu
	CCMenu* pause_restart_menu = (CCMenu*)this->getChildByTag(T_PAUSE_RESTART_MENU);
	CCMoveTo* move_pause_restart_menu = NULL;

	switch (direction)
	{
	case A_MOVE_DOWN:
		move_pause_restart_menu = CCMoveTo::create(0.5f, ccpSub(pause_restart_menu->getPosition(), ccp(0,pause_restart_menu->getContentSize().height/2)));
		break;
	case A_MOVE_UP:
		move_pause_restart_menu = CCMoveTo::create(0.5f, ccpAdd(pause_restart_menu->getPosition(), ccp(0, pause_restart_menu->getContentSize().height * 0.5f)));
		break;
	}
	// run action
	pause_restart_menu->runAction(move_pause_restart_menu);
}
void ECGameScene::SetTowersTouchMode(bool isTouchEnabled) {
	// remove touch delegate in tower
	if (!isTouchEnabled) {
		std::vector<CCSprite*>::iterator towerIter;
		for (towerIter = towers_.begin(); towerIter != towers_.end(); towerIter++) {
			ECTower* tower = dynamic_cast<ECTower*>(*towerIter);
			if (tower) {
				CCDirector* director = CCDirector::sharedDirector();
				director->getTouchDispatcher()->removeDelegate(tower);
			}
		}
	} else {
		// add touch delegate in tower
		std::vector<CCSprite*>::iterator towerIter;
		for (towerIter=towers_.begin(); towerIter!=towers_.end(); towerIter++) {
			ECTower* tower = dynamic_cast<ECTower*>(*towerIter);
			if (tower) {
				CCDirector* director = CCDirector::sharedDirector();
				director->getTouchDispatcher()->addTargetedDelegate(tower, 0, true);
			}
		}
	}
}
void ECGameScene::RemoveSpriteFromSpriteSheet(CCNode* pSender, void* data) {
	CCSprite* sprite = (CCSprite*)pSender;
	bool bool_val = data != NULL;
	if (sprite) {
		game_layer_spritesheet_->removeChild(sprite, bool_val);
	}
}
void ECGameScene::RemoveObjectFromParent(CCNode* pSender, void* data) {
	CCNode* object = pSender;
	bool bool_val = data != NULL;
	if (object) {
		this->removeChild(object, bool_val);
	}
}
int ECGameScene::CalculateFinalScoreForTime(const float game_timer) const {
	int score_reducer = game_timer * GetScoreMultiplier();
	int final_score = MIN_SCORE_FOR_EACH_LEVEL + (MIN_SCORE_FOR_EACH_LEVEL - score_reducer);
	if (final_score >= MIN_SCORE_FOR_EACH_LEVEL) {
		return final_score;
	} else {
		return MIN_SCORE_FOR_EACH_LEVEL;
	}
}
int ECGameScene::CalcualtePercentageForScore(const int final_score) const {
	return ((final_score * 100) / MAX_SCORE_FOR_EACH_LEVEL);
}
int ECGameScene::GetScoreMultiplier() const{
	int level_number = GetCurrentLevelNumber();
	int score_multiplier = 0;
	if ((level_number <= NUMBER_OF_LEVELS_IN_EACH_CHAPTER) ||				// levels: 0 - 15
		((level_number > NUMBER_OF_LEVELS_IN_EACH_CHAPTER) &&				// levels : 16 -
		(level_number <= 2 * NUMBER_OF_LEVELS_IN_EACH_CHAPTER))) {			//			30
			score_multiplier = 100;											// chap: 1-2 => mult = 100;
	}
	else if (((level_number > 2 * NUMBER_OF_LEVELS_IN_EACH_CHAPTER) &&		// levels: 31 -
			(level_number <= 3 * NUMBER_OF_LEVELS_IN_EACH_CHAPTER)) ||		//		   45
			((level_number > 3 * NUMBER_OF_LEVELS_IN_EACH_CHAPTER) &&		// levels: 46 -
			(level_number <= 4 * NUMBER_OF_LEVELS_IN_EACH_CHAPTER))) {		//	       60
				score_multiplier = 10;										// chap: 1-2 => mult = 10;
	}
	return score_multiplier;
}
void ECGameScene::SaveAchievements(const float time, const int score) const{
	ECDataProvider::SetGeneralScore(current_level_.c_str(), score);
	ECDataProvider::SetLevelPLayed(current_level_.c_str(), true);
	ECDataProvider::SetBestTimeForLevel(current_level_.c_str(), time);
	ECDataProvider::SetBestScoreForLevel(current_level_.c_str(), score);

}
void ECGameScene::SaveLevelData() {
	ECDataProviderExt* data_provider = new ECDataProviderExt("level_state.xml", "levels");
	int number_of_stars = GetNumberOfStarstFromPercentage(CalcualtePercentageForScore(final_game_score_));
	// sets is_played -> true and number of stars
	data_provider->SetPlayedAndStarsOnLevelButton(current_level_.c_str(), true, number_of_stars);
	// next level number
	int next_level_number = GetNextLevelNumber();
	// next level name string (eg. level1, level2.....level100)
	CCString* next_level_name_string = CCString::createWithFormat("level%i", next_level_number);
	// unlock next level
	data_provider->SetBlockForLevel(next_level_name_string->getCString(), false);
	// saves modified file - xml
	data_provider->SaveFile();
	delete data_provider;
	data_provider = NULL;
}
void ECGameScene::ccTouchesBegan(CCSet* pTouches, CCEvent* pEvent)
{
	CCSetIterator touchIter;
	std::vector<CCSprite*>::iterator towerIter;
	for (touchIter = pTouches->begin(); touchIter != pTouches->end(); touchIter++)
	{
		CCTouch* touch = (CCTouch*)(*touchIter);
		CCPoint touchPoint = ConvertTouchToPoint(touch);
	}
}
void ECGameScene::ccTouchesMoved(CCSet* pTouches, CCEvent* pEvent)
{
	CCSetIterator touchIter;
	for (touchIter = pTouches->begin(); touchIter != pTouches->end(); touchIter++)
	{
		CCTouch* touch = (CCTouch*)(*touchIter);
		CCPoint touchPoint = ConvertTouchToPoint(touch);
	}
}
void ECGameScene::ccTouchesEnded(CCSet* pTouches, CCEvent* pEvent)
{

}
void ECGameScene::keyBackClicked() {
	PauseGame(NULL);
}
CCPoint ECGameScene::ConvertTouchToPoint(CCTouch* touch)
{
	CCPoint touchPoint = touch->getLocationInView();
	touchPoint = CCDirector::sharedDirector()->convertToGL(touchPoint);
	return touchPoint;
}
bool ECGameScene::IsTouchOnObject(CCTouch* touch, CCSprite* object)
{
	CCPoint touchPoint = CCDirector::sharedDirector()->convertToGL(touch->getLocationInView());
	if (object->boundingBox().containsPoint(touchPoint))
	{
		return true;
	}
	return false;
}
