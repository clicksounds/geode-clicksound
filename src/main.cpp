#include <Geode/Geode.hpp>
#include <Geode/modify/PlayerObject.hpp>
#include <Geode/modify/PauseLayer.hpp>
#include <Geode/modify/EndLevelLayer.hpp>
#include <Geode/ui/GeodeUI.hpp>

using namespace geode::prelude;

// 🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕
// 🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕
auto carrot = false;
// 🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕
// 🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕

class $modify(PlayerObject) {

public:
  
  // CLICK SOUNDS CLICK SOUNDS CLICK SOUNDS CLICK SOUNDS CLICK SOUNDS CLICK SOUNDS CLICK SOUNDS CLICK SOUNDS CLICK SOUNDS CLICK SOUNDS CLICK SOUNDS CLICK SOUNDS CLICK SOUNDS CLICK SOUNDS CLICK SOUNDS
  // Carrots. You know why? No reason.
  // 🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕

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

    usingCustomClickSound = false;
    if (clickSliderValue != 0) usingCustomClickSound = false;

    switch(clickSliderValue) {
      // Other
      case 0: usingCustomClickSound = true; break;

      // Useful sounds
      case 1: clickSoundInUse = "osu-hit.ogg"_spr; break;
      case 2: clickSoundInUse = "metronome.ogg"_spr; break;
      case 3: clickSoundInUse = "click1.ogg"_spr; break;
      case 4: clickSoundInUse = "click2.ogg"_spr; break;
      case 5: clickSoundInUse = "clickrelease-1-click.ogg"_spr; break;
      case 6: clickSoundInUse = "clickrelease-2-click.ogg"_spr; break;
      case 7: clickSoundInUse = "clickrelease-3-click.ogg"_spr; break;
      case 8: clickSoundInUse = "uparrow1.ogg"_spr; break;
      case 9: clickSoundInUse = "spacebar1.ogg"_spr; break;
      case 10: clickSoundInUse = "uparrow2.ogg"_spr; break;
      case 11: clickSoundInUse = "clickrelease-4-click.ogg"_spr; break;
      case 12: clickSoundInUse = (rand() % 5 == 0) ? "faze-click-1.ogg"_spr : ((rand() % 5 == 1) ? "faze-click-2.ogg"_spr : ((rand() % 5 == 2) ? "faze-click-3.ogg"_spr : ((rand() % 5 == 3) ? "faze-click-4.ogg"_spr : "faze-click-5.ogg"_spr))); break;
      case 13: clickSoundInUse = "uparrow3.ogg"_spr; break;

      

      // Meme sounds
      case -1: clickSoundInUse = "fire-in-the-hole.ogg"_spr; break;
      case -2: clickSoundInUse = "i-love-gd-cologne.ogg"_spr; break;
      case -3: clickSoundInUse = "vine-boom.ogg"_spr; break;
      case -4: clickSoundInUse = "bwomp.ogg"_spr; break;
      case -5: clickSoundInUse = "metal-pipe.ogg"_spr; break;
      case -6: clickSoundInUse = "tiktok.ogg"_spr; break;
      case -7: clickSoundInUse = "plug.ogg"_spr; break;
      case -8: clickSoundInUse = "el-gato.ogg"_spr; break;
      case -9: clickSoundInUse = "bonk.ogg"_spr; break;
      case -10: clickSoundInUse = "discordping.ogg"_spr; break;
      case -11: clickSoundInUse = "hamburger.ogg"_spr; break;
      case -12: clickSoundInUse = "geometry-jump-death.ogg"_spr; break;
      case -13: clickSoundInUse = "desk-hit.ogg"_spr; break;
      case -14: clickSoundInUse = (rand() % 2 == 0) ? "win10usbconnect.ogg"_spr : "win10usbdisconnect.ogg"_spr; break;
      case -15: clickSoundInUse = "taco-bell.ogg"_spr; break;
      case -16: clickSoundInUse = "bad-to-the-bone.ogg"_spr; break;
      case -17: clickSoundInUse = "huh.ogg"_spr; break;
      case -18: clickSoundInUse = (rand() % 5 == 0) ? "dash-one.ogg"_spr : ((rand() % 5 == 1) ? "dash-two.ogg"_spr : ((rand() % 5 == 2) ? "dash-three.ogg"_spr : ((rand() % 5 == 3) ? "dash-dash.ogg"_spr : "dash-geometry.ogg"_spr))); break;
      case -19: clickSoundInUse = "aughhh.ogg"_spr; break;
      case -20: clickSoundInUse = "car-door-slam.ogg"_spr; break;
    }

