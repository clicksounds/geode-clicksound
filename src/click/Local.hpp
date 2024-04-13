#pragma once

#include "Pack.hpp"
namespace click {

class Local final {
public:

    Local();

    //! @brief get shared local manager
    static Local* get();

    //! @brief Saves Data
    void save();


private:
    static Local* s_shared;

    click::Pack m_currentClicks;
    click::Pack m_currentRelease;
    std::vector<click::Pack> m_installedPacks;
};

} // namespace click
