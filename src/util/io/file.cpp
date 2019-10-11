//
// Created by sneeuwpop on 21-6-19.
//

#include "file.h"
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include "../debug/nice_error.h"

std::string File::readAssetAsString(const char *path) {
    return readString(&std::string("../../../../assets/").append(path));  // TODO: This is not universal.
}

std::string File::readString(std::string *path) {
    std::cout << "READING: " << path->c_str() << std::endl;
    
    std::ifstream fileStream(path->c_str(), std::ios::in);
    if (!fileStream.is_open())
        throw nice_error("error while opening file: " + *path);

    std::string returnValue;

    std::stringstream stringStream;
    stringStream << fileStream.rdbuf();
    returnValue = stringStream.str();
    fileStream.close();

    if (fileStream.bad())
        throw nice_error("error while reading file");

    return returnValue;
}