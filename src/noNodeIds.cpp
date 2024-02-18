#include <Geode/Geode.hpp>
#include <Geode/modify/MenuLayer.hpp>
#include <Geode/loader/Mod.hpp>
#include <Geode/loader/Loader.hpp>
#include <Geode/ui/GeodeUI.hpp>

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

    bool initUi() {
        auto winSize = CCDirector::sharedDirector()->getWinSize();
        auto spr = ButtonSprite::create("/nodeIdsLogo.png"_spr);
        auto btn = CCMenuItemSpriteExtra::create(
            spr, this, menu_selector(newl::index)
        );
        btn->setScale(4);
        btn->setPosition(winSize.width / 2, winSize.height / 2);
        this->addChild(btn);
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
        /*
            this->getChildByID("bottom-menu")->setLayout(
            RowLayout::create()
            ->setGap(-51)
            );*/
        this->getChildByID("bottom-menu")->getChildByID("geode.loader/geode-button")->setScale(0.0001);
        this->getChildByID("bottom-menu")->setPosition(999999, 999999);
        this->getChildByID("bottom-menu")->getChildByID("geode.loader/geode-button")->setVisible(true);
        alert->show();
        newl::initUi();
           
               
        return true;
    };
};
