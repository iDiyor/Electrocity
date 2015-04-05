//  Copyright (c) 2015 Diyor Islomov. All rights reserved.
//	diyor.islomov@gmail.com || @iDiyor 
#ifndef __EC_DATA_PROVIDER_EXT__
#define __EC_DATA_PROVIDER_EXT__

#include "cocos2d.h"
#include "pugixml.h"
#include "ECGameHelper.h"

#include <string>
#include <iostream>
#include <map>
#include <vector>

using namespace cocos2d;
using namespace pugi;

typedef enum {
	IMAGE_FILE_NAME,
	QUANTITY
} NodeAttributes;

class ECDataProviderExt 
{
public:
	ECDataProviderExt(const std::string& file_name, const std::string& parent_node, const std::string& child_node);
	ECDataProviderExt(const std::string& file_name, const std::string& parent_node);
	virtual ~ECDataProviderExt();
	/**
	* Saves xml file in sandbox. When we need to modify some existing file content, we read, modify and save file with
	* changes. Otherwise, they are loaded in a main memory ...
	*/
	void SaveFile();


	/**********************************
	* Methods related to game scene
	**********************************/
	void LoadPositionXYOfBuildings(std::vector<CCPoint>& dest_vector);
	void LoadPositionXYOfTowers(std::vector<CCPoint>& dest_vector);
	void LoadNodeAttributesForNodeType(NodeType node_type, int* image_file_number, int* quantity, float* scale);
	bool LoadLinesLoopAttribute();

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

	static void MoveXMLFileFromAssetsToWritabalePath(std::string filename);
private:

	// returns location of file on different platforms (Win32, Android)
	std::string GetPathToFile(std::string filename);

	xml_document doc_file_;
	xml_node	level_data_;
	std::string file_name_;
};
#endif //__EC_DATA_PROVIDER_EXT__
