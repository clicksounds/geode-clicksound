#pragma once
#include <Geode/Geode.hpp>
#include "../ui/Spinner.hpp"
using namespace geode::prelude;

class ManageClicksLayer : public cocos2d::CCLayer {
protected:
    CCMenu* m_menu;
    Spinner* m_spinner;
    int m_currentSpinnerFrame = 0;
    bool m_shouldGoToLastScene;
public:
    // create and init methods
    static ManageClicksLayer* create(bool GoToLastSceneOnBack = true);
    static cocos2d::CCScene* scene(bool GoToLastSceneOnBack = true);
    bool init();

    // button actions
    void onClose(CCObject*);
    void onSettings(CCObject*);

    // other methods
    void keyBackClicked();
};