#pragma once
#include <Geode/Geode.hpp>
#include "../jsonReader/Json.hpp"
#include "popup_nodes/CCIndexPackNode.hpp"
using namespace geode::prelude;

class Select : public geode::Popup<> {
protected:
    ScrollLayer* scroll;
    CCNode* Item(auto send) {
        return CCIndexPackNode::create(send);
    }
    bool setup() {
        auto winSize = CCDirector::get()->getWinSize();
		scroll = ScrollLayer::create(ccp(m_mainLayer->getContentSize().width / 2, m_mainLayer->getContentSize().height - 15));
        scroll->setAnchorPoint(ccp(0, 0));
        scroll->setPosition(ccp(110, 10 ));
        m_mainLayer->addChild(scroll);
        return true;
    };
    bool CreateWithArgs() {
        CCNode* NodeScroller = scroll->m_contentLayer;
        int basePosY = 207;
            for (const auto& [filename, data] : ClickJson->memeData) {
                CCNode* Object = Item(data);
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
    static Select* create() {
          auto ret = new Select;
        if (ret && ret->initAnchored(420.f, 210.f)) {
            ret->autorelease();
            ret->CreateWithArgs();
            ret->setID("SoundSelector"_spr);
            return ret;
        }

        CC_SAFE_DELETE(ret);
        return nullptr;
    };
};