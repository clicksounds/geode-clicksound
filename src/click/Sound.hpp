#pragma once

#include <matjson.hpp>

namespace click {

//! @brief A click or release sound.
class Sound final {
public: 
    //! @brief Construct click sound from JSON.
    //! @param json JSON value.
    Sound(matjson::Value const& json);

    std::string const& getName() const { return m_name; }
    std::string const& getPrefix() const { return m_prefix; }

private:
    std::string m_name;
    std::string m_prefix;
};

} // namespace click

template<>
struct matjson::Serialize<click::Sound> {
    static click::Sound from_json(matjson::Value const& json) {
        return click::Sound(json);
    }

    static bool is_json(matjson::Value const& json) {
        return true;
    }
};
