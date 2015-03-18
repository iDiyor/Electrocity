//  Copyright (c) 2015 Diyor Islomov. All rights reserved.
//	diyor.islomov@gmail.com || @iDiyor 
#include "ECAudioManager.h"
#include "ECDataProvider.h"

ECAudioManager::ECAudioManager() {

}

ECAudioManager::~ECAudioManager() {

}

ECAudioManager* ECAudioManager::CreateAudioManagerForScene(SCENE scene) {
  ECAudioManager* audio_manager = new ECAudioManager();
  if (audio_manager && audio_manager->init(scene)) {
    return audio_manager; 
  } else {
    delete audio_manager;
    audio_manager = NULL;
    return NULL;
  }
}

bool ECAudioManager::init(SCENE scene) {
  bool is_success = false;
  do {
		
	  audio_manager_ = SimpleAudioEngine::sharedEngine();

	  is_music_on_ = ECDataProvider::GetSettingsParameter(MUSIC_SETTING);
	  is_sound_on_ = ECDataProvider::GetSettingsParameter(SOUND_SETTING);

	  switch (scene)
	  {
	  case MAIN_MENU_SCENE_AUDIO:
		  {
			  audio_manager_->preloadBackgroundMusic("audio/music_main_menu.mp3");
			  audio_manager_->preloadEffect("audio/click_play.wav"); //play button click
															  //sound applies to all : option, leaderboards
		  }
		  break;
	  case SETTINGS_SCENE_AUDIO:	// settings audio management is temporary solution until best approach is found (singleton)
		  //audio_manager_->preloadBackgroundMusic("audio/music_main_menu.mp3"); 
		  break;
	  case LEVEL_SELECT_SCENE_AUDIO:
		  break;
	  case GAME_SCENE_AUDIO:
		  {
			  // I have no idea whether I need to unload this or not
			  audio_manager_->unloadEffect("audio/click_play.wav");

			  audio_manager_->preloadBackgroundMusic("audio/music_game_scene.mp3");
			  audio_manager_->preloadEffect("audio/click_select.wav");
			  audio_manager_->preloadEffect("audio/action_completed.wav");
			  audio_manager_->preloadEffect("audio/action_win.wav");
		  }
		  break;
	  }

	  scene_ = scene;

    is_success = true;
  }while(0);
  return is_success;
}
void ECAudioManager::PlayBackgroundMusic() {
	if (!is_music_on_)
		return;

	CCLOG("PLAY");
	switch (scene_)
	{
		case MAIN_MENU_SCENE_AUDIO:
			audio_manager_->playBackgroundMusic("audio/music_main_menu.mp3", true);
		break;
		case SETTINGS_SCENE_AUDIO:
			audio_manager_->playBackgroundMusic("audio/music_main_menu.mp3", true);
		break;
		case LEVEL_SELECT_SCENE_AUDIO:
		break;
		case GAME_SCENE_AUDIO:
			audio_manager_->playBackgroundMusic("audio/music_game_scene.mp3", true);
		break;
	}
}
void ECAudioManager::StopBackgroundMusic() {
	if (audio_manager_->isBackgroundMusicPlaying()) 
		audio_manager_->stopBackgroundMusic();
}
void ECAudioManager::PlayButtonClickSound() {
	if (!is_sound_on_) 
		return;

	switch (scene_)
	{
	case MAIN_MENU_SCENE_AUDIO:
		audio_manager_->playEffect("audio/click_play.wav");
		break;
	case SETTINGS_SCENE_AUDIO:
		audio_manager_->playEffect("audio/click_select.wav");
	break;
	case LEVEL_SELECT_SCENE_AUDIO:
		audio_manager_->playEffect("audio/click_select.wav");
	break;
	case GAME_SCENE_AUDIO:
		audio_manager_->playEffect("audio/click_select.wav");
		break;
	}
}
void ECAudioManager::PlayActionWinSound() {
	if (is_sound_on_)
		audio_manager_->playEffect("audio/action_win.wav");
}
void ECAudioManager::PlayActionCompletedSound() {
	if (is_sound_on_)
		audio_manager_->playEffect("audio/action_completed.wav");
}
void ECAudioManager::MusicSetting(bool is_enabled) {
	// for an instant change
	is_music_on_ = is_enabled;

	if (!is_enabled) { // there is always bg music playing. Need to stop it when a user disables the music
		CCLOG("STOP");
		this->StopBackgroundMusic();
	} else {
		CCLOG("RESUME");
		this->PlayBackgroundMusic();
	}

	
	// xml
	ECDataProvider::SetSettingsParameter(MUSIC_SETTING, is_enabled);
}
void ECAudioManager::SoundSetting(bool is_enabled) {
	is_sound_on_ = is_enabled;
	ECDataProvider::SetSettingsParameter(SOUND_SETTING, is_enabled);
}
