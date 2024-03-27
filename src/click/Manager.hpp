#pragma once

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
    bool isContributor(int accountID) {
        return m_contributors.contains(accountID);
    }

private:
    static Manager* s_sharedManager;
    
    std::unordered_set<int> m_contributors;
};

} // namespace click
