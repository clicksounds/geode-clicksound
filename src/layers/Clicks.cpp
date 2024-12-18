#include <Geode/Geode.hpp>
#include <Geode/modify/PlayerObject.hpp>
#include <chrono>
#include <string>
#include <matjson.hpp>

#include "../utils/ee.hpp"
#include "../utils/Clicks.hpp"
#include "../settings/ClickTypes.hpp"

using namespace geode::prelude;

auto oldTime = std::chrono::steady_clock::now();

bool isLessThanOneSecond(auto time) {
      oldTime = std::chrono::steady_clock::now(); // resets the time for ya know
     if (Mod::get()->getSettingValue<bool>("decrease-volume-on-spam-click")) {
        return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - time).count() < 500;
     }
     return false;
}

float calculateVolumeMultiplier() { // ARE YOU CLICKING????
    if ( isLessThanOneSecond(oldTime) ) {
        return 0.3f; // a number i picked
    } else {
        return 1.0f; 
    };
}



class $modify(PlayerObject) {

public:
  
  // CLICK SOUNDS CLICK SOUNDS CLICK SOUNDS CLICK SOUNDS CLICK SOUNDS CLICK SOUNDS CLICK SOUNDS CLICK SOUNDS CLICK SOUNDS CLICK SOUNDS CLICK SOUNDS CLICK SOUNDS CLICK SOUNDS CLICK SOUNDS CLICK SOUNDS

  void pushButton(PlayerButton p0) {
    PlayerObject::pushButton(p0);

  if (Mod::get()->getSettingValue<bool>("only-on-jump")) {
      if (p0 != PlayerButton::Jump) {
        return;
      }
    }
    
    if (!GameManager::sharedState()->getPlayLayer() && !GameManager::sharedState()->getEditorLayer()) return;
    if (PlayLayer::get()) if (this == PlayLayer::get()->m_player2 && !PlayLayer::get()->m_level->m_twoPlayerMode) return;
    if (LevelEditorLayer::get()) {if (this == LevelEditorLayer::get()->m_player2 && !LevelEditorLayer::get()->m_level->m_twoPlayerMode) return;}
    
    matjson::Value settings = Mod::get()->getSettingValue<CRTypeStruct>("clicksound-type").m_type;
    
    if (Mod::get()->getSettingValue<bool>("enable-clicksound") && settings["tab"] != 3) {
      auto fae = FMODAudioEngine::sharedEngine();
      auto system = fae->m_system;

      FMOD::Channel* channel;
      FMOD::Sound* sound;
      
      if (settings["tab"] == 1){
        // system->createSound((Mod::get()->getResourcesDir().parent_path() / Clicks::getClickSprite(settings["click"].as<int>())).string().c_str(), FMOD_DEFAULT, nullptr, &sound);
      } else if (settings["tab"] == 2) {
        // system->createSound((Mod::get()->getResourcesDir().parent_path() / Clicks::getClickSprite(settings["memeClick"].as<int>())).string().c_str(), FMOD_DEFAULT, nullptr, &sound);
      }


      if (Mod::get()->getSettingValue<bool>("use-sfx-volume")) {
        #if defined(GEODE_IS_WINDOWS)
          if (fae->m_sfxVolume != 0) system->playSound(sound, nullptr, false, &channel);
          channel->setVolume( (fae->m_sfxVolume*2.f ) * calculateVolumeMultiplier() );
        #else
          if (Mod::get()->getSettingValue<int64_t>("volume-slider") != 0) system->playSound(sound, nullptr, false, &channel);
          channel->setVolume( (Mod::get()->getSettingValue<int64_t>("volume-slider")/50.f )* calculateVolumeMultiplier() );
        #endif
      } else if (Mod::get()->getSettingValue<int64_t>("volume-slider") != 0) { system->playSound(sound, nullptr, false, &channel);
        channel->setVolume( (Mod::get()->getSettingValue<int64_t>("volume-slider")/50.f) * calculateVolumeMultiplier() );
      } else return;
    }

    if (Mod::get()->getSettingValue<bool>("enable-clicksound") && settings["tab"] == 3) {
      auto fae = FMODAudioEngine::sharedEngine();
      auto system = fae->m_system;

      FMOD::Channel* channel;
      FMOD::Sound* sound;

      system->createSound(settings["customClick"].as<std::string>().c_str(), FMOD_DEFAULT, nullptr, &sound);
      
      if (Mod::get()->getSettingValue<bool>("use-sfx-volume")) {
        #if defined(GEODE_IS_WINDOWS)
          if (fae->m_sfxVolume != 0) system->playSound(sound, nullptr, false, &channel);
          channel->setVolume( (fae->m_sfxVolume*2.f) * calculateVolumeMultiplier() );
        #else
          if (Mod::get()->getSettingValue<int64_t>("volume-slider") != 0) system->playSound(sound, nullptr, false, &channel);
          channel->setVolume( (Mod::get()->getSettingValue<int64_t>("volume-slider")/50.f) * calculateVolumeMultiplier() );
        #endif
      } else {
        if (Mod::get()->getSettingValue<int64_t>("volume-slider") != 0) system->playSound(sound, nullptr, false, &channel);
        channel->setVolume( (Mod::get()->getSettingValue<int64_t>("volume-slider")/50.f) * calculateVolumeMultiplier() );
      }
    }
    if(!Mod::get()->getSettingValue<bool>("enable-clicksound") && !Mod::get()->getSettingValue<bool>("enable-releasesound")){}else{Carrot::carrot=true;}
  }
  
