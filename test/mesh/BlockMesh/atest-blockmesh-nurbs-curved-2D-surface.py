import math
import sys
import string
import coolfluid as cf
import pylab as pl
from matplotlib import pyplot
from mpl_toolkits.mplot3d import Axes3D

# Some shortcuts
root = cf.Core.root()
env = cf.Core.environment()

# Global configuration
env.options().set('log_level', 4)

nurbs = root.create_component('model', 'cf3.mesh.BlockMesh.BlockArrays')


points = nurbs.create_points(dimensions = 2, nb_points = 16)

points[0] =  [ -20,  15 ];
points[1] =  [ -15,   5 ];
points[2] =  [ -15,  -5 ];
points[3] =  [ -20, -15 ];
points[4] =  [  -5,  15 ];
points[5] =  [  -5,   5 ];
points[6] =  [  -5,  -5 ];
points[7] =  [  -5, -15 ];
points[8] =  [   5,  15 ];
points[9] =  [   5,   5 ];
points[10] = [   5,  -5 ];
points[11] = [   5, -15 ];
points[12] = [  15,  15 ];
points[13] = [  20,   5 ];
points[14] = [  20,  -5 ];
points[15] = [  15, -15 ];

#index points into NURBS matrix and add weights
#first row
nurbs.index_points2D(index = 0, weight = 1, x = 0, y = 0)
nurbs.index_points2D(index = 1, weight = 1, x = 1, y = 0)
nurbs.index_points2D(index = 2, weight = 1, x = 2, y = 0)
nurbs.index_points2D(index = 3, weight = 1, x = 3, y = 0)
#second row
nurbs.index_points2D(index = 4, weight = 1, x = 0, y = 1)
nurbs.index_points2D(index = 5, weight = 1, x = 1, y = 1)
nurbs.index_points2D(index = 6, weight = 1, x = 2, y = 1)
nurbs.index_points2D(index = 7, weight = 1, x = 3, y = 1)
#third row
nurbs.index_points2D(index = 8, weight = 1, x = 0, y = 2)
nurbs.index_points2D(index = 9, weight = 1, x = 1, y = 2)
nurbs.index_points2D(index = 10, weight = 1, x = 2, y = 2)
nurbs.index_points2D(index = 11, weight = 1, x = 3, y = 2)
#forth row
nurbs.index_points2D(index = 12, weight = 1, x = 0, y = 3)
nurbs.index_points2D(index = 13, weight = 1, x = 1, y = 3)
nurbs.index_points2D(index = 14, weight = 1, x = 2, y = 3)
nurbs.index_points2D(index = 15, weight = 1, x = 3, y = 3)

#knot vectors for parabolic surface
nurbs.add_knot_vector(knot = [0,0,0,0,1,1,1,1])
nurbs.add_knot_vector(knot = [0,0,0,1,2,2,2])

#doesnt have to move in third direction
#initialize nurbs
nurbs.init_nurbs(u = 0.02, v = 0.05, w = 0)
# Generate a channel mesh
mesh = root.create_component('Mesh', 'cf3.mesh.Mesh')

nurbs.create_mesh(mesh.uri())

mesh.write_mesh(file=cf.URI('nurbs_curved_surface.pvtu'))
