#pragma once
#include <Geode/Geode.hpp>
#include <matjson.hpp>
using namespace geode::prelude;
class Clicks {
protected:
    matjson::Value m_ClickList;
    matjson::Value m_ReleaseList;
    matjson::Value m_BackClickList;
    matjson::Value m_BackReleaseList;
    int m_CM;
    int m_CU;
    int m_RM;
    int m_RU;
public:
    static Clicks* get();
    int getMemeClickLen() {return m_CM;}
    int getClickLen() {return m_CU;}
    int getMemeReleaseLen() {return m_RM;}
    int getReleaseLen() {return m_RU;}
    matjson::Value getClickList() {return m_ClickList;}
    matjson::Value getReleaseList() {return m_ReleaseList;}
    matjson::Value getBackClickList() {return m_BackClickList;}
    matjson::Value getBackReleaseList() {return m_BackReleaseList;}
    void initJson() {
        auto thing = matjson::parse();
        m_ClickList = thing["Reg"]["Clicks"];
        m_ReleaseList = thing["Reg"]["Releases"];
        m_BackClickList = thing["Back"]["Clicks"];
        m_BackReleaseList = thing["Back"]["Releases"];
        m_CU = thing["Len"]["CU"].as<int>();
        m_CM = thing["Len"]["CM"].as<int>();
        m_RM = thing["Len"]["RM"].as<int>();
        m_RU = thing["Len"]["RU"].as<int>();
    }
};
