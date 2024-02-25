#include <Geode/Geode.hpp>
#include <Geode/modify/PlayerObject.hpp>
#include <Geode/modify/PauseLayer.hpp>
#include <Geode/modify/EndLevelLayer.hpp>
#include <Geode/ui/GeodeUI.hpp>

using namespace geode::prelude;

// SETTINGS IN PAUSE MENU (ty viper!)
class $modify(YoureAPansexual,PauseLayer) {
  void YOUAREGAY(CCObject*) {
      geode::openSettingsPopup(Mod::get());
  }

  void customSetup() {
    PauseLayer::customSetup();
    auto menu = this->getChildByID("left-button-menu");
    auto winSize = CCDirector::sharedDirector()->getWinSize();
    auto spr = CCSprite::create("Button.png"_spr);

    auto btn = CCMenuItemSpriteExtra::create(
        spr,
        this,
        menu_selector(YoureAPansexual::YOUAREGAY)
      );
    spr->setScale(0.7f);
    if(Mod::get()->getSettingValue<bool>("settings-button")) {
      btn->setPosition({menu->getContentSize().width/2, btn->getContentSize().height/2});
      btn->setID("beat.hewwo!_:3");
      menu->addChild(btn);
    }
  }
};