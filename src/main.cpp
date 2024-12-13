#include <Geode/Geode.hpp>
#include <Geode/modify/PlayerObject.hpp>
#include <Geode/modify/PauseLayer.hpp>
#include <Geode/ui/GeodeUI.hpp>
#include <Geode/loader/SettingV3.hpp>
#include "SettingsV3/PlaySound.hpp"
#include "SettingsV3/SelectionEnum.hpp"
#include "StaticClasses.hpp"
#include "jsonReader/Getsettingsinfo.hpp"
#include <thread>
#include <Geode/modify/MenuLayer.hpp>
#include <Geode/utils/web.hpp>
#include <Geode/loader/Event.hpp>
using namespace geode::prelude;

// the check to see if you should play the sound or not
bool integrityCheck(PlayerObject* object, PlayerButton Pressed) {
    // play sounds when "only play on jump" settings is enabled and the player input is a jump, left movement, or right movement.
    if (Mod::get()->getSettingValue<bool>("only-on-jump")) {
        if (Pressed != PlayerButton::Jump) {
            return false;
        }
    }
    GJGameLevel* Level;
     if (!PlayLayer::get()) {
        if (!LevelEditorLayer::get()) {
            return false;
        }
        Level = LevelEditorLayer::get()->m_level;
     } else {
        PlayLayer* Pl = PlayLayer::get();
        // Mac fix
        if (Pl->m_isPaused) {
            return false;
        }
        Level = Pl->m_level;
     };
     if (object->m_isSecondPlayer && Level->m_twoPlayerMode || !object->m_isSecondPlayer) {
        return true;
     } else {
        return false;
     }
}

class $modify(PlayerObject) {
public:
        // add it to fields to access later but stored in the object (m_fields->Var)
    struct Fields {
         bool directionUp = false;
         bool directionRight = false;
         bool directionLeft = false;
    };
    
    // For setting bools for setting dir
    void SetupNewDirections(PlayerButton p0, bool Set) { 
        switch (p0) { 
            case PlayerButton::Jump:   m_fields->directionUp = Set; break; 
            case PlayerButton::Right:  m_fields->directionRight = Set; break; 
            case PlayerButton::Left:  m_fields->directionLeft = Set; break; 
            default:break; 
        } 
    }
    // getting the bools in a orderly way
    bool GetNewDirections(PlayerButton p0) { 
        switch (p0) { 
            case PlayerButton::Jump:   return m_fields->directionUp; break; 
            case PlayerButton::Right:  return m_fields->directionRight; break; 
            case PlayerButton::Left:  return m_fields->directionLeft; break; 
            default:break; 
        } 
        return false;
    }
    // click sounds
    bool pushButton(PlayerButton p0) {
        bool ret = PlayerObject::pushButton(p0);
        // check if you can and or check if it is correct
        if (!integrityCheck(this,p0)) {
            return ret;
        };
        auto isClickEnabled = Mod::get()->getSettingValue<bool>("enable-clicksounds");
        auto click_vol = Mod::get()->getSettingValue<int64_t>("click-volume");
        // set the direction bool to true
        SetupNewDirections(p0,true);
        
        // is it enabled or is volume < 0
        if (click_vol <= 0 || !isClickEnabled) return ret;
        
        // Sound Player
        ClickSound->Play();

        return ret;
    }

    // release sounds
    bool releaseButton(PlayerButton p0) {
        bool ret = PlayerObject::releaseButton(p0);
        // Did you click? check
        if (!GetNewDirections(p0)) {
            return ret;
        };
        // check if you can and or check if it is correct
         if (!integrityCheck(this,p0)) {
            return ret;
        };

        auto isReleaseEnabled = Mod::get()->getSettingValue<bool>("enable-releasesounds");
        auto release_vol = Mod::get()->getSettingValue<int64_t>("release-volume");

        // set the direction bool to false
        SetupNewDirections(p0,false);
        // is it enabled or is volume < 0
        if (release_vol <= 0 || !isReleaseEnabled) return ret;

        // Sound Player
        ReleaseSound->Play();

        return ret;
    }
};

bool SendRequest = false;
EventListener<web::WebTask> m_listener;
class $modify(MenuLayer) {
    void SendRequestAPI() {
         m_listener.bind([] (web::WebTask::Event* e) {
                if (web::WebResponse* res = e->getValue()) {
                    if (res->into(Mod::get()->getConfigDir() / "Clicks.zip")) {
                        auto unzip = file::Unzip::create(Mod::get()->getConfigDir() / "Clicks.zip");
                        if (!unzip) {
                            log::error("Failed to unzip file");
                            return;
                        }
                        unzip.unwrap().extractAllTo(Mod::get()->getConfigDir() / "Clicks");
                    }
                } else if (e->isCancelled()) {
                    log::info("The request was cancelled... So sad :(");
                }
            });
            auto req = web::WebRequest();
            m_listener.setFilter(req.get("https://github.com/clicksounds/clicks/archive/refs/heads/main.zip"));
    }
    bool init() {
        if (!MenuLayer::init()) {
            return false;
        }
        if (!SendRequest) {
            SendRequest = true;
            this->SendRequestAPI();
        }
        return true;
    }
};


// on the mod loading
$execute {
    // Does the release-sound path setting change?
    listenForSettingChanges("selection-release", [](ClicksoundSettingValue releaseSoundFile) {
        auto Settings = GetSettingJsonRead("selection-release");
        ReleaseSound->Setsound(Settings.Custom_Sound_Path);
    });
    // Does the click-sound path setting change?
     listenForSettingChanges("selection-clicks", [](ClicksoundSettingValue PressSoundSoundFile) {
        auto Settings = GetSettingJsonRead("selection-clicks");
        ClickSound->Setsound(Settings.Custom_Sound_Path);
    });
    // on boot set Sound Caches
    auto selection_release = GetSettingJsonRead("selection-release");
    ReleaseSound->Setsound(selection_release.Custom_Sound_Path);
    auto selection_clicks = GetSettingJsonRead("selection-clicks");
    ClickSound->Setsound(selection_clicks.Custom_Sound_Path);
}

