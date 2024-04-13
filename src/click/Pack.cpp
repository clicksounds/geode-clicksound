#include "Pack.hpp"

namespace click {

Pack::Pack(matjson::Value const& json, bool isServer) {
    m_name = json["name"].as_string();
    m_id = json["id"].as_string();
    m_category = json["type"].as<click::Pack::Category>();
    m_isServer = isServer;

    // Author sorting (shitty code incoming)
    std::vector<matjson::Value> unsortedAuthors = json["authors"].as<std::vector<matjson::Value>>();
    std::vector<click::Author> sortedAuthors = {};
    std::vector<click::Author> mainAuthors = {};
    std::vector<click::Author> provideAuthors = {};
    std::vector<click::Author> requestAuthors = {};

    for (const matjson::Value& author2 : unsortedAuthors) {
        click::Author author = author2.as<click::Author>();
        if (author.getType() == click::Author::Type::Main)
            mainAuthors.push_back(author);
        else if (author.getType() == click::Author::Type::Provider)
            provideAuthors.push_back(author);
        else if (author.getType() == click::Author::Type::Requester)
            requestAuthors.push_back(author);
        else
            mainAuthors.push_back(author);
    }

    sortedAuthors.reserve(unsortedAuthors.size());
    sortedAuthors.insert(sortedAuthors.end(), mainAuthors.begin(), mainAuthors.end());
    sortedAuthors.insert(sortedAuthors.end(), provideAuthors.begin(), provideAuthors.end());
    sortedAuthors.insert(sortedAuthors.end(), requestAuthors.begin(), requestAuthors.end());
    m_authors = sortedAuthors;

    // Click and Release Files (more shitty code incoming)
    std::vector<matjson::Value> oldClickFiles = json["click-files"].as<std::vector<matjson::Value>>();
    std::vector<matjson::Value> oldReleaseFiles = json["release-files"].as<std::vector<matjson::Value>>();
    std::vector<std::string> newClickFiles = {};
    std::vector<std::string> newReleaseFiles = {};

    for (const matjson::Value& newstring : oldClickFiles)
        newClickFiles.push_back(newstring.as_string());
    for (const matjson::Value& newstring : oldReleaseFiles)
        newReleaseFiles.push_back(newstring.as_string());

    m_clickFiles = newClickFiles;
    m_releaseFiles = newReleaseFiles;
}

} // namespace click
