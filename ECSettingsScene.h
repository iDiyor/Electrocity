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
	
};

#endif //_EC_SETTINGS_SCENE_H_