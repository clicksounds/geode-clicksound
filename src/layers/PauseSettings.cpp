#include <Geode/Geode.hpp>
#include <Geode/modify/PlayerObject.hpp>
#include <Geode/modify/PauseLayer.hpp>
#include <Geode/modify/EndLevelLayer.hpp>
#include <Geode/modify/EditorPauseLayer.hpp>
#include <Geode/ui/GeodeUI.hpp>
class PopupModSettings : public CCLayer {
public:
void ShowPopup(CCObject*) {
		geode::openSettingsPopup(Mod::get());
	}
};

using namespace geode::prelude;

// SETTINGS IN PAUSE MENU (ty viper!)
class $modify(pause,PauseLayer) {


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
      btn->setID("beat.hewwo!_:3");
      menu->addChild(btn);
    }
  }
};

class $modify(editor,EditorPauseLayer) {


  bool init(LevelEditorLayer* po) {
    if (!EditorPauseLayer::init(po)) return false;
    auto menu = this->getChildByID("settings-menu");
    auto winSize = CCDirector::sharedDirector()->getWinSize();
    auto spr = CCSprite::create("Button.png"_spr);
    spr->setScale(0.6f);
    menu->setLayout(RowLayout::create()
      ->setGap(0.f)
      ->setAxisAlignment(AxisAlignment::Center)
      ->setAutoScale(false)
      ->setCrossAxisOverflow(true)
    );
    auto btn = CCMenuItemSpriteExtra::create(
        spr,
        this,
        menu_selector(PopupModSettings::ShowPopup)
      );
    if(Mod::get()->getSettingValue<bool>("settings-button")) {
      btn->setPosition({menu->getContentSize().width/2, btn->getContentSize().height/2});
      btn->setID("beat.hewwo!_:3");
      menu->addChild(btn);
    }
    menu->updateLayout();
    return true;
  }
};
