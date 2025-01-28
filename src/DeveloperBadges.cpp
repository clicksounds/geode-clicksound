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
		IndexModerator = 5
	};
	inline std::set<std::pair<int,Messages>> List = { 
		{13873867,Messages::Owner}, // BeatTaco
		{14284530,Messages::Developer}, // ViperProtogen
		{21207551,Messages::Developer}, // coopeeo
		{25397826,Messages::Contributors}, // Uproxide
		{6253758,Messages::Contributors}, // FireeDev
		{1941705,Messages::Contributors}, // RayDeeUx
		{17389494,Messages::Helper}, // AkuLunical
		{7669473,Messages::IndexModerator}, // Jaid
		{26541718,Messages::IndexModerator} // Cuuhie
	}; 
};


using namespace geode::prelude;

// Developer badges! :3333 (ty kot!)
class buttonfunctionthing : public CCLayer {
  public:
	void Owner(CCObject *sender) {
		FLAlertLayer::create(
		    "About",
		    "This is the <cp>Click Sounds Owner</c>. They own the mod! ^w^",
		    "OK")
		    ->show();
	}
	void Developer(CCObject *sender) {
		FLAlertLayer::create(
		    "About",
		    "This is a <cp>Click Sounds Developer</c>. They programmed a lot of features to Click Sounds. ^w^",
		    "OK")
		    ->show();
	}
	void Contributor(CCObject *sender) {
		FLAlertLayer::create(
		    "About",
		    "This is a <cp>Click Sounds Contributor</c>. They have programmed anything that was added to Click Sounds. ^w^",
		    "OK")
		    ->show();
	}
	void Helper(CCObject *sender) {
		FLAlertLayer::create(
		    "About",
		    "This is a <cp>Click Sounds Helper</c>. These users are helpful to the Click Sounds community. ^w^",
		    "OK")
		    ->show();
	}
	void IndexModerator(CCObject *sender) {
		FLAlertLayer::create(
		    "About",
		    "This is a <cp>Click Sounds Index Moderator</c>. They moderate click packs for Click Sounds. ^w^",
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
			case Contributors::IndexModerator:
				btn->m_pfnSelector = menu_selector(buttonfunctionthing::IndexModerator);
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
