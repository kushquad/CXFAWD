RETURN_DEGREES = False

from operator import add, sub, mul
import math

def magnitude2(v):
	return reduce(add, map(lambda x:x*x, v))

def magnitude(v):
	return math.sqrt(magnitude2(v))

def dotproduct(v1, v2):
	return reduce(add, map(mul, v1, v2))

def cosangleBetween(v1, v2):
	scalarproduct = dotproduct(v1, v2)
	magproduct = math.sqrt(magnitude2(v1)*magnitude2(v2))
	if not magproduct==0:
		return scalarproduct/magproduct
	return -2

def angleBetween(v1, v2):
	res = math.acos(cosangleBetween)
	if RETURN_DEGREES:
		return (180.0/math.pi)*res
	return res

# Note that our coordinate axes are XZ-Y type,
# not the conventional XY-Z in vector mathematics
def crossproduct(vec1, vec2):
	return [vec1[1]*vec2[2]-vec1[2]*vec2[1],
			vec2[0]*vec1[2]-vec1[0]*vec2[2],
			vec1[0]*vec2[1]-vec1[1]*vec2[0]]

def normalize(v):
	try:
		mag = magnitude(v)
		return [vcomp/mag for vcomp in v]
	except:
		return [0,0,0]

def insideFace(p, face):
	p1,p2,p3 = face
	ab = map(sub, p2, p1)
	ac = map(sub, p3, p1)
	pa = map(sub, p1, p)
	pb = map(sub, p2, p)
	pc = map(sub, p3, p)
	abc = magnitude(crossproduct(ab,ac))/2.0
	alpha = magnitude(crossproduct(pb,pc))/(2.0*abc)
	beta = magnitude(crossproduct(pc,pa))/(2.0*abc)
	gamma = 1-alpha-beta
	if (0<=alpha<=1) and (0<=beta<=1) and (0<=gamma<=1):
		return True
	return False

#print insideFace([0,-5,1],[[0,-5,1],[0,5,1],[0,0,-5]])