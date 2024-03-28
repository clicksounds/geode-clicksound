#include <Geode/modify/PauseLayer.hpp>

#include <Geode/loader/Loader.hpp>

#include "../click/Manager.hpp"
#include "../click/SettingsButton.hpp"

class $modify(PauseLayer) {
    void customSetup() {
        PauseLayer::customSetup();

        auto* loader = geode::Loader::get();
        if (!loader->isModLoaded("geode.node-ids")) {
            addNodeIDs();
        }

        auto* mgr = click::Manager::sharedManager();
        if (mgr->getSettingsButton()) {
            addSettingsButton();
        }
    }

    //! @brief Provide our own node IDs if the user doesn't have the mod
    //! installed.
    void addNodeIDs() {
        auto* director = cocos2d::CCDirector::sharedDirector();
        auto winSize = director->getWinSize();

        auto* leftMenu = cocos2d::CCMenu::create();
        leftMenu->setLayout(
            cocos2d::ColumnLayout::create()
                ->setGap(4.0f)
                ->setAxisAlignment(cocos2d::AxisAlignment::End)
                ->setAxisReverse(true)
                ->setCrossAxisOverflow(true)
        );
        leftMenu->setID("left-button-menu");
        leftMenu->setPosition({
            36.0f,
            winSize.height / 2.0f
        });
        leftMenu->setContentSize({
            40.0f,
            winSize.height - 40.0f
        });
        leftMenu->setZOrder(10);
        
        addChild(leftMenu);
    }

    void addSettingsButton() {
        auto* button = click::SettingsButton::create();

        auto* menu = getChildByID("left-button-menu");
        menu->addChild(button);
        menu->updateLayout();
    }
};
