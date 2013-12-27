// Copyright (C) 2010-2013 von Karman Institute for Fluid Dynamics, Belgium
//
// This software is distributed under the terms of the
// GNU Lesser General Public License version 3 (LGPLv3).
// See doc/lgpl.txt and doc/gpl.txt for the license text.

#ifndef cf3_mesh_BlockMesh_NurbsEvaluation_hpp
#define cf3_mesh_BlockMesh_NurbsEvaluation_hpp

#include "common/CF.hpp"
#include "common/Component.hpp"
#include <vector>

#include "mesh/BlockMesh/LibBlockMesh.hpp"
namespace cf3 {
namespace mesh {
namespace BlockMesh {


////////////////////////////////////////////////////////////////////////////////

class NurbsEvaluation : public common::Component
{
public:

  NurbsEvaluation(const std::string& name);
  ~NurbsEvaluation();

  static std::string type_name () { return "NurbsEvalution"; }
  
  /// compute the values of each coordinate
  /// @param param - vector of parameters u, v, w
  /// @param OutPoint - coordinetes of the result point
  void GetOutpoint(std::vector<Real> param, Real OutPoint[]);
  /// Add control points in 4D vector
  /// @param Points 4D vector of matrixes
  void AddPoint(cf3::common::Table<Real>::ConstRow  point, const Real weight, const int x, const int y, const int z);
  /// Add weights for each control points (default 1)
  /// @param Weights - 3D matrix of weights
  void AddWeights(std::vector<std::vector<std::vector<Real> > >Weights);
  /// Add knot vectors for each direction
  /// @param Knots - vector of Kont vectors
  void AddKnots(std::vector<Real>Knots);
  /// Initialise the order vector and num_points vector for computation
  void InitNurbs();
  /// Set weather to work in 2D/3D space
  /// @param dimension 2 or 3 (depends on dimension of computation)
  void SetDimension(int dimension);
  /// validate all values of nurbs definition
  
  bool validate();
  
  std::vector<std::vector<Real> >Knots;
  std::vector<std::vector<std::vector<std::vector<Real> > > >Points;
  
private:
  std::vector<unsigned int>order;
  std::vector<std::vector<std::vector<Real> > >Weights;
  std::vector<unsigned int> num_points;
  int dim;
  
  /// Compute the basis function for NURBS by Cox De-boor algorithm
  /// @param param - vector of parameters u, v, w
  /// @param i - index of computed bases
  /// @param dimension - direction of table block curve(0, 1, 2) 
  /// @param order - orders of the curve in each direction
  /// @param Knots - knot vectors for the curve in each direction
  Real CoxDeBoor(std::vector<Real> param, unsigned int i, int dimension, std::vector<unsigned int>order);

  /// Compute the value of the point with cox de-boor algorithms and weights of points
  /// @param param - vector of parameters u, v, w
  /// @param index - index of computed bases
  Real BSplineBasis(std::vector<Real> param, std::vector<unsigned int> index);
 

};

////////////////////////////////////////////////////////////////////////////////

} // BlockMesh
} // mesh
} // cf3

#endif /* cf3_mesh_BlockMesh_NurbsEvaluation_hpp */

