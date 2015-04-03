//  Copyright (c) 2015 Diyor Islomov. All rights reserved.
//	diyor.islomov@gmail.com || @iDiyor 
#ifndef __EC_LINE_H__
#define __EC_LINE_H__

#include "cocos2d.h"

using namespace cocos2d;

class ECTower;
class ECBuilding;

class ECLine : public CCSprite
{
public:
	virtual bool init();
	CREATE_FUNC(ECLine);

	ECLine();
	~ECLine();
	
	void ResetLineBetweenTowers(ECTower* tower_a, ECTower* tower_b);
	bool CheckCollisionWithBuilding(ECBuilding* building);

	CCPoint GetLineEndPoint();
private:

	CCPoint line_start_point_;
	CCPoint line_end_point_;
};
#endif //__EC_LINE_H__