#include "Sound.hpp"

namespace click {

Sound::Sound(matjson::Value const& json) {
    m_name = json["name"].as_string();
    m_prefix = json["prefix"].as_string();
}

} // namespace click
