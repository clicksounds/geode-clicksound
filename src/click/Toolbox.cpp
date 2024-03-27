#include "Toolbox.hpp"

#include <Geode/utils/file.hpp>
#include <cocos2d.h>

using namespace geode::prelude;

namespace click {

matjson::Value Toolbox::readJSON(std::string const& fileName) {
    auto* fileUtils = CCFileUtils::sharedFileUtils();
    std::string path = fileUtils->fullPathForFilename(
        fileName.c_str(),
        false
    );

    return utils::file::readJson(path).unwrap();
}

CCSprite* Toolbox::createSprite(std::string const& name) {
    auto* sprite = CCSprite::create();

    if (sprite && initSprite(sprite, name)) {
        return sprite;
    }

    return nullptr;
}

bool Toolbox::initSprite(CCSprite* sprite, std::string const& name) {
    if (sprite->initWithFile(name.c_str())) {
        return true;
    }

    if (sprite->initWithSpriteFrameName(name.c_str())) {
        return true;
    }

    return false;
}

} // namespace click
