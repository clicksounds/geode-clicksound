#include "Author.hpp"

namespace click {

Author::Author(matjson::Value const& json) {
    m_name = json["name"].as_string();
    m_authorType = json["type"].as<click::Author::Category>();
    m_gdAccountID = json["gdAccountID"].as_number();
}

} // namespace click
