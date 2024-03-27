#pragma once

#include <Geode/loader/Mod.hpp>
#include <unordered_set>

namespace click {

//! @brief Manager singleton class.
class Manager final {
public:
    //! @brief Get manager singleton.
    //! @return Manager singleton.
    static Manager* sharedManager();

    Manager();
    ~Manager() {}

    //! @brief Get whether an account ID is held by a contributor.
    //! @return Whether an account ID is held by a contributor.
    bool isContributor(int accountID) const {
        return m_contributors.contains(accountID);
    }

    //! @brief Get whether the settings button is enabled.
    //! @return Whether the settings button is enabled.
    bool getSettingsButton() const {
        return geode::Mod::get()->getSettingValue<bool>("settings-button");
    }

private:
    static Manager* s_sharedManager;
    
    std::unordered_set<int> m_contributors;
};

} // namespace click
