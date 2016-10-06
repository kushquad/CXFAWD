GRAVITY = -9.8
LAMINAR_DRAG_COEFFICIENT = 0.5
TURBULENT_DRAG_COEFFICIENT = 0.001
AIR_DENSITY = 1000

# We define only two forces for now
#    - Air Friction (laminar and turbulent)
#    - Gravity
def gravity_kernel(solver, target, time):
	return [0,target.mass*GRAVITY,0]

def laminar_air_drag_kernel(solver, target, time):
	b = LAMINAR_DRAG_COEFFICIENT*target.radius
	return [-b*velcomp for velcomp in target.vel]

def turbulent_air_drag_kernel(solver, target, time):
	return target.radius**2*magnitude2(target.vel)*TURBULENT_DRAG_COEFFICIENT*AIR_DENSITY