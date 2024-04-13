#pragma once

namespace click {

class Local final {
public:

    //! @brief get shared local manager
    static Local* get();


private:
    static Local* s_shared;

};

} // namespace click
