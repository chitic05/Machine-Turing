#pragma once
#include <unordered_map>
#include <vector>
#include <string>

using sectionsMap = std::unordered_map<std::string, std::vector<std::string>>;

class Parser{
    private:
        sectionsMap sections;
    public:
        Parser(const std::string& fileName);
        sectionsMap& getSections();

};
