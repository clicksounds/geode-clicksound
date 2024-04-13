#pragma once

#include <matjson.hpp>
#include <array>

namespace click {

//! @brief Click Pack Author
class Author final {
public:
    //! @brief Contruct Click Pack Author from JSON.
    //! @param json JSON value
    Author(matjson::Value const& json);

    //! @brief Author type enum
    enum class Type { Main = 0, Provider = 1, Requester = 2};

    std::string const& getName() const { return m_name; }
    Type const& getType() const { return m_authorType; }
    int const& getAccount() const { return m_gdAccountID; }
    
private:
    std::string m_name;
    Type m_authorType;
    int m_gdAccountID;
};

} // namespace click

template<>
struct matjson::Serialize<click::Author> {
    static click::Author from_json(matjson::Value const& json) {
        return click::Author(json);
    }

    static bool is_json(matjson::Value const& json) {
        return true;
    }
};

template<>
struct matjson::Serialize<click::Author::Type> {
    static click::Author::Type from_json(matjson::Value const& json) {
        std::string typeString = json.as_string();
        if (typeString == "Main")
            return click::Author::Type::Main;
        else if (typeString == "Provider")
            return click::Author::Type::Provider;
        else if (typeString == "Requester")
            return click::Author::Type::Requester;
        else
            return click::Author::Type::Main;
    }

    static bool is_json(matjson::Value const& json) {
        return false;
    }
};