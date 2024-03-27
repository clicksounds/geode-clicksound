#include <Geode/modify/ProfilePage.hpp>

#include "../click/Manager.hpp"

using namespace geode::prelude;

class $modify(CustomProfilePage, ProfilePage) {
    //! @brief Add developer badge to mod contributors.
    bool init(int accountID, bool p1) {
        if (!ProfilePage::init(accountID, p1)) {
            return false;
        }

        addContributorBadge();
        return true;
    }

    void addContributorBadge() {
        auto* mgr = click::Manager::sharedManager();

        if (!mgr->isContributor(m_accountID)) {
            return;
        }

        auto badge = CCMenuItemSpriteExtra::create(
            CCSprite::create("csBadge.png"_spr),
            this,
            menu_selector(CustomProfilePage::onBadgeClicked)
        );
        badge->setID("cs-badge"_spr);

        auto* leftMenu = getChildByIDRecursive("left-menu");
        leftMenu->addChild(badge);
        leftMenu->updateLayout();
    }

    //! @brief On developer badge clicked.
    void onBadgeClicked(CCObject* sender) {
        FLAlertLayer::create(
            "About",

            "This badge is given to <cp>Click Sound developers</c>. "
            "If you have programmed anything that was added to Click Sound, "
            "you will obtain this badge. ^w^",

            "OK"
        )->show();
    }
};
