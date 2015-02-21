#include "ECScrollView.h"

ECScrollLayer* ECScrollLayer::createWithLayersWithOffset(CCArray* layers, int widthOffset)
{
	ECScrollLayer* layer = new ECScrollLayer();
	if (layer && layer->initWithLayersWithOffset(layers, widthOffset)) {
		layer->autorelease();
		return layer;
	}
	else {
		delete layer;
		layer = NULL;
		return NULL;
	}
}
bool ECScrollLayer::initWithLayersWithOffset(CCArray* layers, int widthOffset)
{
	bool isSuccuess = false;
	do
	{
		CC_BREAK_IF(!CCLayer::init());

		CCAssert(layers->count(), "ECScrollLayer::initWithLayersWithOffset: you must provide at least one layer!");

		this->setTouchEnabled(true);
		this->setVisible(true);
		
		_delegate = NULL;
		_scrollTouch = NULL;

		_stealTouches = true;
		_minimumTouchLengthToSlide = 30.0f;
		_minimumTouchLenghtToChangePage = 100.0f;

		_marginOffSet = CCDirector::sharedDirector()->getWinSize().width;

		_showPageIndicator = true;
		_pagesIndicatorPosition = ccp(0.5f * this->getContentSize().width, ceilf(this->getContentSize().height / 8.0f));
		_pagesIndicatorNormalColor = ccc4(0x96, 0x96, 0x96, 0xFF);
		_pagesIndicatorSelectedColor = ccc4(0xFF, 0xFF, 0xFF, 0xFF);

		_currentScreen = 0;
		
		_pagesWidthOffset = widthOffset;

		_layers = layers;

		this->updatePages();

		isSuccuess = true;
	}while(0);
	return isSuccuess;
}
void ECScrollLayer::updatePages()
{
	int i = 0;
	CCObject* obj = NULL;
	CCARRAY_FOREACH(_layers, obj) {
		CCLayer* layer = dynamic_cast<CCLayer*>(obj);
		if (layer) {
			layer->setAnchorPoint(ccp(0,0));
			layer->setContentSize(CCDirector::sharedDirector()->getWinSize());
			layer->setPosition(ccp((i * (this->getContentSize().width - _pagesWidthOffset)), 0));
			if (!layer->getParent())
				this->addChild(layer);
			i++;
		}
	}
}
void ECScrollLayer::visit()
{
	CCLayer::visit();
	if (_showPageIndicator) {
		int totalScreens = this->getTotalScreens();

		float n = (float)totalScreens;
		float pY = _pagesIndicatorPosition.y;
		float d = 16.0f; // distance between points;
		CCPoint* points = new CCPoint[totalScreens];  // 
		for (int i = 0; i < totalScreens; ++i)
		{
			float pX = _pagesIndicatorPosition.x + d * ((float)i - 0.5f*(n-1.0f));
			CCPoint point = ccp(pX, pY);
			points[i] = ccp(pX, pY);
		}

		//GL
		ccDrawColor4B(_pagesIndicatorNormalColor.r,
					  _pagesIndicatorNormalColor.g,
					  _pagesIndicatorNormalColor.b,
					  _pagesIndicatorNormalColor.a);
		ccPointSize(7 * CC_CONTENT_SCALE_FACTOR());
		ccDrawPoints(points,totalScreens);

		ccDrawColor4B(_pagesIndicatorSelectedColor.r,
					  _pagesIndicatorSelectedColor.g,
					  _pagesIndicatorSelectedColor.b,
					  _pagesIndicatorSelectedColor.a);
		ccPointSize(5 * CC_CONTENT_SCALE_FACTOR());
		ccDrawPoint(points[_currentScreen]);
		delete[] points;	
	}
}
void ECScrollLayer::moveToPageEnded()
{
	if (_prevScreen != _currentScreen) {
		if (_delegate != NULL)
			_delegate->scrollLayerToPageNumber(this, _currentScreen);
	}
	_prevScreen = _currentScreen = this->pageNumberForPosition(this->getPosition());
}
int ECScrollLayer::pageNumberForPosition(CCPoint position)
{
	float pageFloat = -this->getPosition().x / (this->getContentSize().width - _pagesWidthOffset);
	int pageNumber = ceilf(pageFloat);
	if ((float)pageNumber - pageFloat >= 0.5f)
		pageNumber--;

	pageNumber = MAX(0, pageNumber);
	pageNumber = MIN(_layers->count() - 1, pageNumber);
	return pageNumber;
}
CCPoint ECScrollLayer::positionForPageWithNumber(int pageNumber)
{
	return ccp(-pageNumber * (this->getContentSize().width - _pagesWidthOffset), 0.0f);
}
void ECScrollLayer::moveToPage(int page)
{
	if (page < 0 || page >= _layers->count()) {
		CCLOGERROR("ECScrollLayer::moveToPage: %d - wrong page number, out of bounds. ", page);
		return;
	}
	CCMoveTo* changePage = CCMoveTo::create(0.3f, this->positionForPageWithNumber(page));
	CCCallFunc* callFunc = CCCallFunc::create(this,callfunc_selector(ECScrollLayer::moveToPageEnded));
	CCSequence* sequence = CCSequence::create(changePage,callFunc, NULL);
	this->runAction(sequence);
	_currentScreen = page;
}
void ECScrollLayer::selectPage(int page)
{
	if (page < 0 || page >= _layers->count()) {
		CCLOGERROR("ECScrollLayer::moveToPage: %d - wrong page number, out of bounds. ", page);
		return;
	}

	this->setPosition(this->positionForPageWithNumber(page));
	_prevScreen = _currentScreen;
	_currentScreen = page;
}
void ECScrollLayer::addPage(CCLayer* page)
{
	this->addPageWithNumber(page, _layers->count());
}
void ECScrollLayer::addPageWithNumber(CCLayer* layer, int pageNumber)
{
	pageNumber = MIN(pageNumber, _layers->count());
	pageNumber = MAX(pageNumber, 0);

	_layers->insertObject(layer, pageNumber);

	this->updatePages();
	this->moveToPage(_currentScreen);
}
void ECScrollLayer::removePage(CCLayer* page)
{
	_layers->removeObject(page);
	this->removeChild(page,true);

	this->updatePages();

	_prevScreen = _currentScreen;
	_currentScreen = MIN(_currentScreen, _layers->count() - 1);
	this->moveToPage(_currentScreen);
}
void ECScrollLayer::removePageWithNumber(int pageNumber)
{
	if (pageNumber >= 0 && pageNumber < _layers->count()) {
		CCLayer* layer = dynamic_cast<CCLayer*>(_layers->objectAtIndex(pageNumber));
		this->removePage(layer);
	}
}
bool ECScrollLayer::ccTouchBegan(CCTouch* pTouch, CCEvent* pEvent)
{
	if (_scrollTouch == NULL) {
		_scrollTouch = pTouch;	
	} 
	else {
		return false;
	}
		
	CCPoint touchPoint = pTouch->getLocationInView();
	touchPoint = CCDirector::sharedDirector()->convertToGL(touchPoint);

	_startSwipe = touchPoint.x;
	_state = kECScrollLayerStateIdle;
	return true;
}
void ECScrollLayer::ccTouchMoved(CCTouch* pTouch, CCEvent* pEvent)
{
	if (_scrollTouch != pTouch) {
		return;
	}

	CCPoint touchPoint = pTouch->getLocationInView();
	touchPoint = CCDirector::sharedDirector()->convertToGL(touchPoint);

	if ((_state != kECScrollLayerStateSliding)
		&& (fabsf(touchPoint.x - _startSwipe) >= _minimumTouchLengthToSlide))
	{
		_state = kECScrollLayerStateSliding;

		_startSwipe = touchPoint.x;
	}

	if (_delegate != NULL) {
			_delegate->scrollLayerScrollingStarted(this);
	}

	if (_state == kECScrollLayerStateSliding)
	{
		float desiredX = (-_currentScreen * (this->getContentSize().width - _pagesWidthOffset)) + touchPoint.x - _startSwipe;
		int page = this->pageNumberForPosition(ccp(desiredX,0));
		float offSet = desiredX - this->positionForPageWithNumber(page).x;
		if ((page == 0 && offSet > 0) || (page == _layers->count() - 1 && offSet < 0)) 
		{
			offSet -= _marginOffSet * offSet / CCDirector::sharedDirector()->getWinSize().width;
		}
		else
		{
			offSet = 0;
		}
		this->setPosition(ccp(desiredX - offSet, 0));
	}
}
void ECScrollLayer::ccTouchEnded(CCTouch* pTouch, CCEvent* pEvent)
{
	if (_scrollTouch != pTouch) {
		return;
	}
	_scrollTouch = NULL;

	CCPoint touchPoint = pTouch->getLocationInView();
	touchPoint = CCDirector::sharedDirector()->convertToGL(touchPoint);

	int selectedPage = _currentScreen;
	float delta = touchPoint.x - _startSwipe;
	if (fabsf(delta) >= _minimumTouchLenghtToChangePage)
	{
		selectedPage = this->pageNumberForPosition(this->getPosition());
		if (selectedPage == _currentScreen) {
			if (delta < 0.0f && selectedPage < _layers->count() - 1) {
				selectedPage++;
			}
			else if (delta > 0.0f && selectedPage > 0) {
				selectedPage--;
			}
		}
	}
	this->moveToPage(selectedPage);
}
void ECScrollLayer::ccTouchCancelled(CCTouch* pTouch, CCEvent* pEvent)
{
	if (_scrollTouch == pTouch) {
		_scrollTouch = NULL;
		this->selectPage(_currentScreen);
	}

}
int ECScrollLayer::getTotalScreens()
{
	return _layers->count();
}
void ECScrollLayer::registerWithTouchDispatcher()
{
	CCTouchDispatcher* dispatcher = CCDirector::sharedDirector()->getTouchDispatcher();
	int priority = kCCMenuHandlerPriority - 1;
	dispatcher->addTargetedDelegate(this, priority, false);
}
void ECScrollLayer::scrollLayerScrollingStarted(ECScrollLayer* sender)
{

}

void ECScrollLayer::scrollLayerToPageNumber(ECScrollLayer* sender, int page)
{

}





