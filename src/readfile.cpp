#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <deque>
#include <stack>
#include "readfile.h"

bool readvals(std::stringstream &s,int numvals, float* values){
    for (int i = 0; i < numvals; i++) {
        s >> values[i]; 
        if (s.fail()) {
            std::cout << "Failed reading value " << i << " will skip\n"; 
            return false;
        }
    }
    return true; 
}

void readfile(const char* filename, Scene* scene) {
    std::string str, cmd; 
    std::ifstream in;
    in.open(filename); 
    if (in.is_open()) {
        getline (in, str); 
        while (in) {
            if ((str.find_first_not_of(" \t\r\n") != std::string::npos) && (str[0] != '#')) {
                // Ruled out comment and blank lines 

                std::stringstream s(str);
                s >> cmd; 
                int i; 
                float values[12]; // Position and color for light, colors for others
                                    // Up to 10 params for cameras.  
                bool validinput; // Validity of input 
                if (cmd == "light") {
                    validinput = readvals(s, 6, values); // Position/color for lts.
                    if (validinput) {
                        Light light = Light(Vector3f(values[0],values[1],values[2]),Vector3f(values[3],values[4],values[5]));
                        scene->addLight(light);
                    }
                }

                else if (cmd == "ambient") {
                    validinput = readvals(s, 3, values); // colors 
                    if (validinput) {
                        scene->addAmbient(Vector3f(values[0],values[1],values[2]));
                    }
                } else if (cmd == "diffuse") {
                    validinput = readvals(s, 4, values); 
                    if (validinput) {
                        // diffuse per object
                    }
                } else if (cmd == "specular") {
                    validinput = readvals(s, 4, values); 
                    if (validinput) {
                        // specular per object
                    }
                } else if (cmd == "emission") {
                    validinput = readvals(s, 4, values); 
                    if (validinput) {
                        // emission per object
                    }
                } else if (cmd == "shininess") {
                    validinput = readvals(s, 1, values); 
                    if (validinput) {
                        // shininess per object
                    }
                }

                else if (cmd == "sphere") {
                    validinput = readvals(s, 7, values); 
                    if (validinput) {
                        Sphere *sphere = new Sphere(values[0],Vector3f(values[1],values[2],values[3]),Vector3f(values[4],values[5],values[6]));
                        scene->addObject(sphere);
                    }
                }

                else if (cmd == "plane") {
                    validinput = readvals(s, 12, values); 
                    if (validinput) {
                        Plane *plane = new Plane(Vector3f(values[0],values[1],values[2]),Vector3f(values[3],values[4],values[5]),
                            Vector3f(values[6],values[7],values[8]),Vector3f(values[9],values[10],values[11]));
                        scene->addObject(plane);
                    }
                }
                else {
                    std::cerr << "Unknown Command: " << cmd << " Skipping \n"; 
                }
            }
            getline (in, str); 
        }
    } else {
        std::cout << "Unable to Open Input Data File " << filename << "\n"; 
        std::cout << "Displaying default scene\n"; 
        initializeDefaultScene(scene);
    }
}

void initializeDefaultScene(Scene* scene){
    Sphere *sphere = new Sphere(2,Vector3f(0,0,-10),Vector3f(1,0,0));
    Plane *floor = new Plane(Vector3f(0,-3,0),Vector3f(1,-3,0),Vector3f(1,-3,1),Vector3f(0,0,1));
    Light light = Light(Vector3f(5,0,-5));
    scene->addObject(sphere);
    scene->addObject(floor);
    scene->addLight(light);
}
