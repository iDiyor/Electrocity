#include "ECAudioManager.h"
#include "SimpleAudioEngine.h"

using namespace CocosDenshion;

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

	  switch (scene)
	  {
	  case MAIN_MENU_SCENE:
		  {
			  audio_manager_->preloadBackgroundMusic("music_main_menu.mp3");
			  audio_manager_->preloadEffect("click_play.wav"); //play button click
															  //sound applies to all : option, leaderboards
		  }
		  break;
	  case GAME_SCENE:
		  {
			  audio_manager_->preloadBackgroundMusic("music_game_scene.mp3");
			  audio_manager_->preloadEffect("click_select.wav");
			  audio_manager_->preloadEffect("action_completed.wav");
			  audio_manager_->preloadEffect("action_win.wav");

		  }
		  break;
	  }

	  scene_ = scene;

    is_success = true;
  }while(0);
  return is_success;
}
void ECAudioManager::PlayBackgroundMusic() {
	switch (scene_)
	{
	case MAIN_MENU_SCENE:
		audio_manager_->playBackgroundMusic("music_main_menu.mp3", true);
		break;
	case GAME_SCENE:
		audio_manager_->playBackgroundMusic("music_game_scene.mp3", true);
		break;
	}
}
void ECAudioManager::StopBackgroundMusic() {
	audio_manager_->stopBackgroundMusic();
}
void ECAudioManager::PlayButtonClickSound() {

}
