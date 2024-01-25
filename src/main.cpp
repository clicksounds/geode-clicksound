#include <Geode/Geode.hpp>
#include <Geode/modify/PlayerObject.hpp>

using namespace geode::prelude;

class $modify(PlayerObject) {

public:
  TodoReturn pushButton(PlayerButton p0) {
    PlayerObject::pushButton(p0);

  if (Mod::get()->getSettingValue<bool>("OnlyOnJump")) {
      if (p0 != PlayerButton::Jump) {
        return;
      }
    }

    if (!GameManager::sharedState()->getPlayLayer())
      return;

    auto sliderValue = Mod::get()->getSettingValue<int64_t>("clicksound-currentsound");
    auto customSound = Mod::get()->getSettingValue<ghc::filesystem::path>("custom-sound").string();
    bool usingCustomSound;
    std::string soundInUse;

    if (sliderValue != 0) {
      usingCustomSound = false;
      if (sliderValue == 1) {
        soundInUse = "osu-hit.ogg"_spr;
      } else if (sliderValue == 2) {
        soundInUse = "metronome.ogg"_spr;
      } else if (sliderValue == 3) {
        soundInUse = "click1.ogg"_spr;
      } else if (sliderValue == 4) {
        soundInUse = "click2.ogg"_spr;
      } else if (sliderValue == 5) {
        soundInUse = "fire-in-the-hole.ogg"_spr;
      } else if (sliderValue == 6) {
        soundInUse = "i-love-gd-cologne.ogg"_spr;
      } else if (sliderValue == 7) {
        soundInUse = "vine-boom.ogg"_spr;
      } else if (sliderValue == 8) {
        soundInUse = "bwomp.ogg"_spr;
      } else if (sliderValue == 9) {
        soundInUse = "metal-pipe.ogg"_spr;
      } else if (sliderValue == 10) {
        soundInUse = "tiktok.ogg"_spr;
      } else if (sliderValue == 11) {
        soundInUse = "plug.ogg"_spr;
      }
    } 
    
    if (sliderValue == 0) {
      usingCustomSound = true;
    }

    if (Mod::get()->getSettingValue<bool>("enable-clicksound") && !usingCustomSound) {
        FMODAudioEngine::sharedEngine()->playEffect(soundInUse);
    } 
    
    if (Mod::get()->getSettingValue<bool>("enable-clicksound") && usingCustomSound) {
        FMODAudioEngine::sharedEngine()->playEffect(customSound);
    }
  }
};
