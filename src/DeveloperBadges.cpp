#include <Geode/Bindings.hpp>
#include <Geode/Geode.hpp>
#include <Geode/binding/ProfilePage.hpp>
#include <Geode/modify/ProfilePage.hpp>
#include <Geode/ui/GeodeUI.hpp>

namespace Contributors  {
	enum Messages {
		Owner = 1,
		Developer = 2,
		Contributors = 3,
		Helper = 4,
	};
	inline std::set<std::pair<int,Messages>> List = { 
			{13873867,Messages::Owner}, 
			{14284530,Messages::Developer}, 
			{21207551,Messages::Developer}, 
			{25397826,Messages::Contributors}, 
			{6253758,Messages::Contributors}, 
			{1941705,Messages::Contributors}, 
			{17389494,Messages::Helper}
		}; 
};


using namespace geode::prelude;

// Developer badges! :3333 (ty kot!)
class buttonfunctionthing : public CCLayer {
  public:
	void Owner(CCObject *sender) {
		FLAlertLayer::create(
		    "About",
		    "This is the <cp>Click Sound Owner</c>. They own the mod! ^w^",
		    "OK")
		    ->show();
	}
	void Developer(CCObject *sender) {
		FLAlertLayer::create(
		    "About",
		    "This is a <cp>Click Sound Developer</c>. They programmed alot of features to Click Sound. ^w^",
		    "OK")
		    ->show();
	}
	void Contributor(CCObject *sender) {
		FLAlertLayer::create(
		    "About",
		    "This is a <cp>Click Sound Contributor</c>. They have programmed anything that was added to Click Sound. ^w^",
		    "OK")
		    ->show();
	}
	void Helper(CCObject *sender) {
		FLAlertLayer::create(
		    "About",
		    "This is a <cp>Click Sound Helper</c>. They have added anything to Click sounds that doesn't require programming! ^w^",
		    "OK")
		    ->show();
	}
};

class $modify(ProfilePage) {
	bool init(int p0, bool p1) {
		if (!ProfilePage::init(p0, p1))
			return false;

		if (m_mainLayer->getChildByID("my-stuff-hint")) {
			m_mainLayer->removeChildByID("my-stuff-hint"); // Might fix a crash idk
		}
		auto spr = CCSprite::create("csBadge.png"_spr);
		if (!spr) {
			return true; // unable to find sprite
		}

		std::string username = m_usernameLabel->getString();
		auto it = std::find_if(Contributors::List.begin(), Contributors::List.end(),
		                       [p0](const auto &pair) { return pair.first == p0; });

		if (it != Contributors::List.end()) {

			auto btn = CCMenuItemSpriteExtra::create(
			    spr, this, menu_selector(buttonfunctionthing::Developer));

			switch (it->second) {
			case Contributors::Owner:
				btn->m_pfnSelector = menu_selector(buttonfunctionthing::Owner);
				break;
			case Contributors::Contributors:
				btn->m_pfnSelector = menu_selector(buttonfunctionthing::Contributor);
				break;
			case Contributors::Developer:
				btn->m_pfnSelector = menu_selector(buttonfunctionthing::Developer);
				break;
			case Contributors::Helper:
				btn->m_pfnSelector = menu_selector(buttonfunctionthing::Helper);
				break;
			}

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
