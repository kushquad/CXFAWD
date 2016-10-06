#include "input.h"
#include "camera.h"
#include "render.h"

float Input::polarangle;
float Input::azimuthalangle;

float Input::deltapolarangle;
float Input::deltaazimuthalangle;
float Input::deltafov;
int Input::deltaTilt;
int Input::deltaVertical;
int Input::deltaMove;

int Input::strafe;
Vector<float> Input::panvector;
int Input::xOrigin, Input::yOrigin;
float Input::fov;
Camera* Input::camera;
Scene* Input::scene;

void Input::init()
{
	polarangle = 0.0f;
    azimuthalangle = 0.0f;
    deltaMove = 0;
    deltaTilt = 0;
    deltaVertical = 0;
    deltapolarangle = 0;
    deltaazimuthalangle = 0;
    strafe = 0;
    panvector.x = 1, panvector.y = 0, panvector.z = 0;
}

Input::Input() { init(); }

Input::Input(Scene* scene) 
{
	init();
	parent(scene); 
	fov = scene->fov;
}

// Attach scene to input handler
void Input::parent(Scene* scene)
    { this->scene = scene; sceneconnected=true; this->scene->inputhandler = this; }

// Tilt and vertical axis movement controls
void Input::pressNormalKey(unsigned char key, int xx, int yy) 
{ 	
    switch (key)
    {
        case 27:  exit(0); break;
        case 'w': case 'W': deltaVertical = 1; break;
        case 's': case 'S': deltaVertical = -1; break;
        case 'm': case 'M': deltaTilt = -1; break;
        case 'n': case 'N': deltaTilt = 1;  break;
        case 'p': case 'P': scene->pause ^= true; break;
        case 'i': case 'I': scene->playback = true; scene->renderer->frame = 0; break;
        case 'r': case 'R': scene->render = true; break;
        case 'f': case 'F': scene->fullrender = true; scene->renderer->frame = 0; break;
	}
} 

void Input::releaseNormalKey(unsigned char key, int xx, int yy)
{
	switch (key)
    {
        case 'w':
        case 'W':
        case 's':
        case 'S': deltaVertical = 0; break;
        case 'm':
        case 'M':
        case 'n':
        case 'N': deltaTilt = 0;  break;
	}
}

// Forward and backward movement, strafing keys
void Input::pressKey(int key, int xx, int yy) 
{
   switch (key) 
   {
         case GLUT_KEY_UP : deltaMove=1; break;
         case GLUT_KEY_DOWN : deltaMove=-1; break;	 
     	 case GLUT_KEY_RIGHT: strafe=-1; break;
         case GLUT_KEY_LEFT: strafe=1;   break;
   }
}

// Reset all key effects
void Input::releaseKey(int key, int x, int y) 
{ 	
    switch (key) 
    {
         case GLUT_KEY_UP :
         case GLUT_KEY_DOWN : deltaMove = 0; break;
         case GLUT_KEY_LEFT : 
         case GLUT_KEY_RIGHT : strafe = 0; break;       
    }
}

void Input::mouseButton(int button, int state, int x, int y) 
{

    // Left button used for panning of camera
    if(button == GLUT_LEFT_BUTTON) 
    {
        // If pressed, the mouse cursor controls the panning
        // xOrigin and yOrigin store last known position of cursor
        if(state == GLUT_UP)
        {
	        polarangle += deltapolarangle, xOrigin = -1;
	        azimuthalangle += deltaazimuthalangle, yOrigin = -1;
        }
        else
	        xOrigin = x, yOrigin = y;
    }

    // button = 3 or 4 is triggered by mouse wheel up and down
    // This is used to control zoom
    if (button == 3 || button == 4)
        if (state == GLUT_DOWN)
    	    { (button==3)? camera->zoom(-0.5):camera->zoom(0.5); }
}

// Change the camera vector if pan is performed
void Input::mouseMove(int x, int y) 
{ 	
    // If xOrigin is positive => We have panned across XZ plane
    // i.e rotation along y-axis. This rotation is also called yaw.
    if(xOrigin >= 0) 
    {
        deltapolarangle = (xOrigin - x) * 0.003f;

        panvector.x = cos(polarangle + deltapolarangle);
        panvector.z = -sin(polarangle + deltapolarangle);
    }

    // If xOrigin is positive => We have panned across y-axis
    // i.e rotation along camera vector. This rotation is also called pitch.
    if(yOrigin >= 0)
    {
        deltaazimuthalangle = (yOrigin - y) * 0.003f;
        panvector.y = sin(azimuthalangle + deltaazimuthalangle);
    }
}

void Input::setup()
{
 	// Keyboard event processing
    glutIgnoreKeyRepeat(1);

    glutKeyboardFunc(pressNormalKey);
    glutKeyboardUpFunc(releaseNormalKey);
    glutSpecialFunc(pressKey);
    glutSpecialUpFunc(releaseKey);

 	// Mouse event processing
    glutMouseFunc(mouseButton);
    glutMotionFunc(mouseMove);
    
    camera = scene->camera;
}
