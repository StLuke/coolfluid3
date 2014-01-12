import math
import sys
import string
import coolfluid as cf

# Some shortcuts
root = cf.Core.root()
env = cf.Core.environment()

# Global configuration
env.options().set('log_level', 4)

# simulation parameters

sqrt = 0.70710678118 #sqrt(2)/2
pi = 3.14159265359 #pi

nurbs = root.create_component('model', 'cf3.mesh.BlockMesh.BlockArrays')


nurbs.init_nurbs(u = 0.01, v = 0, w = 0) #doesnt have to move in second and third direction

points = nurbs.create_points(dimensions = 2, nb_points = 9)
points[0] = [  0,  0 ]
points[1] = [  1,  1 ]
points[2] = [  0,  1 ]
points[3] = [ -1,  1 ]
points[4] = [ -1,  0 ]
points[5] = [ -1, -1 ]
points[6] = [  0, -1 ]
points[7] = [  1, -1 ]
points[8] = [  1,  0 ]

#index points into NURBS matrix and add weights
nurbs.index_points2D(index = 0, weight = 1, x = 0, y = 0)
nurbs.index_points2D(index = 1, weight = sqrt, x = 1, y = 0)
nurbs.index_points2D(index = 2, weight = 1, x = 2, y = 0)
nurbs.index_points2D(index = 3, weight = sqrt, x = 3, y = 0)
nurbs.index_points2D(index = 4, weight = 1, x = 4, y = 0)
nurbs.index_points2D(index = 5, weight = sqrt, x = 5, y = 0)
nurbs.index_points2D(index = 6, weight = 1, x = 6, y = 0)
nurbs.index_points2D(index = 7, weight = sqrt, x = 7, y = 0)
nurbs.index_points2D(index = 8, weight = 1, x = 8, y = 0)

#knot vector for circle
nurbs.add_knot_vector(knot = [0,0,0, pi/2, pi/2, pi, pi, 3*pi/2, 3*pi/2, 2*pi, 2*pi, 2*pi]);


# Generate a channel mesh
mesh = root.create_component('Mesh', 'cf3.mesh.Mesh')

nurbs.create_mesh(mesh.uri())

mesh.write_mesh(file=cf.URI('nurbs_circle.msh'))
