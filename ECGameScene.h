#ifndef __EC_GAME_SCENE_H__
#define __EC_GAME_SCENE_H__

#include "cocos2d.h"
#include "ECGameHelper.h"

using namespace	cocos2d;

class ECDataProviderExt;
class ECTower;

class ECGameScene : public CCLayer
{
public:
	static CCScene* sceneWithGameLayerToLevel(std::string& level);
	static ECGameScene* createGameLayerToLevel(std::string& level);
	static ECGameScene* sharedGameLayer();
	bool initGameLayerToLayer(std::string& level);

	ECGameScene();
	virtual ~ECGameScene();

	virtual void update(float delta);
	
private:

	enum ScreenLayersZOrder {
		Z_BACKGROUND = 50,
		Z_BUILDING,
		Z_TOWERS,
		Z_LINES,
		Z_UI_LABELS,
		// pause 
		Z_PAUSE_BLACK_LAYER,
		Z_PAUSE_MENU,
		// win
		Z_WIN_INFO_BOARD,
		Z_WIN_RESULT_INFO_LABEL
	};


	// menu appears when pause button is clicked
	// |-----CONTINUE-------|
	// |-----SKIP LEVEL-----|
	// |-----LEVEL SELECT---|
	// |-----MAIN MENU------|
	enum PauseMenuItemsTag {
		T_CONTINUE = 100,
		T_SKIP_LEVEL,
		T_SELECT_LEVEL,
		T_MAIN_MENU
	};

	// elements that added to main scene and called from different methods to animation effects
	// instead of making elements global fields -> accessed by tags
	enum UIElementsTag {
		//labels
		T_LEVEL_TIME_LABEL = 200,
		T_LEVEL_BEST_TIME_LABEL,
		T_LEVEL_LIGHTS_COUNTER,
		
		// pause
		T_PAUSE_RESTART_MENU,
		T_SHADE_PAUSE_BACKGROUND_LAYER,
		T_AFTER_PAUSE_MENU,
		T_STICK,

		// win
		T_WIN_SHADE_BACKGROUND_LAYER,
		T_WIN_INFO_BOARD
	};

	// move UI elements down/up
	typedef enum {
		A_MOVE_DOWN,
		A_MOVE_UP
	} UIElementsMoveDirection;

	static ECGameScene* instance_of_gamelayer_;

	CCSize screen_size_;

	// vectors - arrays
	std::vector<CCSprite*> towers_;					/*!< vector container that stores all the towers in a level */
	std::vector<CCSprite*> buildings_;				/*!< vector container that stores all the buildings(houses) in a level */
	std::vector<CCSprite*> lines_;					/*!< vector container that stores all the lines connected to towers in a level */

	CCSpriteBatchNode* game_layer_spritesheet_;		/*!< main game spritesheet that contains all the sprites(background,
													buildings, towers, line, pause - win state sprites, UI (restart, pause)
													elements of a level */

	// game data loaded from xml file
	ECDataProviderExt* game_level_data_; /*!< data provider for a level. Load xml file and provides access to node element, attributs*/

	// string object representing a current level of the game
	std::string current_level_;
	// game timer fields
	float _gameTimer;
	/** 
	* variable that is assigned to final score of a level in win state and 
	* is intended for using in LabelUpdateTimer method, where temp_game_score_ is updated until it equals or
	* greater than this variable (= final game score of level)
	*/
	float final_game_score_;	
	/**
	* temp variable
	*/
	float temp_final_game_score_;
	CCLabelBMFont* temp_label_;  /*! < temporary label that used in LabelUpdateTimer method to create timer like effect (animation only)
								 on score and best_score labels */
	int _gameLightsCounter;

	void loadGameDataForLevel(const char* level);
	void createTowerInQuant(const int quantity, const char* imgName, const float scale);
	void createBuildingInQuant(const int quantity, const char* imgName, const float scale);
	void createLines(bool isCircleClosed);
	void resetLines();
	void resetGameObjectsOfType(NodeType node);
	void checkForCollision();
	void checkForWinState();

	// ui control methods
	void pauseGame(CCObject* pSender);			/*! < main method that pauses the level and calls other methods
												to implement pause. Called from pause button. */
	void pauseGame();							/*! < pause method which calls method pauseGame(CCObject* pSender) with
												NULL argument. This method used only for CCCallFunc action */
	void pauseGameLoop(bool isPaused);			/*! < stops update method and prevents towers from touch */
	void restartGame(CCObject* pSender);		/*! < just restart current/playing level again */
	void nextLevel(CCObject* pSender);			/*! < restarts and changes game to the next level */
	void levelSelectLayer(CCObject* pSender);	/*! < goes back to level select layer from game layer. From paused or won state, level select button provided as option*/ 
	void pauseMenuItemClicked(CCObject* pSednder); 
	void winGame();
	/**
	* Method that is scheduled to update (like timer) label text to a given number.
	* Just for animation effect. 
	*/
	void LabelUpdateTimer(float delta); 
	void AddStarsBasedOnPercentage(int percentage);
	int GetNumberOfStarstFromPercentage(int percentage);
	void AddStarsOnWinBoard(const int start_number);
	
	
	// returns level number
	int GetCurrentLevelNumber() const;
	// returns next level number. Add +1 to current level
	int GetNextLevelNumber() const;
	// move ui elements (restart, pause button for now)
	void moveUIElementsInDirection(UIElementsMoveDirection direction);
	// controls touch of towers (when game is paused etc..)
	void setTowersTouchMode(bool isTouchEnabled);
	// remove sprites after animation ends
	void removeSpriteFromSpriteSheet(CCNode* pSender, void* data);
	// removes menu and items (i.e. pause menu items ....)
	void removeObjectFromParent(CCNode* pSender, void* data);
	/**
	* calculates final score based on played time 
	*/
	int CalculateFinalScoreForTime(const float game_timer) const; 
	/** 
	* calculates final score in percentage where 100% means 
	* player completed the level in 0 second and got full 
	* final score = 4000 (actually impossible :D) 
	* We need percentage for "stars", where 
	*			0=<x<=55		== *
	*			55=<x<=75		== **
	*			75=<x<=100		== ***
	*/
	int CalcualtePercentageForScore(const int final_score) const;
	/**
	* returns score multiplier value. For chapter 1- 2 == 100,
	* and for chapter 3-4 == 10 (eg. x = game_timer * score_mult
	*					   final_score = 2000 + (2000 - x);
	*/
	int GetScoreMultiplier() const;
	/**
	* saves all achievments of a level (time, best time, score ...)
	* @param time a played time of a level
	* @param score final score got in a level
	*/
	void SaveAchievements(const float time, const int score) const;
	/**
	* save level data that will unlocks next level and marks playing level
	* as played and sets number of stars on level buttom icon
	*/
	void SaveLevelData();
	
	// helper methods
	bool isTouchOnObject(CCTouch* touch, CCSprite* object);
	CCPoint convertTouchToPoint(CCTouch* touch);

	virtual void ccTouchesBegan(CCSet* pTouches, CCEvent* pEvent);
	virtual void ccTouchesMoved(CCSet* pTouches, CCEvent* pEvent);
	virtual void ccTouchesEnded(CCSet* pTouches, CCEvent* pEvent);
};	
#endif// __EC_GAME_SCENE_H__
