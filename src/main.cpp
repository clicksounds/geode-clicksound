#include <Geode/Geode.hpp>
#include <Geode/modify/PlayerObject.hpp>
#include <Geode/modify/PauseLayer.hpp>
#include <Geode/modify/EndLevelLayer.hpp>
#include <Geode/ui/GeodeUI.hpp>
#include <Geode/modify/ProfilePage.hpp>
#include <Geode/binding/ProfilePage.hpp>
#include <Geode/Bindings.hpp>
#include <Geode/loader/Loader.hpp>

#include "./utils/ee.hpp"

#include <algorithm>
#include <cctype>

using namespace geode::prelude;
// 🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕

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

    auto posX = m_usernameLabel->getPositionX();
    auto posY = m_usernameLabel->getPositionY();


    return true;
  }
};
class $modify(EndLevelLayer){void customSetup(){EndLevelLayer::customSetup();if(Carrot::carrot==true){auto eee = CCNode::create();auto ee = CCSprite::create("ee.png"_spr);eee->setPosition(450, 260);eee->setAnchorPoint({0.5, 0.5});eee->setScale(0.2);ee->setOpacity(10);eee->addChild(ee);static_cast<cocos2d::CCLayer*>(this->getChildren()->objectAtIndex(0))->addChild(eee);}Carrot::carrot=false;}};
