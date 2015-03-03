#ifndef __EC_LEVEL_SELECT_LAYER__
#define __EC_LEVEL_SELECT_LAYER__

#include "cocos2d.h"
#include "cocos-ext.h"

using namespace cocos2d;
using namespace cocos2d::extension;

class ECLevelSelectLayer : public CCLayer
{
public:

	static CCScene* scene();
	virtual bool init();

	CREATE_FUNC(ECLevelSelectLayer);

	virtual void scrollViewDidScroll(CCScrollView* view);
	virtual void scrollViewDidZoom(CCScrollView* view);

	virtual void update(float delta);

	ECLevelSelectLayer();
	~ECLevelSelectLayer();

private:

	CCSize screen_size_;

	CCScrollView* scroll_view_;

	void GoMainMenu(CCObject* sender);
};
#endif //__EC_LEVEL_SELECT_LAYER__