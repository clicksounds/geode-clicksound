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
#include <Geode/ui/GeodeUI.hpp>
#include <Geode/utils/web.hpp>
#include <thread>
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

		// check if you can and or check if it is correct
		if (!integrityCheck(this, p0)) {
			return ret;
		};
		auto isClickEnabled = Mod::get()->getSettingValue<bool>("enable-clicksounds");
		auto click_vol = Mod::get()->getSettingValue<int64_t>("click-volume");
		// set the direction bool to true
		SetupNewDirections(p0, true);

		if(!Mod::get()->getSettingValue<bool>("enable-clicksounds") && !Mod::get()->getSettingValue<bool>("enable-releasesounds")){}else{Carrot::carrot=true;}

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
		// Did you click? check
		if (!GetNewDirections(p0)) {
			return ret;
		};
		// check if you can and or check if it is correct
		if (!integrityCheck(this, p0)) {
			return ret;
		};

		auto isReleaseEnabled = Mod::get()->getSettingValue<bool>("enable-releasesounds");
		auto release_vol = Mod::get()->getSettingValue<int64_t>("release-volume");

		if(!Mod::get()->getSettingValue<bool>("enable-clicksounds") && !Mod::get()->getSettingValue<bool>("enable-releasesounds")){}else{Carrot::carrot=true;}

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

void SendRequestAPI(bool forceDownload = false) {
	if (forceDownload) {
		// skip all other checks and start the download
	} else if (!Mod::get()->getSettingValue<bool>("downloadOnStartup")) {
		indexzip.Failed = true;
		indexzip.Finished = true;
		return;
	}

	Loader::get()->queueInMainThread([=] {
		Notification::create("CS: Downloading index...", CCSprite::createWithSpriteFrameName("GJ_timeIcon_001.png"))->show();
		Mod::get()->setSavedValue<bool>("CSINDEXDOWNLOADING", true);
	});
	web::WebRequest().get("https://github.com/clicksounds/clicks/archive/refs/heads/main.zip").listen([=](auto res) {
		if (res->string().unwrapOr("failed") == "failed") {
			Loader::get()->queueInMainThread([=] {
				Notification::create("CS: Download failed.", CCSprite::createWithSpriteFrameName("GJ_deleteIcon_001.png"))->show();
			});
			indexzip.Failed = true;
			indexzip.Finished = true;
			Mod::get()->setSavedValue<bool>("CSINDEXDOWNLOADING", false);
			return;
		}

		if (res->into(Mod::get()->getConfigDir() / "Clicks.zip")) {
			auto indexzipPtr = std::make_shared<decltype(indexzip)>(indexzip);
			std::thread([=] {
				auto unzip = file::Unzip::create(Mod::get()->getConfigDir() / "Clicks.zip");
				if (!unzip) {
					indexzipPtr->Failed = true;
					indexzipPtr->Finished = true;
					Mod::get()->setSavedValue<bool>("CSINDEXDOWNLOADING", false);
					return;
				}

				std::filesystem::remove_all(Mod::get()->getConfigDir() / "Clicks");
				(void) unzip.unwrap().extractAllTo(Mod::get()->getConfigDir() / "Clicks");
				indexzipPtr->Finished = true;

				Loader::get()->queueInMainThread([=] {
					Notification::create("CS: Download successful!", CCSprite::createWithSpriteFrameName("GJ_completesIcon_001.png"))->show();
					
					// delete unnecessary files to save storage space
					std::filesystem::path clicksDir = Mod::get()->getConfigDir() / "Clicks" / "clicks-main";
					for (const auto& entry : std::filesystem::directory_iterator(clicksDir)) {
						if (entry.path().filename() != "Meme" && entry.path().filename() != "Useful") {
							std::filesystem::remove_all(entry.path());
						}
					}
					if (std::filesystem::exists(Mod::get()->getConfigDir() / "Clicks.zip")) {
						std::filesystem::remove(Mod::get()->getConfigDir() / "Clicks.zip");
					}
				});

				Mod::get()->setSavedValue<bool>("CSINDEXDOWNLOADING", false);

				ClickJson->loadData([=](){
					onsettingsUpdate();
				}); 
			}).detach();
		} else {
			Loader::get()->queueInMainThread([=] {
				Notification::create("CS: Download failed.", CCSprite::createWithSpriteFrameName("GJ_deleteIcon_001.png"))->show();
			});
		} }, [](auto prog) {
		// log::debug("download");
	},
	[=]() {
	Loader::get()->queueInMainThread([=] {
		Notification::create("CS: Download failed.", CCSprite::createWithSpriteFrameName("GJ_deleteIcon_001.png"))->show();
	});
		indexzip.Failed = true;
		indexzip.Finished = true;
	});
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

EventListener<web::WebTask> m_listener;

class $modify(MenuLayer) {
	bool init() {
		if (!Mod::get()->getSavedValue<bool>("amv-read") && GJAccountManager::get()->m_accountID == 4470386) {
			auto popup = FLAlertLayer::create("Click Sounds", "it only took you so long to download click sounds lmao", "why beat");
			popup->m_scene = this;
			popup->show();
			Mod::get()->setSavedValue<bool>("amv-read", true);
		}

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
	listenForSettingChanges("selection-release", [](ClicksoundSettingValue releaseSoundFile) {
		onsettingsUpdate();
	});
	// Does the click-sound path setting change?
	listenForSettingChanges("selection-clicks", [](ClicksoundSettingValue PressSoundSoundFile) {
		onsettingsUpdate();
	});
	SpritePicker::secret = Loader::get()->isModLoaded("carrot_devs.carrotmodcarrotcarrotcarrotcarrotcarrot"); // carrot sprite flag
	// sets the persistent directory to the geode folder if the user hasnt ever chosen a path before
	if (Mod::get()->getSavedValue<std::filesystem::path>("cspi-persistent-dir").empty() || !std::filesystem::exists(Mod::get()->getSavedValue<std::filesystem::path>("cspi-persistent-dir"))) {
		Mod::get()->setSavedValue<std::filesystem::path>("cspi-persistent-dir", dirs::getGeodeDir());
	}
}
class $modify(EndLevelLayer){void customSetup(){EndLevelLayer::customSetup();if(Carrot::carrot==true){auto eee = CCNode::create();auto ee = CCSprite::create("ee.png"_spr);eee->setPosition(450, 260);eee->setAnchorPoint({0.5, 0.5});eee->setScale(0.2);ee->setOpacity(10);eee->addChild(ee);static_cast<cocos2d::CCLayer*>(this->getChildren()->objectAtIndex(0))->addChild(eee);}Carrot::carrot=false;}};