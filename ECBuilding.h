#ifndef __EC_BUILDING_H__
#define __EC_BUILDING_H__

#include "cocos2d.h"

class ECTower;
class ECLine;

using namespace cocos2d;

typedef enum {
	LIGHT_OFF = 0,
	LIGHT_ON
} LightState;

class ECBuilding : public CCSprite
{
public:

	static ECBuilding* CreateBuildingWithFileName(const std::string& filename);
	bool InitBuildingWithFileName(const std::string& filename);

	ECBuilding();
	~ECBuilding();

	bool CheckCollisionWithLine(ECLine* line);
	bool CheckCollisionWithLine(ECTower* tower_a, ECTower* tower_b);
	void SetBuildingLight(LightState state);
	int GetLightState();

private:
	std::string building_image_filename_for_on_state_;
	std::string building_image_filename_for_off_state_;
	int light_state_;
};


#endif //__EC_BUILDING_H__