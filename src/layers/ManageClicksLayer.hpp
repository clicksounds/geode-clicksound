#pragma once
#include <Geode/Geode.hpp>
using namespace geode::prelude;

class ManageClicksLayer : public cocos2d::CCLayer {
protected:
    CCMenu* m_menu;
    CCScene* m_lastScene;
public:
    static ManageClicksLayer* create(CCScene*);
    static cocos2d::CCScene* scene(CCScene*);
    bool init();
    void onClose(CCObject*);
    void keyBackClicked();
};