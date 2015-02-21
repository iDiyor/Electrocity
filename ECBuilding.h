#ifndef __EC_BUILDING_H__
#define __EC_BUILDING_H__

#include "cocos2d.h"

class ECTower;

using namespace cocos2d;

enum LIGHT_STATE
{
	LIGHT_OFF = 0,
	LIGHT_ON
};


class ECLine;

class ECBuilding : public CCSprite
{
public:

	static ECBuilding* createBuildingWithFileName(const std::string& fileName);
	bool initBuildingWithFileName(const std::string& fileName);

	ECBuilding();
	~ECBuilding();

	bool checkCollisionWithLine(ECLine* line);
	bool checkCollisionWithLine(ECTower* t1, ECTower* t2);
	void setBuildingLight(LIGHT_STATE state);
	int getLightState();

private:
	std::string buildingImageFileNameForOnState;
	std::string buildingImageFileNameForOffState;
	int lightState;
};


#endif //__EC_BUILDING_H__