from cache import CacheExport
from solver import Solver
from emitter import Emitter
from force import gravity_kernel, laminar_air_drag_kernel
from object import Object

# Set up cache file to write particle cache data to
cacheobj = CacheExport()
cacheobj.clearCache()
solverobj = Solver()

# Create emitter system
em = Emitter(cacheobj, solverobj)
em.pos = [-0.4,1.22,0.0075]

# Add gravity and air drag forces
em.particleforcekernels.append(gravity_kernel)
em.particleforcekernels.append(laminar_air_drag_kernel)

# Ground plane
ground = Object(solverobj)
ground.vertices = [[-100,0,-100],[100,0,-100],
				   [100,0,100],[-100,0,100]]
ground.faces = [[2,1,0],[2,0,3]]
ground.load()

cube = Object(solverobj, '../assets/cube.obj')
cube.applyTranslation([-2.1,0,0.0075])
cube.load()

sphere = Object(solverobj,'../assets/sphere.obj')
sphere.applyTranslation([-0.5,0,1])
sphere.load()

bunny = Object(solverobj, '../assets/stanford_bunny.obj')
bunny.applyTranslation([-2.7,0,-1.1])
bunny.load()

#cone = Object(solverobj, '../assets/cone_new.obj')
#cone.applyTranslation([-2.2,0,1.6])
#cone.load()

solverobj.run()

