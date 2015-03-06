#ifndef __EC_AUDIO_MANAGER_H__ 
#define __EC_AUDIO_MANAGER_H__

#include "cocos2d.h"
#include "SimpleAudioEngine.h"

using namespace cocos2d;
using namespace CocosDenshion;

 typedef enum {
    MAIN_MENU_SCENE_AUDIO,
    GAME_SCENE_AUDIO,
  }SCENE;

class ECAudioManager {
public:

  static ECAudioManager* CreateAudioManagerForScene(SCENE scene);
  bool init(SCENE scene);
 
  ECAudioManager();
  ~ECAudioManager();

  void PlayBackgroundMusic();
  void StopBackgroundMusic();
  void PlayButtonClickSound();
  void PlayActionWinSound();
  void PlayActionCompletedSound();

  void MusicSetting(bool is_enabled);
  void SoundSetting(bool is_enabled);

  SCENE scene_;
  SimpleAudioEngine* audio_manager_;
  bool is_music_on_;
  bool is_sound_on_;

};

#endif //__EC_AUDIO_MANAGER_H__
