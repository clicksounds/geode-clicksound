#include <Geode/Geode.hpp>
#include <Geode/modify/MenuLayer.hpp>
#include <Geode/loader/Mod.hpp>
#include <Geode/loader/Loader.hpp>
#include <Geode/loader/Dirs.hpp>
#include <Geode/ui/GeodeUI.hpp>
#include <Geode/utils/web.hpp>
#include <matjson.hpp>
#include <Geode/ui/BasedButtonSprite.hpp>
#include "../utils/HttpStatusCodes.h"

using namespace geode::prelude;





// CREATE EVENT
class WaitAndHideDownloadProgressEvent : public Event {
public:
    WaitAndHideDownloadProgressEvent();
};

class SetIconDownloadProgressEvent : public Event {
protected:
    NotificationIcon m_icon;

public:
    SetIconDownloadProgressEvent(NotificationIcon const& icon);

    NotificationIcon getIcon() const;
};


class $modify(newl,MenuLayer) {

    struct Fields {
        EventListener<web::WebTask> m_listener;
        Ref<Notification> m_progress;
    };

    bool hasInstalledNodeIds = false;
    bool hasStartedListen = false;

    void index(CCObject*) {
        auto theLoader = Loader::get();
        if (theLoader->isModInstalled("geode.node-ids")) {
            auto isModInstall = theLoader->getInstalledMod("geode.node-ids");
            isModInstall->enable();
            Notification::create("Node IDs Mod Enabled", NotificationIcon::Success, 5.f)->show();
            utils::game::restart();
        } else {
            m_fields->m_progress->show();

            m_fields->m_listener.bind([this] (web::WebTask::Event* e) {
                auto alerterror = FLAlertLayer::create(
                    "Click Sounds Error",
                    "Unable to fetch mod, please download from the <cp>geode website!</c>", //(It could be the Geode Index is still downloading so try again in a few seconds.)",  
                    "OK"
                );


                if (web::WebResponse* res = e->getValue()) {
                    this->m_fields->m_progress->setIcon(NotificationIcon::Success);
                    this->m_fields->m_progress->setString("Download Success - Putting into mods folder");
                    if (res->ok()) {
                        // transformModJsonAndShowPopup(static_cast<matjson::Value>(res->json().unwrap()));
                        // downloaded geode mod
                        if (res->into(fmt::format("{}/geode.node-ids.geode", dirs::getModsDir()))) {
                            this->m_fields->m_progress->setString("Mod Install Success - Restarting Game Momentarily");
                            this->m_fields->m_progress->waitAndHide();
                            utils::game::restart();
                        } else {
                            this->m_fields->m_progress->setIcon(NotificationIcon::Error);
                            this->m_fields->m_progress->setString("Writing mod to disk failed - Unknown Error Occured");
                            this->m_fields->m_progress->waitAndHide();
                        }
                    } else {
                        this->m_fields->m_progress->setIcon(NotificationIcon::Error);
                        this->m_fields->m_progress->setString("Status Code: " + std::to_string(res->code()) + " - " + HttpStatus::reasonPhrase(res->code()));
                        this->m_fields->m_progress->waitAndHide();
                    }
                } else if (web::WebProgress* p = e->getProgress()) {
                    log::info("progress: {}", p->downloadProgress().value_or(0.f));
                    this->m_fields->m_progress->setString(fmt::format("Downloading Node IDs mod - {::0f}%", p->downloadProgress().value_or(0.f)));
                } else if (e->isCancelled()) {
                    log::info("The request was cancelled... So sad :(");
                    // this->getChildByID("bottom-menu")->getChildByID("geode.loader/geode-button")->setVisible(true);
                    this->m_fields->m_progress->setIcon(NotificationIcon::Warning);
                    this->m_fields->m_progress->setString("Download Canceled - Reason: Unknown");
                    this->m_fields->m_progress->waitAndHide();
                }
            });
            web::WebRequest req = web::WebRequest();
            m_fields->m_listener.setFilter(req.get("https://api.geode-sdk.org/v1/mods/geode.node-ids/versions/latest/download"));
        }
    };

    void index2(CCObject*) {
        utils::game::restart();
    };


    void initUi() {
        auto winSize = CCDirector::sharedDirector()->getWinSize();
        auto spr = CCSprite::create("nodeIdsLogo.png"_spr);
        auto btn = CCMenuItemSpriteExtra::create(
            spr, this, menu_selector(newl::index)
        );
        btn->setScale(.7);
        btn->setPosition(winSize.width / 2, (winSize.height / 2)+ 20);
         this->getChildByID("PleaseDONOTREMOVE"_spr)->addChild(btn);
    };

