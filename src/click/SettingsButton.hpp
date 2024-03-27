#pragma once

#include <cocos2d.h>

namespace click {

//! @brief Button to open the mod settings menu.
class SettingsButton final : public cocos2d::CCMenu {
public:
    //! @brief Create settings button.
    //! @return Settings button.
    static SettingsButton* create();

    //! @brief Initialize settings button.
    //! @return Whether the initialization was successful.
    bool init();

private:
    //! @brief Callback when settings button is clicked.
    //! @param sender Sender.
    void onClicked(cocos2d::CCObject* sender);
};

} // namespace click
