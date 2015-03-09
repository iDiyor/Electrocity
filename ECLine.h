#ifndef __EC_LINE_H__
#define __EC_LINE_H__

#include "cocos2d.h"

using namespace cocos2d;

class ECTower;

class ECLine : public CCSprite
{
public:
	virtual bool init();
	CREATE_FUNC(ECLine);

	ECLine();
	~ECLine();
	
	void ResetLineBetweenTowers(ECTower* towerA, ECTower* towerB);

	const CCPoint& GetLineEndPoint();
private:

	//CCPoint _beginLinePoint;
	CCPoint line_end_point_;
};
#endif //__EC_LINE_H__