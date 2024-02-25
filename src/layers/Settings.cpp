#include <Geode/Geode.hpp>
#include "../utils/Settings.hpp"
#include "../utils/Clicks.hpp"

using namespace geode::prelude;

SettingNode* SettingClickValue::createNode(float width) {
    return SettingClickNode::create(this, width);
}

void SettingClickNode::onClickBtn(CCObject*) {
	auto clickSliderValue = Mod::get()->getSettingValue<int64_t>("clicksound-currentsound");
    auto customClickSound = Mod::get()->getSettingValue<ghc::filesystem::path>("custom-clicksound").string();
    bool usingCustomClickSound;
    std::string clickSoundInUse = Clicks::getClickSprite(clickSliderValue);

    usingCustomClickSound = false;
    if (clickSliderValue != 0) usingCustomClickSound = false;
    if (clickSoundInUse == "__USECUSTOM__") usingCustomClickSound = true;
    
    
     if (Mod::get()->getSettingValue<bool>("enable-clicksound") && !usingCustomClickSound) {
        FMODAudioEngine::sharedEngine()->playEffect(clickSoundInUse, 1.0f, 1.0f, 2.0f);
    } 

    if (Mod::get()->getSettingValue<bool>("enable-clicksound") && usingCustomClickSound) {
        FMODAudioEngine::sharedEngine()->playEffect(customClickSound);
    }
}

SettingNode* SettingReleaseValue::createNode(float width) {
    return SettingReleaseNode::create(this, width);
}

void SettingReleaseNode::onReleaseBtn(CCObject*) {
	auto releaseSliderValue = Mod::get()->getSettingValue<int64_t>("releasesound-currentsound");
    auto customReleaseSound = Mod::get()->getSettingValue<ghc::filesystem::path>("custom-releasesound").string();
    bool usingCustomReleaseSound;
    std::string releaseSoundInUse = Clicks::getReleaseSprite(releaseSliderValue);


    if (releaseSliderValue != 0) usingCustomReleaseSound = false;
    if (releaseSoundInUse == "__USECUSTOM__") usingCustomReleaseSound = true;
 
    if (Mod::get()->getSettingValue<bool>("enable-releasesound") && !usingCustomReleaseSound) {
        FMODAudioEngine::sharedEngine()->playEffect(releaseSoundInUse);
    } 

    if (Mod::get()->getSettingValue<bool>("enable-releasesound") && usingCustomReleaseSound) {
        FMODAudioEngine::sharedEngine()->playEffect(customReleaseSound);
    }
}

$on_mod(Loaded) {
    Mod::get()->addCustomSetting<SettingClickValue>("test-click-btn", "none");
    Mod::get()->addCustomSetting<SettingReleaseValue>("test-release-btn", "none");
}
