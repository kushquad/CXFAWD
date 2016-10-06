from operator import add, sub, mul
import math
from rigidbody import RigidBody
from constants import MIN_SPLITS, MAX_SPLITS
import random
import vector

class Particle(RigidBody):
	def __init__(self, manageridx, particlemanager, particleID, life, forcekernels, solver, position=[0,0,0], velocity=[0,0,0], acceleration=[0,0,0], mass=1, radius=1):
		super(Particle, self).__init__(solver)
		self.particlemanager = particlemanager
		self.idx = manageridx
		self.particleID = particleID 
		self.age = 0
		self.life = life
		self.pos = position
		self.vel = velocity
		self.acc = acceleration
		self.forcekernels = forcekernels
		self.radius = radius
		self.mass = mass
		self.store = None
	
	def __str__(self):
		pos_rep = " ".join(map(lambda x:str(x),self.pos))
		vel_rep = " ".join(map(lambda x:str(x),self.vel))
		acc_rep = " ".join(map(lambda x:str(x),self.acc))
		attributes = [str(self.idx),str(self.particleID),str(self.age),str(self.life),pos_rep, vel_rep, acc_rep, str(self.radius), str(self.mass)]
		return " ".join(attributes) 	

	def split(self):
		# Calculate number of splits possible, considering this particle will be destroyed
		if(self.mass<0.5 or vector.magnitude(self.vel)<1.8):
			return False
		print "Split"
		splits = min(len(self.particlemanager.freespace)+1, MIN_SPLITS + int(random.random()*(MAX_SPLITS-MIN_SPLITS+1)))
		if splits<2:
			return False

		# Mass split distribution
		density = self.mass*1.0/(self.radius**3)
		masssplittemp = self.mass*1.0/splits
		masssplit = [masssplittemp for i in xrange(splits)]
		#print self.idx, self.mass, masssplit
		radiussplittemp = (masssplittemp/density)**(1.0/3)
		radiussplit = [radiussplittemp for i in xrange(splits)]
		
		# Velocity split distribution
		#    - Magnitude split (conservation of energy)
		velocitysplit = []
		EMISSION_SPEED = 2
		normalvel = [EMISSION_SPEED*comp for comp in self.store]
		for i in xrange(splits):
			splitvelocity = map(add, normalvel, [random.random()*0.3-0.15, random.random()*0.3-0.15, random.random()*0.3-0.15])
			velocitysplit.append(splitvelocity)
		
		# Parent particle deletion
		# Since self is still a reference to this object, particle will not be
		# garbage collected to cause segmentation fault
		# However, particle still stays in memory, need a way to delete ourself
		self.particlemanager.destroyParticle(self.idx)

		# Particle creation
		for i in xrange(splits):
			self.particlemanager.createParticle(pos=self.pos, vel=velocitysplit[i], mass=masssplit[i], radius=radiussplit[i], perturb=False, collideFlag=1)
		return True
