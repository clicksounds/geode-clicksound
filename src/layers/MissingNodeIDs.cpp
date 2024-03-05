#include <Geode/Geode.hpp>
#include <Geode/modify/MenuLayer.hpp>
#include <Geode/loader/Mod.hpp>
#include <Geode/loader/Loader.hpp>
#include <Geode/ui/GeodeUI.hpp>
#include <Geode/loader/Index.hpp>
#include <Geode/ui/BasedButtonSprite.hpp>

using namespace geode::prelude;

class $modify(newl,MenuLayer) {
    bool hasInstalledNodeIds = false;
    bool hasStartedListen = false;
    void index(CCObject*) {
        try {
            auto theLoader = Loader::get();
            if(newl::hasInstalledNodeIds) {
                auto alerterror = FLAlertLayer::create("Node Ids Already Enabled","<co>Node Ids</c> has already been <cg>enabled</c> and <cg>installed</c>! Please <cy>Restart</c> by clicking OK down below then clicking \"<cl>Restart Game</c>\"\nThank <cr>You</c>!","OK");
                alerterror->show();
            } else if (theLoader->isModInstalled("geode.node-ids")) {
                auto isModInstall = theLoader->getInstalledMod("geode.node-ids");
                geode::openInfoPopup(isModInstall);
            } else {
                auto indexlook = geode::Index::get();
                auto nodeIDSmod =  indexlook->getItemsByModID("geode.node-ids");
                if(nodeIDSmod.size() != 0){
                    auto nodeIDSMod2 = nodeIDSmod.back();
                    auto nodeIdsMetadata = nodeIDSMod2->getMetadata();
                    Mod theNodeIds = Mod(nodeIdsMetadata);
                    Mod* theNodeId2 = &theNodeIds;
                    geode::openIndexPopup(theNodeId2);
                    } else {
                        auto alerterror = FLAlertLayer::create(
                        "Click Sounds Error",
                        "Unable to fetch mod, please download from the <cp>geode website!</c> (It could be the Geode Index is still downloading so try again in a few seconds.)",  
                        "OK"
                    );
                    alerterror->show();
                }
            }
        } catch (const std::exception& e) {
            auto alerterror = FLAlertLayer::create(
                "Click Sounds Error",
                "Unable to fetch mod, please download from the <cp>geode website!</c>",  
                "OK"
            );
            alerterror->show();
        }
    };

    void index2(CCObject*) {
        utils::game::restart();
    };

    void index3(CCObject*) {};


    void initUi() {
        if (!newl::hasStartedListen) {
            newl::hasStartedListen = true;
            auto listener = EventListener<ModInstallFilter>([this](ModInstallEvent* ev) {
                if(std::holds_alternative<UpdateFinished>(ev->status)) newl::hasInstalledNodeIds = true;
            }, ModInstallFilter("geode.node-ids"));
        }
        auto winSize = CCDirector::sharedDirector()->getWinSize();
        auto spr = CCSprite::create("nodeIdsLogo.png"_spr);
        auto btn = CCMenuItemSpriteExtra::create(
            spr, this, menu_selector(newl::index)
        );
        btn->setScale(.7);
        btn->setPosition(winSize.width / 2, (winSize.height / 2)+ 20);
         this->getChildByID("Beat.PleaseDONOTREMOVE")->addChild(btn);
    };

    void initUi2() {
        auto winSize = CCDirector::sharedDirector()->getWinSize();
        auto spr = ButtonSprite::create("Restart Game");
        auto btn = CCMenuItemSpriteExtra::create(
            spr, this, menu_selector(newl::index2)
        );
        btn->setScale(.7);
        btn->setPosition(winSize.width / 2, (winSize.height / 2) - 25);
         this->getChildByID("Beat.PleaseDONOTREMOVE")->addChild(btn);
    };

    static void onModify(auto& self) {
        self.setHookPriority("MenuLayer::init", -16901); // GO LAST :O (IDK WHY IT SAID GO FIRST)
    }

    CCSprite* m_nidscsReplacer;
    bool init() {

        auto winSize = CCDirector::get()->getWinSize();
        if (!MenuLayer::init())
            return false;
        
        if (Loader::get()->isModLoaded("geode.node-ids")) {
            return true;
        };


        auto alert = FLAlertLayer::create(
            "Click Sounds Error",
            "Please install Node Ids by clicking the Node Ids Mod in the middle of your screen!",  
            "OK"
        );
        alert->m_scene = this;
        
        for(auto items : CCArrayExt<CCNode*>(this->getChildren())) {
            items->setVisible(false);
        };
            for(auto items : CCArrayExt<CCNode*>(this->getChildByID("bottom-menu")->getChildren())) {
            items->setVisible(false);
        };
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
        menu->setID("Beat.PleaseDONOTREMOVE");
        menu->setPosition(winSize.width / 2, 0);
        this->addChild(menu);
	if (Loader::get()->getLoadedMod("undefined0.minecraft_menu")) {
		m_fields->m_nidscsReplacer = CircleButtonSprite::createWithSprite(
                "nobglogo.png"_spr,
                1.0f,
                CircleBaseColor::Green,
                CircleBaseSize::MediumAlt
            );
            
		auto btnee = CCMenuItemSpriteExtra::create(m_fields->m_nidscsReplacer, this, menu_selector(newl::index));
		btnee->setID("beat.replacementforgonebtn"_spr);
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