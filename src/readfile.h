#include "scene.hpp"

bool readvals (std::stringstream &s, int numvals, float * values, std::string * filename = NULL) ;
void readfile (const char* filename, Scene* scene) ;
void readMesh (std::string, Mesh*);
void initializeDefaultScene(Scene* scene) ;
