//  Copyright (c) 2015 Diyor Islomov. All rights reserved.
//	diyor.islomov@gmail.com || @iDiyor 
#include "ECAudioManager.h"
#include "ECDataProvider.h"

const char* MAIN_MENU_BACKGROUND_MUSIC  = "audio/music_main_menu.mp3";
const char* GAME_BACKGROUND_MUSIC 	    = "audio/music_game_scene.mp3";
const char* CLICK_PLAY 				    = "audio/click_play.wav";
const char* CLICK_SELECT 				= "audio/click_select.wav";
const char* ACTION_COMPLETED 			= "audio/action_completed.wav";
const char* ACTION_WIN 					= "audio/action_win.wav";


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
		
    is_success = true;
  }while(0);
  return is_success;
}
void ECAudioManager::PlayBackgroundMusic() {
	if (!ECDataProvider::GetSettingsParameter(MUSIC_SETTING))
		return;
	PlayBackgroundMusic(MAIN_MENU_SCENE_AUDIO);
}
void ECAudioManager::PlayBackgroundMusic(SCENE in_scene) {
	if (!ECDataProvider::GetSettingsParameter(MUSIC_SETTING))
			return;

	switch (in_scene)
	{
		case MAIN_MENU_SCENE_AUDIO:
			SimpleAudioEngine::sharedEngine()->playBackgroundMusic(MAIN_MENU_BACKGROUND_MUSIC, true);
		break;
		case SETTINGS_SCENE_AUDIO:
			SimpleAudioEngine::sharedEngine()->playBackgroundMusic(MAIN_MENU_BACKGROUND_MUSIC, true);
		break;
		case LEVEL_SELECT_SCENE_AUDIO:
		break;
		case GAME_SCENE_AUDIO:
			SimpleAudioEngine::sharedEngine()->playBackgroundMusic(GAME_BACKGROUND_MUSIC, true);
		break;
	}
}
void ECAudioManager::StopBackgroundMusic() {
	if (SimpleAudioEngine::sharedEngine()->isBackgroundMusicPlaying())
		SimpleAudioEngine::sharedEngine()->stopBackgroundMusic();
}
void ECAudioManager::PlayButtonClickSound(SCENE in_scene) {
	if (!ECDataProvider::GetSettingsParameter(SOUND_SETTING))
		return;

	switch (in_scene)
	{
	case MAIN_MENU_SCENE_AUDIO:
		SimpleAudioEngine::sharedEngine()->playEffect(CLICK_PLAY);
		break;
	case SETTINGS_SCENE_AUDIO:
		SimpleAudioEngine::sharedEngine()->playEffect(CLICK_SELECT);
	break;
	case LEVEL_SELECT_SCENE_AUDIO:
		SimpleAudioEngine::sharedEngine()->playEffect(CLICK_PLAY);
	break;
	case GAME_SCENE_AUDIO:
		SimpleAudioEngine::sharedEngine()->playEffect(CLICK_SELECT);
		break;
	}
}
void ECAudioManager::PlayActionWinSound() {
	if (ECDataProvider::GetSettingsParameter(SOUND_SETTING))
		SimpleAudioEngine::sharedEngine()->playEffect(ACTION_WIN);
}
void ECAudioManager::PlayActionCompletedSound() {
	if (ECDataProvider::GetSettingsParameter(SOUND_SETTING))
		SimpleAudioEngine::sharedEngine()->playEffect(ACTION_COMPLETED);
}
void ECAudioManager::MusicSetting(bool is_enabled, SCENE in_scene) {
	// for an instant change
	//is_music_on_ = is_enabled;

	// xml
	ECDataProvider::SetSettingsParameter(MUSIC_SETTING, is_enabled);

	if (!is_enabled) { // there is always bg music playing. Need to stop it when a user disables the music
		//CCLOG("STOP");
		StopBackgroundMusic();
	} else {
		//CCLOG("RESUME");
		PlayBackgroundMusic(in_scene);
	}
}
void ECAudioManager::SoundSetting(bool is_enabled) {
	//is_sound_on_ = is_enabled;
	ECDataProvider::SetSettingsParameter(SOUND_SETTING, is_enabled);
}
