#pragma once

#include <cocos2d.h>
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

    //! @brief Create a sprite from either file or sprite frame.
    //! @param name Image file name or sprite frame name.
    //! @return Sprite.
    static cocos2d::CCSprite* createSprite(std::string const& image);

    //! @brief Initialize a sprite from either file or sprite frame.
    //! @param name Image file name or sprite frame name.
    //! @return Whether the initialization was successful.
    static bool initSprite(cocos2d::CCSprite* sprite, std::string const& name);
};

} // namespace click
