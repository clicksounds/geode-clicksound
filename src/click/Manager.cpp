#include "Manager.hpp"

#include <matjson/stl_serialize.hpp>
#include "Toolbox.hpp"

namespace click {

Manager* Manager::s_sharedManager = nullptr;

Manager* Manager::sharedManager() {
    if (s_sharedManager == nullptr) {
        s_sharedManager = new (std::nothrow) Manager();
    }

    return s_sharedManager;
}

Manager::Manager() {
    // Read contributors from file.
    auto contributors = Toolbox::readJSON("contributors.json"_spr);
    m_contributors = contributors["contributors"]
        .as<std::unordered_set<int>>();
}

} // namespace click
