#pragma once

using namespace geode::prelude;

class StoreLayer : public cocos2d::CCLayer {
protected:
    std::string m_idk;
    CCMenu* m_menu;
public:
    static StoreLayer* create();
    static cocos2d::CCScene* scene();
    bool init();
    void onClose(CCObject*);
    void keyBackClicked();
};