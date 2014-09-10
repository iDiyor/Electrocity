#ifndef __MAIN_MENU_H__
#define __MAIN_MENU_H__

#include "cocos2d.h"

using namespace cocos2d;

enum {
	kPlayButton,
};


class ECMainMenuLayer : public CCLayer
{
public:

	virtual bool init();
	static CCScene* scene();
	CREATE_FUNC(ECMainMenuLayer);

	ECMainMenuLayer();
	~ECMainMenuLayer();

private:

	void onButtonClicked(CCObject* pSender);
};


#endif __MAIN_MENU_H__
