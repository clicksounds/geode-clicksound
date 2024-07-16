#pragma once
#include <Geode/Geode.hpp>
using namespace geode::prelude;

class Spinner : public cocos2d::CCSprite {
protected:
    int m_currentSpinnerFrame = 0;
public:
    static Spinner* create();
    bool init();
    void spinnerAnimation(float dt);
};