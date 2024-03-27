#include <Geode/modify/PauseLayer.hpp>

#include "../click/Manager.hpp"
#include "../click/SettingsButton.hpp"

using namespace geode::prelude;

class $modify(PauseLayer) {
    void customSetup() {
        PauseLayer::customSetup();

        auto* mgr = click::Manager::sharedManager();
        if (mgr->getSettingsButton()) {
            addSettingsButton();
        }
    }

    void addSettingsButton() {
        auto* button = click::SettingsButton::create();

        auto* menu = getChildByID("left-button-menu");
        menu->addChild(button);
        menu->updateLayout();
    }
};
