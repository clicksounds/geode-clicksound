#include <Geode/Geode.hpp>
#include <Geode/modify/PlayerObject.hpp>

#include "../utils/ee.hpp"
#include "../utils/Clicks.hpp"

using namespace geode::prelude;

class $modify(PlayerObject) {

public:
  
  // CLICK SOUNDS CLICK SOUNDS CLICK SOUNDS CLICK SOUNDS CLICK SOUNDS CLICK SOUNDS CLICK SOUNDS CLICK SOUNDS CLICK SOUNDS CLICK SOUNDS CLICK SOUNDS CLICK SOUNDS CLICK SOUNDS CLICK SOUNDS CLICK SOUNDS

  void pushButton(PlayerButton p0) {
    PlayerObject::pushButton(p0);

  if (Mod::get()->getSettingValue<bool>("OnlyOnJump")) {
      if (p0 != PlayerButton::Jump) {
        return;
      }
    }
    

    if (!GameManager::sharedState()->getPlayLayer() && !GameManager::sharedState()->getEditorLayer()) return;
    if (PlayLayer::get()) if (this == PlayLayer::get()->m_player2) return;
    if (LevelEditorLayer::get()) if (LevelEditorLayer::get()->m_player2) return;
    
    auto clickSliderValue = Mod::get()->getSettingValue<int64_t>("clicksound-currentsound");
    auto customClickSound = Mod::get()->getSettingValue<ghc::filesystem::path>("custom-clicksound").string();
    bool usingCustomClickSound;
    std::string clickSoundInUse = Clicks::getClickSprite(clickSliderValue);

    usingCustomClickSound = false;
    if (clickSliderValue != 0) usingCustomClickSound = false;
    if (clickSoundInUse == "__USECUSTOM__") usingCustomClickSound = true;
    
    if (Mod::get()->getSettingValue<bool>("enable-clicksound") && !usingCustomClickSound) {
      if (Mod::get()->getSettingValue<bool>("separate-volume")) {
        auto system = FMODAudioEngine::sharedEngine()->m_system;

        FMOD::Channel* channel;
        FMOD::Sound* sound;

        // System::createSound's first arg requires full c_string path
        system->createSound((Mod::get()->getResourcesDir().parent_path() / clickSoundInUse).string().c_str(), FMOD_DEFAULT, nullptr, &sound);
        system->playSound(sound, nullptr, false, &channel);
        channel->setVolume(Mod::get()->getSettingValue<int64_t>("volume-slider")/50.f);
      } else {
        FMODAudioEngine::sharedEngine()->playEffect(clickSoundInUse, 1.0f, 1.0f, 2.0f);
      }
    } 

    if (Mod::get()->getSettingValue<bool>("enable-clicksound") && usingCustomClickSound) {
      if (Mod::get()->getSettingValue<bool>("separate-volume")) {
        auto system = FMODAudioEngine::sharedEngine()->m_system;

        FMOD::Channel* channel;
        FMOD::Sound* sound;

        system->createSound(customClickSound.c_str(), FMOD_DEFAULT, nullptr, &sound);
        system->playSound(sound, nullptr, false, &channel);
        channel->setVolume(Mod::get()->getSettingValue<int64_t>("volume-slider")/50.f); 
      } else {
        FMODAudioEngine::sharedEngine()->playEffect(customClickSound, 1.0f, 1.0f, 2.0f);
      }
    }
    if(!Mod::get()->getSettingValue<bool>("enable-clicksound") && !Mod::get()->getSettingValue<bool>("enable-releasesound")){}else{Carrot::carrot=true;}
  }
  
  // RELEASE SOUNDS RELEASE SOUNDS RELEASE SOUNDS RELEASE SOUNDS RELEASE SOUNDS RELEASE SOUNDS RELEASE SOUNDS RELEASE SOUNDS RELEASE SOUNDS RELEASE SOUNDS RELEASE SOUNDS RELEASE SOUNDS RELEASE SOUNDS 
  
 void releaseButton(PlayerButton p0) {
    PlayerObject::releaseButton(p0);

  if (Mod::get()->getSettingValue<bool>("OnlyOnJump")) {
      if (p0 != PlayerButton::Jump) {
        return;
      }
    }
    
    
    if (!GameManager::sharedState()->getPlayLayer() && !GameManager::sharedState()->getEditorLayer()) return;
    if (PlayLayer::get()) if (this == PlayLayer::get()->m_player2) return;
    if (LevelEditorLayer::get()) if (LevelEditorLayer::get()->m_player2) return;

    auto releaseSliderValue = Mod::get()->getSettingValue<int64_t>("releasesound-currentsound");
    auto customReleaseSound = Mod::get()->getSettingValue<ghc::filesystem::path>("custom-releasesound").string();
    bool usingCustomReleaseSound;
    std::string releaseSoundInUse = Clicks::getReleaseSprite(releaseSliderValue);


    if (releaseSliderValue != 0) usingCustomReleaseSound = false;
    if (releaseSoundInUse == "__USECUSTOM__") usingCustomReleaseSound = true;
 
    if (Mod::get()->getSettingValue<bool>("enable-releasesound") && !usingCustomReleaseSound) {
      if (Mod::get()->getSettingValue<bool>("separate-volume")) {
        auto system = FMODAudioEngine::sharedEngine()->m_system;

        FMOD::Channel* channel;
        FMOD::Sound* sound;

        // System::createSound's first arg requires full c_string path
        system->createSound((Mod::get()->getResourcesDir().parent_path() / releaseSoundInUse).string().c_str(), FMOD_DEFAULT, nullptr, &sound);
        system->playSound(sound, nullptr, false, &channel);
        channel->setVolume(Mod::get()->getSettingValue<int64_t>("volume-slider")/50.f);  
      } else {
        FMODAudioEngine::sharedEngine()->playEffect(releaseSoundInUse, 1.0f, 1.0f, 2.0f);
      }
    } 

    if (Mod::get()->getSettingValue<bool>("enable-releasesound") && usingCustomReleaseSound) {
      if (Mod::get()->getSettingValue<bool>("separate-volume")) {
        auto system = FMODAudioEngine::sharedEngine()->m_system;

        FMOD::Channel* channel;
        FMOD::Sound* sound;

        system->createSound(customReleaseSound.c_str(), FMOD_DEFAULT, nullptr, &sound);
        system->playSound(sound, nullptr, false, &channel);
        channel->setVolume(Mod::get()->getSettingValue<int64_t>("volume-slider")/50.f);  
      } else {
        FMODAudioEngine::sharedEngine()->playEffect(customReleaseSound, 1.0f, 1.0f, 2.0f);
      }
    }
    if(!Mod::get()->getSettingValue<bool>("enable-clicksound") && !Mod::get()->getSettingValue<bool>("enable-releasesound")){}else{Carrot::carrot=true;}
 }
};
