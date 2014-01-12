import math
import sys
import string
import coolfluid as cf

# Some shortcuts
root = cf.Core.root()
env = cf.Core.environment()

# Global configuration
env.options().set('log_level', 4)

nurbs = root.create_component('model', 'cf3.mesh.BlockMesh.BlockArrays')

#doesnt have to move in second and third direction
nurbs.init_nurbs(u = 0.01, v = 0.01, w = 0) 

points = nurbs.create_points(dimensions = 3, nb_points = 16)

points[0] =  [ -15, 0, 15 ];
points[1] =  [ -15, 5, 5 ];
points[2] =  [ -15,  5, -5 ];
points[3] =  [ -15,  0, -15 ];
points[4] =  [ -5, 5, 15 ];
points[5] =  [ -5, 10, 5 ];
points[6] =  [  -5,  10, -5 ];
points[7] =  [ -5,  5, -15 ];
points[8] =  [ 5, 5, 15 ];
points[9] =  [ 5, 10, 5 ];
points[10] = [ 5,  10, -5 ];
points[11] = [ 15,  5, -15 ];
points[12] = [ 15, 0, 15 ];
points[13] = [ 15, 5, 5 ];
points[14] = [ 15, 5, -5 ];
points[15] = [ 15, 0, -15 ];

block_nodes = nurbs.create_blocks(1)
block_nodes[0] = [0, 1, 2, 3, 4, 5, 6, 7, 9, 10, 11, 12, 13, 14, 15]

#index points into NURBS matrix and add weights
#first row
nurbs.index_points3D(index = 0, weight = 1, x = 0, y = 0, z = 0)
nurbs.index_points3D(index = 1, weight = 1, x = 1, y = 0, z = 0)
nurbs.index_points3D(index = 2, weight = 1, x = 2, y = 0, z = 0)
nurbs.index_points3D(index = 3, weight = 1, x = 3, y = 0, z = 0)
#second row
nurbs.index_points3D(index = 4, weight = 1, x = 0, y = 1, z = 0)
nurbs.index_points3D(index = 5, weight = 1, x = 1, y = 1, z = 0)
nurbs.index_points3D(index = 6, weight = 1, x = 2, y = 1, z = 0)
nurbs.index_points3D(index = 7, weight = 1, x = 3, y = 1, z = 0)
#third row
nurbs.index_points3D(index = 8, weight = 1, x = 0, y = 2, z = 0)
nurbs.index_points3D(index = 9, weight = 1, x = 1, y = 2, z = 0)
nurbs.index_points3D(index = 10, weight = 1, x = 2, y = 2, z = 0)
nurbs.index_points3D(index = 11, weight = 1, x = 3, y = 2, z = 0)
#forth row
nurbs.index_points3D(index = 12, weight = 1, x = 0, y = 3, z = 0)
nurbs.index_points3D(index = 13, weight = 1, x = 1, y = 3, z = 0)
nurbs.index_points3D(index = 14, weight = 1, x = 2, y = 3, z = 0)
nurbs.index_points3D(index = 15, weight = 1, x = 3, y = 3, z = 0)

#knot vectors for parabolic surface
nurbs.add_knot_vector(knot = [0,0,0,0,1,1,1,1])
nurbs.add_knot_vector(knot = [0,0,0,1,2,2,2])

# Generate a channel mesh
mesh = root.create_component('Mesh', 'cf3.mesh.Mesh')

nurbs.create_mesh(mesh.uri())

mesh.write_mesh(file=cf.URI('nurbs_circle.msh'))
