#include "Toolbox.hpp"

#include <Geode/utils/file.hpp>
#include <cocos2d.h>

namespace click {

matjson::Value Toolbox::readJSON(std::string const& fileName) {
    auto* fileUtils = cocos2d::CCFileUtils::sharedFileUtils();
    std::string path = fileUtils->fullPathForFilename(
        fileName.c_str(),
        false
    );

    return geode::utils::file::readJson(path).unwrap();
}

cocos2d::CCSprite* Toolbox::createSprite(std::string const& name) {
    auto* sprite = cocos2d::CCSprite::create();

    if (sprite && initSprite(sprite, name)) {
        return sprite;
    }

    return nullptr;
}

bool Toolbox::initSprite(cocos2d::CCSprite* sprite, std::string const& name) {
    if (sprite->initWithFile(name.c_str())) {
        return true;
    }

    if (sprite->initWithSpriteFrameName(name.c_str())) {
        return true;
    }

    return false;
}

} // namespace click
