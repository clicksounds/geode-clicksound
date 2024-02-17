#include <Geode/Geode.hpp>
#include <Geode/modify/MenuLayer.hpp>
#include <Geode/loader/Mod.hpp>
#include <Geode/loader/Loader.hpp>

using namespace geode::prelude;
// test disabling the menulayer
class $modify(MenuLayer) { 
static void onModify(auto& self) {
        self.setHookPriority("MenuLayer::init", 10000000); // GO FIRST :O
    }
     bool init() {
        if (!MenuLayer::init())
            return false;
        
        if (Loader::get()->isModLoaded("geode.node-ids")) {
            return true;
        };

           auto alert = FLAlertLayer::create(
             "Blud Doesn't have NodeIDs",
             "Please install node ids from <cp>geode index!</c> ",  
              "OK"
            );
            alert->m_scene = this;
            alert->show();
        
        return true;
     };
};