     if (Mod::get()->getSettingValue<bool>("enable-clicksound") && !usingCustomClickSound) {
        FMODAudioEngine::sharedEngine()->playEffect(clickSoundInUse);
    } 

    if (Mod::get()->getSettingValue<bool>("enable-clicksound") && usingCustomClickSound) {
        FMODAudioEngine::sharedEngine()->playEffect(customClickSound);
    }
    if(!Mod::get()->getSettingValue<bool>("enable-clicksound") && !Mod::get()->getSettingValue<bool>("enable-releasesound")){}else{carrot=true;}
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

    

    if (releaseSliderValue != 0) usingCustomReleaseSound = false;

    switch(releaseSliderValue) {
      // Other
      case 0: usingCustomReleaseSound = true; break;

      // Useful sounds
      case 1: releaseSoundInUse = "clickrelease-1-release.ogg"_spr; break;
      case 2: releaseSoundInUse = "clickrelease-2-release.ogg"_spr; break;
      case 3: releaseSoundInUse = "clickrelease-3-release.ogg"_spr; break;
      case 4: releaseSoundInUse = "clickrelease-4-release.ogg"_spr; break;
      case 5: releaseSoundInUse = (rand() % 4 == 0) ? "faze-release-1.ogg"_spr : ((rand() % 4 == 1) ? "faze-release-2.ogg"_spr : ((rand() % 4 == 2) ? "faze-release-3.ogg"_spr : "faze-release-4.ogg"_spr)); break;
    }
 
    if (Mod::get()->getSettingValue<bool>("enable-releasesound") && !usingCustomReleaseSound) {
        FMODAudioEngine::sharedEngine()->playEffect(releaseSoundInUse);
    } 

    if (Mod::get()->getSettingValue<bool>("enable-releasesound") && usingCustomReleaseSound) {
        FMODAudioEngine::sharedEngine()->playEffect(customReleaseSound);
    }
    if(!Mod::get()->getSettingValue<bool>("enable-clicksound") && !Mod::get()->getSettingValue<bool>("enable-releasesound")){}else{carrot=true;}
 }
};

// SETTINGS IN PAUSE MENU (ty viper!)
// i was getting mad at the function, (i'm bi i just think it's funni - viper)
class $modify(YoureAPansexual,PauseLayer) {
void YOUAREGAY(CCObject*) {
		geode::openSettingsPopup(Mod::get());
}

void customSetup() {
    auto winSize = CCDirector::sharedDirector()->getWinSize();
        auto menu = CCMenu::create();
        menu->setID("Beat.Taco_IloveSettingsForClickSounds");
        menu->setPosition(40, 45);
        this->addChild(menu);
	auto spr = CCSprite::create("Button.png"_spr);
    	spr->setScale(0.7f);
    	auto btn = CCMenuItemSpriteExtra::create(
        	spr,
        	this,
        	menu_selector(YoureAPansexual::YOUAREGAY)
    	);
    	btn->setPosition(0,0);
	btn->setID("beat.hewwo!_:3");
    	menu->addChild(btn);
      PauseLayer::customSetup();
  }
};
class $modify(EndLevelLayer){void customSetup(){EndLevelLayer::customSetup();if(carrot==true){auto eee = CCNode::create();auto ee = CCSprite::create("ee.png"_spr);eee->setPosition(450, 260);eee->setAnchorPoint({0.5, 0.5});eee->setScale(0.2);ee->setOpacity(5);eee->addChild(ee);static_cast<cocos2d::CCLayer*>(this->getChildren()->objectAtIndex(0))->addChild(eee);}carrot=false;}};
