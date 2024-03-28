#include <Geode/modify/EditorPauseLayer.hpp>

#include <Geode/binding/CCMenuItemSpriteExtra.hpp>
#include <Geode/loader/Loader.hpp>
#include <Geode/utils/cocos.hpp>
#include <Geode/utils/NodeIDs.hpp>

#include "../click/Manager.hpp"
#include "../click/SettingsButton.hpp"

class $modify(EditorPauseLayer) {
    bool init(LevelEditorLayer* levelEditorLayer) {
        if (!EditorPauseLayer::init(levelEditorLayer)) {
            return false;
        }

        auto* loader = geode::Loader::get();
        if (!loader->isModLoaded("geode.node-ids")) {
            addNodeIDs();
        }

        auto* mgr = click::Manager::sharedManager();
        if (mgr->getSettingsButton()) {
            addSettingsButton();
        }

        return true;
    }

    //! @brief Provide our own node IDs if the user doesn't have the mod
    //! installed.
    void addNodeIDs() {
        using namespace geode::cocos;
        using namespace geode::node_ids;

        // This is not how Node IDs implements this, but it works.
        auto* settingsButton = findFirstChildRecursive<cocos2d::CCNode>(
            this,
            [](auto* node) {
                return isSpriteFrameName(
                    node,
                    "GJ_optionsBtn_001.png"
                );
            }
        );

        auto* settingsMenu = detachAndCreateMenu(
            this,
            "settings-menu",
            cocos2d::RowLayout::create()
                ->setAxisReverse(true),
            settingsButton
        );
        settingsMenu->setContentSize({ 95.0f, 50.0f });
        settingsMenu->updateLayout();
    }

    void addSettingsButton() {
        auto* button = click::SettingsButton::create();

        auto* menu = getChildByID("settings-menu");
        menu->addChild(button);
        menu->updateLayout();
    }
};
