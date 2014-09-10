#include "Building.h"

ECBuilding::ECBuilding()
{

}
ECBuilding::~ECBuilding()
{

}
ECBuilding* ECBuilding::createBuildingWithFileName(const char* fileName)
{
	ECBuilding* building = new ECBuilding();
	if (building && building->initBuildingWithFileName(fileName))
	{
		building->autorelease();
		return building;
	}
	else
	{
		delete building;
		building = NULL;
		return NULL;
	}
}
bool ECBuilding::initBuildingWithFileName(const char* fileName)
{
	bool _isSuccess = false;
	do
	{
		CC_BREAK_IF(!CCSprite::initWithSpriteFrameName(fileName));


		_isSuccess = true;
	}while(0);
	return _isSuccess;
}



