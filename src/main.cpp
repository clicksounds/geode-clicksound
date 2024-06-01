#include <Geode/Geode.hpp>
#include "./StoreLayer.hpp"
using namespace geode::prelude;


#include <Geode/modify/MenuLayer.hpp>
class $modify(StoreLayerButton, MenuLayer) {
	bool init() {
		if (!MenuLayer::init()) {
			return false;
		}

		auto myButton = CCMenuItemSpriteExtra::create(
			CCSprite::createWithSpriteFrameName("csBtn.png"_spr),
			this,
			menu_selector(StoreLayerButton::onMyButton)
		);
		auto menu = this->getChildByID("bottom-menu");
		menu->addChild(myButton);
		myButton->setID("store"_spr);
		menu->updateLayout();
		return true;
	}

	void onMyButton(CCObject*) {
		auto director = CCDirector::sharedDirector();
        auto store = StoreLayer::scene(director->getRunningScene());
		director->pushScene(CCTransitionFade::create(0.5f, store));
	}
};
