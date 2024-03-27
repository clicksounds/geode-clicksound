#pragma once

namespace click {

//! @brief Manager singleton class.
class Manager final {
public:
    //! @brief Get manager singleton.
    //! @return Manager singleton.
    static Manager* sharedManager();

    Manager();
    ~Manager() {}

private:
    static Manager* s_sharedManager;
};

} // namespace click
