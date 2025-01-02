#include <Geode/Geode.hpp>
#include <Geode/ui/GeodeUI.hpp>
#include <Geode/modify/ProfilePage.hpp>
#include <Geode/binding/ProfilePage.hpp>
#include <Geode/Bindings.hpp>

using namespace geode::prelude;

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
  bool init(int p0, bool p1) {
    if (!ProfilePage::init(p0, p1)) return false;

    std::set<int> csContributors = {13873867, 14284530, 21207551, 25397826, 6253758, 1941705};
                                                  

    if (m_mainLayer->getChildByID("my-stuff-hint")) {
      m_mainLayer->removeChildByID("my-stuff-hint"); // Might fix a crash idk
    }
    auto spr = CCSprite::create("csBadge.png"_spr);
    if (!spr) {
      return true; // unable to find sprite
    }

    std::string username = m_usernameLabel->getString();

    if (csContributors.count(p0) > 0) 
    {
      
    auto btn = CCMenuItemSpriteExtra::create(
           spr, this, menu_selector(buttonfunctionthing::onButtonClick)
      );


    btn->setPosition(14.500, -132);
    btn->setZOrder(26);
    btn->setID("cs-badge"_spr);
    this->getChildByIDRecursive("left-menu")->addChild(btn);
    this->getChildByIDRecursive("left-menu")->updateLayout();
        }

    auto posX = m_usernameLabel->getPositionX();
    auto posY = m_usernameLabel->getPositionY();


    return true;
  }
};
