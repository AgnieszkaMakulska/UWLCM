#pragma once

#include <iostream>

#include <libcloudph++/common/hydrostatic.hpp>
#include <libcloudph++/common/theta_std.hpp>
#include <libcloudph++/common/theta_dry.hpp>

#include <boost/math/special_functions/sin_pi.hpp>
#include <boost/math/special_functions/cos_pi.hpp>


// simulation parameters container
// TODO: write them to rt_params directly in main()
struct user_params_t
{
  int nt, outfreq, spinup, rng_seed;
  setup::real_t dt, z_rlx_sclr;
  std::string outdir;
  bool th_src, rv_src, uv_src, w_src;
};

namespace setup 
{
  namespace hydrostatic = libcloudphxx::common::hydrostatic;
  namespace theta_std = libcloudphxx::common::theta_std;
  namespace theta_dry = libcloudphxx::common::theta_dry;

  const real_t D =  3.75e-6; // large-scale wind horizontal divergence [1/s]

  // container for constants that appear in forcings, some are not needed in all cases, etc...
  // TODO: make forcing functions part of case class
  struct ForceParameters_t
  {
    real_t q_i, heating_kappa, F_0, F_1, rho_i, D, F_sens, F_lat, u_fric;
  };

  template<class concurr_t>
  class CasesCommon
  {
    public:
    ForceParameters_t ForceParameters;

    //th, rv and surface fluxes relaxation time and height
    const quantity<si::time, real_t> tau_rlx = 300 * si::seconds;

    virtual void setopts(typename concurr_t::solver_t::rt_params_t &params, int nx, int nz, const user_params_t &user_params) {assert(false);};
    virtual void setopts(typename concurr_t::solver_t::rt_params_t &params, int nx, int ny, int nz, const user_params_t &user_params) {assert(false);};
    virtual void intcond(concurr_t &solver, arr_1D_t &rhod, arr_1D_t &th_e, arr_1D_t &rv_e, int rng_seed) =0;
    virtual void env_prof(arr_1D_t &th_e, arr_1D_t &rv_e, arr_1D_t &th_ref, arr_1D_t &pre_ref, arr_1D_t &rhod, arr_1D_t &w_LS, arr_1D_t &hgt_fctr_vctr, arr_1D_t &hgt_fctr_sclr, int nz, const user_params_t &user_params) =0;

    // ctor
    // TODO: these are DYCOMS definitions, move them there
    CasesCommon()
    {
      ForceParameters.heating_kappa = 85; // m^2/kg
      ForceParameters.F_0 = 70; // w/m^2
      ForceParameters.F_1 = 22; // w/m^2
      ForceParameters.q_i = 8e-3; // kg/kg
      ForceParameters.D = D; // large-scale wind horizontal divergence [1/s]
      ForceParameters.rho_i = 1.12; // kg/m^3
      ForceParameters.F_sens = 16; //W/m^2, sensible heat flux
      ForceParameters.F_lat = 93; //W/m^2, latent heat flux
      ForceParameters.u_fric = 0.25; // m/s; friction velocity
    }

    protected:
  
    // function enforcing cyclic values in horizontal directions
    // 2D version
    template<class arr_t>
    void make_cyclic(arr_t arr,
      typename std::enable_if<arr_t::rank_ == 2>::type* = 0)
    { arr(arr.extent(0) - 1, blitz::Range::all()) = arr(0, blitz::Range::all()); }
  
    // 3D version
    template<class arr_t>
    void make_cyclic(arr_t arr,
      typename std::enable_if<arr_t::rank_ == 3>::type* = 0)
    { 
      arr(arr.extent(0) - 1, blitz::Range::all(), blitz::Range::all()) = 
        arr(0, blitz::Range::all(), blitz::Range::all()); 
      arr(blitz::Range::all(), arr.extent(1) - 1, blitz::Range::all()) = 
        arr(blitz::Range::all(), 0, blitz::Range::all());
    }
  };
};
