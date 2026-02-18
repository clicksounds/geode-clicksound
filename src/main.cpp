#include "ButtonSprites/Sprite.hpp"
#include "SettingsV3/PlaySound.hpp"
#include "SettingsV3/SelectionEnum.hpp"
#include "StaticClasses.hpp"
#include "jsonReader/Getsettingsinfo.hpp"
#include "jsonReader/Json.hpp"
#include "ee.hpp"
#include <Geode/Geode.hpp>
#include <Geode/loader/Event.hpp>
#include <Geode/loader/SettingV3.hpp>
#include <Geode/modify/MenuLayer.hpp>
#include <Geode/modify/PauseLayer.hpp>
#include <Geode/modify/PlayerObject.hpp>
#include <Geode/modify/EndLevelLayer.hpp>
#include <Geode/modify/CCTouchDispatcher.hpp>
#include <Geode/modify/CCEGLView.hpp>
#include <Geode/ui/GeodeUI.hpp>
#include <Geode/utils/web.hpp>
#include <Geode/utils/async.hpp>
#include <thread>
#include <cstdint>
using namespace geode::prelude;

void onsettingsUpdate() {
	auto selection_release = GetSettingJsonRead("selection-release");
	ReleaseSound->Setsound(selection_release.Custom_Sound_Path);

	auto selection_clicks = GetSettingJsonRead("selection-clicks");
	ClickSound->Setsound(selection_clicks.Custom_Sound_Path);
	
	if (ClickJson->hassomedata) {

		Custom_OnClick = selection_clicks.M_Tab == 2;
		if (selection_clicks.M_Tab == 0) {
			auto list = ClickJson->GetMemeClicks();
			auto sound = list.find(selection_clicks.Current_Sound_Meme);
			if (sound != list.end()) {
				ClickSoundIndex->SetSounds(sound->second.clicks, "click-volume", "selection-clicks");
			}
		} else {
			auto list = ClickJson->GetUsefulClicks();
			auto sound = list.find(selection_clicks.Current_Sound_Useful);
			if (sound != list.end()) {
				ClickSoundIndex->SetSounds(sound->second.clicks, "click-volume", "selection-clicks");
			}
		}

		Custom_OnLetGo = selection_release.M_Tab == 2;
		if (selection_release.M_Tab == 0) {
			auto list = ClickJson->GetMemeReleases();
			auto sound = list.find(selection_release.Current_Sound_Meme);
			if (sound != list.end()) {
				ReleaseSoundIndex->SetSounds(sound->second.releases, "release-volume", "selection-release");
			}
		} else {
			auto list = ClickJson->GetUsefulReleases();
			auto sound = list.find(selection_release.Current_Sound_Useful);
			if (sound != list.end()) {
				ReleaseSoundIndex->SetSounds(sound->second.releases, "release-volume", "selection-release");
			}
		}
	}
}

// the check to see if you should play the sound or not 
bool integrityCheck(PlayerObject *object, PlayerButton Pressed) {
	// play sounds when "only play on jump" settings is enabled and the player input is a jump, left movement, or right movement.
	#ifndef GEODE_IS_MAC
		if (Mod::get()->getSettingValue<bool>("sounds-everywhere")) return false;
	#endif
	
	if (Mod::get()->getSettingValue<bool>("only-on-jump")) {
		if (Pressed != PlayerButton::Jump) {
			return false;
		}
	}
	GJGameLevel *Level;
	if (!PlayLayer::get()) {
		if (!LevelEditorLayer::get()) {
			return false;
		}
		Level = LevelEditorLayer::get()->m_level;
	} else {
		PlayLayer *Pl = PlayLayer::get();
		// Mac fix
		if (Pl->m_isPaused) {
			return false;
		}
		Level = Pl->m_level;
	};
	GJBaseGameLayer *LayerCheck = GJBaseGameLayer::get();
	if (!LayerCheck) {
		return false;
	}
	if (Level->m_twoPlayerMode && LayerCheck->m_player2 == object || LayerCheck->m_player1 == object) {
		return true;
	} else {
		return false;
	}
}

