from constants import TIME_STEP_LENGTH
from operator import add, sub, mul
import vector
from rigidbody import RigidBody

class Collider:
	# Used for active pointobject - passive object collision
	def __init__(self, pointobject, genobject):
		self.objA = pointobject 
		self.objAactive = True
		self.objB = genobject
		self.objBactive = False

	def checkCollision(self):
		activeobject_position = self.objA.pos
		activeobject_velocity = self.objA.vel
		
		# First optimization, check if active object inside bounding box
		# If not, we can return - no collision to check/act upon
		passiveobject_boundingBox = self.objB.boundingBox
		for i in xrange(3):
			if not (passiveobject_boundingBox[0][i] <= activeobject_position[i] <= passiveobject_boundingBox[1][i]):
				return False

		# Second optimization - of all faces, do collision tests with
		# only the faces which are pointed towards the object
		passiveobject_faces = self.objB.faces
		passiveobject_normals = self.objB.facenormals
		passiveobject_facecenters = self.objB.facecenters

		l = len(passiveobject_faces)
		for i in xrange(0,l):
			vectortoface = map(sub, activeobject_position, passiveobject_facecenters[i])
			# Check for edge cases - this checks if angle is acute
			#print self.objB.name, vectortoface, passiveobject_normals[i], passiveobject_faces[i]
			if 0<=vector.cosangleBetween(vectortoface, passiveobject_normals[i])<=1:
				# Check if in next time step, we will cross the face
				temp = RigidBody(self.objA.solver)
				temp.forcekernels = self.objA.forcekernels[:]
				temp.acc = 0
				temp.vel = activeobject_velocity
				temp.pos = activeobject_position
				temp.applyDynamics(TIME_STEP_LENGTH)
				nextpos = temp.pos
				newvectortoface = map(sub, nextpos, passiveobject_facecenters[i])
				costheta = vector.cosangleBetween(newvectortoface, passiveobject_normals[i])
				#print costheta
				#print "Particle data : ", activeobject_position, activeobject_velocity
				#print "After coll : ", temp.pos, temp.vel
				if not (0<=costheta<=1):
					self.p = activeobject_position
					self.v = temp.vel
					self.vo = activeobject_velocity
					self.c = passiveobject_facecenters[i]
					self.n = passiveobject_normals[i]
					self.i = i
					self.t = (vector.dotproduct(self.c,self.n) - vector.dotproduct(self.p,self.n))/(vector.dotproduct(self.v,self.n))
					if not(0<=self.t<=1):
						continue
					pn = map(add, self.p, [self.t*v0 for v0 in self.v])
					if not vector.insideFace(pn,[self.objB.vertices[x] for x in self.objB.faces[i]]):
						continue
					self.pn = pn
					return True 
		return False
				
	def onCollision(self):
		objA = self.objA
		objB = self.objB
		p = self.p
		v = self.v
		c = self.c
		n = self.n
		i = self.i
		vo = self.vo

		# Get velocity components along normal and along plane
		n = vector.normalize(n)
		temp = vector.dotproduct(v,n)
		normalvector = [temp*normalcomp for normalcomp in n]
		planevector = map(sub, v, normalvector)

		# Modify velocity due to frictional impulse along plane surface
		frictionimpulsemagnitude = objB.friction*(1)*vector.magnitude(normalvector)
		planevectormagnitude = vector.magnitude(planevector)
		#print frictionimpulsemagnitude, planevectormagnitude
		if frictionimpulsemagnitude<planevectormagnitude:
			planevectorunit = vector.normalize(planevector) 	
			frictionvector = [frictionimpulsemagnitude*planevectorunitcomp for planevectorunitcomp in planevectorunit]
			#print planevector, frictionvector
			planevector = map(sub, planevector, frictionvector)
			#print planevector
			#print vector.magnitude(planevector)
		else:
			planevector = [0,0,0]

		# Modify velocity due to elastic impulse along plane normal
		normalvector = [-objB.elasticity*normalvectorcomp for normalvectorcomp in normalvector]

		# Modify objA's characteristics
		# No change in acceleration, since no other forces were introduced during this period
		# Friction and elastic impulses were treated separately
		temp = map(add, planevector, normalvector)
		objA.pos = self.pn[:]
		objA.vel = temp[:]
		objA.collideFlag = 1
		objA.store = normalvector