    void initUi2() {
        auto winSize = CCDirector::sharedDirector()->getWinSize();
        auto spr = ButtonSprite::create("Restart Game");
        auto btn = CCMenuItemSpriteExtra::create(
            spr, this, menu_selector(newl::index2)
        );
        btn->setScale(.7);
        btn->setPosition(winSize.width / 2, (winSize.height / 2) - 25);
         this->getChildByID("PleaseDONOTREMOVE"_spr)->addChild(btn);
    };

    static void onModify(auto& self) {
        self.setHookPriority("MenuLayer::init", -16901); // GO LAST :O (IDK WHY IT SAID GO FIRST)
    }

    bool init() {

        auto winSize = CCDirector::get()->getWinSize();
        //if (!MenuLayer::init())
          //  return false;
        MenuLayer::init();
        
        if (Loader::get()->isModLoaded("geode.node-ids")) {
            return true;
        };


        auto alert = FLAlertLayer::create(
            "Click Sounds Error",
            "Please install/enable Node IDs by clicking the Node IDs Mod Logo in the middle of your screen!\n After installed, we will automaticly restart your game.",  
            "OK"
        );
        //alert->m_scene = this;
        
        for(auto items : CCArrayExt<CCNode*>(this->getChildren())) {
            items->setVisible(false);
        };
            for(auto items : CCArrayExt<CCNode*>(this->getChildByID("bottom-menu")->getChildren())) {
            items->setVisible(false);
        };

        m_fields->m_progress = Notification::create("Requested download of the Node IDs mod", NotificationIcon::Loading, 0.f);

        this->getChildByID("bottom-menu")->setVisible(true);
            if (this->getChildByID("close-menu")) {
            this->getChildByID("close-menu")->setVisible(true);
            }
        
        this->getChildByID("bottom-menu")->setLayout(
                RowLayout::create()
                ->setGap(14)
                 ->setGrowCrossAxis(true)
                 ->setCrossAxisReverse(true)
                 ->setAutoScale(true)
                 ->setAxisAlignment(AxisAlignment::Center)
                );

        
        auto menu = CCMenu::create();
        menu->setLayout(
                RowLayout::create()
                ->setGap(14)
                 ->setGrowCrossAxis(true)
                 ->setCrossAxisReverse(true)
                 ->setAutoScale(true)
                 ->setAxisAlignment(AxisAlignment::Center)
                );
        menu->setID("PleaseDONOTREMOVE"_spr);
        menu->setPosition(winSize.width / 2, 0);
        this->addChild(menu);
	if (Loader::get()->getLoadedMod("undefined0.minecraft_menu")) {
		auto ee = CircleButtonSprite::createWithSprite(
                "nobglogo.png"_spr,
                1.0f,
                CircleBaseColor::Green,
                CircleBaseSize::MediumAlt
            );
            
		auto btnee = CCMenuItemSpriteExtra::create(ee, this, menu_selector(newl::index));
		btnee->setID("replacementforgonebtn"_spr);
		this->getChildByID("bottom-menu")->addChild(btnee);
	}
    if (auto GeodeButton =  typeinfo_cast<CCMenuItemSpriteExtra*>(this->getChildByIDRecursive("geode.loader/geode-button"))) {
            // If the button is somehow not in the right spot and this cannot be removed as the game would crash so it would always move.
            	GeodeButton->removeFromParent();
		this->getChildByID("bottom-menu")->addChild(GeodeButton);
	}
        
        this->getChildByID("bottom-menu")->getChildByID("geode.loader/geode-button")->setVisible(true);
        this->getChildByID("bottom-menu")->setVisible(true);
        menu->setPosition(winSize.width / 2, (winSize.height / 2)+ 30);
        this->getChildByID("bottom-menu")->setPosition(winSize.width / 2, 0);
          this->getChildByID("bottom-menu")->setContentSize({284.500,45.000});
         this->getChildByID("bottom-menu")->setScale(1.1);
        menu->setContentSize({284.500,45.000});
        menu->setScale(0.9);
        menu->setVisible(true);
        alert->show();
        newl::initUi2();
        newl::initUi();
        menu->updateLayout();
        this->getChildByID("bottom-menu")->updateLayout();
           
               
        return true;
    };
};