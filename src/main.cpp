#include <Geode/Geode.hpp>
#include "./StoreLayer.hpp"
using namespace geode::prelude;


#include <Geode/modify/MenuLayer.hpp>
class $modify(MyMenuLayer, MenuLayer) {
	bool init() {
		if (!MenuLayer::init()) {
			return false;
		}

		auto myButton = CCMenuItemSpriteExtra::create(
			CCSprite::createWithSpriteFrameName("csBtn"_spr),
			this,
			menu_selector(MyMenuLayer::onMyButton)
		);
		auto menu = this->getChildByID("bottom-menu");
		menu->addChild(myButton);
		myButton->setID("store"_spr);
		menu->updateLayout();
		return true;
	}

	void onMyButton(CCObject*) {
        auto store = StoreLayer::scene();
		CCDirector::sharedDirector()->pushScene(CCTransitionFade::create(0.5f, store));
	}
};
