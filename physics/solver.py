from collider import Collider
from constants import MAX_ITERATIONS, TIME_STEP_LENGTH

class Solver:
	def __init__(self):
		self.maxsimulationiterations = MAX_ITERATIONS
		self.rigidbodies = []
		self.colliders = []
		self.passivebodies = []
		self.time = 0
		self.emitters = []
		
	def run(self):
		for iteration in xrange(0, self.maxsimulationiterations):
			for j in xrange(2):
				self.emitters[0].createParticle()
			self.time = iteration
			print "\nFrame "+str(iteration),
			# Phase 1 : Collision event
			l = len(self.colliders)
			print "Collision checks : "+str(l)
			for i in xrange(l):
				#print i,
				colliderpairs = self.colliders[i]
				if colliderpairs:
					'''
					Calls optimized methods for collision check
						- check bounding box intersection
						- back face normal test
					'''
					if colliderpairs.objA.collideFlag==1:
						colliderpairs.objA.collideFlag=2
						continue
					if colliderpairs.checkCollision():	
						#print "Collision"				
						colliderpairs.onCollision()
						
						# Phase 2 : Splitting logic for rigid bodies
						if colliderpairs.objA.split():
							print "Split"
							self.colliders[i] = None

			# Phase 3 : Dynamics for all rigid bodies
			for rigidbody in self.rigidbodies:
				if rigidbody:
					if rigidbody.collideFlag==0:
						rigidbody.applyDynamics(TIME_STEP_LENGTH)
					elif rigidbody.collideFlag==2:
						rigidbody.updatePos(TIME_STEP_LENGTH)
						rigidbody.collideFlag=0
					rigidbody.postDynamics()
			#print self.rigidbodies[1]