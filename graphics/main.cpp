#include "scene.h"
#include "camera.h"
#include "render.h"
#include "input.h"
#include "particle.h"
#include "OBJparsing.h"

void drawSawblade(int frame)
{
	glColor4f(1,1,1,1);	
	OBJParsing o;
	string filename = "../assets/sawblade_new.obj";
	o.Load(filename.c_str());
	glPushMatrix();
		glTranslatef(0,1.5,0.0075);
		glRotatef(frame*12,0,0,1);
		glTranslatef(0,-1.5,-0.0075);
		o.Draw();
		
	glPopMatrix();
}

void drawSlab()
{
	glColor4f(1,1,1,1);	
	OBJParsing o;
	string filename = "../assets/slab_new.obj";
	o.Load(filename.c_str());
	glPushMatrix();
		glTranslatef(0,0,0);
		o.Draw();
	glPopMatrix();
}

void drawBunny()
{
	glColor4f(1,1,1,1);	
	OBJParsing o;
	string filename = "../assets/stanford_bunny.obj";
	o.Load(filename.c_str());
	glPushMatrix();
		glTranslatef(-2.7,0,-1.1);
		o.Draw();
	glPopMatrix();
}

void drawCube()
{
	glColor4f(0.4,0,0,1);	
	OBJParsing o;
	string filename = "../assets/cube_new.obj";
	o.Load(filename.c_str());
	glPushMatrix();
	glTranslatef(-2.1,0,0.0075);
		o.Draw();
	glPopMatrix();
}

void drawSphere()
{
	glColor4f(0,0.4,0,1);	
	OBJParsing o;
	string filename = "../assets/sphere.obj";
	o.Load(filename.c_str());
	glPushMatrix();
		glTranslatef(-0.5,0,1);
		o.Draw();
	glPopMatrix();
}

void drawCone()
{
	glColor4f(0,0,0.4,1);	
	OBJParsing o;
	string filename = "../assets/cone_new.obj";
	o.Load(filename.c_str());
	glPushMatrix();
		glTranslatef(-2.2,0,1.6);
		o.Draw();
	glPopMatrix();
}

void renderScene(int frame)
{
	initLighting();

	glColor4f(0,0,0,1);
	glBegin(GL_QUADS);
		glVertex3f(-100,-0.05,-100);
		glVertex3f(-100,-0.05,100);
		glVertex3f(100,-0.05,100);
		glVertex3f(100,-0.05,-100);
	glEnd();

	drawBunny();
	drawSphere();
	drawCube();
	drawCone();
	drawSlab();
	drawSawblade(frame);
}

int main(int argc, char **argv)
{
	ParticleCache pc("../cache/particle.cch");
	pc.readParticleCache();

    Scene scene; 
    Input input(&scene);
    Camera camera(&scene);
    Render render(&scene, &renderScene, &pc);
    scene.setup(argc, argv);
    
    return 0;
}
