#include <Geode/modify/EditorPauseLayer.hpp>

#include "../click/Manager.hpp"
#include "../click/SettingsButton.hpp"

class $modify(EditorPauseLayer) {
    bool init(LevelEditorLayer* levelEditorLayer) {
        if (!EditorPauseLayer::init(levelEditorLayer)) {
            return false;
        }

        auto* mgr = click::Manager::sharedManager();
        if (mgr->getSettingsButton()) {
            addSettingsButton();
        }

        return true;
    }

    void addSettingsButton() {
        auto* button = click::SettingsButton::create();

        auto* menu = getChildByID("settings-menu");
        menu->addChild(button);
        menu->updateLayout();
    }
};
