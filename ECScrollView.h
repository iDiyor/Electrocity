//  Copyright (c) 2015 Diyor Islomov. All rights reserved.
//	diyor.islomov@gmail.com || @iDiyor 
#ifndef __EC_SCROLL_VIEW__
#define __EC_SCROLL_VIEW__

#include "cocos2d.h"

using namespace cocos2d;

class ECScrollLayer;

enum 
{
	kECScrollLayerStateIdle,
	kECScrollLayerStateSliding,
};

class ECScrollViewDelegate
{
public:

	virtual ~ECScrollViewDelegate() {};
	virtual void scrollLayerScrollingStarted(ECScrollLayer* sender) = 0;
	virtual void scrollLayerToPageNumber(ECScrollLayer* sender, int page) = 0;

private:

};


class ECScrollLayer : public CCLayer , public ECScrollViewDelegate
{
public:
	
	ECScrollViewDelegate* _delegate;

	int _current_screen;
	int _prev_screen;
	float _start_swipe;
	float _minimum_touch_length_to_slide;
	float _minimum_touch_lenght_to_change_page;

	bool _show_page_indicator;
	
	CCPoint _pages_indicator_position;
	ccColor4B _pages_indicator_selected_color;
	ccColor4B _pages_indicator_normal_color;

	int _state;
	bool _steal_touches;

	CCTouch* _scroll_touch;
	
	CCArray* _layers;

	float _pages_width_offset;
	float _margin_off_set;

	//
	int getTotalScreens();

	static ECScrollLayer* createWithLayersWithOffset(CCArray* layers, int widthOffset);
	bool initWithLayersWithOffset(CCArray* layers, int widthOffset);

	void updatePages();
	void addPageWithNumber(CCLayer* layer, int pageNumber);
	void addPage(CCLayer* page);
	void removePage(CCLayer* page);
	void removePageWithNumber(int number);
	void moveToPage(int page);
	void selectPage(int page);

	int pageNumberForPosition(CCPoint position);
	void moveToPageEnded();
	CCPoint positionForPageWithNumber(int pageNumber);

	virtual void visit();

	virtual void registerWithTouchDispatcher();

	virtual bool ccTouchBegan(CCTouch* pTouch, CCEvent* pEvent);
	virtual void ccTouchMoved(CCTouch* pTouch, CCEvent* pEvent);
	virtual void ccTouchEnded(CCTouch* pTouch, CCEvent* pEvent);
	virtual void ccTouchCancelled(CCTouch* pTouch, CCEvent* pEvent);

	virtual void scrollLayerScrollingStarted(ECScrollLayer* sender);
	virtual void scrollLayerToPageNumber(ECScrollLayer* sender, int page);

private:


};



#endif //__EC_SCROLL_VIEW__
