//  Copyright (c) 2015 Diyor Islomov. All rights reserved.
//	diyor.islomov@gmail.com || @iDiyor 
#ifndef __EC_AUDIO_MANAGER_H__ 
#define __EC_AUDIO_MANAGER_H__

#include "cocos2d.h"
#include "SimpleAudioEngine.h"

using namespace cocos2d;
using namespace CocosDenshion;

 typedef enum {
    MAIN_MENU_SCENE_AUDIO,
	SETTINGS_SCENE_AUDIO,
	LEVEL_SELECT_SCENE_AUDIO,
    GAME_SCENE_AUDIO,
  }SCENE;

class ECAudioManager {
public:

  static ECAudioManager* CreateAudioManagerForScene(SCENE scene);
  bool init(SCENE scene);
 
  ECAudioManager();
  ~ECAudioManager();

  static void PlayBackgroundMusic();
  static void PlayBackgroundMusic(SCENE in_scene);
  static void StopBackgroundMusic();
  static void PlayButtonClickSound(SCENE in_scene);
  static void PlayActionWinSound();
  static void PlayActionCompletedSound();

  static void MusicSetting(bool is_enabled, SCENE in_scene);
  static void SoundSetting(bool is_enabled);

private:
  //bool is_music_on_;
  //bool is_sound_on_;

};

#endif //__EC_AUDIO_MANAGER_H__
