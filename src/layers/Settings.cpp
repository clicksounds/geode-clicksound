#include <Geode/Geode.hpp>
#include "../utils/Settings.hpp"
#include "../utils/Clicks.hpp"

using namespace geode::prelude;


SettingNode* SectionSettingValue::createNode(float width) {
    return SectionSettingNode::create(this, width);
}



SettingNode* SettingClickValue::createNode(float width) {
    return SettingClickNode::create(this, width);
}
// TYSM JOUCA AND FIREE
// TYSM JOUCA AND FIREE
// TYSM JOUCA AND FIREE
// TYSM JOUCA AND FIREE
// TYSM JOUCA AND FIREE
void SettingClickNode::onClickBtn(CCObject*) {
	auto clickSliderValue = Mod::get()->getSettingValue<int64_t>("clicksound-currentsound");
    auto customClickSound = Mod::get()->getSettingValue<ghc::filesystem::path>("custom-clicksound").string();
    auto usingCustomClickSound = false;
    std::string clickSoundInUse = Clicks::getClickSprite(clickSliderValue);

    usingCustomClickSound = false;
    if (clickSoundInUse == "__USECUSTOM__") usingCustomClickSound = true;
    
    
    if (!usingCustomClickSound) {
      auto fae = FMODAudioEngine::sharedEngine();
      auto system = fae->m_system;

      FMOD::Channel* channel;
      FMOD::Sound* sound;

      // System::createSound's first arg requires full c_string path
      system->createSound((Mod::get()->getResourcesDir().parent_path() / clickSoundInUse).string().c_str(), FMOD_DEFAULT, nullptr, &sound);
        
      if (Mod::get()->getSettingValue<bool>("use-sfx-volume")) {
        #if defined(GEODE_IS_WINDOWS)
          if (fae->m_sfxVolume != 0) system->playSound(sound, nullptr, false, &channel);
          channel->setVolume(fae->m_sfxVolume*2.f);
        #else
          if (Mod::get()->getSettingValue<int64_t>("volume-slider") != 0) system->playSound(sound, nullptr, false, &channel);
          channel->setVolume(Mod::get()->getSettingValue<int64_t>("volume-slider")/50.f);
        #endif
      } else {
        if (Mod::get()->getSettingValue<int64_t>("volume-slider") != 0) system->playSound(sound, nullptr, false, &channel);
        channel->setVolume(Mod::get()->getSettingValue<int64_t>("volume-slider")/50.f);
      }
    } 

    if (usingCustomClickSound) {
      auto fae = FMODAudioEngine::sharedEngine();
      auto system = fae->m_system;

      FMOD::Channel* channel;
      FMOD::Sound* sound;

      system->createSound(customClickSound.c_str(), FMOD_DEFAULT, nullptr, &sound);
        
      if (Mod::get()->getSettingValue<bool>("use-sfx-volume")) {
        #if defined(GEODE_IS_WINDOWS)
          if (fae->m_sfxVolume != 0) system->playSound(sound, nullptr, false, &channel);
          channel->setVolume(fae->m_sfxVolume*2.f);
        #else
          if (Mod::get()->getSettingValue<int64_t>("volume-slider") != 0) system->playSound(sound, nullptr, false, &channel);
          channel->setVolume(Mod::get()->getSettingValue<int64_t>("volume-slider")/50.f);
        #endif
      } else {
        if (Mod::get()->getSettingValue<int64_t>("volume-slider") != 0) system->playSound(sound, nullptr, false, &channel);
        channel->setVolume(Mod::get()->getSettingValue<int64_t>("volume-slider")/50.f);
      }
    }
}

SettingNode* SettingReleaseValue::createNode(float width) {
    return SettingReleaseNode::create(this, width);
}

void SettingReleaseNode::onReleaseBtn(CCObject*) {
	auto releaseSliderValue = Mod::get()->getSettingValue<int64_t>("releasesound-currentsound");
    auto customReleaseSound = Mod::get()->getSettingValue<ghc::filesystem::path>("custom-releasesound").string();
    bool usingCustomReleaseSound;
    std::string releaseSoundInUse = Clicks::getReleaseSprite(releaseSliderValue);


    if (releaseSliderValue != 0) usingCustomReleaseSound = false;
    if (releaseSoundInUse == "__USECUSTOM__") usingCustomReleaseSound = true;
 
    if (!usingCustomReleaseSound) {
      auto fae = FMODAudioEngine::sharedEngine();
      auto system = fae->m_system;

      FMOD::Channel* channel;
      FMOD::Sound* sound;

      // System::createSound's first arg requires full c_string path
      system->createSound((Mod::get()->getResourcesDir().parent_path() / releaseSoundInUse).string().c_str(), FMOD_DEFAULT, nullptr, &sound);
        
      if (Mod::get()->getSettingValue<bool>("use-sfx-volume")) {
        #if defined(GEODE_IS_WINDOWS)
          if (fae->m_sfxVolume != 0) system->playSound(sound, nullptr, false, &channel);
          channel->setVolume(fae->m_sfxVolume*2.f);
        #else
          if (Mod::get()->getSettingValue<int64_t>("volume-slider") != 0) system->playSound(sound, nullptr, false, &channel);
          channel->setVolume(Mod::get()->getSettingValue<int64_t>("volume-slider")/50.f);
        #endif
      } else {
        if (Mod::get()->getSettingValue<int64_t>("volume-slider") != 0) system->playSound(sound, nullptr, false, &channel);
        channel->setVolume(Mod::get()->getSettingValue<int64_t>("volume-slider")/50.f);
      }
    } 

    if (usingCustomReleaseSound) {
      auto fae = FMODAudioEngine::sharedEngine();
      auto system = fae->m_system;

      FMOD::Channel* channel;
      FMOD::Sound* sound;

      system->createSound(customReleaseSound.c_str(), FMOD_DEFAULT, nullptr, &sound);
        
      if (Mod::get()->getSettingValue<bool>("use-sfx-volume")) {
        #if defined(GEODE_IS_WINDOWS)
          if (fae->m_sfxVolume != 0) system->playSound(sound, nullptr, false, &channel);
          channel->setVolume(fae->m_sfxVolume*2.f);
        #else
          if (Mod::get()->getSettingValue<int64_t>("volume-slider") != 0) system->playSound(sound, nullptr, false, &channel);
          channel->setVolume(Mod::get()->getSettingValue<int64_t>("volume-slider")/50.f);
        #endif
      } else {
        if (Mod::get()->getSettingValue<int64_t>("volume-slider") != 0) system->playSound(sound, nullptr, false, &channel);
        channel->setVolume(Mod::get()->getSettingValue<int64_t>("volume-slider")/50.f);
      }
    }
}

$on_mod(Loaded) {
    Mod::get()->addCustomSetting<SettingClickValue>("test-click-btn", "none");
    Mod::get()->addCustomSetting<SettingReleaseValue>("test-release-btn", "none");
    Mod::get()->addCustomSetting<SectionSettingValue>("clicksound-section", "none");
    Mod::get()->addCustomSetting<SectionSettingValue>("releasesound-section", "none");
    Mod::get()->addCustomSetting<SectionSettingValue>("misc-section", "none");
}
