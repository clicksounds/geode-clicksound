#pragma once
#include <Geode/Geode.hpp>
#include "../jsonReader/Json.hpp"
#include "popup_nodes/CCIndexPackNode.hpp"
#include "SelectionEnum.hpp"
using namespace geode::prelude;

class Select : public geode::Popup<> {
protected:
    std::function<void(std::string)> m_settings;
    ScrollLayer* scroll;
    CCNode* Item(auto send, auto modid, bool meme) {
        return CCIndexPackNode::create(send,[=]() {
            m_settings(modid);
            this->onClose(nullptr);
        }
        );
    }
    bool setup() {
        auto winSize = CCDirector::get()->getWinSize();
		scroll = ScrollLayer::create(ccp(m_mainLayer->getContentSize().width / 2, m_mainLayer->getContentSize().height - 15));
        scroll->setAnchorPoint(ccp(0, 0));
        scroll->setPosition(ccp(110, 10 ));
        m_mainLayer->addChild(scroll);
        return true;
    };
    bool CreateWithArgs(bool meme, bool clicksound, std::function<void(std::string)> setting) {
        CCNode* NodeScroller = scroll->m_contentLayer;
        m_settings = setting;
        int basePosY = 207;
        auto json = (meme) ? ClickJson->memeData : ClickJson->usefulData;
            for (const auto& [filename, data] : json) {
                 if (data.clicks.empty() && clicksound) {
                    continue;
                 };
                if (data.releases.empty() && !clicksound) {
                    continue;
                 };
                CCNode* Object = Item(data,filename,clicksound);
                Object->setPositionY(basePosY);
                NodeScroller->addChild(Object);
            }

        float height = std::max<float>(scroll->getContentSize().height, 35 * NodeScroller->getChildrenCount());
        NodeScroller->setContentSize(ccp(NodeScroller->getContentSize().width, height));
        CCArrayExt<CCNode*> objects = NodeScroller->getChildren();
        int i = -1;
        for (auto* obj : objects) {
                i++;
                obj->setPositionY(height - (35 * i));
            }
        scroll->moveToTop();

        return true;
    };
public:
    static Select* create(bool meme = false, bool clicksound = true, std::function<void(std::string)> setting = [=](std::string x) {}) {
          auto ret = new Select;
        if (ret && ret->initAnchored(420.f, 210.f)) {
            ret->autorelease();
            ret->CreateWithArgs(meme,clicksound,setting);
            ret->setID("SoundSelector"_spr);
            return ret;
        }

        CC_SAFE_DELETE(ret);
        return nullptr;
    };
};