bool Carrot::carrot = false;
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
		case PlayerButton::Jump:
			m_fields->directionUp = Set;
			break;
		case PlayerButton::Right:
			m_fields->directionRight = Set;
			break;
		case PlayerButton::Left:
			m_fields->directionLeft = Set;
			break;
		default:
			break;
		}
	}
	// getting the bools in a orderly way
	bool GetNewDirections(PlayerButton p0) {
		switch (p0) {
		case PlayerButton::Jump:
			return m_fields->directionUp;
			break;
		case PlayerButton::Right:
			return m_fields->directionRight;
			break;
		case PlayerButton::Left:
			return m_fields->directionLeft;
			break;
		default:
			break;
		}
		return false;
	}
	// click sounds
	bool pushButton(PlayerButton p0) {
		bool ret = PlayerObject::pushButton(p0);
		Mod* csMod = Mod::get();

		// check if you can and or check if it is correct
		if (!integrityCheck(this, p0)) {
			return ret;
		};
		auto isClickEnabled = csMod->getSettingValue<bool>("enable-clicksounds") && csMod->getSettingValue<bool>("enable-master");
		auto click_vol = csMod->getSettingValue<int64_t>("click-volume");
		// set the direction bool to true
		SetupNewDirections(p0, true);

		if((!csMod->getSettingValue<bool>("enable-clicksounds") && !csMod->getSettingValue<bool>("enable-releasesounds")) || !csMod->getSettingValue<bool>("enable-master")){}else{Carrot::carrot=true;}

		// is it enabled or is volume < 0
		if (click_vol <= 0 || !isClickEnabled)
			return ret;

		// Sound Player
		if (Custom_OnClick) {
			ClickSound->Play();
		} else {
			ClickSoundIndex->PlayRandom();
		}

		return ret;
	}

	// release sounds
	bool releaseButton(PlayerButton p0) {
		bool ret = PlayerObject::releaseButton(p0);
		Mod* csMod = Mod::get();

		// Did you click? check
		if (!GetNewDirections(p0)) {
			return ret;
		};
		// check if you can and or check if it is correct
		if (!integrityCheck(this, p0)) {
			return ret;
		};

		auto isReleaseEnabled = csMod->getSettingValue<bool>("enable-releasesounds") && csMod->getSettingValue<bool>("enable-master");
		auto release_vol = csMod->getSettingValue<int64_t>("release-volume");

		if((!csMod->getSettingValue<bool>("enable-clicksounds") && !csMod->getSettingValue<bool>("enable-releasesounds")) || !csMod->getSettingValue<bool>("enable-master")){}else{Carrot::carrot=true;}

		// set the direction bool to false
		SetupNewDirections(p0, false);
		// is it enabled or is volume < 0
		if (release_vol <= 0 || !isReleaseEnabled)
			return ret;

		// Sound Player
		if (Custom_OnLetGo) {
			ReleaseSound->Play();
		} else {
			ReleaseSoundIndex->PlayRandom();
		}

		return ret;
	}
};

// 'sounds everywhere' setting
class $modify(csTouchDispatcher, CCTouchDispatcher) {
	void touches(CCSet* pTouches, CCEvent* pEvent, unsigned int uIndex) {
		CCTouchDispatcher::touches(pTouches, pEvent, uIndex);

		if (uIndex != 0 && uIndex != 2) return;

		#ifdef GEODE_IS_MAC
			if (PlayLayer::get()) return;
		#endif

		Mod* csMod = Mod::get();
		bool soundsEverywhere = csMod->getSettingValue<bool>("sounds-everywhere");
		bool isMasterEnabled = csMod->getSettingValue<bool>("enable-master");
		bool isClickEnabled = csMod->getSettingValue<bool>("enable-clicksounds");
		bool isReleaseEnabled = csMod->getSettingValue<bool>("enable-releasesounds");

		if (!soundsEverywhere) return;

		if ((!isClickEnabled && !isReleaseEnabled) || !isMasterEnabled){}else{Carrot::carrot=true;}

		// touch start
		if (uIndex == 0) {
			int64_t clickVolume = csMod->getSettingValue<int64_t>("click-volume");

			if (clickVolume <= 0 || !isClickEnabled || !isMasterEnabled)
				return;

			if (Custom_OnClick) {
				ClickSound->Play();
			} else {
				ClickSoundIndex->PlayRandom();
			}
		}

		// touch end
		if (uIndex == 2) {
			int64_t releaseVolume = csMod->getSettingValue<int64_t>("release-volume");

			if (releaseVolume <= 0 || !isReleaseEnabled || !isMasterEnabled)
				return;

			if (Custom_OnLetGo) {
				ReleaseSound->Play();
			} else {
				ReleaseSoundIndex->PlayRandom();
			}
		}
	}
};

async::TaskHolder<web::WebResponse> m_downloadTask;

