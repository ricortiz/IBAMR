// Filename: StaggeredStokesPhysicalBoundaryHelper.h
// Created on 28 Aug 2012 by Boyce Griffith
//
// Copyright (c) 2002-2010, Boyce Griffith
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
//    * Redistributions of source code must retain the above copyright notice,
//      this list of conditions and the following disclaimer.
//
//    * Redistributions in binary form must reproduce the above copyright
//      notice, this list of conditions and the following disclaimer in the
//      documentation and/or other materials provided with the distribution.
//
//    * Neither the name of New York University nor the names of its
//      contributors may be used to endorse or promote products derived from
//      this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.

#ifndef included_StaggeredStokesPhysicalBoundaryHelper
#define included_StaggeredStokesPhysicalBoundaryHelper

/////////////////////////////// INCLUDES /////////////////////////////////////

// IBAMR INCLUDES
#include <ibamr/StokesSpecifications.h>

// IBTK INCLUDES
#include <ibtk/StaggeredPhysicalBoundaryHelper.h>

// SAMRAI INCLUDES
#include <CellData.h>

/////////////////////////////// CLASS DEFINITION /////////////////////////////

namespace IBAMR
{
/*!
 * \brief Class StaggeredStokesPhysicalBoundaryHelper provides helper functions
 * to enforce physical boundary conditions for a staggered grid discretization
 * of the incompressible (Navier-)Stokes equations.
 */
class StaggeredStokesPhysicalBoundaryHelper
    : public IBTK::StaggeredPhysicalBoundaryHelper
{
public:
    /*!
     * \brief Default constructor.
     */
    StaggeredStokesPhysicalBoundaryHelper();

    /*!
     * \brief Destructor.
     */
    ~StaggeredStokesPhysicalBoundaryHelper();

    /*!
     * \brief At open boundaries, set normal velocity ghost cell values to
     * enforce the discrete divergence-free condition in the ghost cell abutting
     * the physical boundary.
     */
    void
    enforceDivergenceFreeConditionAtBoundary(
        int u_data_idx,
        int coarsest_ln=-1,
        int finest_ln=-1) const;

    /*!
     * \brief At open boundaries, set normal velocity ghost cell values to
     * enforce the discrete divergence-free condition in the ghost cell abutting
     * the physical boundary.
     */
    void
    enforceDivergenceFreeConditionAtBoundary(
        SAMRAI::tbox::Pointer<SAMRAI::pdat::SideData<NDIM,double> > u_data,
        SAMRAI::tbox::Pointer<SAMRAI::hier::Patch<NDIM> > patch) const;

    /*!
     * \brief At open boundaries, set pressure ghost cell values to
     * enforce normal traction boundary condition at the boundary.
     */
    void
    enforceNormalTractionBoundaryConditions(
        int p_data_idx,
        int u_new_data_idx,
        bool homogeneous_bcs,
        int coarsest_ln=-1,
        int finest_ln=-1) const;

    /*!
     * \brief At open boundaries, set pressure ghost cell values to
     * enforce normal traction boundary condition at the boundary.
     */
    void
    enforceNormalTractionBoundaryConditions(
        SAMRAI::tbox::Pointer<SAMRAI::pdat::CellData<NDIM,double> > p_data,
        SAMRAI::tbox::Pointer<SAMRAI::pdat::SideData<NDIM,double> > u_new_data,
        bool homogeneous_bcs,
        SAMRAI::tbox::Pointer<SAMRAI::hier::Patch<NDIM> > patch) const;

    /*!
     * Set the problem coefficients to use when computing normal traction
     * boundary conditions.
     */
    void
    setStokesSpecifications(
        const StokesSpecifications* problem_coefs);

    /*!
     * Set the patch data index to use for u(n) when computing normal traction
     * boundary conditions
     */
    void
    setCurrentVelocityDataIndex(
        int u_current_data_idx);

    /*!
     * \brief Cache boundary coefficient data.
     */
    void
    cacheBcCoefData(
        int u_data_idx,
        SAMRAI::tbox::Pointer<SAMRAI::hier::Variable<NDIM> > u_var,
        std::vector<SAMRAI::solv::RobinBcCoefStrategy<NDIM>*>& u_bc_coefs,
        double velocity_bc_fill_time,
        double traction_bc_fill_time,
        const SAMRAI::hier::IntVector<NDIM>& gcw_to_fill,
        SAMRAI::tbox::Pointer<SAMRAI::hier::PatchHierarchy<NDIM> > hierarchy);

    /*!
     * \brief Clear cached boundary coefficient data.
     */
    virtual void
    clearBcCoefData();

protected:
    // Cached hierarchy-related information.
    std::vector<std::map<int,std::vector<SAMRAI::tbox::Pointer<SAMRAI::pdat::ArrayData<NDIM,bool  > > > > > d_neumann_bdry_locs;
    std::vector<std::map<int,std::vector<SAMRAI::tbox::Pointer<SAMRAI::pdat::ArrayData<NDIM,double> > > > > d_neumann_bdry_vals;

    // Problem specification objects.
    const StokesSpecifications* d_problem_coefs;

    // Current velocity patch data index.
    int d_u_current_data_idx;

private:
    /*!
     * \brief Copy constructor.
     *
     * \note This constructor is not implemented and should not be used.
     *
     * \param from The value to copy to this object.
     */
    StaggeredStokesPhysicalBoundaryHelper(
        const StaggeredStokesPhysicalBoundaryHelper& from);

    /*!
     * \brief Assignment operator.
     *
     * \note This operator is not implemented and should not be used.
     *
     * \param that The value to assign to this object.
     *
     * \return A reference to this object.
     */
    StaggeredStokesPhysicalBoundaryHelper&
    operator=(
        const StaggeredStokesPhysicalBoundaryHelper& that);
};
}// namespace IBAMR

/////////////////////////////// INLINE ///////////////////////////////////////

//#include <ibamr/StaggeredStokesPhysicalBoundaryHelper.I>

//////////////////////////////////////////////////////////////////////////////

#endif //#ifndef included_StaggeredStokesPhysicalBoundaryHelper