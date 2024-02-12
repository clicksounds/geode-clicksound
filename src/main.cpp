#include <Geode/Geode.hpp>
#include <Geode/modify/PlayerObject.hpp>
#include <Geode/modify/PauseLayer.hpp>
#include <Geode/modify/EndLevelLayer.hpp>
#include <Geode/ui/GeodeUI.hpp>
// For developer badges
#include <Geode/modify/ProfilePage.hpp>
#include <Geode/binding/ProfilePage.hpp>
#include <Geode/Bindings.hpp>
#include <Geode/loader/Loader.hpp>
#include <algorithm>
#include <cctype>

using namespace geode::prelude;

// 🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕
// 🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕
// 🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕
// 🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕
auto carrot = false;
// 🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕
// 🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕
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

    
    
    // this code block is not approved by mat, but im not changing it.
    if (GameManager::sharedState()->getPlayLayer() && !GameManager::sharedState()->getEditorLayer())
      nullptr;
    else if (GameManager::sharedState()->getEditorLayer() && !GameManager::sharedState()->getPlayLayer())
      nullptr;
    else return;



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
      case 14: clickSoundInUse = (rand() % 5 == 0) ? "bloody-click-1.ogg"_spr : ((rand() % 5 == 1) ? "bloody-click-2.ogg"_spr : ((rand() % 5 == 2) ? "bloody-click-3.ogg"_spr : ((rand() % 5 == 3) ? "bloody-click-4.ogg"_spr : "bloody-click-5.ogg"_spr))); break;
      case 15: clickSoundInUse = "spaceuk.ogg"_spr; break;
      case 16: clickSoundInUse = "npesta-click.ogg"_spr; break;
      case 17: clickSoundInUse = "cursed.ogg"_spr; break;
      case 18: clickSoundInUse = "spacebar2-click.ogg"_spr; break;
      case 19: clickSoundInUse = "click3.ogg"_spr; break;        
      case 20: clickSoundInUse = "Viper_Clicks_SpaceBar_" + std::to_string((rand() % 5) + 1) + ".ogg"_spr; break;
      

      // Meme sounds
      case -1: clickSoundInUse = "fire-in-the-hole.ogg"_spr; break;
      case -2: clickSoundInUse = "i-love-gd-cologne.ogg"_spr; break;
      case -3: clickSoundInUse = "vine-boom.ogg"_spr; break;
      case -4: clickSoundInUse = "bwomp.ogg"_spr; break;
      case -5: clickSoundInUse = "metal-pipe.ogg"_spr; break;
      case -6: clickSoundInUse = "tiktok-click.ogg"_spr; break;
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
      case -21: clickSoundInUse = "spectre.ogg"_spr; break;
      case -22: clickSoundInUse = (rand() % 500 == 0) ? "congregation-jumpscare.ogg"_spr : "osu-hit.ogg"_spr; break;
      case -23: clickSoundInUse = "soda.ogg"_spr; break;
    }
     if (Mod::get()->getSettingValue<bool>("enable-clicksound") && !usingCustomClickSound) {
        FMODAudioEngine::sharedEngine()->playEffect(clickSoundInUse, 1.0f, 1.0f, 0.5f);
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
    
    
    if (GameManager::sharedState()->getPlayLayer() && !GameManager::sharedState()->getEditorLayer())
      nullptr;
    else if (GameManager::sharedState()->getEditorLayer() && !GameManager::sharedState()->getPlayLayer())
      nullptr;
    else
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
      case 6: releaseSoundInUse = (rand() % 5 == 0) ? "bloody-release-1.ogg"_spr : ((rand() % 5 == 1) ? "bloody-release-2.ogg"_spr : ((rand() % 5 == 2) ? "bloody-release-3.ogg"_spr : ((rand() % 5 == 3) ? "bloody-release-4.ogg"_spr : "bloody-release-5.ogg"_spr))); break;
      case 7: releaseSoundInUse = "npesta-release.ogg"_spr; break;
      case 8: releaseSoundInUse = "spacebar2-release.ogg"_spr; break;
      case 9: releaseSoundInUse = "Viper_SpaceBar_release_" + std::to_string((rand() % 3) + 1) + ".ogg"_spr; break;

      // Meme sounds
      case -1: releaseSoundInUse = "tiktok-release.ogg"_spr; break;
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
class $modify(YoureAPansexual,PauseLayer) {
  void YOUAREGAY(CCObject*) {
      geode::openSettingsPopup(Mod::get());
  }

  void customSetup() {
    PauseLayer::customSetup();

    auto winSize = CCDirector::sharedDirector()->getWinSize();

    auto menu = this->getChildByID("left-button-menu");

    auto spr = CCSprite::create("Button.png"_spr);

    if(Mod::get()->getSettingValue<bool>("settings-button")) {
      spr->setScale(0.7f);
    }

    auto btn = CCMenuItemSpriteExtra::create(
        spr,
        this,
        menu_selector(YoureAPansexual::YOUAREGAY)
      );

    if(Mod::get()->getSettingValue<bool>("settings-button")) {
      btn->setPosition({menu->getContentSize().width/2, btn->getContentSize().height/2});
      btn->setID("beat.hewwo!_:3");
      menu->addChild(btn);
    }
  }
};

// Developer badges! :3333 (ty kot!)
class buttonfunctionthing : public CCLayer {
  public:
  void onButtonClick(CCObject* sender) {
    FLAlertLayer::create(
      "About",
      "This badge is given to <cp>Click Sound developers</c>. If you have programmed anything that was added to Click Sound, You will obtain this badge. ^w^",
      "OK"
    )->show();
  }
};

class $modify(ProfilePage) {
  bool m_hasBeenOpened = false;

  bool init(int p0, bool p1) {
    if (!ProfilePage::init(p0, p1)) return false;

    std::set<int> csContributors = {13873867, 14284530, 21207551, 25397826, 6253758};
                                                  

    m_mainLayer->removeChildByID("my-stuff-hint");
    auto mainMenu = static_cast<cocos2d::CCLayer*>(this->getChildByIDRecursive("main-menu"));
    auto spr = CCSprite::create("csBadge.png"_spr);
    auto btn = CCMenuItemSpriteExtra::create(
           spr, this, menu_selector(buttonfunctionthing::onButtonClick)
      );
    auto contentSize = mainMenu->getContentSize();


    btn->setPosition(14.500, -132);
    btn->setZOrder(26);
    btn->setID("cs-badge");
    mainMenu->addChild(btn);
    btn->setVisible(false);

    std::string username = m_usernameLabel->getString();

    if (csContributors.count(p0) > 0) 
    {
      btn->setVisible(true);
        }
    else
    {
      // how many times do we have to tell you this old man
    }

    auto posX = m_usernameLabel->getPositionX();
    auto posY = m_usernameLabel->getPositionY();


    return true;
  }
};
class $modify(EndLevelLayer){void customSetup(){EndLevelLayer::customSetup();if(carrot==true){auto eee = CCNode::create();auto ee = CCSprite::create("ee.png"_spr);eee->setPosition(450, 260);eee->setAnchorPoint({0.5, 0.5});eee->setScale(0.2);ee->setOpacity(10);eee->addChild(ee);static_cast<cocos2d::CCLayer*>(this->getChildren()->objectAtIndex(0))->addChild(eee);}carrot=false;}};
