// Copyright (C) 2010-2013 von Karman Institute for Fluid Dynamics, Belgium
//
// This software is distributed under the terms of the
// GNU Lesser General Public License version 3 (LGPLv3).
// See doc/lgpl.txt and doc/gpl.txt for the license text.

#include "common/Builder.hpp"
#include "common/Core.hpp"
#include "common/Exception.hpp"
#include "common/EventHandler.hpp"
#include "common/Log.hpp"
#include "common/OptionComponent.hpp"
#include "common/OptionList.hpp"
#include "common/OptionT.hpp"
#include "common/Table.hpp"
#include "common/XML/SignalFrame.hpp"
#include "common/XML/SignalOptions.hpp"
#include "common/Timer.hpp"

#include "common/PE/Comm.hpp"
#include "mesh/BlockMesh/NurbsEvaluation.hpp"
#include <vector>

namespace cf3 {
namespace mesh {
namespace BlockMesh {

using namespace cf3::common;
using namespace cf3::mesh;

common::ComponentBuilder < NurbsEvaluation, Component, LibBlockMesh > NurbsEvaluation_Builder;

NurbsEvaluation::NurbsEvaluation(const std::string& name) : Component(name)
{
}

NurbsEvaluation::~NurbsEvaluation()
{
}



//------------------------------------------------------------	CoxDeBoor()
//
Real NurbsEvaluation::CoxDeBoor(std::vector<Real> param, unsigned int i, int dimension, std::vector<unsigned int>order) {
	if(order[dimension]==1)
	{
		if( Knots[dimension][i] <= param[dimension] && param[dimension] <= Knots[dimension][i+1] ) {
			return 1.0f;
		}
		return 0.0f;
	}
	Real Den1 = Knots[dimension][i+order[dimension]-1] - Knots[dimension][i];
	Real Den2 = Knots[dimension][i+order[dimension]] - Knots[dimension][i+1];
	Real Eq1=0,Eq2=0;
    std::vector<unsigned int> new_order;
    for (int k=0; k<=dimension; k++)
        new_order.push_back(order[dimension]-1);
	if(Den1>0) {
		Eq1 = (param[dimension]-Knots[dimension][i]) / Den1 * CoxDeBoor(param,i,dimension,new_order);
	}
	if(Den2>0) {
		Eq2 = (Knots[dimension][i+order[dimension]]-param[dimension]) / Den2 * CoxDeBoor(param,i+1,dimension,new_order);
	}
	return Eq1+Eq2;
}

Real NurbsEvaluation::BSplineBasis(std::vector<Real> param, std::vector<unsigned int> index) {
    Real Den = 0;
    for (unsigned int i=0; i < num_points[0]; i++)
        for (unsigned int j=0; j < num_points[1]; j++)
            for (unsigned int k=0; k < num_points[2]; k++) 
                Den += (Weights[k][j][i] *  
                    CoxDeBoor(param, i, 0, order) * 
                    CoxDeBoor(param, j, 1, order) * 
                    CoxDeBoor(param, k, 2, order)); 
    if (Den == 0)
        return 0.0f;
    return (Weights[index[2]][index[1]][index[0]] * 
        CoxDeBoor(param, index[0], 0, order) * 
        CoxDeBoor(param, index[1], 1, order) * 
        CoxDeBoor(param, index[2], 2, order) / Den);
}

void NurbsEvaluation::GetOutpoint(std::vector<Real> param, Real OutPoint[]){

	// sum the effect of all CV's on the curve at this point to 
	// get the evaluated curve point
	// 
	for(unsigned int i=0; i < num_points[0]; i++) {
        for (unsigned int j=0; j < num_points[1]; j++) {
            for (unsigned int k=0; k < num_points[2]; k++) {
                std::vector<unsigned int> index;
                index.push_back(i);
                index.push_back(j);
                index.push_back(k);
                // calculate the effect of this point on the curve
                Real Val = BSplineBasis(param,index);

                // sum effect of CV on this part of the curve
                OutPoint[0] += Val * Points[k][j][i][0];
                OutPoint[1] += Val * Points[k][j][i][1];
                OutPoint[2] += Val * Points[k][j][i][2];
            }
        }
	}
}

void NurbsEvaluation::AddPoint(cf3::common::Table<Real>::ConstRow  point, const Real weight, const int x, const int y, const int z) {
    if (Points.size() <= x) {
		Points.resize(x+1);
		Weights.resize(x+1);
	}
    if (Points[x].size() <= y) {
		Points[x].resize(y+1);
		Weights[x].resize(y+1);
	}
    if (Points[y].size() <= z) {
		Points[y].resize(z+1);
		Weights[y].resize(z+1);
	}
	std::vector<Real> p;
	p.push_back(point[0]);
	p.push_back(point[1]);
	p.push_back(((point.size()==2)?0:point[2]));
    Points[x][y][z] = p;
    Weights[x][y][z] = weight;
}

void NurbsEvaluation::AddKnots(std::vector<Real> k) {
    Knots.push_back(k);
}
void NurbsEvaluation::InitNurbs(){
	num_points.push_back(Points[0][0].size());
    num_points.push_back(Points[0].size());
    num_points.push_back(Points.size());
    
    //degree +1; m = p + n + 1 where n is the number of control points and p 
    //is the degree of the desired blending function m is length of knot std::vector.
    order.push_back(Knots[0].size() - num_points[0]); 
    order.push_back(Knots[1].size() - num_points[1]);
    order.push_back(Knots[2].size() - num_points[2]);
}

void NurbsEvaluation::SetDimension(int dimension) {
    if (dimension == 2) 
        dim = 2;
    else 
        dim =3;
}

bool NurbsEvaluation::validate() {
	Knots.resize(3);
	//check that each row and column has same amount of points
	for (int i=0; i < Points.size(); i++) {
		if (Points[i].size() != Points[0].size())
			return false;
		for (int j=0; j < Points[i].size(); j++)
			if (Points[i][j].size() != Points[i][0].size())
				return false;
	}
	//
	
	//make knots vectors for constan parameters
	std::vector<Real> normalized_knot_vector;
	normalized_knot_vector.push_back(0);
	normalized_knot_vector.push_back(0);
	normalized_knot_vector.push_back(1);
	
	
	if (Points.size() == 1)
		Knots[2] = normalized_knot_vector;
	if (Points[0].size() == 1)
		Knots[1] = normalized_knot_vector;
	//check knot vectors
	for (int i=0;  Knots.size(); i++) {
		if (Knots[i].size() < 3)
			return false;
		for (int j=1; j < Knots[i].size(); j++)
			if (Knots[i][j] > Knots[i][j-1])
				return false;
	} 
}

} // BlockMesh
} // mesh
} // cf3
