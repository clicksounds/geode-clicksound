#include <Geode/Geode.hpp>
#include "ClickTypes.hpp"
#include "ClickTypeNodes.hpp"
#include "Sections.hpp"
#include "SoundTest.hpp"
#include "../utils/Clicks.hpp"

using namespace geode::prelude;
Clicks* Clicks::gget = nullptr;
Clicks* Clicks::get() {
  if (gget == nullptr) {
    gget = new (std::nothrow) Clicks();
    gget->initJson();
  }

  return gget;
}

SettingNode* SectionSettingValue::createNode(float width) {
    return SectionSettingNode::create(this, width);
}

SettingNode* ClickTypeValue::createNode(float width) {
    return ClickTypeNode::create(this, width, "Click");
}

SettingNode* ReleaseTypeValue::createNode(float width) {
    return ClickTypeNode::create(this, width, "Release");
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

    if (Mod::get()->getSettingValue<bool>("enable-clicksound") && settings["tab"] == 3) {
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
        channel->setVolume(fae->m_sfxVolume*2.f);
      #else
        if (Mod::get()->getSettingValue<int64_t>("volume-slider") != 0) system->playSound(sound, nullptr, false, &channel);
        channel->setVolume(Mod::get()->getSettingValue<int64_t>("volume-slider")/50.f);
      #endif
    } else {
      if (Mod::get()->getSettingValue<int64_t>("volume-slider") != 0) system->playSound(sound, nullptr, false, &channel);
      channel->setVolume(Mod::get()->getSettingValue<int64_t>("volume-slider")/50.f );
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
    Mod::get()->addCustomSetting<ClickTypeValue>("clicksound-type", matjson::parse(R"({"tab":1,"click":1,"memeClick":1,"customClick":"..."})"));
    Mod::get()->addCustomSetting<ReleaseTypeValue>("releasesound-type", matjson::parse(R"({"tab":1,"click":1,"memeClick":1,"customClick":"..."})"));
    Mod::get()->addCustomSetting<SectionSettingValue>("releasesound-section", "none");
    Mod::get()->addCustomSetting<SectionSettingValue>("misc-section", "none");
}
