#include <Geode/modify/ProfilePage.hpp>

#include <Geode/binding/CCMenuItemSpriteExtra.hpp>
#include <Geode/loader/Loader.hpp>

#include "../click/Toolbox.hpp"
#include "../click/Manager.hpp"

class $modify(CustomProfilePage, ProfilePage) {
    bool init(int accountID, bool p1) {
        if (!ProfilePage::init(accountID, p1)) {
            return false;
        }

        auto* loader = geode::Loader::get();
        if (!loader->isModLoaded("geode.node-ids")) {
            addNodeIDs();
        }

        addContributorBadge();
        return true;
    }

    //! @brief Provide our own node IDs if the user doesn't have the mod
    //! installed.
    void addNodeIDs() {
        auto* director = cocos2d::CCDirector::sharedDirector();
        auto winSize = director->getWinSize();

        auto* leftMenu = cocos2d::CCMenu::create();
        leftMenu->setLayout(
            cocos2d::ColumnLayout::create()
                ->setGap(6.0f)
                ->setAxisAlignment(cocos2d::AxisAlignment::End)
                ->setAxisReverse(true)
        );
        leftMenu->setID("left-menu");
        leftMenu->setPosition({
            (winSize.width / 2.0f) - 195.0f,
            (winSize.height / 2.0f) + 12.0f,
        });
        leftMenu->setContentSize({ 60.0f, 90.0f });
        leftMenu->setZOrder(10);

        m_mainLayer->addChild(leftMenu);
    }

    //! @brief Add developer badge to mod contributors.
    void addContributorBadge() {
        auto* mgr = click::Manager::sharedManager();

        if (!mgr->isContributor(m_accountID)) {
            return;
        }

        auto badge = CCMenuItemSpriteExtra::create(
            click::Toolbox::createSprite("clickIcon_001.png"_spr),
            this,
            menu_selector(CustomProfilePage::onBadgeClicked)
        );
        badge->setID("cs-badge"_spr);

        auto* leftMenu = getChildByIDRecursive("left-menu");
        leftMenu->addChild(badge);
        leftMenu->updateLayout();
    }

    //! @brief On developer badge clicked.
    void onBadgeClicked(cocos2d::CCObject* sender) {
        FLAlertLayer::create(
            "About",

            "This badge is given to <cp>Click Sound developers</c>. "
            "If you have programmed anything that was added to Click Sound, "
            "you will obtain this badge. ^w^",

            "OK"
        )->show();
    }
};
