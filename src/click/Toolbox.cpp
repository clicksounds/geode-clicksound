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

} // namespace click
