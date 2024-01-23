#include <Geode/Geode.hpp>
#include <Geode/modify/PlayerObject.hpp>

using namespace geode::prelude;

class $modify(PlayerObject) {

public:
  TodoReturn pushButton(PlayerButton p0) {
    PlayerObject::pushButton(p0);

  if (Mod::get()->getSettingValue<bool>("OnlyOnJump")) {
      if (p0 != PlayerButton::Jump) {
        return; //
      }
    }

    if (!GameManager::sharedState()->getPlayLayer())
      return;

    // get value of slider
    auto sliderValue = Mod::get()->getSettingValue<int64_t>("clicksound-currentsound");
    
    // use slider value to change 
    std::string soundInUse;
    // <-------------------------------- Add sounds here!
    if (sliderValue == 1) {
        soundInUse = "osu-hit.ogg"_spr;
    } else if (sliderValue == 2) {
        soundInUse = "vine-boom.ogg"_spr;
    } else if (sliderValue == 3) {
        soundInUse = "bwomp.ogg"_spr;
    } else if (sliderValue == 4) {
        soundInUse = "fire-in-the-hole.ogg"_spr;
    } else if (sliderValue == 5) {
        soundInUse = "i-love-gd-cologne.ogg"_spr;
    } else if (sliderValue == 6) {
        soundInUse = "metronome.ogg"_spr;
    } else if (sliderValue == 7) {
        soundInUse = "click1.ogg"_spr;
    }

    // play sound from slider value pick sound
    FMODAudioEngine::sharedEngine()->playEffect(soundInUse);
  }
};
