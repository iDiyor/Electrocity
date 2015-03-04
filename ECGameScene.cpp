#include "ECGameScene.h"
#include "ECTower.h"
#include "ECBuilding.h"
#include "ECLine.h"
#include "ECDataProviderExt.h"
#include "ECSceneManager.h"
#include "ECDataProvider.h"

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
	_gameTimer(0),
	_gameLightsCounter(0),
	temp_label_(NULL),
	temp_final_game_score_(0)
{

}
ECGameScene::~ECGameScene()
{
	delete game_level_data_;
	game_level_data_ = NULL;

	// removing cached sprite files
	CCSpriteFrameCache::sharedSpriteFrameCache()->removeSpriteFramesFromFile("game_layer_spritesheet.plist");
}
CCScene* ECGameScene::sceneWithGameLayerToLevel(std::string& level)
{
	CCScene* _scene = NULL;
	do
	{
		_scene = CCScene::create();
		CC_BREAK_IF(!_scene);

		ECGameScene* _layer = ECGameScene::createGameLayerToLevel(level);
		CC_BREAK_IF(!_layer);

		_scene->addChild(_layer);
	}while (0);
	return _scene;
}
ECGameScene* ECGameScene::createGameLayerToLevel(std::string& level)
{
	ECGameScene* gameLayer = new ECGameScene();
	if (gameLayer && gameLayer->initGameLayerToLayer(level))
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
ECGameScene* ECGameScene::sharedGameLayer()
{
	CC_ASSERT(instance_of_gamelayer_);
	return instance_of_gamelayer_;
}
bool ECGameScene::initGameLayerToLayer(std::string& level)
{
	bool _isSuccess = false;
	do
	{
		CC_BREAK_IF(!CCLayer::init());
		this->setTouchEnabled(true);

		instance_of_gamelayer_ = this;

		screen_size_ = CCDirector::sharedDirector()->getWinSize();

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
		CCSprite* gamePauseButtonSprite = CCSprite::createWithSpriteFrameName("pause_button.png");
		CCSprite* gamePauseButtonSpriteSelected = CCSprite::createWithSpriteFrameName("pause_button_selected.png");
		CCMenuItemSprite* gamePauseButton = CCMenuItemSprite::create(gamePauseButtonSprite,
																	 gamePauseButtonSpriteSelected,
																	 NULL,
																	 this,
																	 menu_selector(ECGameScene::pauseGame));
		gamePauseButton->setPosition(ccp(screen_size_.width - gamePauseButton->getContentSize().width * 0.5f - 2,
										 screen_size_.height - gamePauseButton->getContentSize().height * 0.5f - 2));

		// restart
		CCSprite* gameRestartButtonSprite = CCSprite::createWithSpriteFrameName("restart_button.png");
		CCSprite* gameRestartButtonSpriteSelected = CCSprite::createWithSpriteFrameName("restart_button_selected.png");
		CCMenuItemSprite* gameRestartButton = CCMenuItemSprite::create(gameRestartButtonSprite,
																	   gameRestartButtonSpriteSelected,
																	   NULL,
																	   this,
																	   menu_selector(ECGameScene::restartGame));
		gameRestartButton->setPosition(ccp(gamePauseButton->getPosition().x - gameRestartButton->getContentSize().width - 5,
										   gamePauseButton->getPosition().y));

		CCMenu* gameControlMenu = CCMenu::create(gamePauseButton, gameRestartButton, NULL);
		gameControlMenu->setPosition(0, 0);
		this->addChild(gameControlMenu, Z_UI_LABELS, T_PAUSE_RESTART_MENU);


		// loading level data
		game_level_data_ = new ECDataProviderExt("data4.xml", "levels" ,level);
		// set up level
		this->loadGameDataForLevel(level.c_str());
		// seting current level
		current_level_.assign(level);

		// labels - fonts
		// current level
		int currentLevelNumber = GetCurrentLevelNumber();
		CCString* currentLevelString = CCString::createWithFormat("Level %i", currentLevelNumber);
		CCLabelBMFont* currentLevelLabel = CCLabelBMFont::create(currentLevelString->getCString(), "general_font.fnt");
		currentLevelLabel->setPosition(ccp(screen_size_.width - currentLevelLabel->getContentSize().width * 0.6f ,
										   currentLevelLabel->getContentSize().height * 0.5f));
		this->addChild(currentLevelLabel);

		// timer label
		CCLabelBMFont* levelTimeLabel = CCLabelBMFont::create("Time: 0", "general_font.fnt");
		levelTimeLabel->setAnchorPoint(ccp(0.0f, 0.5f));
		levelTimeLabel->setPosition(ccp(levelTimeLabel->getContentSize().width * 0.05f,
										screen_size_.height - levelTimeLabel->getContentSize().height * 0.6f));
		this->addChild(levelTimeLabel, Z_UI_LABELS, T_LEVEL_TIME_LABEL);

		// best time label
		float best_time = ECDataProvider::GetBestTimeForLevel(current_level_);
		CCString* best_time_string = CCString::createWithFormat("Best time: %.1f", best_time);
		CCLabelBMFont* levelBestTime = CCLabelBMFont::create(best_time_string->getCString(), "general_font.fnt");
		levelBestTime->setAnchorPoint(ccp(0.0f, 0.5f));
		levelBestTime->setPosition(ccpSub(levelTimeLabel->getPosition(), ccp(0, levelBestTime->getContentSize().height)));
		this->addChild(levelBestTime, Z_UI_LABELS, T_LEVEL_BEST_TIME_LABEL);

		// building lights on counter
		CCLabelBMFont* lightsCounterLabel = CCLabelBMFont::create(CCString::createWithFormat("0 | %i", buildings_.size())->getCString(), "general_font.fnt");
		lightsCounterLabel->setPosition(ccp(screen_size_.width * 0.5f, levelTimeLabel->getPosition().y));
		this->addChild(lightsCounterLabel, Z_UI_LABELS, T_LEVEL_LIGHTS_COUNTER);

		this->scheduleUpdate();

		_isSuccess = true;
	}while(0);
	return _isSuccess;
}
void ECGameScene::update(float delta)
{
	// loop through towers and find moving ones then update line
	for (unsigned int i = 0; i < towers_.size(); i++)
	{
		ECTower* tower = static_cast<ECTower*>(towers_.at(i));
		if (tower->getTowerState() == ON_TOWER_MOVED || tower->getTowerState() == ON_TOWER_ENDED)
		{
			this->resetLines();
			this->checkForCollision();
		}
	}

	// check for win state
	this->checkForWinState();

	// timer, updating game timer label
	_gameTimer += delta;
	CCLabelBMFont* gameTimerLabel = (CCLabelBMFont*)this->getChildByTag(T_LEVEL_TIME_LABEL);
	gameTimerLabel->setString(CCString::createWithFormat("Time: %.1f", _gameTimer)->getCString());

}
void ECGameScene::loadGameDataForLevel(const char* level)
{
	//load specific node(building,tower) attributes -> call methods with args
	std::map<std::string, int> nodeAttributes;

	//variables for each attributes
	char imageFileName[20];
	int nodeImageNumber = 0;
	int nodeQuantity	= 0;
	float nodeScale		= 0;

	// loading building attributes | assigning attributes
	game_level_data_->loadNodeAttributesForNodeType(kBuilding, &nodeImageNumber, &nodeQuantity, &nodeScale);
	// building full image file name with sprite number
	std::sprintf(imageFileName, "house_off_%i.png", nodeImageNumber);
	//creating buildings according to attributes
	this->createBuildingInQuant(nodeQuantity, imageFileName, nodeScale);

	//clear for next use
	nodeAttributes.clear();

	// loading towers attributes | assigning attributes
	game_level_data_->loadNodeAttributesForNodeType(kTower, &nodeImageNumber, &nodeQuantity, &nodeScale);
	// building full image file name with sprite number
	std::sprintf(imageFileName, "tower_%i.png", nodeImageNumber);
	//creating buildings according to attributes
	this->createTowerInQuant(nodeQuantity, imageFileName, nodeScale);

	//creating lines
	bool islinesCircleClosed = game_level_data_->loadLinesLoopAttribute();
	this->createLines(islinesCircleClosed);

	//clear for next use
	nodeAttributes.clear();
}
void ECGameScene::createTowerInQuant(const int amount, const char* imgName, const float scale)
{
	for (unsigned int i = 0; i < amount; i++)
	{
		ECTower* tower = ECTower::createTowerWithFileName(imgName);
		tower->setScale(scale);
		game_layer_spritesheet_->addChild(tower, Z_TOWERS);
		towers_.push_back(tower);
	}
	this->resetGameObjectsOfType(kTower);
}
void ECGameScene::createBuildingInQuant(const int amount, const char* imgName, const float scale)
{
	for (unsigned int i = 0; i < amount; i++)
	{
		ECBuilding* building = ECBuilding::createBuildingWithFileName(imgName);
		building->setScale(scale);
		game_layer_spritesheet_->addChild(building, Z_BUILDING);
		buildings_.push_back(building);
	}
	this->resetGameObjectsOfType(kBuilding);
}
void ECGameScene::resetGameObjectsOfType(NodeType node)
{
	std::vector<CCPoint>	nodePositions;
	std::vector<CCSprite*>	nodesArray;

	switch (node)
	{
	case kBuilding:
		{
			game_level_data_->loadPositionXYOfBuildings(nodePositions);
			nodesArray = buildings_;
		}
		break;
	case kTower:
		{
			game_level_data_->loadPositionXYOfTowers(nodePositions);
			nodesArray = towers_;
		}
		break;
	}

	// loop pos and node
	for (unsigned int i = 0; i < nodesArray.size(); i++)
	{
		CCSprite* node = dynamic_cast<CCSprite*>(nodesArray.at(i));
		CCPoint positionToSet = (CCPoint)nodePositions.at(i);
		node->setPosition(positionToSet);
	}

	nodePositions.clear();
	nodesArray.clear();
}
void ECGameScene::createLines(bool isCircleClosed)
{
	int linesQuantity = 0;
	if (!isCircleClosed) {
		linesQuantity = towers_.size() - 1;
	} else {
		linesQuantity = towers_.size();
	}

	for (unsigned int i = 0; i < linesQuantity; i++)
	{
		ECLine* line = ECLine::create();
		game_layer_spritesheet_->addChild(line, Z_LINES);
		lines_.push_back(line);
	}
	this->resetLines();
}
void ECGameScene::resetLines()
{
	int counterA = 0;
	int counterB = 0;

	for (unsigned int i = 0; i < towers_.size(); i++)
	{
		if (i == towers_.size() - 1) //if i points to the last tower in the array
		{
			counterA = 0; //first tower in array
			counterB = i; //last tower in array
		}
		else
		{
			counterA = i;		//couple first
			counterB = 1 + i;	// couple second
		}

		ECTower* towerOne = dynamic_cast<ECTower*>(towers_.at(counterA));
		ECTower* towerTwo = dynamic_cast<ECTower*>(towers_.at(counterB));
		ECLine* line = NULL;

		// i must not point to a greater number than lines` number in the array
		if (i < lines_.size())
		{
			line = dynamic_cast<ECLine*>(lines_.at(i));
		}

		if (towerOne && towerTwo && line)
		{
			// updating line between two towers | position & scaleX
			line->resetLineBetweenTowers(towerOne, towerTwo);
		}
	}
}
void ECGameScene::checkForCollision()
{
	_gameLightsCounter = 0;

	std::vector<CCSprite*>::iterator buildingIter;
	std::vector<CCSprite*>::iterator lineIter;
	for (buildingIter=buildings_.begin(); buildingIter!=buildings_.end(); buildingIter++)
	{
		ECBuilding* building = dynamic_cast<ECBuilding*>(*buildingIter);
		if (building)
		{
			if (building->getLightState() == LIGHT_ON)
				building->setBuildingLight(LIGHT_OFF);
		}

		for (lineIter=lines_.begin(); lineIter!=lines_.end(); lineIter++)
		{
			ECLine* line = dynamic_cast<ECLine*>(*lineIter);
			if (line)
			{
				if (building->checkCollisionWithLine(line)) {
					building->setBuildingLight(LIGHT_ON);
					// increment light counter and when it equals to all houses number == win state
					_gameLightsCounter++;

				}
			}
		}
	}
	// update on lights counter
	CCLabelBMFont* on_light_counter_label = (CCLabelBMFont*)this->getChildByTag(T_LEVEL_LIGHTS_COUNTER);
	on_light_counter_label->setString(CCString::createWithFormat("%i | %i", _gameLightsCounter, buildings_.size())->getCString());
}
void ECGameScene::checkForWinState() {

	// win situation
	if (_gameLightsCounter == buildings_.size()) {
		// unschedule update, disable touch on towers
		this->pauseGameLoop(true);
		CCCallFunc* callPauseGameFunction = CCCallFunc::create(this, callfunc_selector(ECGameScene::winGame));
		// delay to show win sprites, just for effect
		CCDelayTime* delayBeforePausGameFunctionCall = CCDelayTime::create(0.5f);
		CCSequence* sequence = CCSequence::create(delayBeforePausGameFunctionCall, callPauseGameFunction, NULL);
		this->runAction(sequence);
	}
		//CCLOG("COUNTER: %i", _gameLightsCounter);
}
void ECGameScene::pauseGame(CCObject* pSender) {
	CCLOG("PAUSE BUTTON");

	// pause the loop | update method.
	this->pauseGameLoop(true);

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
																menu_selector(ECGameScene::pauseMenuItemClicked));
	continueButton->setTag(T_CONTINUE);
	continueButton->setPosition(ccp(stick->getPosition().x, stick->getPosition().y + stick->getContentSize().height - continueButton->getContentSize().height * 0.6f));
	// skip level
	CCSprite* skipLevelButtonSprite = CCSprite::createWithSpriteFrameName("skip_level_button.png");
	CCSprite* skipLevelButtonSpriteSelected = CCSprite::createWithSpriteFrameName("skip_level_button_selected.png");
	CCMenuItemSprite* skipLevelButton = CCMenuItemSprite::create(skipLevelButtonSprite,
																 skipLevelButtonSpriteSelected,
																 NULL,
																 this,
																 menu_selector(ECGameScene::pauseMenuItemClicked));
	skipLevelButton->setTag(T_SKIP_LEVEL);
	skipLevelButton->setPosition(ccpSub(continueButton->getPosition(), ccp(0, skipLevelButton->getContentSize().height * 0.8f)));
	// select level
	CCSprite* selectLevelButtonSprite = CCSprite::createWithSpriteFrameName("level_select_button.png");
	CCSprite* selectLevelButtonSpriteSelected = CCSprite::createWithSpriteFrameName("level_select_button_selected.png");
	CCMenuItemSprite* selectLevelButton = CCMenuItemSprite::create(selectLevelButtonSprite,
																   selectLevelButtonSpriteSelected,
																   NULL,
																   this,
																   menu_selector(ECGameScene::pauseMenuItemClicked));
	selectLevelButton->setTag(T_SELECT_LEVEL);
	selectLevelButton->setPosition(ccpSub(skipLevelButton->getPosition(), ccp(0, selectLevelButton->getContentSize().height * 0.8f)));
	// main menu
	CCSprite* mainMenuButtonSprite = CCSprite::createWithSpriteFrameName("main_menu_button.png");
	CCSprite* mainMenuButtonSpriteSelected = CCSprite::createWithSpriteFrameName("main_menu_button_selected.png");
	CCMenuItemSprite* mainMenuButton = CCMenuItemSprite::create(mainMenuButtonSprite,
																   mainMenuButtonSpriteSelected,
																   NULL,
																   this,
																   menu_selector(ECGameScene::pauseMenuItemClicked));
	mainMenuButton->setTag(T_MAIN_MENU);
	mainMenuButton->setPosition(ccpSub(selectLevelButton->getPosition(), ccp(0, mainMenuButton->getContentSize().height * 0.8f)));

	// menu
	CCMenu* afterPauseOptionMenu = CCMenu::create(continueButton, skipLevelButton, selectLevelButton, mainMenuButton, NULL);
	afterPauseOptionMenu->setPosition(ccp(0,0));
	this->addChild(afterPauseOptionMenu, Z_PAUSE_MENU, T_AFTER_PAUSE_MENU);

	// move up stick
	CCMoveBy* moveStickUp = CCMoveBy::create(0.2f, ccp(0, stick->getContentSize().height * 0.9f));
	stick->runAction(moveStickUp);

	// move up menu items | buttons
	CCMoveBy* moveMenuUp = CCMoveBy::create(0.2f, ccp(0, stick->getContentSize().height * 0.9f));
	afterPauseOptionMenu->runAction(moveMenuUp);

	// move restart - pause buttons up
	this->moveUIElementsInDirection(A_MOVE_UP);
}
void ECGameScene::pauseGame() {
	this->pauseGame(NULL);
}
void ECGameScene::pauseGameLoop(bool isPaused) {
	if (isPaused) {
		// disable touch in towers
		this->setTowersTouchMode(false);
		// unschedule game update
		this->unscheduleUpdate();

	} else {
		// schedule update of game loop
		this->scheduleUpdate();
		// enable touch in towers
		this->setTowersTouchMode(true);
	}
}
void ECGameScene::restartGame(CCObject* pSender) {
	// just restart scene with current level
	ECSceneManager::GoGameSceneWithLevel(current_level_);
}
void ECGameScene::nextLevel(CCObject* pSender) {

	int nextLevelNumber = GetNextLevelNumber();
	CCString* nextLevel = CCString::createWithFormat("level%i",nextLevelNumber);
	std::string nextLevelString (nextLevel->getCString());
	ECSceneManager::GoGameSceneWithLevel(nextLevelString);
}
void ECGameScene::levelSelectLayer(CCObject* pSender) {
	ECSceneManager::GoLevelSelectScene();
}
void ECGameScene::pauseMenuItemClicked(CCObject* pSender) {
	CCMenuItem* item = (CCMenuItem*)pSender;
	switch (item->getTag())
	{
	case T_CONTINUE:
		{
			//shaded transparent background
			CCLayerColor* pauseShadeTransparentBackgroundLayer = (CCLayerColor*)this->getChildByTag(T_SHADE_PAUSE_BACKGROUND_LAYER);
			this->removeChild(pauseShadeTransparentBackgroundLayer, true);

			// stick
			CCSprite* stick = (CCSprite*)this->getChildByTag(T_STICK);
			CCMoveBy* moveStickDown = CCMoveBy::create(0.3f, ccp(0, -stick->getContentSize().height));
			CCCallFuncND* removeFunctionForStick = CCCallFuncND::create(this, callfuncND_selector(ECGameScene::removeObjectFromParent), (void*)true);
			CCSequence* stickSequence = CCSequence::create(moveStickDown, removeFunctionForStick, NULL);
			stick->runAction(stickSequence);

			// menu
			CCMenu* afterPauseMenu = (CCMenu*)this->getChildByTag(T_AFTER_PAUSE_MENU);
			CCMoveBy* moveMenuDown = CCMoveBy::create(0.3f, ccp(0, -stick->getContentSize().height));
			CCCallFuncND* removeFunctionForMenu = CCCallFuncND::create(this, callfuncND_selector(ECGameScene::removeObjectFromParent), (void*)true);
			CCSequence* menuSequence = CCSequence::create(moveMenuDown, removeFunctionForMenu, NULL);
			afterPauseMenu->runAction(menuSequence);

			// move restart - pause buttons down
			this->moveUIElementsInDirection(A_MOVE_DOWN);

			// continue the loop
			this->pauseGameLoop(false);
		}
		break;
	case T_SKIP_LEVEL:
		{
			this->nextLevel(NULL);
		}
		break;

	case T_SELECT_LEVEL:
		ECSceneManager::GoLevelSelectScene();
		break;

	case T_MAIN_MENU:
		ECSceneManager::GoMainMenuScene();
		break;
	}
}
void ECGameScene::winGame() {

	// pause game loop
	//this->pauseGameLoop(true);
	// move restart - pause buttons down
	this->moveUIElementsInDirection(A_MOVE_UP);

	// saves achievements
	int score = CalculateFinalScoreForTime(_gameTimer);
	// a variable used in label update/timer
	final_game_score_ = score;
	// saving achievment in database
	this->SaveAchievements(_gameTimer, score);
	this->SaveLevelData();

	float best_time = ECDataProvider::GetBestTimeForLevel(current_level_);
	int best_score = ECDataProvider::GetBestScoreForLevel(current_level_);


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
	CCString* played_time_string = CCString::createWithFormat("Time: %.1f", _gameTimer);
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
	CCScaleTo* level_completed_stamp_sprite_animation = CCScaleTo::create(0.2f, 1.0f);
	CCSequence* level_completed_stamp_sprite_sequence = CCSequence::create(level_completed_stamp_sprite_delay_time, // delay time and give some time for the win info board to complete it's animation
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
															  menu_selector(ECGameScene::restartGame));
	//replayButton->setPosition(ccpSub(winInfoBoard->getPosition(), ccp(winInfoBoard->getContentSize().width * 1.5f, 0)));
	replayButton->setPosition(winInfoBoard->getPosition().x - winInfoBoard->getContentSize().width * 0.35f,
							  screen_size_.height * 0.5f - winInfoBoard->getContentSize().height * 0.55f);
	// Next level
	CCSprite* nextLevelButtonSprite = CCSprite::createWithSpriteFrameName("go_next_level_button.png");
	CCSprite* nextLevelButtonSpriteSelected = CCSprite::createWithSpriteFrameName("go_next_level_button_selected.png");

	CCMenuItemSprite* nextLevelButton = CCMenuItemSprite::create(nextLevelButtonSprite, nextLevelButtonSpriteSelected, NULL, this,
																 menu_selector(ECGameScene::nextLevel));
	nextLevelButton->setPosition(ccp(winInfoBoard->getPosition().x + winInfoBoard->getContentSize().width * 0.01f,
									 replayButton->getPosition().y));

	// Level select page
	CCSprite* levelSelectButtonSprite = CCSprite::createWithSpriteFrameName("go_level_menu_button.png");
	CCSprite* levelSelectButtonSpriteSelected = CCSprite::createWithSpriteFrameName("go_level_menu_button_selected.png");

	CCMenuItemSprite* levelSelectButton = CCMenuItemSprite::create(levelSelectButtonSprite, levelSelectButtonSpriteSelected, NULL, this,
																 menu_selector(ECGameScene::levelSelectLayer));
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
void ECGameScene::moveUIElementsInDirection(UIElementsMoveDirection direction) {
	// pause_restart_menu
	CCMenu* pauseRestartMenu = (CCMenu*)this->getChildByTag(T_PAUSE_RESTART_MENU);
	CCMoveTo* movePauseRestartMenu = NULL;

	switch (direction)
	{
	case A_MOVE_DOWN:
		movePauseRestartMenu = CCMoveTo::create(0.5f, ccpSub(pauseRestartMenu->getPosition(), ccp(0,pauseRestartMenu->getContentSize().height/2)));
		break;
	case A_MOVE_UP:
		movePauseRestartMenu = CCMoveTo::create(0.5f, ccpAdd(pauseRestartMenu->getPosition(), ccp(0, pauseRestartMenu->getContentSize().height * 0.5f)));
		break;
	}
	// run action
	pauseRestartMenu->runAction(movePauseRestartMenu);
}
void ECGameScene::setTowersTouchMode(bool isTouchEnabled) {
	// remove touch delegate in tower
	if (!isTouchEnabled) {
		std::vector<CCSprite*>::iterator towerIter;
		for (towerIter=towers_.begin(); towerIter!=towers_.end(); towerIter++) {
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
void ECGameScene::removeSpriteFromSpriteSheet(CCNode* pSender, void* data) {
	CCSprite* sprite = (CCSprite*)pSender;
	bool boolVal = data != NULL;
	if (sprite) {
		game_layer_spritesheet_->removeChild(sprite, boolVal);
	}
}
void ECGameScene::removeObjectFromParent(CCNode* pSender, void* data) {
	CCNode* object = pSender;
	bool boolVal = data != NULL;
	if (object) {
		this->removeChild(object, boolVal);
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
	ECDataProvider::SetBestTimeForLevel(time, current_level_);
	ECDataProvider::SetBestScoreForLevel(score, current_level_);
	ECDataProvider::SetGeneralScore(score);
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
		CCPoint touchPoint = convertTouchToPoint(touch);
	}
}
void ECGameScene::ccTouchesMoved(CCSet* pTouches, CCEvent* pEvent)
{
	CCSetIterator touchIter;
	for (touchIter = pTouches->begin(); touchIter != pTouches->end(); touchIter++)
	{
		CCTouch* touch = (CCTouch*)(*touchIter);
		CCPoint touchPoint = convertTouchToPoint(touch);
	}
}
void ECGameScene::ccTouchesEnded(CCSet* pTouches, CCEvent* pEvent)
{

}
CCPoint ECGameScene::convertTouchToPoint(CCTouch* touch)
{
	CCPoint touchPoint = touch->getLocationInView();
	touchPoint = CCDirector::sharedDirector()->convertToGL(touchPoint);
	return touchPoint;
}
bool ECGameScene::isTouchOnObject(CCTouch* touch, CCSprite* object)
{
	CCPoint touchPoint = CCDirector::sharedDirector()->convertToGL(touch->getLocationInView());
	if (object->boundingBox().containsPoint(touchPoint))
	{
		return true;
	}
	return false;
}
