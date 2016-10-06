#ifndef SCENE_H
#define SCENE_H

#include "vector.h"
#include "GL/glut.h"
#include <glm/glm.hpp>
#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <unistd.h>
#include <cstdio>
#include <sstream>
#include <ctime>

class Camera;
class Input;
class Render;
class Object;
class Model;
class ParticleCache;
class OBJParsing;

class Scene
{
    public:
        // Window title
        std::string windowtitle;    
         
        // Requested windows position
        int windowx;
        int windowy;

        // Requested windows mode
        int windowmode; 
                        
        // Window settings for scene
        static int width;
        static int height;
            
        // FOV setting for scene (in degrees)
        static float fov;
            
        // Global background colour for scene
        // Default is blue sky color
        Vector<float> bgcolor;
         
        bool render;
        bool pause;
        bool playback;
        bool fullrender;

        // Connectable nodes
        Render* renderer;
        Input* inputhandler;
        Camera* camera;

        // No of frames to cycle through
        int maxframes;

	    Scene();
	    static void windowResize(int, int);
	    void setup(int, char**);
};
#endif
