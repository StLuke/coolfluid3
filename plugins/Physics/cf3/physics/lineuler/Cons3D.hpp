// Copyright (C) 2010-2013 von Karman Institute for Fluid Dynamics, Belgium
//
// This software is distributed under the terms of the
// GNU Lesser General Public License version 3 (LGPLv3).
// See doc/lgpl.txt and doc/gpl.txt for the license text.

#include <iostream>

#ifndef cf3_physics_LinEuler_Cons3D_hpp
#define cf3_physics_LinEuler_Cons3D_hpp

#include "cf3/common/BasicExceptions.hpp"
#include "cf3/common/StringConversion.hpp"
#include "cf3/math/Defs.hpp"

#include "cf3/physics/Variables.hpp"

#include "cf3/physics/lineuler/LinEuler3D.hpp"

namespace cf3 {
namespace physics {
namespace LinEuler {

///////////////////////////////////////////////////////////////////////////////////////

/// Conservative variables for Linearized Euler 3D
/// @author Tiago Quintino
/// @author Willem Deconinck
class LinEuler_API Cons3D : public VariablesT<Cons3D> {

public: //typedefs

  typedef LinEuler3D     MODEL;

  enum { Rho = 0, Rho0U = 1, Rho0V = 2, Rho0W = 3, P = 4 };




public: // functions

  /// constructor
  /// @param name of the component
  Cons3D ( const std::string& name );

  /// virtual destructor
  virtual ~Cons3D();

  /// Get the class name
  static std::string type_name () { return "Cons3D"; }

  /// compute physical properties
  template < typename CV, typename SV, typename GM >
  static void compute_properties ( const CV& coord,
                                   const SV& sol,
                                   const GM& grad_vars,
                                   MODEL::Properties& p )
  {
    /// CHECKED
    p.coords    = coord;       // cache the coordiantes locally
    p.vars      = sol;         // cache the variables locally
    p.grad_vars = grad_vars;   // cache the gradient of variables locally

    p.rho   = sol[Rho];                 // acoustic density
    p.rho0u = sol[Rho0U];               // rho0.u
    p.rho0v = sol[Rho0V];               // rho0.v
    p.rho0w = sol[Rho0W];               // rho0.w
    p.p     = sol[P];                   // acoustic pressure

    p.u = p.rho0u / p.rho0;                   // velocity along XX, rho0.u / rho0
    p.v = p.rho0v / p.rho0;                   // velocity along YY, rho0.v / rho0
    p.w = p.rho0w / p.rho0;                   // velocity along ZZ, rho0.w / rho0

    p.H = p.p/( p.rho+p.rho0 ) + 0.5*(p.u*p.u + p.v*p.v + p.w*p.w);

    if (p.P0 + p.p <= 0)
    {
      std::cout << "rho   : " << p.rho   << std::endl;
      std::cout << "rho0u : " << p.rho0u << std::endl;
      std::cout << "rho0v : " << p.rho0v << std::endl;
      std::cout << "rho0w : " << p.rho0w << std::endl;
      std::cout << "P     : " << p.p     << std::endl;
      std::cout << "u     : " << p.u     << std::endl;
      std::cout << "v     : " << p.v     << std::endl;
      std::cout << "w     : " << p.w     << std::endl;
      std::cout << "H     : " << p.H     << std::endl;

      throw common::FailedToConverge( FromHere(), "Pressure is negative at coordinates ["
                               + common::to_str(coord[XX]) + ","
                               + common::to_str(coord[YY]) + ","
                               + common::to_str(coord[ZZ])
                               + "]");
    }

  }

  template < typename VectorT >
  static void compute_variables ( const MODEL::Properties& p, VectorT& vars )
  {
    /// CHECKED
    vars[Rho]   = p.rho;
    vars[Rho0U] = p.rho0u;
    vars[Rho0V] = p.rho0v;
    vars[Rho0W] = p.rho0w;
    vars[P]     = p.p;
  }

