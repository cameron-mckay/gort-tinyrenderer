//
// Created by CameronM on 3/9/2026.
//
#include "model.h"
#include <iostream>
#include <fstream>

Model::Model(std::string path) {
    std::ifstream file(path);
    std::ofstream out("output.txt");
    std::string line;

    if (file.is_open()) {
        while (std::getline(file, line)) {
            int i = line.find(' ');
            int j;
            std::string type = line.substr(0, i);
            if (type != "v" && type != "f") continue;
            j = line.find(' ', i + 1);
            std::string s1 = line.substr(i + 1, j);
            i = j;
            j = line.find(' ', i + 1);
            std::string s2 = line.substr(i + 1, j);
            std::string s3 = line.substr(j+1, line.length()-1);

            if (type == "v") {
                vec3 vertex;
                vertex.x = std::stof(s1);
                vertex.y = std::stof(s2);
                vertex.z = std::stof(s3);
                vertices.push_back(vertex);
            } else if (type == "f") {
                face face;
                face.v0 = std::stoi(s1.substr(0,s1.find('/')))-1;
                face.v1 = std::stoi(s2.substr(0,s2.find('/')))-1;
                face.v2 = std::stoi(s3.substr(0,s3.find('/')))-1;
                faces.push_back(face);
            }
        }
        std::cout << "Vertices: " << vertices.size() << std::endl;
        std::cout << "Faces: " << faces.size() << std::endl;
        file.close();

        if (out.is_open()) {
            for (int i = 0; i < vertices.size(); i++) {
                out << "v " << vertices[i].x << " " << vertices[i].y << " " << vertices[i].z << std::endl;
            }
            out << "# " << vertices.size() << " vertices" << std::endl;
            out << std::endl;

            for (int i = 0; i < faces.size(); i++) {
                out << "f " << faces[i].v0 << " " << faces[i].v1 << " " << faces[i].v2 << std::endl;
            }
            out << "# " << faces.size() << " faces" << std::endl;
            out << std::endl;

            out.close();
        }
    } else {
        std::cerr << "Unable to open file: " << path << std::endl;
    }
}
