#include <Geode/Geode.hpp>
#include <Geode/modify/MenuLayer.hpp>
#include <Geode/loader/Mod.hpp>
#include <Geode/loader/Loader.hpp>
#include <Geode/ui/GeodeUI.hpp>
#include <Geode/loader/Index.hpp>

using namespace geode::prelude;

// test disabling the menulayer
class $modify(newl,MenuLayer) { 
    void index(CCObject*) {
        auto nodeIDSmod = geode::Index::get()->getItemsByModID("geode.node-ids").back();
        auto nodeIdsMetadata = nodeIDSmod->getMetadata();
        Mod theNodeIds = Mod(nodeIdsMetadata);
        Mod* theNodeId2 = &theNodeIds;
        geode::openIndexPopup(theNodeId2);
    };
    void index2(CCObject*) {
        utils::game::restart();
    };

    CCMenuItemSpriteExtra* initUi() {
        auto winSize = CCDirector::sharedDirector()->getWinSize();
        // auto spr = ButtonSprite::create("/nodeIdsLogo.png"_spr);
        auto spr = CCSprite::create("nodeIdsLogo.png"_spr);
        auto btn = CCMenuItemSpriteExtra::create(
            spr, this, menu_selector(newl::index)
        );
        btn->setScale(1.7);
        btn->setPosition(winSize.width / 2, winSize.height / 2);
        this->getChildByID("bottom-menu")->addChild(btn);
        return btn;
    };
    CCMenuItemSpriteExtra* initUi2() {
        auto winSize = CCDirector::sharedDirector()->getWinSize();
        // auto spr = ButtonSprite::create("/nodeIdsLogo.png"_spr);
        auto spr = ButtonSprite::create("Restart Game");
        auto btn = CCMenuItemSpriteExtra::create(
            spr, this, menu_selector(newl::index2)
        );
        btn->setScale(1.7);
        btn->setPosition(winSize.width / 2, winSize.height / 2);
        btn->setVisible(false);
        this->getChildByID("bottom-menu")->addChild(btn);
        return btn;
    };

    static void onModify(auto& self) {
        self.setHookPriority("MenuLayer::init", -100); // GO FIRST :O
    }
    bool init() {
        auto winSize = CCDirector::get()->getWinSize();
        if (!MenuLayer::init())
            return false;
        
        if (Loader::get()->isModLoaded("geode.node-ids")) {
            return true;
        };

        auto alert = FLAlertLayer::create(
            "Click Sounds Error",
            "Please install node ids from the <cp>geode index!</c> ",  
            "OK"
        );
        alert->m_scene = this;
        
        for(auto items : CCArrayExt<CCNode*>(this->getChildren())) {
            items->setVisible(false);
        };
            for(auto items : CCArrayExt<CCNode*>(this->getChildByID("bottom-menu")->getChildren())) {
            items->setVisible(false);
        };
        this->getChildByID("bottom-menu")->setVisible(true);
            if (this->getChildByID("close-menu")) {
            this->getChildByID("close-menu")->setVisible(true);
            }
        
            this->getChildByID("bottom-menu")->setLayout(RowLayout::create()->setGap(-51));
        this->getChildByID("bottom-menu")->getChildByID("geode.loader/geode-button")->setScale(0.2);
        this->getChildByID("bottom-menu")->getChildByID("geode.loader/geode-button")->setPosition(99999, 99999);
        this->getChildByID("bottom-menu")->setPosition(winSize.width / 2, winSize.height / 2);
        this->getChildByID("bottom-menu")->getChildByID("geode.loader/geode-button")->setVisible(false);
        this->getChildByID("bottom-menu")->setVisible(false);
        alert->show();
        auto hello = newl::initUi();
        auto hello2 = newll::initUi2();
        auto listener = EventListener<ModInstallFilter>(+[](ModInstallEvent* ev) {
        hello->setVisible(false);
        hello2->setVisible(true);
        }, ModInstallFilter("geode.node-ids"));
           
               
        return true;
    };
};


