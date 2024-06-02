#include <Geode/Geode.hpp>
#include "./ManageClicksLayer.hpp"
#include <cctype>
#include <algorithm>
#include <matjson.hpp>
using namespace geode::prelude;

ManageClicksLayer* ManageClicksLayer::create(CCScene* lastScene = nullptr) {
    auto ret = new ManageClicksLayer();
    ret->m_lastScene = lastScene;
    if (ret && ret->init()) {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
};

CCScene* ManageClicksLayer::scene(CCScene* lastScene = nullptr) {
    auto layer = ManageClicksLayer::create(lastScene);
    auto scene = CCScene::create();
    scene->addChild(layer);
    return scene;
}


bool ManageClicksLayer::init() {

    m_menu = CCMenu::create();
    // bg
    auto winSize = CCDirector::sharedDirector()->getWinSize();

    auto bg = CCSprite::create("GJ_gradientBG.png");
    auto bgSize = bg->getTextureRect().size;

    bg->setAnchorPoint({ 0.0f, 0.0f });
    bg->setScaleX((winSize.width + 10.0f) / bgSize.width);
    bg->setScaleY((winSize.height + 10.0f) / bgSize.height);
    bg->setPosition({ -5.0f, -5.0f });
    bg->setColor({ 68, 70, 77 });

    this->addChild(bg);

    // back button
    CCSprite* backSpr = CCSprite::createWithSpriteFrameName("GJ_arrow_01_001.png");
    CCMenuItemSpriteExtra* backBtn = CCMenuItemSpriteExtra::create(backSpr, this, menu_selector(ManageClicksLayer::onClose));
    backBtn->setPosition(-winSize.width / 2 + 25.0f, winSize.height / 2 - 25.0f);
    m_menu->addChild(backBtn);


    this->addChild(m_menu);
    
    return true;
}

void ManageClicksLayer::keyBackClicked() {
    this->onClose(nullptr);
}

void ManageClicksLayer::onClose(CCObject*) {
    CCScene* scene;
    if (m_lastScene == nullptr) {
        scene = MenuLayer::scene(false);
    } else {
        scene = m_lastScene;
    }
    CCDirector::sharedDirector()->pushScene(CCTransitionFade::create(0.5f, scene));
}