from rigidbody import RigidBody
from operator import add, sub, mul
import random
from particle import Particle
from collider import Collider
import vector

MIN_LIFE = 200
MAX_LIFE = 300
MAX_PARTICLES = 480
GEN_RATE = 30
'''
Represents an emitter of particles.
An emitter is responsible for managing the whole
particle system. 
'''
class Emitter(RigidBody):
	def __init__(self, cacheobj, solver, maxparticles=MAX_PARTICLES, genrate=GEN_RATE, minlife=MIN_LIFE, maxlife=MAX_LIFE):
		super(Emitter, self).__init__(solver)
		self.maxparticles = maxparticles
		
		# Particle array for management
		self.particlearray = self.maxparticles*[None]
		self.particlecount = 0
		self.particleID = 0
		self.freespace = range(self.maxparticles)
		self.cache = cacheobj
		self.particleforcekernels = []
		self.emittervelocity = [-5,6,0]
		self.solver = solver
		self.solver.emitters.append(self)
		self.minlife = minlife
		self.maxlife = maxlife

	def createParticle(self, pos=None, vel=None, acc=None, mass=1, radius=1, perturb=True, collideFlag=0):
		# Populate attributes if particle is being emitted by emitter
		pos = pos or self.pos
		vel = vel or map(add, self.vel, self.emittervelocity)
		acc = acc or self.acc

		# Velocity perturbation
		if perturb:
			mvel = vector.magnitude(vel)*(1+random.random()*0.5-0.25)
			uvel = vector.normalize(vel)
			uvel[0] += random.random()*0.3 - 0.15
			uvel[1] += random.random()*0.2 - 0.1
			uvel[2] += random.random()*0.38 - 0.24
			uvel = vector.normalize(uvel)
			vel =  [mvel*uvelcomp for uvelcomp in uvel][:]

		# Use an auxillary data structure to record
		# free spaces in the particlearray to store
		# new particle data
		freelocation = self.freespace[-1]
		
		# Increment ID of particle, particlecount
		life = self.minlife + int(random.random()*(self.maxlife-self.minlife))
		newparticle = Particle(freelocation, self, self.particleID+1, life, self.particleforcekernels, self.solver, pos, vel, acc, mass, radius)
		newparticle.collideFlag = collideFlag
		self.particlearray[freelocation] = newparticle
		self.freespace.pop()

		for passivebody in self.solver.passivebodies:
			self.solver.colliders.append(Collider(newparticle, passivebody))
		
		self.particleID += 1
		self.particlecount += 1

	def destroyParticle(self, idx):
		# Update particle count
		self.particlecount -= 1
		self.freespace.append(idx)
		# Check the memory management on this - possible memory leak
		self.particlearray[idx] = None

	def generateParticles(self):
		spaceleft = self.maxparticles - self.particlecount
		for i in xrange(0, min(spaceleft, genrate)):
			createParticle()

	def currentParticleData(self, time):
		datastring = []
		for i in xrange(0,self.maxparticles):
			if self.particlearray[i]:
				datastring.append(str(time)+" "+str(self.particlearray[i]))
		return "\n".join(datastring)+'\n'

	def postDynamics(self):
		self.cache.exportToCache(self.currentParticleData(self.solver.time))
