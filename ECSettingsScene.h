//  Copyright (c) 2015 Diyor Islomov. All rights reserved.
//	diyor.islomov@gmail.com || @iDiyor 
#ifndef _EC_SETTINGS_SCENE_H_
#define _EC_SETTINGS_SCENE_H_

#include "cocos2d.h"

USING_NS_CC;

class ECAudioManager;

class ECSettingsScene : CCLayer
{
public:

	bool init();
	static CCScene* scene();

	CREATE_FUNC(ECSettingsScene);

	ECSettingsScene();
	~ECSettingsScene();

private:

	/**
	* Tags for toggle menu items
	* Music, about and sound
	*/
	enum {
		T_MUSIC,
		T_ABOUT,
		T_SOUND
	};

	void GoMainMenu(CCObject* sender);
	/**
	* Music - About - Sound -> MAS
	* Turns on/off music
	* Turns on/off sound effects
	* shows/hides about info board
	*/
	void OnMASSettingsChanged(CCObject* sender);
	void OnAboutClicked();
	void OnTrashcanClicked(CCObject* sender);

	CCSize screen_size_;
	CCSprite* about_board_;

	ECAudioManager* audio_manager_;
};

#endif //_EC_SETTINGS_SCENE_H_
