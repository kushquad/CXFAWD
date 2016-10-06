#ifndef INPUT_H
#define INPUT_H

#include "scene.h"
#include "camera.h"

class Input
{
	public:
        /*  
            Angles of rotation for moving the camera
	        polarangle is the angle made by the camera vector projected on XZ plane, with the x-axis
            Y angle is the angle made by the camera vector with the XZ plane (or 90-angle made with Y axis)
         	axisangle is the rotation about the camera vector (used for roll)
        */
        static float polarangle;
        static float azimuthalangle;

        static float deltapolarangle;
        static float deltaazimuthalangle;
        static float deltafov;
		static int deltaTilt;
        static int deltaVertical;
        static int deltaMove;
        
        static float fov;
        static int strafe;
        static Vector<float> panvector;

        // Store mouse position
        static int xOrigin, yOrigin;

        static Scene* scene;
        static Camera* camera;
        bool sceneconnected;
        
        void init();
        Input();
        Input(Scene* scene);

        // Attach scene to camera
        void parent(Scene* scene);
        static void pressNormalKey(unsigned char key, int xx, int yy);
        static void releaseNormalKey(unsigned char key, int xx, int yy);
        static void pressKey(int key, int xx, int yy);
        static void releaseKey(int key, int x, int y);
        static void mouseButton(int button, int state, int x, int y);
        static void mouseMove(int x, int y);
        void setup();
};
#endif