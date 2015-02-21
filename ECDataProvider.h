#ifndef __EC_DATA_PROVIDER_H__
#define __EC_DATA_PROVIDER_H__

#include "cocos2d.h"

USING_NS_CC;

class ECDataProvider {
public:

	/**
	*	setters - best time(in float) and best score(in int)
	*/
	static void SetBestTimeForLevel(const float time, const std::string& level);
	static void SetBestScoreForLevel(const int score, const std::string& level);
	static void SetGeneralScore(const int score);
	/**
	*	getters - best time(float) and best score(int)
	*/
	static float GetBestTimeForLevel(const std::string& level);
	static int GetBestScoreForLevel(const std::string& level);
	static int GetGeneralScore();
};
#endif //__EC_DATA_PROVIDER_H__