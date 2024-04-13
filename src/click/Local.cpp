#include "Local.hpp"

namespace click {

Local* Local::s_shared = nullptr;

Local* Local::get() {
    if (s_shared == nullptr) {
        s_shared = new (std::nothrow) Local();
    }

    return s_shared;
}

} // namespace click
