from operator import add, mul
'''
Represents any rigid body.
This might be a collider, emitter, particle etc.
'''
class RigidBody(object):
	
	def __init__(self, solver):
		self.pos = [0,0,0]
		self.vel = [0,0,0]
		self.acc = [0,0,0]
		self.mass = 1
		self.radius = 1
		self.forcekernels = []
		self.collideFlag = 0
		self.solver = solver
		self.solver.rigidbodies.append(self)

	def updatePos(self, timestep):
		deltapos = [timestep*velcomp for velcomp in self.vel]
		self.pos = map(add, self.pos, deltapos)

	def updateVel(self, timestep):
		deltavel = [timestep*acccomp for acccomp in self.acc]
		self.vel = map(add, self.vel, deltavel)

	def updateAcc(self, timestep, force=[0,0,0]):
		self.acc = [forcecomp*1.0/self.mass for forcecomp in force]

	def calculateNetForce(self, timestep):
		force = [0,0,0]
		for kernel in self.forcekernels:
			force = map(add, force, kernel(self.solver, self, timestep))
		return force

	def applyDynamics(self, timestep):
		force = self.calculateNetForce(timestep)
		self.updateAcc(timestep, force)
		self.updateVel(timestep)
		self.updatePos(timestep)

	def postDynamics(self):
		pass

# r = RigidBody()
# from force import gravity_kernel, laminar_air_drag_kernel
# r.forcekernels.append(gravity_kernel)
# r.forcekernels.append(laminar_air_drag_kernel)
# for i in xrange(24):
# 	r.applyDynamics(1.0/24)
# 	print r.pos