  /// compute the physical flux
  template < typename FM >
  static void flux( const MODEL::Properties& p,
                    FM& flux)
  {
    /// CHECKED
    flux(0,XX) = p.u0[XX] * p.rho   +  p.rho0u;
    flux(1,XX) = p.u0[XX] * p.rho0u +  p.p;
    flux(2,XX) = p.u0[XX] * p.rho0v ;
    flux(3,XX) = p.u0[XX] * p.rho0w ;
    flux(4,XX) = p.u0[XX] * p.p     + p.rho0u * p.c * p.c;

    flux(0,YY) = p.u0[YY] * p.rho   + p.rho0v;
    flux(1,YY) = p.u0[YY] * p.rho0u ;
    flux(2,YY) = p.u0[YY] * p.rho0v + p.p;
    flux(3,YY) = p.u0[YY] * p.rho0w ;
    flux(4,YY) = p.u0[YY] * p.p     + p.rho0v * p.c * p.c;

    flux(0,ZZ) = p.u0[ZZ] * p.rho   + p.rho0w;
    flux(1,ZZ) = p.u0[ZZ] * p.rho0u ;
    flux(2,ZZ) = p.u0[ZZ] * p.rho0v ;
    flux(3,ZZ) = p.u0[ZZ] * p.rho0w + p.p;
    flux(4,ZZ) = p.u0[ZZ] * p.p     + p.rho0w * p.c * p.c;


  }

  /// compute the physical flux
  template < typename FM , typename GV>
  static void flux( const MODEL::Properties& p,
                    const GV& direction,
                    FM& flux)
  {
    /// CHECKED
    const Real u0n = p.u0[XX] * direction[XX] +
                     p.u0[YY] * direction[YY] +
                     p.u0[ZZ] * direction[ZZ] ;

    const Real un = p.u * direction[XX] +
                    p.v * direction[YY] +
                    p.w * direction[ZZ] ;

    flux[0] = u0n*p.rho   + p.rho0*un;
    flux[1] = u0n*p.rho0u + p.p*direction[XX];
    flux[2] = u0n*p.rho0v + p.p*direction[YY];
    flux[3] = u0n*p.rho0w + p.p*direction[ZZ];
    flux[4] = u0n*p.p     + p.rho0*un*p.c*p.c;
  }

  /// compute the eigen values of the flux jacobians
  template < typename GV, typename EV >
  static void flux_jacobian_eigen_values(const MODEL::Properties& p,
                                         const GV& direction,
                                         EV& Dv)
  {
    /// CHECKED
    const Real u0n = p.u0[XX] * direction[XX] +
                     p.u0[YY] * direction[YY] +
                     p.u0[ZZ] * direction[ZZ] ;

    Dv[0] = u0n;
    Dv[1] = u0n;
    Dv[2] = u0n;
    Dv[3] = u0n + p.c;
    Dv[4] = u0n - p.c;
  }

  /// compute the eigen values of the flux jacobians
  template < typename GV, typename EV, typename OP >
  static void flux_jacobian_eigen_values(const MODEL::Properties& p,
                                         const GV& direction,
                                         EV& Dv,
                                         OP& op )

  {
      /// CHECKED
      const Real u0n = p.u0[XX] * direction[XX] +
                       p.u0[YY] * direction[YY] +
                       p.u0[ZZ] * direction[ZZ] ;

      const Real op_u0n = op(u0n);

      Dv[0] = op_u0n;
      Dv[1] = op_u0n;
      Dv[2] = op_u0n;
      Dv[3] = op_u0n + p.c;
      Dv[4] = op_u0n - p.c;
  }

