#include "render.h"
#include "particle.h"

int Render::frame;
Scene* Render::scene;
void (* Render::renderfunction)(int fr);
ParticleCache* Render::pc;

void Render::init(void (*renderfunction)(int fr))
{
	this->renderfunction = renderfunction;
	frame = 0;
}

Render::Render(void (*renderfunction)(int fr)) { init(renderfunction); }

Render::Render(Scene* scene, void (*renderfunction)(int fr), ParticleCache *pc)
{
	init(renderfunction);
	parent(scene);
	this->pc = pc;
}

void Render::parent(Scene *scene)
	{ this->scene = scene; this->scene->renderer = this; }

void Render::snapshot(std::string filename)
{
	int x = scene->width;
	int y = scene->height;

	// Allocate 3 channels for RGB snapshots
    long imageSize = x * y * 3;
    unsigned char *data = (unsigned char*)malloc(sizeof(char)*imageSize);
    glReadPixels(0, 0, x, y, GL_BGR, GL_UNSIGNED_BYTE, data);
    
    // Construct header for TGA
    int xa= x % 256;
    int xb= (x-xa)/256;int ya= y % 256;
    int yb= (y-ya)/256;
    unsigned char header[18]={0,0,2,0,0,0,0,0,0,0,0,0,(char)xa,(char)xb,(char)ya,(char)yb,24,0};
    
    // Write header and data to binary file
    FILE *fp;
    fp = fopen(filename.c_str(), "wb");
    fwrite(header, sizeof (char),18,fp);
    fwrite(data, sizeof (char), imageSize, fp);
    fclose(fp);

    // Cleanup
    free(data);
    data=NULL;
}

void Render::rendermetafunction()
{
	// Log frame number
	//std::cout<<"Frame "<<frame<<"\n";
	
	// Set background color
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(scene->bgcolor.x, scene->bgcolor.y, scene->bgcolor.z, 1.0f);
	
	// Camera solving
	scene->camera->update();
	scene->camera->solve();

	pc->renderParticleCache(frame);

	(*renderfunction)(frame);

	// Full render of all frames
	if(scene->fullrender)
	{
		std::ostringstream o;
		o<<"../renders/"<<time(NULL)<<"_frame_f_"<<frame;
		snapshot(o.str());
		if(frame+1==scene->maxframes)
			scene->fullrender = false;
	}

	// Render of single frame
	if(scene->render)
	{
		std::ostringstream o;
		o<<"../testrenders/"<<time(NULL)<<"frame_r_"<<frame;
		snapshot(o.str());
		scene->render = false;
	}

	glutSwapBuffers();
	if(scene->fullrender)
	{
		frame += 1;
		return;
	}

	// Playback at 24 fps
	if(scene->playback)
		usleep(41000);

	// Count frames if not paused or in playback mode
	if(!scene->pause || scene->playback)
	{
		frame += 1;
		if(frame >= scene->maxframes)
		{	
			if(scene->playback)
				scene->playback = false;
			frame = 0;
		}
	}
}

void Render::changeFrame(int frameno)
	{ frame = frameno; }

void Render::setup()
{
	// Global graphic settings
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST); 
	glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST); 
	glEnable(GL_LINE_SMOOTH);

	glutDisplayFunc(rendermetafunction);
	glutIdleFunc(rendermetafunction);
}