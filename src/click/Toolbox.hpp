#pragma once

#include <matjson.hpp>
#include <string>

namespace click {

//! @brief Static class with several helper methods.
class Toolbox final {
public:
    //! @brief Load a JSON file from the resources directory.
    //! @param fileName File name.
    //! @return Parsed JSON data.
    static matjson::Value readJSON(std::string const& fileName);
};

} // namespace click
