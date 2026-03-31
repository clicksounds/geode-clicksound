#include <Geode/Geode.hpp>

using namespace geode::prelude;

$on_mod(Loaded) {
    listenForKeybindSettingPresses("master-toggle-keybind", [](Keybind const& keybind, bool down, bool repeat, double timestamp) {
        static geode::Ref<Notification> notification;
        if (down && !repeat) {
            if (notification) {
                notification->cancel();
            }
            Mod::get()->setSettingValue<bool>("enable-master", !Mod::get()->getSettingValue<bool>("enable-master"));
            notification = Notification::create(
                fmt::format("CS: Master has been {}", 
                    Mod::get()->getSettingValue<bool>("enable-master") ? "enabled" : "disabled"
                ), 
                CCSprite::createWithSpriteFrameName(
                    Mod::get()->getSettingValue<bool>("enable-master") ? "GJ_completesIcon_001.png" : "GJ_deleteIcon_001.png"
                )
            );
            notification->show();
        }
    });
};