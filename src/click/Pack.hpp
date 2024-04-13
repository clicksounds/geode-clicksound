#pragma once

#include "Author.hpp"
#include <matjson.hpp>
#include <array>

namespace click {

//! @brief A click or release pack.
class Pack final {
public: 
    //! @brief Construct Click Pack from JSON.
    //! @param json JSON value.
    //! @param isServer is this a mod on the server?
    Pack(matjson::Value const& json, bool isServer = false);

    //! @brief Category Types
    enum class Category { Useful = 0, Meme = 1, Unknown = 2 };

    std::string const& getName() const { return m_name; }
    std::string const& getID() const { return m_id; }
    Category const& getCategory() const { return m_category; }
    std::vector<click::Author> const& getAuthors() const { return m_authors; }
    std::vector<std::string> const& getClickFiles() const { return m_clickFiles; }
    std::vector<std::string> const& getReleaseFiles() const { return m_releaseFiles; }
    bool const& isServer() const { return m_isServer; }

private:
    std::string m_name;
    std::string m_id;
    Category m_category;
    std::vector<click::Author> m_authors;
    std::vector<std::string> m_clickFiles;
    std::vector<std::string> m_releaseFiles;
    bool m_isServer;
};

} // namespace click

template<>
struct matjson::Serialize<click::Pack> {
    static click::Pack from_json(matjson::Value const& json, bool isServer=false) {
        return click::Pack(json, isServer);
    }

    static bool is_json(matjson::Value const& json) {
        return true;
    }
};

template<>
struct matjson::Serialize<click::Pack::Category> {
    static click::Pack::Category from_json(matjson::Value const& json) {
        std::string category = json.as_string();
        if (category == "Useful") {
            return click::Pack::Category::Useful;
        } else if (category == "Meme") {
            return click::Pack::Category::Meme;
        } else {
            return click::Pack::Category::Unknown;
        }
    }

    static bool is_json(matjson::Value const& json) {
        return false;
    }
};
