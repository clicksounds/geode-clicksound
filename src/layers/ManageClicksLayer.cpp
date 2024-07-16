#include <Geode/Geode.hpp>
#include <Geode/loader/Mod.hpp>
#include <Geode/ui/GeodeUI.hpp>

#include "./ManageClicksLayer.hpp"
#include "../ui/Spinner.hpp"

#include <cctype>
#include <algorithm>
#include <matjson.hpp>

using namespace geode::prelude;

ManageClicksLayer* ManageClicksLayer::create(bool GoToLastSceneOnBack) {
    auto ret = new ManageClicksLayer();
    ret->m_shouldGoToLastScene = false;
    if (ret && ret->init()) {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
};

CCScene* ManageClicksLayer::scene(bool GoToLastSceneOnBack) {
    auto layer = ManageClicksLayer::create(GoToLastSceneOnBack);
    auto scene = CCScene::create();
    scene->addChild(layer);
    return scene;
}


bool ManageClicksLayer::init() {

    auto director = CCDirector::sharedDirector();
    auto winSize = director->getWinSize();

    m_menu = CCMenu::create();
    m_menu->setAnchorPoint({ 0.5f, 0.5f });
    m_menu->setContentSize({ winSize.width, winSize.height });
    m_menu->setPosition({0,0});

    // bg
    auto bg = CCSprite::create("GJ_gradientBG.png");
    auto bgSize = bg->getTextureRect().size;

    bg->setAnchorPoint({ 0.0f, 0.0f });
    bg->setScaleX((winSize.width + 10.0f) / bgSize.width);
    bg->setScaleY((winSize.height + 10.0f) / bgSize.height);
    bg->setPosition({ -5.0f, -5.0f });
    bg->setColor({ 68, 70, 77 });

    this->addChild(bg);

    // back button
    CCSprite* backSpr = CCSprite::createWithSpriteFrameName("arrowback.png"_spr);
    CCMenuItemSpriteExtra* backBtn = CCMenuItemSpriteExtra::create(backSpr, this, menu_selector(ManageClicksLayer::onClose));
    backBtn->setPosition(25.0f, winSize.height - 25.0f);
    m_menu->addChild(backBtn);

    // bottom left buttons
    // settings button
    CCSprite* settingsSpr = CCSprite::createWithSpriteFrameName("settings.png"_spr);
    CCMenuItemSpriteExtra* settingsBtn = CCMenuItemSpriteExtra::create(settingsSpr, this, menu_selector(ManageClicksLayer::onSettings));
    settingsBtn->setPosition(25.0f, 25.0f);
    m_menu->addChild(settingsBtn);

    // spinner
    m_spinner = Spinner::create();
    m_spinner->setAnchorPoint({ 0.5f, 0.5f });
    m_spinner->setPosition({ winSize.width / 2, winSize.height / 2 });
    m_menu->addChild(m_spinner);

    this->addChild(m_menu);
    
    return true;
}

// button actions
void ManageClicksLayer::onClose(CCObject*) {
    if (m_shouldGoToLastScene == false) {
        CCDirector::sharedDirector()->pushScene(CCTransitionFade::create(0.5f, MenuLayer::scene(false)));
    } else {
        CCDirector::sharedDirector()->popSceneWithTransition(0.5f, PopTransition::kPopTransitionFade);
    }
}

void ManageClicksLayer::onSettings(CCObject*) {
    geode::openSettingsPopup(Mod::get());
}

// key back clicked
void ManageClicksLayer::keyBackClicked() {
    onClose(nullptr);
}