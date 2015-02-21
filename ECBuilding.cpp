#include "ECBuilding.h"
#include "ECLine.h"
#include "ECTower.h"

#include <stdio.h>

ECBuilding::ECBuilding()
{
	lightState = LIGHT_OFF;
}
ECBuilding::~ECBuilding()
{

}
ECBuilding* ECBuilding::createBuildingWithFileName(const std::string& fileName)
{
	ECBuilding* _building = new ECBuilding();
	if (_building && _building->initBuildingWithFileName(fileName))
	{
		_building->autorelease();
		return _building;
	}
	else
	{
		delete _building;
		_building = NULL;
		return NULL;
	}
}
bool ECBuilding::initBuildingWithFileName(const std::string& fileName)
{
	bool _isSuccess = false;
	do
	{
		CC_BREAK_IF(!CCSprite::initWithSpriteFrameName(fileName.c_str()));

		buildingImageFileNameForOffState = fileName;

		std::string houseNameInString = buildingImageFileNameForOffState.substr(0,5);							// "house"
		std::size_t imageNumberInString = buildingImageFileNameForOffState.find_last_of("_");					// position of last "_"
		std::string imageFileNumberAndFormat = buildingImageFileNameForOffState.substr(imageNumberInString);	// get from last "_" to the end => "_number.png"
		houseNameInString.append("_on");																		// "house_on
		houseNameInString.append(imageFileNumberAndFormat);														// "house_on_number.png"

		buildingImageFileNameForOnState = houseNameInString;

		_isSuccess = true;
	}while(0);
	return _isSuccess;
}
bool ECBuilding::checkCollisionWithLine(ECLine* line)
{
	CCPoint pLineBegin = line->getPosition();
	CCPoint pLineEnd = line->getLineEndPoint();

	float scale = this->getScale();

	CCPoint pLeftBottom = ccp(this->getPosition().x - (this->getContentSize().width * (0.09375f * scale)),
										this->getPosition().y + (this->getContentSize().height * (0.269231f * scale)));
	CCPoint pLeftTop = ccp(pLeftBottom.x, pLeftBottom.y + (this->getContentSize().height * (0.15385f * scale)));
	CCPoint pRightTop = ccp(pLeftTop.x + (this->getContentSize().width * (0.125f * scale)), pLeftTop.y);
	CCPoint pRightBottom = ccp(pRightTop.x, pLeftBottom.y);	

	float s;
	float t;

	if ((ccpLineIntersect(pLineBegin, pLineEnd, pLeftBottom, pLeftTop, &s, &t) && s >= 0 && s <= 1 && t >= 0 && t <= 1) ||
		(ccpLineIntersect(pLineBegin, pLineEnd, pLeftTop, pRightTop, &s, &t) && s >= 0 && s <= 1 && t >= 0 && t <= 1) ||
		(ccpLineIntersect(pLineBegin, pLineEnd, pRightTop, pRightBottom, &s, &t) && s >= 0 && s <= 1 && t >= 0 && t <= 1) || 
		(ccpLineIntersect(pLineBegin, pLineEnd, pLeftBottom, pRightBottom, &s, &t) && s >= 0 && s <= 1 && t >= 0 && t <= 1)) 
	{
		return true;
	}
	return false;
}
bool ECBuilding::checkCollisionWithLine(ECTower* towerA, ECTower* towerB)
{
	CCPoint pLineBegin = ccp(towerA->getPosition().x - (towerA->getContentSize().width * 0.5f * 0.116f),
							  towerA->getPosition().y + (towerA->getContentSize().height * 0.37f));
	CCPoint pLineEnd = ccp(towerB->getPosition().x - (towerB->getContentSize().width * 0.5f * 0.116f),
							  towerB->getPosition().y + (towerB->getContentSize().height * 0.37f));

	float scale = this->getScale();

	CCPoint pLeftBottom = ccp(this->getPosition().x - (this->getContentSize().width * (0.09375f * scale)),
										this->getPosition().y + (this->getContentSize().height * (0.269231f * scale)));
	CCPoint pLeftTop = ccp(pLeftBottom.x, pLeftBottom.y + (this->getContentSize().height * (0.15385f * scale)));
	CCPoint pRightTop = ccp(pLeftTop.x + (this->getContentSize().width * (0.125f * scale)), pLeftTop.y);
	CCPoint pRightBottom = ccp(pRightTop.x, pLeftBottom.y);	

	float s;
	float t;

	
	if ((ccpLineIntersect(pLineBegin, pLineEnd, pLeftBottom, pLeftTop, &s, &t) && s >= 0 && s <= 1 && t >= 0 && t <= 1) ||
		(ccpLineIntersect(pLineBegin, pLineEnd, pLeftTop, pRightTop, &s, &t) && s >= 0 && s <= 1 && t >= 0 && t <= 1) ||
		(ccpLineIntersect(pLineBegin, pLineEnd, pRightTop, pRightBottom, &s, &t) && s >= 0 && s <= 1 && t >= 0 && t <= 1) || 
		(ccpLineIntersect(pLineBegin, pLineEnd, pLeftBottom, pRightBottom, &s, &t) && s >= 0 && s <= 1 && t >= 0 && t <= 1)) 
	{
		return true;
	}
	return false;
}
void ECBuilding::setBuildingLight(LIGHT_STATE state)
{
	// create on image file name
	CCSpriteFrame* buildingFrame = NULL;
	
	switch (state)
	{
	case LIGHT_OFF:
		{		
			buildingFrame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(buildingImageFileNameForOffState.c_str());
			this->setDisplayFrame(buildingFrame);
			lightState = LIGHT_OFF;
		}	
		break;
	case LIGHT_ON:
		{	
			buildingFrame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(buildingImageFileNameForOnState.c_str());
			this->setDisplayFrame(buildingFrame);
			lightState = LIGHT_ON;
		}
		break;
	}
}
int ECBuilding::getLightState()
{
	return lightState;
}