  /// decompose the eigen structure of the flux jacobians projected on the gradients
  template < typename GV, typename EM, typename EV >
  static void flux_jacobian_eigen_structure(const MODEL::Properties& p,
                                            const GV& direction,
                                            EM& Rv,
                                            EM& Lv,
                                            EV& Dv)
  {
      /// CHECKED
      const Real nx = direction[XX];
      const Real ny = direction[YY];
      const Real nz = direction[ZZ];

      // state is not used as Linearized Euler is, well, linear

      const Real u0n = p.u0[XX] * direction[XX] +
                       p.u0[YY] * direction[YY] +
                       p.u0[ZZ] * direction[ZZ] ;

      const Real inv_c2  = p.inv_c*p.inv_c;

      Rv << nx,  ny,  nz,  0.5*p.inv_c,  0.5*p.inv_c,
            0., -nz,  ny,  0.5*nx,      -0.5*nx,
            nz,  0., -nx,  0.5*ny,      -0.5*ny,
           -ny,  nx,  0.,  0.5*nz,      -0.5*nz,
            0.,  0.,  0.,  0.5*p.c,      0.5*p.c;

      Lv << nx,  0.,  nz, -ny, -nx*inv_c2,
            ny, -nz,  0.,  nx, -ny*inv_c2,
            nz,  ny, -nx,  0., -nz*inv_c2,
            0.,  nx,  ny,  nz,  p.inv_c,
            0., -nx, -ny, -nz,  p.inv_c;

//      Rv <<  nx,  ny,  nz, 0.5*p.inv_c,   0.5*p.inv_c,
//             0., -nz,  ny, 0.5*nx,       -0.5*nx,
//             nz,  0., -nx, 0.5*ny,       -0.5*ny,
//            -ny,  nx,  0., 0.5*nz,       -0.5*nz,
//             0.,  0.,  0., 0.5*p.c,       0.5*p.c;

//      Lv <<   nx,   0.,  nz,  -ny,   -nx*inv_c2,
//              ny,  -nz,  0.,   nx,   -ny*inv_c2,
//              nz,   ny, -nx,   0.,   -nz*inv_c2,
//              0.,   nx,  ny,   nz,      p.inv_c,
//              0.,  -nx, -ny,  -nz,      p.inv_c;

      Dv[0] = u0n;
      Dv[1] = u0n;
      Dv[2] = u0n;
      Dv[3] = u0n + p.c;
      Dv[4] = u0n - p.c;
  }

  /// compute the PDE residual
  template < typename JM, typename RV >
  static void residual(const MODEL::Properties& p,
                       JM         flux_jacob[],
                       RV&        res)
  {
    throw common::NotImplemented(FromHere(), "Cons3D::residual()");

//    const Real gamma_minus_3 = p.gamma - 3.;

//    const Real uu = p.u * p.u;
//    const Real uv = p.u * p.v;
//    const Real vv = p.v * p.v;

    JM& A = flux_jacob[XX];

  //    A.setZero(); // assume are zeroed

//    A(0,0) = p.u0[XX];
//    A(0,1) = 1.;
//    A(0,2) = 0.;
//    A(0,3) = 0.;
//    A(1,0) = 0.;
//    A(1,1) = p.u0[XX];
//    A(1,2) = 0.;
//    A(1,3) = 1.;
//    A(2,0) = 0.;
//    A(2,1) = 0.;
//    A(2,2) = p.u0[XX];
//    A(2,3) = 0.;
//    A(3,0) = 0.;
//    A(3,1) = p.c * p.c;
//    A(3,2) = 0.;
//    A(3,3) = p.u0[XX];

    JM& B = flux_jacob[YY];

    //    B.setZero(); // assume are zeroed

//    B(0,0) = p.u0[YY];
//    B(0,1) = 0.;
//    B(0,2) = 1.;
//    B(0,3) = 0.;
//    B(1,0) = 0.;
//    B(1,1) = p.u0[YY];
//    B(1,2) = 0.;
//    B(1,3) = 0.;
//    B(2,0) = 0.;
//    B(2,1) = 0.;
//    B(2,2) = p.u0[YY];
//    B(2,3) = 1.;
//    B(3,0) = 0.;
//    B(3,1) = 0.;
//    B(3,2) = p.c * p.c;
//    B(3,3) = p.u0[YY];

    JM& C = flux_jacob[YY];

    //    C.setZero(); // assume are zeroed

//    C(0,0) = p.u0[YY];
//    C(0,1) = 0.;
//    C(0,2) = 1.;
//    C(0,3) = 0.;
//    C(1,0) = 0.;
//    C(1,1) = p.u0[YY];
//    C(1,2) = 0.;
//    C(1,3) = 0.;
//    C(2,0) = 0.;
//    C(2,1) = 0.;
//    C(2,2) = p.u0[YY];
//    C(2,3) = 1.;
//    C(3,0) = 0.;
//    C(3,1) = 0.;
//    C(3,2) = p.c * p.c;
//    C(3,3) = p.u0[YY];

    res = A * p.grad_vars.col(XX) + B * p.grad_vars.col(YY) + C * p.grad_vars.col(ZZ);

  }

}; // Cons3D

////////////////////////////////////////////////////////////////////////////////////

} // LinEuler
} // physics
} // cf3

#endif // cf3_physics_LinEuler_Cons3D_hpp
