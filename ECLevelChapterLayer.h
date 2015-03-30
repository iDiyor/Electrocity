//  Copyright (c) 2015 Diyor Islomov. All rights reserved.
//	diyor.islomov@gmail.com || @iDiyor 
#ifndef __EC_LEVEL_CHAPTER_LAYER_H__
#define __EC_LEVEL_CHAPTER_LAYER_H__

#include <string>
#include <iostream>
#include <map>
#include <vector>

#include "cocos2d.h"

using namespace cocos2d;

class ECLevelChapterLayer : public CCLayer
{
public:

	bool InitWithBackgroundImage(const char* background_img_filename, const int page_number);
	static ECLevelChapterLayer* CreateWithBackgroundImage(const char* background_img_filename, const int page_number);

	ECLevelChapterLayer();
	~ECLevelChapterLayer();

	void SelectLevel(CCObject* sender);

	void CreateLevelSelectButtons(int row, 
								  int col, 
								  int number_order_from, 
								  std::vector<int>& v_open_levels,
								  std::map<int,int>& mp_played_with_stars_buttons);

private:

	CCSize screen_size_;
};
#endif //__EC_LEVEL_CHAPTER_LAYER_H__