#include <Geode/Geode.hpp>
#include <Geode/modify/MenuLayer.hpp>
#include <Geode/loader/Mod.hpp>
#include <Geode/loader/Loader.hpp>

using namespace geode::prelude;
// test disabling the menulayer
class $modify(MenuLayer) { 
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
             "Please install node ids from <cp>geode index!</c> ",  
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
             this->getChildByID("close-menu")->setVisible(true);
            this->getChildByID("bottom-menu")->setScale(4);
              this->getChildByID("bottom-menu")->setPosition(winSize.width / 2, winSize.height / 2);
             this->getChildByID("bottom-menu")->setLayout(
                RowLayout::create()
                ->setGap(-51)
             );
            this->getChildByID("bottom-menu")->getChildByID("geode.loader/geode-button")->setVisible(true);
            alert->show();
        return true;
     };
};