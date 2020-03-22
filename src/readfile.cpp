#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <deque>
#include <stack>
#include "readfile.h"

bool readvals(std::stringstream &s,int numvals, float* values, std::string* filename){
    for (int i = 0; i < numvals; i++) {
        s >> values[i]; 
        if (s.fail()) {
            std::cout << "Failed reading value " << i << " will skip\n"; 
            return false;
        }
    }
    std::string x;
    if (s >> x){
        filename->assign(x);
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
                float values[15]; // Position and color for light, colors for others
                bool validinput; // Validity of input 
                if (cmd == "light") {
                    std::string type;
                    s >> type;
                    if (type == "point") {
                        validinput = readvals(s, 6, values); // Position/color for lts.
                        if (validinput) {
                            PointLight *light = new PointLight(Vector3f(values[0],values[1],values[2]),Vector3f(values[3],values[4],values[5]));
                            scene->addLight(light);
                        }
                    } else if (type == "area"){
                        validinput = readvals(s, 15, values); // Position/color for lts.
                        if (validinput) {
                            Rect *rect = new Rect(Vector3f(values[0],values[1],values[2]),Vector3f(values[3],values[4],values[5])
                                ,Vector3f(values[6],values[7],values[8]),Vector3f(values[9],values[10],values[11]),Vector3f(values[10],values[13],values[14]));
                            AreaLight *light = new AreaLight(rect);
                            scene->addLight(light);
                            scene->addObject(rect);
                        }
                    }
                } else if (cmd == "ambient") {
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
                } else if (cmd == "sphere") {
                    validinput = readvals(s, 7, values); 
                    if (validinput) {
                        Sphere *sphere = new Sphere(values[0],Vector3f(values[1],values[2],values[3]),Vector3f(values[4],values[5],values[6]));
                        scene->addObject(sphere);
                    }
                } else if (cmd == "plane") {
                    validinput = readvals(s, 12, values); 
                    if (validinput) {
                        Plane *plane = new Plane(Vector3f(values[0],values[1],values[2]),Vector3f(values[3],values[4],values[5]),
                            Vector3f(values[6],values[7],values[8]),Vector3f(values[9],values[10],values[11]));
                        scene->addObject(plane);
                    }
                } else if (cmd == "triangle") {
                    validinput = readvals(s, 12, values); 
                    if (validinput) {
                        Triangle *triangle = new Triangle(Vector3f(values[0],values[1],values[2]),Vector3f(values[3],values[4],values[5]),
                            Vector3f(values[6],values[7],values[8]),Vector3f(values[9],values[10],values[11]));
                        scene->addObject(triangle);
                    }
                } else if (cmd == "mesh") {
                    std::string filename;
                    validinput = readvals(s, 4, values, &filename);
                    if (validinput) {
                        Mesh *mesh = new Mesh(Vector3f(values[1],values[2],values[3]));
                        std::cout << filename << std::endl;
                        readMesh(filename,mesh);
                        mesh->addBoundingBox();
                        scene->addObject(mesh);
                    }
                } else {
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

void readMesh(std::string filename, Mesh* mesh){
    std::vector<Vector3f> vecv;
    std::vector<Vector3f> vecn;
    std::ifstream infile(filename);
    std::string line;
    while (std::getline(infile, line)){
        std::stringstream ss(line);
        std::string type, v, n;
        if (ss >> type) {
            if (type == "f"){
                Triangle *triangle;
                std::vector<int> points;
                while (ss >> v){
                    for (int i = 0; i < v.length(); i++) {
                        if(v[i]=='/') {
                            points.push_back(std::stoi(n)); n="";
                            break;
                        } else {
                            n += v[i];
                        }
                    }
                }
                try {
                    triangle = new Triangle(vecv[points[0]-1],vecv[points[1]-1],vecv[points[2]-1]);
                    triangle->setVertexNormals(vecn[points[0]-1],vecn[points[1]-1],vecn[points[2]-1]);
                    mesh->addTriangle(triangle);
                } catch(...) {
                    std::cout << "out" << std::endl;
                }
                
            } else {
                std::string n1, n2, n3;
                ss >> n1;
                ss >> n2; 
                ss >> n3;
                if (type == "v") {
                    vecv.push_back(Vector3f(std::stof(n1),std::stof(n2),std::stof(n3)-10));
                } else if (type == "vn") {
                    vecn.push_back(Vector3f(std::stof(n1),std::stof(n2),std::stof(n3)));
                }
            }
        }
    }
}

void initializeDefaultScene(Scene* scene){
    Sphere *sphere = new Sphere(2,Vector3f(0,0,-10),Vector3f(1,0,0));
    Plane *floor = new Plane(Vector3f(0,-3,0),Vector3f(1,-3,0),Vector3f(1,-3,1),Vector3f(0,0,1));
    PointLight *light = new PointLight(Vector3f(5,0,-5));
    scene->addObject(sphere);
    scene->addObject(floor);
    scene->addLight(light);
}
