//  Copyright (c) 2015 Diyor Islomov. All rights reserved.
//	diyor.islomov@gmail.com || @iDiyor 
#include "ECDataProvider.h"

#define BEST_TIME_KEY "BEST_TIME"
#define BEST_SCORE_KEY "BEST_SCORE"
#define GENERAL_SCORE_KEY "GENERAL_SCORE"
#define MUSIC_KEY "MUSIC_SETTING"
#define SOUND_KEY "SOUND_SETTING"
#define LAUNCH_COUNTER_KEY "LUANCH_COUNT"

void ECDataProvider::SetBestTimeForLevel(const float time, const std::string& level) {
	std::string key_string = level + "_" + BEST_TIME_KEY;	// level1_BEST_TIME
	if ((GetBestTimeForLevel(level) == 0.0f) || 
		(time < GetBestTimeForLevel(level))) {						/*! < call GetBestTimeForLevel("arg") with passed "level" parameter 
																		 * because the method has its own string builder(eg. level1_BEST_TIME)*/
		CCUserDefault::sharedUserDefault()->setFloatForKey(key_string.c_str(), time);	
	}
}
void ECDataProvider::SetBestScoreForLevel(const int score, const std::string& level) {
	std::string key_string = level + "_" + BEST_SCORE_KEY;	// level1_BEST_SCORE
	if ((GetBestScoreForLevel(level) == 0) ||
		(score > GetBestScoreForLevel(level))) {
			CCUserDefault::sharedUserDefault()->setIntegerForKey(key_string.c_str(), score);
	}
}
void ECDataProvider::SetGeneralScore(const int score) {
	
	// request to delete score
	if (score == 0)
		CCUserDefault::sharedUserDefault()->setIntegerForKey(GENERAL_SCORE_KEY, score);
	
	if ((GetGeneralScore() == 0) ||
		(GetGeneralScore() > 0)) {
			int old_score = GetGeneralScore();
			int new_score = old_score + score;
			CCUserDefault::sharedUserDefault()->setIntegerForKey(GENERAL_SCORE_KEY, new_score);
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
float ECDataProvider::GetBestTimeForLevel(const std::string& level) {
	std::string key_string = level + "_" + BEST_TIME_KEY; // string builder
	return CCUserDefault::sharedUserDefault()->getFloatForKey(key_string.c_str());
}
int ECDataProvider::GetBestScoreForLevel(const std::string& level) {
	std::string key_string = level + "_" + BEST_SCORE_KEY;
	return CCUserDefault::sharedUserDefault()->getIntegerForKey(key_string.c_str());
}
int ECDataProvider::GetGeneralScore() {
	return CCUserDefault::sharedUserDefault()->getIntegerForKey(GENERAL_SCORE_KEY);
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