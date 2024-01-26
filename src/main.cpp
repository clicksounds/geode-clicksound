#include <Geode/Geode.hpp>
#include <Geode/modify/PlayerObject.hpp>

using namespace geode::prelude;

class $modify(PlayerObject) {

public:
  
  // CLICK SOUNDS CLICK SOUNDS CLICK SOUNDS CLICK SOUNDS CLICK SOUNDS CLICK SOUNDS CLICK SOUNDS CLICK SOUNDS CLICK SOUNDS CLICK SOUNDS CLICK SOUNDS CLICK SOUNDS CLICK SOUNDS CLICK SOUNDS CLICK SOUNDS

  TodoReturn pushButton(PlayerButton p0) {
    PlayerObject::pushButton(p0);

  if (Mod::get()->getSettingValue<bool>("OnlyOnJump")) {
      if (p0 != PlayerButton::Jump) {
        return;
      }
    }

    if (!GameManager::sharedState()->getPlayLayer())
      return;

    auto clickSliderValue = Mod::get()->getSettingValue<int64_t>("clicksound-currentsound");
    auto customClickSound = Mod::get()->getSettingValue<ghc::filesystem::path>("custom-clicksound").string();
    bool usingCustomClickSound;
    std::string clickSoundInUse;

    if (clickSliderValue != 0) {
      usingCustomClickSound = false;
      if (clickSliderValue == 1) {
        clickSoundInUse = "osu-hit.ogg"_spr;
      } else if (clickSliderValue == 2) {
        clickSoundInUse = "metronome.ogg"_spr;
      } else if (clickSliderValue == 3) {
        clickSoundInUse = "click1.ogg"_spr;
      } else if (clickSliderValue == 4) {
        clickSoundInUse = "click2.ogg"_spr;
      } else if (clickSliderValue == 5) {
        clickSoundInUse = "click3.ogg"_spr;
      } else if (clickSliderValue == 6) {
        clickSoundInUse = "fire-in-the-hole.ogg"_spr;
      } else if (clickSliderValue == 7) {
        clickSoundInUse = "i-love-gd-cologne.ogg"_spr;
      } else if (clickSliderValue == 8) {
        clickSoundInUse = "vine-boom.ogg"_spr;
      } else if (clickSliderValue == 9) {
        clickSoundInUse = "bwomp.ogg"_spr;
      } else if (clickSliderValue == 10) {
        clickSoundInUse = "metal-pipe.ogg"_spr;
      } else if (clickSliderValue == 11) {
        clickSoundInUse = "tiktok.ogg"_spr;
      } else if (clickSliderValue == 12) {
        clickSoundInUse = "plug.ogg"_spr;
      } else if (clickSliderValue == 13) {
        clickSoundInUse = "el-gato.ogg"_spr;
      }
    } 
    
    if (clickSliderValue == 0) {
      usingCustomClickSound = true;
    }

    if (Mod::get()->getSettingValue<bool>("enable-clicksound") && !usingCustomClickSound) {
        FMODAudioEngine::sharedEngine()->playEffect(clickSoundInUse);
    } 
    
    if (Mod::get()->getSettingValue<bool>("enable-clicksound") && usingCustomClickSound) {
        FMODAudioEngine::sharedEngine()->playEffect(customClickSound);
    }
  }
  
  // RELEASE SOUNDS RELEASE SOUNDS RELEASE SOUNDS RELEASE SOUNDS RELEASE SOUNDS RELEASE SOUNDS RELEASE SOUNDS RELEASE SOUNDS RELEASE SOUNDS RELEASE SOUNDS RELEASE SOUNDS RELEASE SOUNDS RELEASE SOUNDS 
  
 TodoReturn releaseButton(PlayerButton p0) {
    PlayerObject::releaseButton(p0);

  if (Mod::get()->getSettingValue<bool>("OnlyOnJump")) {
      if (p0 != PlayerButton::Jump) {
        return;
      }
    }

    if (!GameManager::sharedState()->getPlayLayer())
      return;

    auto releaseSliderValue = Mod::get()->getSettingValue<int64_t>("releasesound-currentsound");
    auto customReleaseSound = Mod::get()->getSettingValue<ghc::filesystem::path>("custom-releasesound").string();
    bool usingCustomReleaseSound;
    std::string releaseSoundInUse;

    if (releaseSliderValue != 0) {
      usingCustomReleaseSound = false;
      if (releaseSliderValue == 1) {
        releaseSoundInUse = "release1.ogg"_spr;
      }
    } 
    
    if (releaseSliderValue == 0) {
      usingCustomReleaseSound = true;
    }

    if (Mod::get()->getSettingValue<bool>("enable-releasesound") && !usingCustomReleaseSound) {
        FMODAudioEngine::sharedEngine()->playEffect(releaseSoundInUse);
    } 
    
    if (Mod::get()->getSettingValue<bool>("enable-releasesound") && usingCustomReleaseSound) {
        FMODAudioEngine::sharedEngine()->playEffect(customReleaseSound);
    }
  }

};
