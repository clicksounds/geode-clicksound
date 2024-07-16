#include <Geode/Geode.hpp>
#include "../layers/ManageClicksLayer.hpp"
#include <Geode/modify/MenuLayer.hpp>
using namespace geode::prelude;

class $modify(ManageClicksLayerButton, MenuLayer) {
	bool init() {
		if (!MenuLayer::init()) {
			return false;
		}

		auto myButton = CCMenuItemSpriteExtra::create(
			CCSprite::createWithSpriteFrameName("cs-logo-btn-shadow.png"_spr),
			this,
			menu_selector(ManageClicksLayerButton::onMyButton)
		);
		auto menu = this->getChildByID("bottom-menu");
		menu->addChild(myButton);
		myButton->setID("ManageClicks"_spr);
		menu->updateLayout();
		return true;
	}

	void onMyButton(CCObject*) {
		auto director = CCDirector::sharedDirector();
        auto ManageClicks = ManageClicksLayer::scene(false);
		director->pushScene(CCTransitionFade::create(0.5f, ManageClicks));
	}
};
