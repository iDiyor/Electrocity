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

	int _currentScreen;
	int _prevScreen;
	float _startSwipe;
	float _minimumTouchLengthToSlide;
	float _minimumTouchLenghtToChangePage;

	bool _showPageIndicator;
	
	CCPoint _pagesIndicatorPosition;
	ccColor4B _pagesIndicatorSelectedColor;
	ccColor4B _pagesIndicatorNormalColor;

	int _state;
	bool _stealTouches;

	CCTouch* _scrollTouch;
	
	CCArray* _layers;

	float _pagesWidthOffset;
	float _marginOffSet;

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