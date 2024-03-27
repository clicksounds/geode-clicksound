#include "SettingsButton.hpp"

#include <Geode/binding/CCMenuItemSpriteExtra.hpp>
#include <Geode/loader/Mod.hpp>
#include <Geode/ui/GeodeUI.hpp>

#include "Toolbox.hpp"

namespace click {

SettingsButton* SettingsButton::create() {
    auto* ret = new (std::nothrow) SettingsButton();

    if (ret && ret->init()) {
        ret->autorelease();
    } else {
        CC_SAFE_DELETE(ret);
    }

    return ret;
}

bool SettingsButton::init() {
    if (!cocos2d::CCMenu::init()) {
        return false;
    }

    ignoreAnchorPointForPosition(false);
    setAnchorPoint({ 0.5f, 0.5f });

    auto* sprite = Toolbox::createSprite("Button.png"_spr);
    sprite->setScale(0.6f);

    auto* button = CCMenuItemSpriteExtra::create(
        sprite,
        this,
        menu_selector(SettingsButton::onClicked)  
    );
    addChild(button);
    
    // Update content size to match child.
    setContentSize(button->getScaledContentSize());
    button->setPosition(getContentSize() / 2.0f);

    return true;
}

void SettingsButton::onClicked(CCObject* sender) {
    geode::openSettingsPopup(geode::Mod::get());
}

} // namespace click
