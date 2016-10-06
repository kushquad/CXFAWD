/* Viewing setup and camera mechanics

		 y
		 |
		 |
		 |_ _ _ x
		/
	   /  xz plane of movement (strafe, forward, backward)
	  /
	 z
	 
	 Camera pan is rotation about y-axis and xz-plane.
	 Camera zoom is achieved through changing field of view angle in perspective.
	 Camera roll is rotation about the camera vector.
	 2D Movement happens only in XZ-plane at constant Y, so we consider only XZ components of the camera vector.
	 Strafing is achieved by moving along vectors perpendicular to XZ component of camera vector.
	 Moving upward and downward is done by moving along Y-axis.
	 Moving forward and backward is moving along the XZ component of the camera vector.
*/
#include "camera.h"
#include "input.h"

void Camera::init()
{
    tiltangle = 0.0f;
    fov = 45.0f;
    sceneconnected = false;
    position.x = 0, position.y = 0.5, position.z = 0;
    direction.x = 1, direction.y = 0, direction.z = 0;
}

// Camera constructors
Camera::Camera() 
{
    init();
}

Camera::Camera(Scene *scene)
{ 
    init();
    parent(scene); 
}

// Attach scene to camera
void Camera::parent(Scene* scene)
    { this->scene = scene; sceneconnected=true; this->scene->camera = this;}
    
void Camera::moveForward(bool dir, float distance=0.02)
{
    position.x += direction.x * (dir?-1:1)*distance;
    position.z += direction.z * (dir?-1:1)*distance;
}

// If the current camera vector along ground is vxi + vzj,
// the left and right perpendicular vectors will be -vxi + vzj and vxi - vzj
void Camera::strafe(bool dir, float distance=0.02)
{
    position.x += (dir?-1:1)*direction.z * distance;
    position.z += (dir?1:-1)*direction.x * distance;
}

void Camera::moveVertical(bool dir, float distance=0.02)
    { position.y += (dir?-1:1)*distance; }

// Limit tilt to only between -45° and 45°
void Camera::tilt(bool dir, float angle=0.005)
{ 
    float tiltchange= (dir?1:-1)*angle; 
    if(tiltangle+tiltchange>=-45.0 && tiltangle+tiltchange<=45.0)
        tiltangle+=tiltchange;
} 

void Camera::update()
{
    if(scene->inputhandler->deltaMove)
        scene->inputhandler->deltaMove==1?moveForward(DIR_FORW):moveForward(DIR_BACK);
    if(scene->inputhandler->strafe)
        scene->inputhandler->strafe==1?strafe(DIR_LEFT):strafe(DIR_RIGHT);
    pan(scene->inputhandler->panvector);
    if(scene->inputhandler->deltaTilt)
        scene->inputhandler->deltaTilt==1?tilt(ROT_LEFT):tilt(ROT_RIGHT);
    if(scene->inputhandler->deltaVertical)
        scene->inputhandler->deltaVertical==1?moveVertical(DIR_UP):moveVertical(DIR_DOWN);
    if(scene->inputhandler->deltafov)
        zoom(scene->inputhandler->deltafov);
}

/* Places camera at required position and direction
    and renders viewport accordingly.
    Loads identity matrix before making a call to gluLookAt
    Additionally rolls/tilts the camera by required amount.
*/
void Camera::solve()
{
    glLoadIdentity();
    gluLookAt( position.x,             position.y,             position.z,
               position.x+direction.x, position.y+direction.y, position.z+direction.z,
               0.0f,                   1.0f,                   0.0f  );
    glTranslatef(position.x, position.y, position.z);
    glRotatef(tiltangle, direction.x, direction.y, direction.z);
    glTranslatef(-position.x, -position.y, -position.z);
}

/* 
	Zoom is controlled by changing fov angle
	Small fov angle or smaller fov => High zoom 
	Large fov angle or larger fov => Low zoom
    Argument to zoom is the change in fov, not fov directly.
*/
void Camera::zoom(float deltafov=0.05)
{
    // Increase or decrease zoom using mouse wheel
    // Zoom is controlled by changing fov angle
    // Small fov angle or smaller fov => High zoom 
    // Large fov angle or larger fov => Low zoom
    if(fov>=1.0f && fov<=45.0f)
        fov += deltafov;
    if(fov<=1.0f)
        fov = 1.0f;
    if(fov>=45.0f)
        fov = 45.0f;
    // Set up perspective projection again to reflect FOV change
    scene->fov = fov;
    scene->windowResize(scene->width, scene->height);
}

// Pan across XZ plane - yaw
// Pan across Y plane - pitch
void Camera::pan(Vector<float> panvector)
    { direction.x = panvector.x, direction.y = panvector.y, direction.z = panvector.z; }