#ifndef __EC_DATA_PROVIDER_EXT__
#define __EC_DATA_PROVIDER_EXT__

#include "cocos2d.h"
#include "pugixml.h"
#include <string>
#include <iostream>
#include <map>
#include <vector>

#include "ECGameHelper.h"

using namespace cocos2d;
using namespace pugi;

typedef enum {
	kImgName,
	kAmount
} NodeAttributes;

class ECDataProviderExt 
{
public:
	ECDataProviderExt(const std::string& file_name, const std::string& parent, const std::string& child_node);
	ECDataProviderExt(const std::string& file_name, const std::string& parent);
	virtual ~ECDataProviderExt();
	/**
	* Saves open file. When we need to modify some existing file content, we read, modify and save file with
	* changes
	*/
	void SaveFile();

	/**********************************
	* Methods related to game scene
	**********************************/
	void loadPositionXYOfBuildings(std::vector<CCPoint>& destVector);
	void loadPositionXYOfTowers(std::vector<CCPoint>& destVector);
	void loadNodeAttributesForNodeType(NodeType nodeType, int* imageFileNumber, int* amount, float* scale);
	bool loadLinesLoopAttribute();

	/**********************************
	* Methods related to level select scene
	* map<level number, number of stars>
	**********************************/
	void LoadDataForLevelSelectButtons(std::vector<int>& v_open_buttons, 
									   std::map<int, int>& mp_played_with_stars_buttons);
	void SetPlayedAndStarsOnLevelButton(const std::string& level, bool is_played, const int stars_number);
	void SetPlayedLevel(const std::string& level, bool is_played);
	void SetStarsNumberForLevel(const std::string& level, const int stars_number);
	/**
	* Sets block state of a level button. Used to unlock next level when
	* current level is completed. Enables next level button icon.
	*/
	void SetBlockForLevel(const std::string& level, bool is_blocked);
private:
	xml_document doc_file_;
	xml_node	level_data_;
	std::string file_name_;
};
#endif //__EC_DATA_PROVIDER_EXT__