################################################################################

coolfluid_add_test( UTEST utest-blockmesh-2d
                    CPP utest-blockmesh-2d.cpp
                    LIBS coolfluid_mesh coolfluid_mesh_blockmesh)

################################################################################

coolfluid_add_test( UTEST  utest-blockmesh-2d-mpi
                    CPP    utest-blockmesh-2d-mpi.cpp
                    LIBS   coolfluid_mesh coolfluid_mesh_blockmesh
                    MPI    8 )

################################################################################

coolfluid_add_test( UTEST utest-blockmesh-3d-mpi
                    CPP utest-blockmesh-3d-mpi.cpp
                    LIBS coolfluid_mesh coolfluid_mesh_blockmesh coolfluid_mesh_generation coolfluid_testing
                    ARGUMENTS 12 10 10
                    MPI 4 )

################################################################################

coolfluid_add_test(UTEST utest-blockmesh-channelgenerator
                   PYTHON utest-blockmesh-channelgenerator.py
                   MPI 4)

################################################################################

coolfluid_add_test(ATEST atest-blockmesh-backstep
                   PYTHON atest-blockmesh-backstep.py
                   MPI 16)

################################################################################

coolfluid_add_test(ATEST atest-blockmesh-multiregion
                   PYTHON atest-blockmesh-multiregion.py)

################################################################################

coolfluid_add_test(ATEST atest-blockmesh-backstep2d-zoltan
                   PYTHON atest-blockmesh-backstep2d-zoltan.py
                   MPI 8)

################################################################################

coolfluid_add_test(ATEST atest-blockmesh-nurbs-parabolic-surface
                   PYTHON atest-blockmesh-nurbs-parabolic-surface.py)

################################################################################

coolfluid_add_test(ATEST atest-blockmesh-nurbs-circle
                   PYTHON atest-blockmesh-nurbs-circle.py)

################################################################################

coolfluid_add_test(ATEST atest-blockmesh-nurbs-curved-2D-surface
                   PYTHON atest-blockmesh-nurbs-curved-2D-surface.py)
