//  Copyright (c) 2015 Diyor Islomov. All rights reserved.
//	diyor.islomov@gmail.com || @iDiyor 
#ifndef __EC_LEVEL_SELECT_LAYER__
#define __EC_LEVEL_SELECT_LAYER__

#include "cocos2d.h"
#include "cocos-ext.h"

using namespace cocos2d;
using namespace cocos2d::extension;

class ECAudioManager;

class ECLevelSelectLayer : public CCLayer
{
public:

	static CCScene* scene();
	bool init();

	CREATE_FUNC(ECLevelSelectLayer);

	virtual void scrollViewDidScroll(CCScrollView* view);
	virtual void scrollViewDidZoom(CCScrollView* view);

	virtual void update(float delta);

	ECLevelSelectLayer();
	~ECLevelSelectLayer();

private:

	void GoMainMenu(CCObject* sender);

	CCSize screen_size_;
	CCScrollView* scroll_view_;
	ECAudioManager* audio_manager_;
	
};
#endif //__EC_LEVEL_SELECT_LAYER__