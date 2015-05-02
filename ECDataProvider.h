//  Copyright (c) 2015 Diyor Islomov. All rights reserved.
//	diyor.islomov@gmail.com || @iDiyor 
#ifndef __EC_DATA_PROVIDER_H__
#define __EC_DATA_PROVIDER_H__

#include "cocos2d.h"

USING_NS_CC;

// settings (global)
typedef enum {
	MUSIC_SETTING,
	SOUND_SETTING,
}SettingsParameter;

class ECDataProvider {
public:
	
	/**
	*	setters - best time(in float) and best score(in int)
	*/
	static void SetBestTimeForLevel(const char* level, const float time);
	static void SetBestScoreForLevel(const char* level, const int score);
	static void SetGeneralScore(const char* level, const int score);
	static void SetLevelPLayed(const char* level, bool is_played);
	/**
	* Controls state of music and sound. Saves state values in xml file and
	* when loaded sets buttons parameter based on that.
	*/
	static void SetSettingsParameter(SettingsParameter parameter, bool is_enabled);
	/** 
	* Sets the number of game launches
	* When game is launched for the first time, settings (music,sounds...) are 
	* created and saved inside the app (xml)
	* At the moment, it is used only once. +1
	*/
	static void SetGameLaunchCounter(const int number);
	/**
	*	getters - best time(float) and best score(int)
	*/
	static float GetBestTimeForLevel(const char* level);
	static int GetBestScoreForLevel(const char* level);
	static int GetGeneralScore();
	static bool GetLevelPLayed(const char* level);
	/**
	* Returns loaded data of music, sound to set their state;
	*/
	static bool GetSettingsParameter(SettingsParameter parameter);
	/** 
	* Returns the number of game launches
	*/
	static int GetGameLaunchCounter();
};
#endif //__EC_DATA_PROVIDER_H__
