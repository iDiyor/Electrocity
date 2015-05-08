//  Copyright (c) 2015 Diyor Islomov. All rights reserved.
//	diyor.islomov@gmail.com || @iDiyor 
#ifndef __EC_GAME_SCENE_H__
#define __EC_GAME_SCENE_H__

#include "cocos2d.h"
#include "ECGameHelper.h"

using namespace	cocos2d;

class ECDataProviderExt;
class ECTower;
class ECAudioManager;

class ECGameScene : public CCLayer
{
public:
	static CCScene* SceneWithGameLayerToLevel(std::string& level);
	static ECGameScene* CreateGameLayerToLevel(std::string& level);
	static ECGameScene* SharedGameLayer();
	bool InitGameLayerToLayer(std::string& level);

	ECGameScene();
	~ECGameScene();

	void update(float delta);
	
private:

	typedef enum {
		IDLE = 10,
		PLAYING,
		PAUSED,
		WON
	}GameState;


	typedef enum {
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
	}ScreenLayersZOrder ;


	// menu appears when pause button is clicked
	// |-----CONTINUE-------|
	// |-----SKIP LEVEL-----|
	// |-----LEVEL SELECT---|
	// |-----MAIN MENU------|
	typedef enum {
		T_CONTINUE = 100,
		T_SKIP_LEVEL,
		T_SELECT_LEVEL,
		T_MAIN_MENU
	}PauseMenuItemsTag;

	// elements that added to main scene and called from different methods to animation effects
	// instead of making elements global fields -> accessed by tags
	typedef enum {
		//labels
		T_LEVEL_TIME_LABEL = 150,
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
	} UIElementsTag;

	// move UI elements down/up
	typedef enum {
		A_MOVE_DOWN,
		A_MOVE_UP
	} UIElementsMoveDirection;


	void LoadGameDataForLevel(const char* level);
	void CreateTowerInQuant(const int quantity, const char* image_filename, const float scale);
	void CreateBuildingInQuant(const int quantity, const char* image_filename, const float scale);
	void CreateLines(bool is_lines_loop);
	void ResetLines();
	void AttachLines();
	void AttachExtraInsideLine(int tower_a_index, int tower_b_index);
	void AttachExtraOutsideLine(int tower_a_index, const char* file_name, float pos_x, float pos_y);
	void ResetGameObjectsOfType(NodeType node_type);
	bool CheckForLineAndBuildingCollision();
	bool CheckForTowerAndBuildingCollision(ECTower* tower);
	bool CheckForWinState();

	// ui control methods
	void PauseGame(CCObject* pSender);			/*! < main method that pauses the level and calls other methods
												to implement pause. Called from pause button. */
	void PauseGame();							/*! < pause method which calls method pauseGame(CCObject* pSender) with
												NULL argument. This method used only for CCCallFunc action */
	void PauseGameLoop(bool is_paused);			/*! < stops update method and prevents towers from touch */
	void RestartGame(CCObject* pSender);		/*! < just restart current/playing level again */
	void NextLevel(CCObject* pSender);			/*! < restarts and changes game to the next level */
	void LevelSelectLayer(CCObject* pSender);	/*! < goes back to level select layer from game layer. From paused or won state, level select button provided as option*/ 
	void PauseMenuItemClicked(CCObject* pSednder); 
	void WinGame();
	
	/**
	* Method that is scheduled to update (like timer) label text to a given number.
	* Just for animation effect. 
	*/
	void LabelUpdateTimer(float delta); 
	void AddStarsBasedOnPercentage(int percentage);
	int GetNumberOfStarstFromPercentage(int percentage);
	void AddStarsOnWinBoard(const int start_number);
	void PlayCompletedSoundEffect();
	
	
	// returns level number
	int GetCurrentLevelNumber() const;
	// returns next level number. Add +1 to current level
	int GetNextLevelNumber() const;
	// move ui elements (restart, pause button for now)
	void MoveUIElementsInDirection(UIElementsMoveDirection direction);
	// controls touch of towers (when game is paused etc..)
	void SetTowersTouchMode(bool is_touch_enabled);
	// remove sprites after animation ends
	void RemoveSpriteFromSpriteSheet(CCNode* sender, void* data);
	// removes menu and items (i.e. pause menu items ....)
	void RemoveObjectFromParent(CCNode* sender, void* data);
	/**
	* calculates final score based on played time 
	*/
	int CalculateFinalScoreForTime(const float game_timer) const; 
	/** 
	* calculates final score in percentage where 100% means 
	* player completed the level in 0 second and got full 
	* final score = 4000 (actually impossible :D) 
	* We need a percentage for "stars", where 
	*			0=<x<=55		== *
	*			55=<x<=75		== **
	*			75=<x<=100		== ***
	*/
	int CalcualtePercentageForScore(const int final_score) const;
	/**
	* returns score multiplier value. For the chapter 1- 2 == 100,
	* and for the chapter 3-4 == 10 (eg. x = game_timer * score_mult
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
	bool IsTouchOnObject(CCTouch* touch, CCSprite* object);
	CCPoint ConvertTouchToPoint(CCTouch* touch);

	virtual void ccTouchesBegan(CCSet* pTouches, CCEvent* pEvent);
	virtual void ccTouchesMoved(CCSet* pTouches, CCEvent* pEvent);
	virtual void ccTouchesEnded(CCSet* pTouches, CCEvent* pEvent);

	// used in Android back button
	virtual void keyBackClicked();
	/////////////////////////////////////////////////////////////////////////////////
	// 
	/////////////////////////////////////////////////////////////////////////////////
	static ECGameScene* instance_of_gamelayer_;

	CCSize screen_size_;

	// collections - vectors
	std::vector<CCSprite*> towers_;					/*!< vector container that stores all the towers in a level */
	std::vector<CCSprite*> buildings_;				/*!< vector container that stores all the buildings(houses) in a level */
	std::vector<CCSprite*> buildings_with_on_lights_;
	std::vector<CCSprite*> lines_;					/*!< vector container that stores all the lines connected to towers in a level */
	std::vector<CCSprite*> lines_shadow_;

	CCSpriteBatchNode* game_layer_spritesheet_;		/*!< main game spritesheet that contains all the sprites(background,
													buildings, towers, line, pause - win state sprites, UI (restart, pause)
													elements of a level */

	// game data loaded from xml file
	ECDataProviderExt* game_level_data_; /*!< data provider for a level. Load xml file and provides access to node element, attributs*/

	// string object representing a current level of the game
	std::string current_level_;
	// game timer fields
	float game_timer_;
	/** 
	* variable that is assigned to final score of a level in win state and 
	* is intended for using in LabelUpdateTimer method, where temp_game_score_ is updated until it equals or
	* greater than this variable (= final game score of level)
	*/
	float final_game_score_;	
	/**
	* temp variables
	*/
	float temp_final_game_score_;
	CCLabelBMFont* temp_label_;  /*! < a temporary label that used in LabelUpdateTimer method to create timer like effect (animation only)
								 on score and best_score labels */
	int on_light_building_counter_;
	//ECAudioManager* audio_manager_;
	// game state
	GameState game_state_;
};	
#endif// __EC_GAME_SCENE_H__
