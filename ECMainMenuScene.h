#ifndef __EC_MAIN_MENU_H__
#define __EC_MAIN_MENU_H__

#include "cocos2d.h"

using namespace cocos2d;



class ECMainMenuLayer : public CCLayer
{
public:

	virtual bool init();
	static CCScene* scene();
	CREATE_FUNC(ECMainMenuLayer);

	ECMainMenuLayer();
	~ECMainMenuLayer();

private:

	enum {
	        T_PLAY,
		T_SETTINGS,
		T_LEADERBOARDS,

	};

	/*
	* General method for all buttons in main menu
	* Play
	* Settings
	*/
	void onButtonClicked(CCObject* pSender);

	CCSize screen_size_;
};


#endif //__EC_MAIN_MENU_H__
