#pragma once
#include <matjson.hpp>
class Clicks {
protected:
    static matjson::Value* m_ClickList;
    static matjson::Value* m_ReleaseList;
public:
    static const char* getClickSprite(int);
    static const char* getReleaseSprite(int);
    static const matjson::Value* getClickList() {return m_ClickList;}
    static const matjson::Value* getReleaseList() {return m_ReleaseList;}
};