#ifndef __EC_AUDIO_MANAGER_H__ 
#define __EC_AUDIO_MANAGER_H__

#include "cocos2d.h"

using namespace cocos2d;

class ECAudioManager {
public:

  typedef enum {
    MAIN_MENU_SCENE,
    GAME_SCENE,
  }SCENE;

  static ECAudioManager* CreateAudioManagerForScene(SCENE scene);
  bool init(SCENE scene);
 
  ECAudioManager();
  ~ECAudioManager();

  void PlayBackgroundMusic();
  void StopBackgroundMusic();
  void PlayButtonClickSound();
  void PlayActionWinSound();
  void PlayActionCompletedSound();

  SCENE scene_;
  SimpleAudioEngine* audio_manager_;

};

#endif //__EC_AUDIO_MANAGER_H__
