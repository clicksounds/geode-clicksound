#include <Geode/Geode.hpp>
#include <Geode/modify/PlayerObject.hpp>

using namespace geode::prelude;

class $modify(PlayerObject) {

public:
  TodoReturn pushButton(PlayerButton p0) {
     if (Mod::get()->getSettingValue<bool>("OnlyOnJump")) {
        if (p0 != PlayerButton::Jump) {
          return; //
        }
     }
    PlayerObject::pushButton(p0);

    if (!GameManager::sharedState()->getPlayLayer())
      return;

                        // get value of slider
                        auto sliderValue = Mod::get()->getSettingValue<int64_t>("clicksound-currentsound");
    
                        // use slider value to change 
                        std::string soundInUse;
                        if (sliderValue == 1) {
                            soundInUse = "osu-hit.mp3"_spr;
                        } else if (sliderValue == 2) {
                            soundInUse = "vine-boom.mp3"_spr;
                        } else if (sliderValue == 3) {
                            soundInUse = "bwomp.mp3"_spr;
                        } else if (sliderValue == 4) {
                            soundInUse = "fire-in-the-hole.mp3"_spr;
                        }

    // play sound from slider value pick sound
    FMODAudioEngine::sharedEngine()->playEffect(soundInUse);
  }
};
