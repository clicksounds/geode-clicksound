#include "Local.hpp"

namespace click {

Local* Local::s_shared = nullptr;

Local* Local::get() {
    if (s_shared == nullptr) {
        s_shared = new (std::nothrow) Local();
    }

    return s_shared;
}

void Local::save() {}

Local::Local() {
    // osu hit (default)
    m_installedPacks.push_back(Pack(matjson::parse(R"(
        {
            "$schema": "https://clicksounds.github.io/clicks/pack.schema.json",
            "id": "peppy.osuhit",
            "type": "Useful",
            "name": "osu! Hit Sound",
            "authors": [
                {
                    "name": "peppy (Dean Herbert) ",
                    "type": "Main"
                }
            ],
            "click-files": [
                "peppy.osuhit_Clicks_Useful_1.ogg"
            ],
            "release-files": []
        }
    )"), false));

}

} // namespace click
