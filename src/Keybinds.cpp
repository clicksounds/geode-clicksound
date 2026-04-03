#include <Geode/Geode.hpp>

using namespace geode::prelude;

static void registerToggleKeybind(std::string keybindId, std::string settingKey, std::string displayName) {
    listenForKeybindSettingPresses(keybindId, [settingKey, displayName](Keybind const& keybind, bool down, bool repeat, double timestamp) {
        static geode::Ref<Notification> notification;
        if (down && !repeat) {
            if (notification) notification->cancel();

            auto newVal = !Mod::get()->getSettingValue<bool>(settingKey);
            Mod::get()->setSettingValue<bool>(settingKey, newVal);

            notification = Notification::create(
                fmt::format("CS: {} has been {}", displayName, newVal ? "enabled" : "disabled"),
                CCSprite::createWithSpriteFrameName(newVal ? "GJ_completesIcon_001.png" : "GJ_deleteIcon_001.png")
            );
            notification->show();
        }
    });
}

$on_mod(Loaded) {
    registerToggleKeybind("master-toggle-keybind", "enable-master", "Master");
    registerToggleKeybind("sounds-everywhere-keybind", "sounds-everywhere", "Sounds everywhere");
};