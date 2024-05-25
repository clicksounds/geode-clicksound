#include <Geode/Geode.hpp>
#include <Geode/modify/EndLevelLayer.hpp>
#include <Geode/ui/GeodeUI.hpp>

#include "./utils/ee.hpp"

using namespace geode::prelude;

// :3

bool Carrot::carrot = false;
// 🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕🥕
class $modify(EndLevelLayer){void customSetup(){EndLevelLayer::customSetup();if(Carrot::carrot==true){auto eee = CCNode::create();auto ee = CCSprite::create("ee.png"_spr);eee->setPosition(450, 260);eee->setAnchorPoint({0.5, 0.5});eee->setScale(0.2);ee->setOpacity(10);eee->addChild(ee);static_cast<cocos2d::CCLayer*>(this->getChildren()->objectAtIndex(0))->addChild(eee);}Carrot::carrot=false;}};
