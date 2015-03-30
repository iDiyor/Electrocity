#include "ECSettingsScene.h"

ECSettingsScene::ECSettingsScene(){

}

ECSettingsScene::~ECSettingsScene(){

}

CCScene* ECSettingsScene::scene() {
  CCScene* scene = NULL;
  do {
		scene = CCScene::create();
		CC_BREAK_IF(!scene);

		ECSettingsScene* layer = ECSettingsScene::create();
		CC_BREAK_IF(!layer);

		scene->addChild(layer);
    
  }while(0);
  return scene;
}

bool ECSettingsScene::init() {
  bool is_success = false;
  do {
		CC_BREAK_IF(!CCLayer::init());
    
		CCSize screen_size = CCDirector::sharedDirector()->getWinSize();
    
		CCLabelTTF* label = CCLabelTTF::create("Settings", "Arial", 24);
		label->setPosition(ccp(100,100));
		this->addChild(label);





		is_success = true;   

  }while(0);
  return is_success;
}