  // RELEASE SOUNDS RELEASE SOUNDS RELEASE SOUNDS RELEASE SOUNDS RELEASE SOUNDS RELEASE SOUNDS RELEASE SOUNDS RELEASE SOUNDS RELEASE SOUNDS RELEASE SOUNDS RELEASE SOUNDS RELEASE SOUNDS RELEASE SOUNDS 
  
 void releaseButton(PlayerButton p0) {
    PlayerObject::releaseButton(p0);

  if (Mod::get()->getSettingValue<bool>("only-on-jump")) {
      if (p0 != PlayerButton::Jump) {
        return;
      }
    }
    
    
    if (!GameManager::sharedState()->getPlayLayer() && !GameManager::sharedState()->getEditorLayer()) return;

    if (PlayLayer::get()) if (this == PlayLayer::get()->m_player2 && !PlayLayer::get()->m_level->m_twoPlayerMode) return;
    if (LevelEditorLayer::get()) if (this == LevelEditorLayer::get()->m_player2 && !LevelEditorLayer::get()->m_level->m_twoPlayerMode) return;

    matjson::Value settings = Mod::get()->getSettingValue<CRTypeStruct>("releasesound-type").m_type;
 
    if (Mod::get()->getSettingValue<bool>("enable-releasesound") && settings["tab"] != 3) {
      auto fae = FMODAudioEngine::sharedEngine();
      auto system = fae->m_system;

      FMOD::Channel* channel;
      FMOD::Sound* sound;

      if (settings["tab"] == 1){
        // system->createSound((Mod::get()->getResourcesDir().parent_path() / Clicks::getReleaseSprite(settings["click"].as<int>())).string().c_str(), FMOD_DEFAULT, nullptr, &sound);
      } else if (settings["tab"] == 2) {
        // system->createSound((Mod::get()->getResourcesDir().parent_path() / Clicks::getReleaseSprite(settings["memeClick"].as<int>())).string().c_str(), FMOD_DEFAULT, nullptr, &sound);
      }
      
      if (Mod::get()->getSettingValue<bool>("use-sfx-volume")) {
        #if defined(GEODE_IS_WINDOWS)
          if (fae->m_sfxVolume != 0) system->playSound(sound, nullptr, false, &channel);
          channel->setVolume((fae->m_sfxVolume*2.f) * calculateVolumeMultiplier() );
        #else
          if (Mod::get()->getSettingValue<int64_t>("volume-slider") != 0) system->playSound(sound, nullptr, false, &channel);
          channel->setVolume( (Mod::get()->getSettingValue<int64_t>("volume-slider")/50.f) * calculateVolumeMultiplier() );
        #endif
      } else {
        if (Mod::get()->getSettingValue<int64_t>("volume-slider") != 0) system->playSound(sound, nullptr, false, &channel);
        channel->setVolume( (Mod::get()->getSettingValue<int64_t>("volume-slider")/50.f ) * calculateVolumeMultiplier() );
      }
    }

    if (Mod::get()->getSettingValue<bool>("enable-releasesound") && settings["tab"] == 3) {
      auto fae = FMODAudioEngine::sharedEngine();
      auto system = fae->m_system;

      FMOD::Channel* channel;
      FMOD::Sound* sound;

      system->createSound(settings["customClick"].as<std::string>().c_str(), FMOD_DEFAULT, nullptr, &sound);
      
      if (Mod::get()->getSettingValue<bool>("use-sfx-volume")) {
        #if defined(GEODE_IS_WINDOWS)
          if (fae->m_sfxVolume != 0) system->playSound(sound, nullptr, false, &channel);
          channel->setVolume(fae->m_sfxVolume*2.f);
        #else
          if (Mod::get()->getSettingValue<int64_t>("volume-slider") != 0) system->playSound(sound, nullptr, false, &channel);
          channel->setVolume((Mod::get()->getSettingValue<int64_t>("volume-slider")/50.f)* calculateVolumeMultiplier());
        #endif
      } else {
        if (Mod::get()->getSettingValue<int64_t>("volume-slider") != 0) system->playSound(sound, nullptr, false, &channel);
        channel->setVolume((Mod::get()->getSettingValue<int64_t>("volume-slider")/50.f) * calculateVolumeMultiplier());
      }
    }
    if(!Mod::get()->getSettingValue<bool>("enable-clicksound") && !Mod::get()->getSettingValue<bool>("enable-releasesound")){}else{Carrot::carrot=true;}
 }
};
