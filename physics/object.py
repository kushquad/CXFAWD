from operator import add, sub, mul
import math
import vector
from objreader import OBJreader

class Object:	
	def __init__(self, solver, filename=None):
		# Geometric properties
		self.vertices = []
		solver.passivebodies.append(self)
		'''
		Only faces allowed are triangles
		readFromObj() converts any quads/polygons to triangles
		'''
		self.faces = []
		self.facenormals = []
		self.facecenters = []
		self.metacenter = None
		self.name = filename

		# Material properties
		self.elasticity = 0.28
		self.friction = 0.3

		if filename:
			o = OBJreader(filename)
			self.faces = o.faces
			self.vertices = o.vertices
			self.load()

	def calculateBoundingBox(self):
		maxbound = self.vertices[0][:]
		minbound = self.vertices[0][:]
		for vertex in self.vertices:
			for i in xrange(3):
				maxbound[i] = max(maxbound[i], vertex[i])
				minbound[i] = min(minbound[i], vertex[i])
		self.boundingBox = (map(add,[-1,-1,-1], minbound),map(add, [1,1,1], maxbound))

	def calculateObjectMetacenter(self):
		l = len(self.vertices)
		metacenter = [0,0,0]
		for i in xrange(0,l):
			metacenter = map(add, self.vertices[i], metacenter)
		self.metacenter = [metacentercomp*1.0/l for metacentercomp in metacenter]

	def calculateFaceCenters(self):
		l = len(self.faces)
		for i in xrange(l):
			face = self.faces[i]
			facevertices = [self.vertices[vertidx] for vertidx in face]
			center = [0,0,0]
			for facevertex in facevertices:
				center = map(add, center, facevertex)
			self.facecenters.append([centercomp/3.0 for centercomp in center])

	def applyTranslation(self, translationvector):
		l = len(self.vertices)
		for i in xrange(0,l):
			self.vertices[i] = map(add, self.vertices[i],translationvector)

	def calculateFaceNormals(self):
		for face in self.faces:
			basepoint = self.vertices[face[0]]
			point1 = self.vertices[face[1]]
			point2 = self.vertices[face[2]]
			point1vec = map(sub, point1, basepoint)
			point2vec = map(sub, point2, basepoint)
			normal = vector.normalize(vector.crossproduct(point1vec, point2vec))
			self.facenormals.append(normal)

	def load(self):
		self.calculateFaceNormals()
		self.calculateFaceCenters()
		self.calculateBoundingBox()

# from solver import Solver
# solverobj = Solver()
# # Ground plane
# ground = Object(solverobj)
# ground.vertices = [[-100,0,-100],[100,0,-100],
# 				   [100,0,100],[-100,0,100]]
# ground.faces = [[2,1,0],[2,0,3]]
# ground.applyTranslation([0,0,0])
# ground.calculateFaceNormals()
# ground.calculateFaceCenters()
# ground.calculateBoundingBox()
# print ground.facenormals, ground.facecenters
# print ground.boundingBox
