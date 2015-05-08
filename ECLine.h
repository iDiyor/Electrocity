//  Copyright (c) 2015 Diyor Islomov. All rights reserved.
//	diyor.islomov@gmail.com || @iDiyor 
#ifndef __EC_LINE_H__
#define __EC_LINE_H__

#include "cocos2d.h"

using namespace cocos2d;

class ECTower;
class ECBuilding;

// function of a line, whether real or acts as shadow
enum LineType {
	LINE_REAL,
	LINE_SHADOW
};

class ECLine : public CCSprite
{
public:

	virtual bool init();
	CREATE_FUNC(ECLine);

	ECLine();
	~ECLine();

	void SetLineType(LineType line_type);

	bool CheckCollisionWithBuilding(ECBuilding* building);

	void AttachLineToTowers(ECTower* tower_a, ECTower* tower_b);
	void UpdatePosition();
private:

	LineType line_type_;

	CCPoint line_head_;
	CCPoint line_tail_;

	ECTower* tower_a_;
	ECTower* tower_b_;
};
#endif //__EC_LINE_H__