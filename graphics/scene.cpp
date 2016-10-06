#include "scene.h"
#include "camera.h"
#include "input.h"
#include "render.h"

int Scene::width = 1280;
int Scene::height = 720;
float Scene::fov = 45.0;

// Scene Constructor
Scene::Scene()
{
    windowtitle = "Dreamworks FX Challenge";
    windowx = 100;
    windowy = 100;
    windowmode = GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA;
    //Vector<float> tempcolor(153.0/255.0, 131.0/255.0, 1.0f);
    Vector<float> tempcolor(0.8, 0.8, 0.8);
    bgcolor = tempcolor;
    maxframes = 63;
    render = false;
    pause = false;
    playback = false;
    fullrender = false;
}

// Resize window   
void Scene::windowResize(int w, int h)
{
    // Set up perspective projection
    float aspratio =  w * 1.0 / h;
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glViewport(0, 0, w, h);
    gluPerspective(fov, aspratio, 0.1f, 1000.0f);
    glMatrixMode(GL_MODELVIEW);

    // Update scene settings for width and height
    width = w;
    height = h;
}

// Inital call to make GLUT calls to set up scene
void Scene::setup(int argc, char **argv)
{
    //if(renderer.sceneconnected && inputhandler.sceneconnected && camera.sceneconnected)
    if(camera->sceneconnected)
    {
        glutInit(&argc, argv);
        glutInitDisplayMode(windowmode);
        glutInitWindowPosition(windowx, windowy);
        glutInitWindowSize(width, height);
        glutCreateWindow(windowtitle.c_str());
        glutReshapeFunc(windowResize);
        
        inputhandler->setup();
        renderer->setup();
        glutMainLoop();
    }
}