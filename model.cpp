//
// Created by CameronM on 3/9/2026.
//
#include "model.h"
#include <iotream>
#include <fstream>
#include <iostream>

Model::Model(std::string path) {
    std::ifstream file(path);
    std::string line;

    if (file.is_open()) {
        while (std::getline(file, line)) {
            int w1 = line.find(' ')+1;
            


            if (line[0] == 'v') {

            } else if (line[0] == 'f') {

            }
        }
        file.close();
    } else {
        std::cerr << "Unable to open file: " << path << std::endl;
    }
}
