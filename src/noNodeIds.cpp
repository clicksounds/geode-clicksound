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
        if (nodeIDSmod) {
        auto nodeIdsMetadata = nodeIDSmod->getMetadata();
        Mod theNodeIds = Mod(nodeIdsMetadata);
        Mod* theNodeId2 = &theNodeIds;
        geode::openIndexPopup(theNodeId2);
        }
        else {
            auto alerterror = FLAlertLayer::create(
            "Click Sounds Error",
            "Unable to fetch mod, please download from the <cp>geode website!</c> ",  
            "OK"
        );
        alerterror->show();
        //this->getChildByID("bottom-menu")->getChildByID("geode.loader/geode-button")->setVisible(true);
        }
    };
    void index2(CCObject*) {
        utils::game::restart();
    };

    void initUi() {
        auto winSize = CCDirector::sharedDirector()->getWinSize();
        // auto spr = ButtonSprite::create("/nodeIdsLogo.png"_spr);
        auto spr = CCSprite::create("nodeIdsLogo.png"_spr);
        auto btn = CCMenuItemSpriteExtra::create(
            spr, this, menu_selector(newl::index)
        );
        btn->setScale(.7);
        btn->setPosition(18.5, 28);
        this->getChildByID("bottom-menu")->addChild(btn);
    };
    void initUi2() {
        auto winSize = CCDirector::sharedDirector()->getWinSize();
        // auto spr = ButtonSprite::create("/nodeIdsLogo.png"_spr);
        auto spr = ButtonSprite::create("Restart Game");
        auto btn = CCMenuItemSpriteExtra::create(
            spr, this, menu_selector(newl::index2)
        );
        btn->setScale(.7);
        btn->setPosition(18.5, -97);
        this->getChildByID("bottom-menu")->addChild(btn);
    };

    static void onModify(auto& self) {
        self.setHookPriority("MenuLayer::init", -100); // GO LAST :O (IDK WHY IT SAID GO FIRST)
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
        
            this->getChildByID("bottom-menu")->setLayout(
                RowLayout::create()
                ->setGap(14)
                 ->setGrowCrossAxis(true)
                 ->setCrossAxisReverse(true)
                 ->setAutoScale(true)
                );
        this->getChildByID("bottom-menu")->getChildByID("geode.loader/geode-button")->setScale(0.2);
        this->getChildByID("bottom-menu")->getChildByID("geode.loader/geode-button")->setPosition(99999, 99999);
        this->getChildByID("bottom-menu")->setPosition(winSize.width / 2, winSize.height / 2);
         this->getChildByID("bottom-menu")->setScale(1.1);
        this->getChildByID("bottom-menu")->setVisible(true);
        alert->show();
        newl::initUi2();
        newl::initUi();
         this->getChildByID("bottom-menu")->updateLayout();
        /*auto listener = EventListener<ModInstallFilter>(+[](ModInstallEvent* ev) {
        this->setVisible(false);
        hello2->setVisible(true);
        }, ModInstallFilter("geode.node-ids"));*/
           
               
        return true;
    };
};


