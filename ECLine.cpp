#include "ECLine.h"
#include "ECTower.h"

ECLine::ECLine()
{

}
ECLine::~ECLine()
{

}
bool ECLine::init()
{
	bool _isSuccess = false;
	do
	{
		CC_BREAK_IF(!CCSprite::initWithSpriteFrameName("line.png"));

		this->setAnchorPoint(ccp(0.0f, 0.5f));

		_isSuccess = true;
	}while(0);
	return _isSuccess;
}
void ECLine::resetLineBetweenTowers(ECTower* towerA, ECTower* towerB) 
{
	CCPoint towerPointA = ccp(towerA->getPosition().x - (towerA->getContentSize().width * 0.5f * 0.116f),
							  towerA->getPosition().y + (towerA->getContentSize().height * 0.37f));
	CCPoint towerPointB = ccp(towerB->getPosition().x - (towerB->getContentSize().width * 0.5f * 0.116f),
							  towerB->getPosition().y + (towerB->getContentSize().height * 0.37f));
 
	float diffX = towerPointB.x - towerPointA.x; // x difference
	float diffY = towerPointB.y - towerPointA.y; // y difference

	//float diff = ccpDistance(towerPointB, towerPointA);

	float length = sqrtf((diffX*diffX) + (diffY*diffY)); // distance between two points
	float angleInRad = atan2f(diffY, diffX); 
	float angleInDeg = CC_RADIANS_TO_DEGREES(angleInRad) * -1;

	this->setPosition(towerPointA);
	this->setRotation(angleInDeg);
	this->setScaleX(length / this->getContentSize().width);

	//_beginLinePoint = this->getPosition();
	_endLinePoint = ccpAdd(this->getPosition(), ccpMult(ccp(cos(angleInRad), sin(angleInRad)), length));
}
const CCPoint& ECLine::getLineEndPoint()
{
	return _endLinePoint;
}
