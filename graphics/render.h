#ifndef RENDER_H
#define RENDER_H

#include "scene.h"
#include "camera.h"

class Render
{
	public:
		static int frame;
		static Scene* scene;
		static void (*renderfunction)(int fr);
		static ParticleCache* pc;

		void init(void (*renderfunction)(int fr));
		Render(void (*renderfunction)(int fr));
		Render(Scene* scene, void (*renderfunction)(int fr),ParticleCache *);
		void parent(Scene *scene);
		static void snapshot(std::string);
		static void rendermetafunction();
		void changeFrame(int frameno);
		void setup();
};

#endif
