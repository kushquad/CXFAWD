#include "particle.h"
#include "vector.h"

void Particle::render()
{
	glColor3f(255,255,0);
	const float lengthfactor = 1.0/48;
	glVertex3f(position.x-velocity.x*lengthfactor, position.y-velocity.y*lengthfactor, position.z-velocity.z*lengthfactor);
	glColor3f(244,203,66);
	glVertex3f(position.x+velocity.x*lengthfactor, position.y+velocity.y*lengthfactor, position.z+velocity.z*lengthfactor);
}

ParticleCache::ParticleCache(std::string cachename)
	{ cachefilename = cachename; }

void ParticleCache::readParticleCache()
{
	std::ifstream cachefile(cachefilename.c_str());
	std::string line;
	int currenttime = 0;
	int temptime;
	std::vector<Particle> tempcache;

	while(!cachefile.eof())
	{
		Particle temp;
		cachefile>>temptime;
		if(temptime!=currenttime)
		{
			std::cerr<<"Read data from "<<tempcache.size()<<" particles for frame "<<currenttime<<".\n";
			particlecache.push_back(tempcache);
			tempcache.clear();
			currenttime = temptime;
		}

		cachefile>>temp.particleID;
		cachefile>>temp.managerindex;
		cachefile>>temp.age;
		cachefile>>temp.life;
		cachefile>>temp.position.x>>temp.position.y>>temp.position.z;
		cachefile>>temp.velocity.x>>temp.velocity.y>>temp.velocity.z;
		cachefile>>temp.acceleration.x>>temp.acceleration.y>>temp.acceleration.z;
		cachefile>>temp.radius;
		cachefile>>temp.mass;
		tempcache.push_back(temp);
	}
	std::cerr<<"Read data from "<<tempcache.size()<<" particles for frame "<<currenttime<<".\n";
	particlecache.push_back(tempcache);
	std::cerr<<"Read "<<particlecache.size()<<" frames.\n";
}

void ParticleCache::renderParticleCache(int frame)
{
	//glPointSize(5.0);
	glLineWidth(3);
	glBegin(GL_LINES);
	std::cerr<<"Render Particle Frame : "<<frame<<"\n";
	std::vector<Particle> p = particlecache[frame];
	std::cerr<<p[0].position.x<<" "<<p[0].position.y<<" "<<p[0].position.z;
	int l = p.size();
	for(int i=0;i<l;i++)
		p[i].render();
	glEnd();
	glLineWidth(1);
}