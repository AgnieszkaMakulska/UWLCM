#pragma once

#include <libmpdata++/solvers/mpdata_rhs_vip_prs.hpp>
#include <libmpdata++/output/hdf5_xdmf.hpp>

using namespace libmpdataxx; // TODO: get rid of it?

template <class ct_params_t>
class slvr_common : public 
  output::hdf5_xdmf<
    solvers::mpdata_rhs_vip_prs<ct_params_t>
  >
{
  using parent_t = output::hdf5_xdmf<solvers::mpdata_rhs_vip_prs<ct_params_t>>;

  public:
  using real_t = typename ct_params_t::real_t;

  protected:

  real_t dx, dz; // 0->dx, 1->dy ! TODO
  int spinup; // number of timesteps

  // relaxation stuff
  bool relax_th_rv;

  // surface precip stuff
  real_t prec_vol;
  std::ofstream f_prec;
  
  // spinup stuff
  virtual bool get_rain() = 0;
  virtual void set_rain(bool) = 0;

  void hook_ante_loop(int nt) 
  {
    if (spinup > 0)
    {
      blitz::secondIndex k;
      set_rain(false);
    }

    parent_t::hook_ante_loop(nt); 

    // open file for output of precitpitation volume
    f_prec.open(this->outdir+"/prec_vol.dat");
    prec_vol = 0.;
  }

  void hook_ante_step()
  {
    if (spinup != 0 && spinup == this->timestep)
    {
      // turn autoconversion on only after spinup (if spinup was specified)
      set_rain(true);
    }
    parent_t::hook_ante_step(); 
  }

  void hook_post_step()
  {
    parent_t::hook_post_step(); 

    // recording total precipitation volume through the lower boundary
    if(this->rank==0)
    {
      f_prec << this->timestep << " "  << prec_vol << "\n";
      prec_vol = 0.;
    }
  }

  public:

  struct rt_params_t : parent_t::rt_params_t 
  { 
    typename ct_params_t::real_t dx = 0, dz = 0;
    int spinup = 0; // number of timesteps during which autoconversion is to be turned off
    bool relax_th_rv = true;
    typename parent_t::arr_t *th_e, *rv_e, *th_ref, *rhod, *w_LS, *hgt_fctr_sclr, *hgt_fctr_vctr;
  };

  // ctor
  slvr_common( 
    typename parent_t::ctor_args_t args, 
    const rt_params_t &p
  ) : 
    parent_t(args, p),
    dx(p.dx),
    dz(p.dz),
    spinup(p.spinup),
    relax_th_rv(p.relax_th_rv)
  {
    assert(dx != 0);
    assert(dz != 0);
  }  
};
