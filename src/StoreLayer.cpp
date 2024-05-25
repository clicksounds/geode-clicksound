#include <Geode/Geode.hpp>
#include "./StoreLayer.hpp"
#include <cctype>
#include <algorithm>
#include <matjson.hpp>
using namespace geode::prelude;

StoreLayer* StoreLayer::create() {
    auto ret = new StoreLayer();
    if (ret && ret->init()) {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
};

CCScene* StoreLayer::scene() {
    auto layer = StoreLayer::create();
    auto scene = CCScene::create();
    scene->addChild(layer);
    return scene;
}


bool StoreLayer::init() {

    m_menu = CCMenu::create();
    // bg
    auto winSize = CCDirector::sharedDirector()->getWinSize();

    auto bg = CCSprite::create("GJ_gradientBG.png");
    auto bgSize = bg->getTextureRect().size;

    bg->setAnchorPoint({ 0.0f, 0.0f });
    bg->setScaleX((winSize.width + 10.0f) / bgSize.width);
    bg->setScaleY((winSize.height + 10.0f) / bgSize.height);
    bg->setPosition({ -5.0f, -5.0f });
    bg->setColor({ 0, 102, 255 });

    this->addChild(bg);

    // back button
    CCSprite* backSpr = CCSprite::createWithSpriteFrameName("GJ_arrow_01_001.png");
    CCMenuItemSpriteExtra* backBtn = CCMenuItemSpriteExtra::create(backSpr, this, menu_selector(StoreLayer::onClose));
    backBtn->setPosition(-winSize.width / 2 + 25.0f, winSize.height / 2 - 25.0f);
    m_menu->addChild(backBtn);
}