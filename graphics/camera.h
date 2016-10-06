#ifndef CAMERA_H
#define CAMERA_H

#include "scene.h"

class Camera
{
	public:
      static const bool DIR_FORW = 0;
      static const bool DIR_BACK = 1;
      static const bool DIR_LEFT = 0;
      static const bool DIR_RIGHT = 1;
      static const bool DIR_UP = 0;
      static const bool DIR_DOWN = 1;
      static const bool ROT_LEFT = 0;
      static const bool ROT_RIGHT = 1;

      float tiltangle;
        
     	// Vectors describing the camera's position and direction it points in    
      Vector<float> position;
      Vector<float> direction;
     
     	// Field of view of the camera
      float fov;
     
     	// Parent scene and scene connection tracker
      Scene* scene;
      bool sceneconnected;

      void init();
      Camera();
      Camera(Scene *scene);
      void parent(Scene *scene);
      void moveForward(bool, float);
      void strafe(bool, float);
      void moveVertical(bool, float);
      void update();
      void solve();
      void tilt(bool, float);
      void zoom(float);
      void pan(Vector<float>);
};

#endif