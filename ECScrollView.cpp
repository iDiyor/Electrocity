//  Copyright (c) 2015 Diyor Islomov. All rights reserved.
//	diyor.islomov@gmail.com || @iDiyor 
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
		_scroll_touch = NULL;

		_steal_touches = true;
		_minimum_touch_length_to_slide = 30.0f;
		_minimum_touch_lenght_to_change_page = 100.0f;

		_margin_off_set = CCDirector::sharedDirector()->getWinSize().width;

		_show_page_indicator = true;
		_pages_indicator_position = ccp(0.5f * this->getContentSize().width, ceilf(this->getContentSize().height / 8.0f));
		_pages_indicator_normal_color = ccc4(0x96, 0x96, 0x96, 0xFF);
		_pages_indicator_selected_color = ccc4(0xFF, 0xFF, 0xFF, 0xFF);

		_current_screen = 0;
		
		_pages_width_offset = widthOffset;

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
			layer->setPosition(ccp((i * (this->getContentSize().width - _pages_width_offset)), 0));
			if (!layer->getParent())
				this->addChild(layer);
			i++;
		}
	}
}
void ECScrollLayer::visit()
{
	CCLayer::visit();
	if (_show_page_indicator) {
		int totalScreens = this->getTotalScreens();

		float n = (float)totalScreens;
		float pY = _pages_indicator_position.y;
		float d = 16.0f * CC_CONTENT_SCALE_FACTOR(); // distance between points;
		CCPoint* points = new CCPoint[totalScreens];  // 
		for (int i = 0; i < totalScreens; ++i)
		{
			float pX = _pages_indicator_position.x + d * ((float)i - 0.5f*(n-1.0f));
			CCPoint point = ccp(pX, pY);
			points[i] = ccp(pX, pY);
		}

		//GL
		ccDrawColor4B(_pages_indicator_normal_color.r,
					  _pages_indicator_normal_color.g,
					  _pages_indicator_normal_color.b,
					  _pages_indicator_normal_color.a);
		ccPointSize(7 * CC_CONTENT_SCALE_FACTOR());
		ccDrawPoints(points,totalScreens);

		ccDrawColor4B(_pages_indicator_selected_color.r,
					  _pages_indicator_selected_color.g,
					  _pages_indicator_selected_color.b,
					  _pages_indicator_selected_color.a);
		ccPointSize(5 * CC_CONTENT_SCALE_FACTOR());
		ccDrawPoint(points[_current_screen]);
		delete[] points;	
	}
}
void ECScrollLayer::moveToPageEnded()
{
	if (_prev_screen != _current_screen) {
		if (_delegate != NULL)
			_delegate->scrollLayerToPageNumber(this, _current_screen);
	}
	_prev_screen = _current_screen = this->pageNumberForPosition(this->getPosition());
}
int ECScrollLayer::pageNumberForPosition(CCPoint position)
{
	float pageFloat = -this->getPosition().x / (this->getContentSize().width - _pages_width_offset);
	int pageNumber = ceilf(pageFloat);
	if ((float)pageNumber - pageFloat >= 0.5f)
		pageNumber--;

	pageNumber = MAX(0, pageNumber);
	pageNumber = MIN(_layers->count() - 1, pageNumber);
	return pageNumber;
}
CCPoint ECScrollLayer::positionForPageWithNumber(int pageNumber)
{
	return ccp(-pageNumber * (this->getContentSize().width - _pages_width_offset), 0.0f);
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
	_current_screen = page;
}
void ECScrollLayer::selectPage(int page)
{
	if (page < 0 || page >= _layers->count()) {
		CCLOGERROR("ECScrollLayer::moveToPage: %d - wrong page number, out of bounds. ", page);
		return;
	}

	this->setPosition(this->positionForPageWithNumber(page));
	_prev_screen = _current_screen;
	_current_screen = page;
}
int ECScrollLayer::getCurrentPage() {
	return _current_screen;
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
	this->moveToPage(_current_screen);
}
void ECScrollLayer::removePage(CCLayer* page)
{
	_layers->removeObject(page);
	this->removeChild(page,true);

	this->updatePages();

	_prev_screen = _current_screen;
	_current_screen = MIN(_current_screen, _layers->count() - 1);
	this->moveToPage(_current_screen);
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
	if (_scroll_touch == NULL) {
		_scroll_touch = pTouch;
	} 
	else {
		return false;
	}
		
	CCPoint touchPoint = pTouch->getLocationInView();
	touchPoint = CCDirector::sharedDirector()->convertToGL(touchPoint);

	_start_swipe = touchPoint.x;
	_state = kECScrollLayerStateIdle;
	return true;
}
void ECScrollLayer::ccTouchMoved(CCTouch* pTouch, CCEvent* pEvent)
{
	if (_scroll_touch != pTouch) {
		return;
	}

	CCPoint touchPoint = pTouch->getLocationInView();
	touchPoint = CCDirector::sharedDirector()->convertToGL(touchPoint);

	if ((_state != kECScrollLayerStateSliding)
		&& (fabsf(touchPoint.x - _start_swipe) >= _minimum_touch_length_to_slide))
	{
		_state = kECScrollLayerStateSliding;

		_start_swipe = touchPoint.x;
	}

	if (_delegate != NULL) {
			_delegate->scrollLayerScrollingStarted(this);
	}

	if (_state == kECScrollLayerStateSliding)
	{
		float desiredX = (-_current_screen * (this->getContentSize().width - _pages_width_offset)) + touchPoint.x - _start_swipe;
		int page = this->pageNumberForPosition(ccp(desiredX,0));
		float offSet = desiredX - this->positionForPageWithNumber(page).x;
		if ((page == 0 && offSet > 0) || (page == _layers->count() - 1 && offSet < 0)) 
		{
			offSet -= _margin_off_set * offSet / CCDirector::sharedDirector()->getWinSize().width;
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
	if (_scroll_touch != pTouch) {
		return;
	}
	_scroll_touch = NULL;

	CCPoint touchPoint = pTouch->getLocationInView();
	touchPoint = CCDirector::sharedDirector()->convertToGL(touchPoint);

	int selectedPage = _current_screen;
	float delta = touchPoint.x - _start_swipe;
	if (fabsf(delta) >= _minimum_touch_lenght_to_change_page)
	{
		selectedPage = this->pageNumberForPosition(this->getPosition());
		if (selectedPage == _current_screen) {
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
	if (_scroll_touch == pTouch) {
		_scroll_touch = NULL;
		this->selectPage(_current_screen);
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





