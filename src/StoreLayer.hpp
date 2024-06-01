#pragma once

using namespace geode::prelude;

class StoreLayer : public cocos2d::CCLayer {
protected:
    CCMenu* m_menu;
    CCScene* m_lastScene;
public:
    static StoreLayer* create(CCScene*);
    static cocos2d::CCScene* scene(CCScene*);
    bool init();
    void onClose(CCObject*);
    void keyBackClicked();
};