#include "ECDataProviderExt.h"
#include "cocos2d.h"

USING_NS_CC;

ECDataProviderExt::ECDataProviderExt(const std::string& file_name, const std::string& parent, const std::string& child_node)
{
	// method #1
	//std::string filePath = CCFileUtils::sharedFileUtils()->fullPathForFilename(fileName.c_str());
	//xml_parse_result result = doc_file_.load_file(filePath.c_str());

	// method #2
	std::string fullFilePath = CCFileUtils::sharedFileUtils()->fullPathForFilename(file_name.c_str());
	unsigned char* pBuffer = NULL;
	unsigned long bufferSize = 0;
	pBuffer = CCFileUtils::sharedFileUtils()->getFileData(fullFilePath.c_str(), "r", &bufferSize);
	
	xml_parse_result result = doc_file_.load_buffer(pBuffer, bufferSize);
	
	if (result) {
		CCLOG("%s : Loaded successfully: %s", file_name.c_str(), result.description());
		// assining loaded level and finding selected level data
		level_data_ = doc_file_.child(parent.c_str()).child(child_node.c_str());
		file_name_ = file_name;
	} else {
		CCLOG("%s : Error description: %s" , file_name.c_str() ,result.description());
	}
}
ECDataProviderExt::ECDataProviderExt(const std::string& file_name, const std::string& parent)
{
	// method #1
	//std::string filePath = CCFileUtils::sharedFileUtils()->fullPathForFilename(fileName.c_str());
	//xml_parse_result result = doc_file_.load_file(filePath.c_str());

	// method #2
	std::string fullFilePath = CCFileUtils::sharedFileUtils()->fullPathForFilename(file_name.c_str());
	unsigned char* pBuffer = NULL;
	unsigned long bufferSize = 0;
	pBuffer = CCFileUtils::sharedFileUtils()->getFileData(fullFilePath.c_str(), "r", &bufferSize);
	
	xml_parse_result result = doc_file_.load_buffer(pBuffer, bufferSize);
	
	if (result) {
		CCLOG("%s : Loaded successfully: %s", file_name.c_str(), result.description());
		// assining loaded level and finding selected level data
		level_data_ = doc_file_.child(parent.c_str());
		file_name_ = file_name;
	} else {
		CCLOG("%s : Error description: %s" , file_name.c_str() ,result.description());
	}
}
ECDataProviderExt::~ECDataProviderExt()
{

}
void ECDataProviderExt::SaveFile() {
	doc_file_.save_file(file_name_.c_str());
}
void ECDataProviderExt::loadPositionXYOfBuildings(std::vector<CCPoint>& destVector)
{
	xml_node buildings = level_data_.child("buildings");
	for (xml_node_iterator it = buildings.begin(); it != buildings.end(); ++it)
		{
			float positionX = it->attribute("pos_x").as_float();
			float positionY = it->attribute("pos_y").as_float();
			CCPoint loadedPosToPush = ccp(positionX, positionY);
			destVector.push_back(loadedPosToPush);
		}
}
void ECDataProviderExt::loadPositionXYOfTowers(std::vector<CCPoint>& destVector)
{
	xml_node towers = level_data_.child("towers");
	for (xml_node_iterator it = towers.begin(); it != towers.end(); ++it)
		{
			float positionX = it->attribute("pos_x").as_float();
			float positionY = it->attribute("pos_y").as_float();
			CCPoint loadedPosToPush = ccp(positionX, positionY);
			destVector.push_back(loadedPosToPush);
		}
}
void ECDataProviderExt::loadNodeAttributesForNodeType(NodeType nodeType, int* imageFileNumber, int* amount, float* scale)
{
	xml_node node;

	switch (nodeType)	
	{
	case kBuilding:
		node = level_data_.child("buildings");
		break;
	case kTower:
		node = level_data_.child("towers");
		break;
	}

	for (xml_attribute_iterator it = node.attributes_begin(); 
									it != node.attributes_end(); ++it)
		{
			//std::string key = it->name();	//atributues key names
			//int value		= it->as_int(); //attributes key values
			//destMap.insert(std::pair<std::string,int>(key, value));
			*imageFileNumber = node.attribute("image_file_number").as_int();
			*amount			= node.attribute("quantity").as_int();
			*scale			= node.attribute("scale").as_float();
		}
}
bool ECDataProviderExt::loadLinesLoopAttribute()
{
	xml_node lines = level_data_.child("lines");
	return lines.attribute("loop_closed").as_bool();
}
/*****************************************************************************************
*****************************************************************************************/
void ECDataProviderExt::LoadDataForLevelSelectButtons(std::vector<int>& v_open_buttons, 
													 std::map<int, int>& mp_played_with_stars_buttons) {
	int count = 1;
	for (xml_node_iterator it = level_data_.begin(); it != level_data_.end(); ++it) {
		
		count++;

		bool is_level_blocked = it->attribute("is_blocked").as_bool();
		bool is_level_played = it->attribute("is_played").as_bool();
		int	number_of_stars = it->attribute("number_of_stars").as_int();
		// open levels others by default are blocked		
		if (!is_level_blocked) {
			v_open_buttons.push_back(count);
		}
		// played and number of stars on buttons
		if (is_level_played && number_of_stars > 0) {
			mp_played_with_stars_buttons.insert(std::pair<int, int>(count, number_of_stars));
		}
		// stars on level button
		/*if (number_of_stars > 0) {
			std::string level (it->name());
			mp_stars_on_button.insert(std::pair<std::string, int>(level, number_of_stars));
			CCLog("NUMBER_OF_NODES: %s", level.c_str());
		}*/
	}
	CCLog("NUMBER_OF_NODES: %i", count);
	
	count = 1;
}
void ECDataProviderExt::SetPlayedLevel(const std::string& level, bool is_played) {
	xml_node level_node = level_data_.child(level.c_str());
	level_node.attribute("is_played").set_value(is_played);

	CCLOG("Level: %s" , level.c_str());
}
void ECDataProviderExt::SetStarsNumberForLevel(const std::string& level, const int stars_number) {
	xml_node level_node = level_data_.child(level.c_str());
	level_node.attribute("number_of_stars").set_value(stars_number);
	CCLOG("Level: %s |Stars #: %i" , level.c_str(), stars_number);
}
void ECDataProviderExt::SetPlayedAndStarsOnLevelButton(const std::string& level, bool is_played, const int stars_number) {
	this->SetPlayedLevel(level, is_played);
	this->SetStarsNumberForLevel(level, stars_number);
}
void ECDataProviderExt::SetBlockForLevel(const std::string& level, bool is_blocked) {
	xml_node level_node = level_data_.child(level.c_str());
	level_node.attribute("is_blocked").set_value(is_blocked);
}
