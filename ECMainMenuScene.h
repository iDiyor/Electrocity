//  Copyright (c) 2015 Diyor Islomov. All rights reserved.
//	diyor.islomov@gmail.com || @iDiyor 
#ifndef __EC_MAIN_MENU_H__
#define __EC_MAIN_MENU_H__

#include "cocos2d.h"

using namespace cocos2d;

class ECAudioManager;

class ECMainMenuLayer : public CCLayer
{
public:

	bool init();
	static CCScene* scene();
	CREATE_FUNC(ECMainMenuLayer);

	ECMainMenuLayer();
	~ECMainMenuLayer();

private:

	enum {
			T_PLAY,
			T_SETTINGS,
			T_LEADERBOARDS,
			T_PSL_MENU, //PLAY - SETTINGS - LEADERBOARDS button container menu tag
			T_TRANSPARENT_BLACK_LAYER,
			T_EXIT_DIALOG,
			T_EXIT_DIALOG_YES,
			T_EXIT_DIALOG_NO
	};

	/*
	* General method for all buttons in main menu
	* Play
	* Settings
	*/
	void onButtonClicked(CCObject* sender);
	// 
	void ShowExitDialog();
	void OnExitDialogButtonClicked(CCObject* sender);

	// used in Android back button
	virtual void keyBackClicked();

	CCSize screen_size_;
	//ECAudioManager* audio_manager_;
};


#endif //__EC_MAIN_MENU_H__