void SendRequestAPI(bool forceDownload = false) {
    if (!forceDownload && !Mod::get()->getSettingValue<bool>("downloadOnStartup")) {
        return;
    }

    Loader::get()->queueInMainThread([] {
        Notification::create(
            "CS: Downloading index...",
            CCSprite::createWithSpriteFrameName("GJ_timeIcon_001.png")
        )->show();
        Mod::get()->setSavedValue<bool>("CSINDEXDOWNLOADING", true);
    });

    web::WebRequest req;
    req.onProgress([](web::WebProgress const& p) {
        // log::debug("progress: {}", p.downloadProgress().value_or(0.f));
    });

    m_downloadTask.spawn(
        req.get("https://github.com/clicksounds/clicks/archive/refs/heads/main.zip"),
        [=](web::WebResponse res) {
            if (!res.ok()) {
                Loader::get()->queueInMainThread([] {
                    Notification::create(
                        "CS: Download failed.",
                        CCSprite::createWithSpriteFrameName("GJ_deleteIcon_001.png")
                    )->show();
                });
                Mod::get()->setSavedValue<bool>("CSINDEXDOWNLOADING", false);
                return;
            }

            auto zipPath = Mod::get()->getConfigDir() / "Clicks.zip";
            if (!res.into(zipPath)) {
                Loader::get()->queueInMainThread([] {
                    Notification::create(
                        "CS: Download failed.",
                        CCSprite::createWithSpriteFrameName("GJ_deleteIcon_001.png")
                    )->show();
                });
                Mod::get()->setSavedValue<bool>("CSINDEXDOWNLOADING", false);
                return;
            }

            auto indexzipPtr = std::make_shared<decltype(indexzip)>(indexzip);

            std::thread([=] {
                auto unzip = file::Unzip::create(zipPath);
                if (!unzip) {
                    Mod::get()->setSavedValue<bool>("CSINDEXDOWNLOADING", false);
                    return;
                }

                auto clicksRoot = Mod::get()->getConfigDir() / "Clicks";
                std::filesystem::remove_all(clicksRoot);
                (void) unzip.unwrap().extractAllTo(clicksRoot);

                Loader::get()->queueInMainThread([=] {
                    Notification::create(
                        "CS: Download successful!",
                        CCSprite::createWithSpriteFrameName("GJ_completesIcon_001.png")
                    )->show();

                    std::filesystem::path clicksDir =
                        clicksRoot / "clicks-main";

                    static const std::unordered_set<std::string> skipList = {
                        "Meme",
                        "Useful",
                        "featured_list.json"
                    };

                    for (auto const& entry : std::filesystem::directory_iterator(clicksDir)) {
                        if (!skipList.contains(entry.path().filename().string())) {
                            std::filesystem::remove_all(entry.path());
                        }
                    }

                    if (std::filesystem::exists(zipPath)) {
                        std::filesystem::remove(zipPath);
                    }
                });

                Mod::get()->setSavedValue<bool>("CSINDEXDOWNLOADING", false);

                ClickJson->loadData([] {
                    onsettingsUpdate();
                });
            }).detach();
        }
    );
}

void extractDefaultClickPack() {
    auto mod = Mod::get();
	std::filesystem::path configPath = mod->getConfigDir();

    std::filesystem::create_directories(configPath / "Clicks" / "clicks-main" / "Useful");

    std::filesystem::path destinationPath = configPath / "Clicks" / "clicks-main" / "Useful" / "clicksounds.default";
    std::filesystem::path resourcePath = mod->getResourcesDir() / "clicksounds.default.packgen.zip";

    if (std::filesystem::is_directory(destinationPath) && std::filesystem::exists(destinationPath)) return;

    auto unzip = file::Unzip::create(resourcePath);
    if (!unzip) return;
    (void) unzip.unwrap().extractAllTo(destinationPath);
	mod->setSavedValue("CSINDEXRELOAD", true);
}

class $modify(MenuLayer) {
	bool init() {
		// make sure default click pack exists because little kids love bitching
		extractDefaultClickPack();

		if (!indexzip.StartedDownloading) {
			indexzip.StartedDownloading = true;

			// on boot set Sound Caches
			ClickJson->loadData([=]() {
				onsettingsUpdate();
			});

			SendRequestAPI();
		}

		return MenuLayer::init();
	}
};

// on the mod loading
$on_mod(Loaded) {
	// Does the release-sound path setting change?
	Mod::get()->setSavedValue<bool>("CSINDEXDOWNLOADING", false);
	listenForSettingChanges<ClicksoundSettingValue>("selection-release", [](ClicksoundSettingValue releaseSoundFile) {
		onsettingsUpdate();
	});
	// Does the click-sound path setting change?
	listenForSettingChanges<ClicksoundSettingValue>("selection-clicks", [](ClicksoundSettingValue PressSoundSoundFile) {
		onsettingsUpdate();
	});
	SpritePicker::secret = Loader::get()->isModLoaded("carrot_devs.carrotmodcarrotcarrotcarrotcarrotcarrot"); // carrot sprite flag
	// sets the persistent directory to the geode folder if the user hasnt ever chosen a path before
	if (Mod::get()->getSavedValue<std::filesystem::path>("cspi-persistent-dir").empty() || !std::filesystem::exists(Mod::get()->getSavedValue<std::filesystem::path>("cspi-persistent-dir"))) {
		Mod::get()->setSavedValue<std::filesystem::path>("cspi-persistent-dir", dirs::getGeodeDir());
	}
}
class $modify(EndLevelLayer){void customSetup(){EndLevelLayer::customSetup();if(Carrot::carrot==true){auto eee = CCNode::create();auto ee = CCSprite::create("ee.png"_spr);eee->setPosition(450, 260);eee->setAnchorPoint({0.5, 0.5});eee->setScale(0.2);ee->setOpacity(10);eee->addChild(ee);static_cast<cocos2d::CCLayer*>(this->getChildren()->objectAtIndex(0))->addChild(eee);}Carrot::carrot=false;}};