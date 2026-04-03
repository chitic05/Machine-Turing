#include "parser.h"
#include <fstream>
#include <iostream>
#include <filesystem>
namespace fs = std::filesystem;
Parser::Parser(const std::string& fileName){
    fs::path path(fileName);
    if(path.is_relative())
        path = fs::current_path() / path; // tranformam intr-un path absolut

    std::ifstream fin(path);
    if(!fin){
        std::cerr << "Failed to open file: "<< fileName <<'\n';
        return;
    }
    std::string section = "none";
    std::string line;
    while(std::getline(fin, line)){
        // Remove comments (starting with # or ;)
        size_t commentPos = line.find_first_of("#;");
        if(commentPos != std::string::npos){
            line = line.substr(0, commentPos);
        }
        // Trim trailing whitespace
        line.erase(line.find_last_not_of(" \t\r\n") + 1);
        
        if(line != ""){
            if(line[0] == '.'){
                section = line;
                continue;
            }
            

            if(section != "none"){
                sections[section].push_back(line);
            }

        }
    }
    fin.close();
}

sectionsMap& Parser::getSections(){
    return sections;
}