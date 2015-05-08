//  Copyright (c) 2015 Diyor Islomov. All rights reserved.
//	diyor.islomov@gmail.com || @iDiyor 
#include "ECDataProvider.h"
#include "ECDataProviderExt.h"
#include "ECGameHelper.h"
#include "NativeUtils.h"


#define BEST_TIME_KEY "BEST_TIME"
#define BEST_SCORE_KEY "BEST_SCORE"
#define GENERAL_SCORE_KEY "GENERAL_SCORE"
#define MUSIC_KEY "MUSIC_SETTING"
#define SOUND_KEY "SOUND_SETTING"
#define LAUNCH_COUNTER_KEY "LUANCH_COUNT"
#define LEVEL_PLAYED_KEY "_PLAYED" // level_name + _PLAYED = level1_PLAYED
#define CHAPTER_SELECT_PAGE "CHAPTER_PAGE"

void ECDataProvider::SetBestTimeForLevel(const char* level, const float time) {
	//std::string key_string = level + "_" + BEST_TIME_KEY;	// level1_BEST_TIME
	char key_char[20];
	strcpy(key_char, level);
	strcat(key_char, "_");
	strcat(key_char, BEST_TIME_KEY);

	if ((GetBestTimeForLevel(level) == 0.0f) || 
		(time < GetBestTimeForLevel(level))) {						/*! < call GetBestTimeForLevel("arg") with passed "level" parameter 
																		 * because the method has its own string builder(eg. level1_BEST_TIME)*/
		CCUserDefault::sharedUserDefault()->setFloatForKey(key_char, time);	
	}
}
void ECDataProvider::SetBestScoreForLevel(const char* level, const int score) {
	//std::string key_string = level + "_" + BEST_SCORE_KEY;	// level1_BEST_SCORE
	char key_char[20];
	strcpy(key_char, level);
	strcat(key_char, "_");
	strcat(key_char, BEST_SCORE_KEY);

	if ((GetBestScoreForLevel(level) == 0) ||
		(score > GetBestScoreForLevel(level))) {
			CCUserDefault::sharedUserDefault()->setIntegerForKey(key_char, score);
	}
}
void ECDataProvider::SetGeneralScore(const char* level, const int score) {
	
	// request to delete score
	if (score == 0)
		CCUserDefault::sharedUserDefault()->setIntegerForKey(GENERAL_SCORE_KEY, score);
	
	if (!GetLevelPLayed(level) || GetBestScoreForLevel(level) < score) {
		if ((GetGeneralScore() == 0) || (GetGeneralScore() > 0)) {
					int old_score = GetGeneralScore();
					int new_score = old_score + score;
					CCUserDefault::sharedUserDefault()->setIntegerForKey(GENERAL_SCORE_KEY, new_score);
					NativeUtils::submitScore(LEADERBOARD_HIGH_SCORE, new_score);
			}
	}
}
void ECDataProvider::SetLevelPLayed(const char* level, bool is_played) {
	//std::string level_key = level + LEVEL_PLAYED_KEY;
	char key_char[20];
	strcpy(key_char, level);
	strcat(key_char, "_");
	strcat(key_char, LEVEL_PLAYED_KEY);

	CCUserDefault::sharedUserDefault()->setBoolForKey(key_char, is_played);
}
void ECDataProvider::SetChapterPage(int page_number) {
	if ((page_number == 1 && !GetLevelPLayed("level16")) || 
		(page_number == 2 && !GetLevelPLayed("level31")) ||
		(page_number == 3 && !GetLevelPLayed("level46"))) {
		CCUserDefault::sharedUserDefault()->setIntegerForKey(CHAPTER_SELECT_PAGE, 0);
	} else {
		CCUserDefault::sharedUserDefault()->setIntegerForKey(CHAPTER_SELECT_PAGE, page_number);
	}
}

void ECDataProvider::SetSettingsParameter(SettingsParameter parameter, bool is_enabled) {
	switch (parameter)
	{
	case MUSIC_SETTING:
		CCUserDefault::sharedUserDefault()->setBoolForKey(MUSIC_KEY, is_enabled);
		break;
	case SOUND_SETTING:
		CCUserDefault::sharedUserDefault()->setBoolForKey(SOUND_KEY, is_enabled);
		break;
	}
}
void ECDataProvider::SetGameLaunchCounter(const int number) {
	CCUserDefault::sharedUserDefault()->setIntegerForKey(LAUNCH_COUNTER_KEY, number);
}
float ECDataProvider::GetBestTimeForLevel(const char* level) {
	//std::string key_string = level + "_" + BEST_TIME_KEY; // string builder
	char key_char[20];
	strcpy(key_char, level);
	strcat(key_char, "_");
	strcat(key_char, BEST_TIME_KEY);

	return CCUserDefault::sharedUserDefault()->getFloatForKey(key_char);
}
int ECDataProvider::GetBestScoreForLevel(const char* level) {
	//std::string key_string = level + "_" + BEST_SCORE_KEY;
	char key_char[20];
	strcpy(key_char, level);
	strcat(key_char, "_");
	strcat(key_char, BEST_SCORE_KEY);

	return CCUserDefault::sharedUserDefault()->getIntegerForKey(key_char);
}
int ECDataProvider::GetGeneralScore() {
	return CCUserDefault::sharedUserDefault()->getIntegerForKey(GENERAL_SCORE_KEY);
}
bool ECDataProvider::GetLevelPLayed(const char* level) {
	//std::string level_key = level + LEVEL_PLAYED_KEY;
	char key_char[20];
	strcpy(key_char, level);
	strcat(key_char, "_");
	strcat(key_char, LEVEL_PLAYED_KEY);

	return CCUserDefault::sharedUserDefault()->getBoolForKey(key_char, false);
}
int ECDataProvider::GetChapterPage() {
	return CCUserDefault::sharedUserDefault()->getIntegerForKey(CHAPTER_SELECT_PAGE);
}
bool ECDataProvider::GetSettingsParameter(SettingsParameter parameter) {
	bool result = false;
	switch (parameter)
	{
	case MUSIC_SETTING:
		result = CCUserDefault::sharedUserDefault()->getBoolForKey(MUSIC_KEY);
		break;
	case SOUND_SETTING:
		result = CCUserDefault::sharedUserDefault()->getBoolForKey(SOUND_KEY);
		break;
	}
	return result;
}
int ECDataProvider::GetGameLaunchCounter() {
	return CCUserDefault::sharedUserDefault()->getIntegerForKey(LAUNCH_COUNTER_KEY);
}
