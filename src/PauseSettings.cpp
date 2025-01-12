#include <Geode/Geode.hpp>
#include <Geode/modify/PauseLayer.hpp>
#include <Geode/modify/EditorPauseLayer.hpp>
#include <Geode/ui/GeodeUI.hpp>
#include <Geode/loader/SettingV3.hpp>
// No migration Needed
using namespace geode::prelude;

class PopupModSettings : public CCLayer {
public:
    void ShowPopup(CCObject*) {
		 geode::openSettingsPopup(Mod::get(), true);
	}
};

class $modify(PauseLayer) {
  void customSetup() {
    PauseLayer::customSetup();
    auto menu = this->getChildByID("left-button-menu");
    auto winSize = CCDirector::sharedDirector()->getWinSize();
    auto spr = CCSprite::create("Button.png"_spr);

    auto btn = CCMenuItemSpriteExtra::create(
        spr,
        this,
        menu_selector(PopupModSettings::ShowPopup)
      );
    spr->setScale(0.7f);
    if(Mod::get()->getSettingValue<bool>("settings-button")) {
      btn->setPosition({menu->getContentSize().width/2, btn->getContentSize().height/2});
      btn->setID("SettingButton"_spr);
      menu->addChild(btn);
      menu->updateLayout();
    }
  }
};

class $modify(EditorPauseLayer) {
  bool init(LevelEditorLayer* po) {
    if (!EditorPauseLayer::init(po)) return false;
    auto menu = this->getChildByID("settings-menu");
    auto winSize = CCDirector::sharedDirector()->getWinSize();
    auto spr = CCSprite::create("Button.png"_spr);
    spr->setScale(0.6f);
  
    auto btn = CCMenuItemSpriteExtra::create(
        spr,
        this,
        menu_selector(PopupModSettings::ShowPopup)
      );
    if(Mod::get()->getSettingValue<bool>("settings-button")) {
      btn->setPosition({menu->getContentSize().width/2, btn->getContentSize().height/2});
      btn->setID("SettingButton"_spr);
      menu->addChild(btn);
    }
    menu->updateLayout();
    return true;
  }
};
