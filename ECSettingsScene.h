#ifndef _EC_SETTINGS_SCENE_H_
#define _EC_SETTINGS_SCENE_H_

#include "cocos2d.h"

USING_NS_CC;

class ECSettingsScene : CCLayer
{
public:

	virtual bool init();
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

	CCSize screen_size_;
	CCSprite* about_board_;
};

#endif //_EC_SETTINGS_SCENE_H_