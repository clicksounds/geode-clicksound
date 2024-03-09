#include <Geode/Geode.hpp>
#include <Geode/modify/PlayerObject.hpp>

#include "../utils/ee.hpp"
#include "../utils/Clicks.hpp"

using namespace geode::prelude;

template<typename T, typename U> constexpr size_t OFFSETBYMEMBER(U T::* member)
{
    return (char*)&((T*)nullptr->*member) - (char*)nullptr;
}
#define OBM(member) OFFSETBYMEMBER(member)
//std::cout << OBM(&FMODAudioEngine::m_sfxVolume) << std::endl;
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
    std::cout << OBM(&FMODAudioEngine::m_sfxVolume) << std::endl;

    if (!GameManager::sharedState()->getPlayLayer() && !GameManager::sharedState()->getEditorLayer()) return;
    if (PlayLayer::get()) if (this == PlayLayer::get()->m_player2 && !PlayLayer::get()->m_level->m_twoPlayerMode) return;
    if (LevelEditorLayer::get()) if (this == LevelEditorLayer::get()->m_player2 && !LevelEditorLayer::get()->m_level->m_twoPlayerMode) return;
    
    auto clickSliderValue = Mod::get()->getSettingValue<int64_t>("clicksound-currentsound");
    auto customClickSound = Mod::get()->getSettingValue<ghc::filesystem::path>("custom-clicksound").string();
    bool usingCustomClickSound;
    std::string clickSoundInUse = Clicks::getClickSprite(clickSliderValue);

    usingCustomClickSound = false;
    if (clickSliderValue != 0) usingCustomClickSound = false;
    if (clickSoundInUse == "__USECUSTOM__") usingCustomClickSound = true;
    
    if (Mod::get()->getSettingValue<bool>("enable-clicksound") && !usingCustomClickSound) {
      auto fae = FMODAudioEngine::sharedEngine();
      auto system = fae->m_system;

      FMOD::Channel* channel;
      FMOD::Sound* sound;

      system->createSound((Mod::get()->getResourcesDir().parent_path() / clickSoundInUse).string().c_str(), FMOD_DEFAULT, nullptr, &sound);
      system->playSound(sound, nullptr, false, &channel);

      if (Mod::get()->getSettingValue<bool>("separate-volume")) {
        channel->setVolume(Mod::get()->getSettingValue<int64_t>("volume-slider")/50.f);
        log::debug("Click Sound Volume (cs settings): {}", Mod::get()->getSettingValue<int64_t>("volume-slider"));
      } else {
        channel->setVolume(GameManager::sharedState()->m_sfxVolume*2.f);
        log::debug("Click Sound Volume (sfx slider): {}", OBM(&FMODAudioEngine::m_sfxVolume));
      }
    }

    if (Mod::get()->getSettingValue<bool>("enable-clicksound") && usingCustomClickSound) {
      auto fae = FMODAudioEngine::sharedEngine();
      auto system = fae->m_system;

      FMOD::Channel* channel;
      FMOD::Sound* sound;

      system->createSound(customClickSound.c_str(), FMOD_DEFAULT, nullptr, &sound);
      system->playSound(sound, nullptr, false, &channel);

      if (Mod::get()->getSettingValue<bool>("separate-volume")) {
        channel->setVolume(Mod::get()->getSettingValue<int64_t>("volume-slider")/50.f); 
      } else {
        channel->setVolume(GameManager::sharedState()->m_sfxVolume*2.f); 
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

    auto releaseSliderValue = Mod::get()->getSettingValue<int64_t>("releasesound-currentsound");
    auto customReleaseSound = Mod::get()->getSettingValue<ghc::filesystem::path>("custom-releasesound").string();
    bool usingCustomReleaseSound;
    std::string releaseSoundInUse = Clicks::getReleaseSprite(releaseSliderValue);


    if (releaseSliderValue != 0) usingCustomReleaseSound = false;
    if (releaseSoundInUse == "__USECUSTOM__") usingCustomReleaseSound = true;
 
    if (Mod::get()->getSettingValue<bool>("enable-releasesound") && !usingCustomReleaseSound) {
      auto fae = FMODAudioEngine::sharedEngine();
      auto system = fae->m_system;

      FMOD::Channel* channel;
      FMOD::Sound* sound;

      system->createSound((Mod::get()->getResourcesDir().parent_path() / releaseSoundInUse).string().c_str(), FMOD_DEFAULT, nullptr, &sound);
      system->playSound(sound, nullptr, false, &channel);

      if (Mod::get()->getSettingValue<bool>("separate-volume")) {
        channel->setVolume(Mod::get()->getSettingValue<int64_t>("volume-slider")/50.f); 
      } else {
        channel->setVolume(GameManager::sharedState()->m_sfxVolume*2.f); 
      }
    }

    if (Mod::get()->getSettingValue<bool>("enable-releasesound") && usingCustomReleaseSound) {
      auto fae = FMODAudioEngine::sharedEngine();
      auto system = fae->m_system;

      FMOD::Channel* channel;
      FMOD::Sound* sound;

      system->createSound(customReleaseSound.c_str(), FMOD_DEFAULT, nullptr, &sound);
      system->playSound(sound, nullptr, false, &channel);

      if (Mod::get()->getSettingValue<bool>("separate-volume")) {
        channel->setVolume(Mod::get()->getSettingValue<int64_t>("volume-slider")/50.f); 
      } else {
        channel->setVolume(GameManager::sharedState()->m_sfxVolume*2.f); 
      }
    }
    if(!Mod::get()->getSettingValue<bool>("enable-clicksound") && !Mod::get()->getSettingValue<bool>("enable-releasesound")){}else{Carrot::carrot=true;}
 }
};
