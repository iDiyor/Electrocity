#ifndef __BUILDING_H__
#define __BUILDING_H__

#include "cocos2d.h"

using namespace cocos2d;

class ECBuilding : public CCSprite
{
public:

	static ECBuilding* createBuildingWithFileName(const char* fileName);
	bool initBuildingWithFileName(const char* fileName);

	ECBuilding();
	~ECBuilding();
};


#endif //__BUILDING_H__