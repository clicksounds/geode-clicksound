#include <Geode/Geode.hpp>
using namespace geode::prelude;


#include <Geode/modify/MenuLayer.hpp>
class $modify(MyMenuLayer, MenuLayer) {
	bool init() {
		if (!MenuLayer::init()) {
			return false;
		}

		auto myButton = CCMenuItemSpriteExtra::create(
			CCSprite::createWithSpriteFrameName("GJ_likeBtn_001.png"),
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
		FLAlertLayer::create("Geode", "Hello from my custom mod!", "OK")->show();
	}
};
