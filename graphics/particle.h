#ifndef PARTICLE_H
#define PARTICLE_H

#include "scene.h"
#include "vector.h"

struct Particle
{
	int particleID;
	int managerindex;
	int age;
	int life;
	Vector<float> position;
	Vector<float> velocity;
	Vector<float> acceleration;
	float radius;
	float mass;

	void render();
};

class ParticleCache
{	
	public:
		std::string cachefilename;
		std::vector< std::vector<Particle> > particlecache;

		ParticleCache(std::string);
		void readParticleCache();
		void renderParticleCache(int);
};
#endif