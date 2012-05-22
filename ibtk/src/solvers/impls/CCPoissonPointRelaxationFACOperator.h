// Filename: CCPoissonPointRelaxationFACOperator.h
// Created on 10 Feb 2005 by Boyce Griffith
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

#ifndef included_CCPoissonPointRelaxationFACOperator
#define included_CCPoissonPointRelaxationFACOperator

/////////////////////////////// INCLUDES /////////////////////////////////////

// PETSc INCLUDES
#include <petscmat.h>

// IBTK INCLUDES
#include <ibtk/CCPoissonHypreLevelSolver.h>
#include <ibtk/CCPoissonPETScLevelSolver.h>
#include <ibtk/CartCellRobinPhysBdryOp.h>
#include <ibtk/PoissonFACPreconditionerStrategy.h>

// SAMRAI INCLUDES
#include <LocationIndexRobinBcCoefs.h>

// C++ STDLIB INCLUDES
#include <map>

/////////////////////////////// CLASS DEFINITION /////////////////////////////

namespace IBTK
{
/*!
 * \brief Class CCPoissonPointRelaxationFACOperator is a concrete
 * PoissonFACPreconditionerStrategy for solving elliptic equations of the form
 * \f$ \mbox{$L u$} = \mbox{$(C I + \nabla \cdot D \nabla) u$} = f \f$ using a
 * globally second-order accurate cell-centered finite-volume discretization,
 * with support for Robin and periodic boundary conditions.
 *
 * This class provides operators that are used by class FACPreconditioner to
 * solve scalar Poisson-type equations of the form \f[ (C I + \nabla \cdot D
 * \nabla) u = f \f] using a cell-centered, globally second-order accurate
 * finite-volume discretization, where
 *
 * - \f$ C \f$, \f$ D \f$ and \f$ f \f$ are independent of \f$ u \f$,
 * - \f$ C \f$ is a cell-centered scalar field,
 * - \f$ D \f$ is a side-centered scalar field of diffusion coefficients, and
 * - \f$ f \f$ is a cell-centered scalar function.
 *
 * Robin boundary conditions may be specified at physical boundaries; see class
 * SAMRAI::solv::RobinBcCoefStrategy.
 *
 * By default, the class is configured to solve the Poisson problem \f$
 * -\nabla^2 u = f \f$, subject to homogeneous Dirichlet boundary conditions.
 *
 * Sample parameters for initialization from database (and their default
 * values): \verbatim

 smoother_choice = "additive"                 // see setSmootherChoice()

 prolongation_method = "LINEAR_REFINE"        // see setProlongationMethod()
 restriction_method = "CONSERVATIVE_COARSEN"  // see setRestrictionMethod()

 coarse_solver_choice = "block_jacobi"        // see setCoarsestLevelSolverChoice()
 coarse_solver_tolerance = 1.0e-6             // see setCoarsestLevelSolverTolerance()
 coarse_solver_max_iterations = 10            // see setCoarsestLevelSolverMaxIterations()

 hypre_solver = { ... }                       // SAMRAI::tbox::Database for initializing class CCPoissonHypreLevelSolver

 petsc_solver = { ... }                       // SAMRAI::tbox::Database for initializing class CCPoissonPETScLevelSolver
 \endverbatim
*/
class CCPoissonPointRelaxationFACOperator
    : public PoissonFACPreconditionerStrategy
{
public:
    /*!
     * \brief Constructor.
     */
    CCPoissonPointRelaxationFACOperator(
        const std::string& object_name,
        SAMRAI::tbox::Pointer<SAMRAI::tbox::Database> input_db=NULL);

    /*!
     * \brief Destructor.
     */
    ~CCPoissonPointRelaxationFACOperator();

    /*!
     * \name Functions for specifying the Poisson problem.
     */
    //\{

    /*!
     * \brief Set the SAMRAI::solv::PoissonSpecifications object used to specify
     * the coefficients for the scalar Poisson equation.
     */
    void
    setPoissonSpecifications(
        const SAMRAI::solv::PoissonSpecifications& poisson_spec);

    /*!
     * \brief Set the SAMRAI::solv::RobinBcCoefStrategy object used to specify
     * physical boundary conditions.
     *
     * \note \a bc_coef may be NULL.  In this case, homogeneous Dirichlet
     * boundary conditions are employed.
     *
     * \param bc_coef  Pointer to an object that can set the Robin boundary condition coefficients
     */
    void
    setPhysicalBcCoef(
        SAMRAI::solv::RobinBcCoefStrategy<NDIM>* bc_coef);

    /*!
     * \brief Set the SAMRAI::solv::RobinBcCoefStrategy objects used to specify
     * physical boundary conditions.
     *
     * \note Any of the elements of \a bc_coefs may be NULL.  In this case,
     * homogeneous Dirichlet boundary conditions are employed for that data
     * depth.
     *
     * \param bc_coefs  Vector of pointers to objects that can set the Robin boundary condition coefficients
     */
    void
    setPhysicalBcCoefs(
        const std::vector<SAMRAI::solv::RobinBcCoefStrategy<NDIM>*>& bc_coefs);

    /*!
     * \brief Set the SAMRAI::solv::RobinBcCoefStrategy objects used to specify
     * physical boundary conditions.
     *
     * \note Any of the elements of \a bc_coefs may be NULL.  In this case,
     * homogeneous Dirichlet boundary conditions are employed for that data
     * depth.
     *
     * \param bc_coefs  Vector of pointers to objects that can set the Robin boundary condition coefficients
     */
    void
    setPhysicalBcCoefs(
        const blitz::TinyVector<SAMRAI::solv::RobinBcCoefStrategy<NDIM>*,NDIM>& bc_coefs);

    //\}

    /*!
     * \name Functions for configuring the solver.
     */
    //\{

    /*!
     * \brief Specify the smoother type.
     *
     * Select from:
     * - \c "additive"
     * - \c "multiplicative"
     *
     * \note The smoother is always additive between processors ("processor
     * block Gauss-Seidel").
     */
    void
    setSmootherChoice(
        const std::string& smoother_choice);

    /*!
     * \brief Specify the coarse level solver.
     *
     * Select from:
     * - \c "block_jacobi"
     * - \c "hypre"
     * - \c "petsc"
     */
    void
    setCoarsestLevelSolverChoice(
        const std::string& coarse_solver_choice);

    //\}

    /*!
     * \name Implementation of FACPreconditionerStrategy interface.
     */
    //\{

    /*!
     * \brief Perform a given number of relaxations on the error.
     *
     * \param error error vector
     * \param residual residual vector
     * \param level_num level number
     * \param num_sweeps number of sweeps to perform
     * \param performing_pre_sweeps boolean value that is true when pre-smoothing sweeps are being performed
     * \param performing_post_sweeps boolean value that is true when post-smoothing sweeps are being performed
     */
    void
    smoothError(
        SAMRAI::solv::SAMRAIVectorReal<NDIM,double>& error,
        const SAMRAI::solv::SAMRAIVectorReal<NDIM,double>& residual,
        int level_num,
        int num_sweeps,
        bool performing_pre_sweeps,
        bool performing_post_sweeps);

    /*!
     * \brief Solve the residual equation Ae=r on the coarsest level of the
     * patch hierarchy.
     *
     * \param error error vector
     * \param residual residual vector
     * \param coarsest_ln coarsest level number
     */
    bool
    solveCoarsestLevel(
        SAMRAI::solv::SAMRAIVectorReal<NDIM,double>& error,
        const SAMRAI::solv::SAMRAIVectorReal<NDIM,double>& residual,
        int coarsest_ln);

    /*!
     * \brief Compute composite grid residual on a range of levels.
     *
     * \param residual residual vector
     * \param solution solution vector
     * \param rhs source (right hand side) vector
     * \param coarsest_level_num coarsest level number
     * \param finest_level_num finest level number
     */
    void
    computeResidual(
        SAMRAI::solv::SAMRAIVectorReal<NDIM,double>& residual,
        const SAMRAI::solv::SAMRAIVectorReal<NDIM,double>& solution,
        const SAMRAI::solv::SAMRAIVectorReal<NDIM,double>& rhs,
        int coarsest_level_num,
        int finest_level_num);

    //\}

protected:
    /*!
     * \brief Compute implementation-specific hierarchy-dependent data.
     */
    void
    initializeOperatorStateSpecialized(
        const SAMRAI::solv::SAMRAIVectorReal<NDIM,double>& solution,
        const SAMRAI::solv::SAMRAIVectorReal<NDIM,double>& rhs,
        int coarsest_reset_ln,
        int finest_reset_ln);

    /*!
     * \brief Remove implementation-specific hierarchy-dependent data.
     */
    void
    deallocateOperatorStateSpecialized(
        int coarsest_reset_ln,
        int finest_reset_ln);

private:
    /*!
     * \brief Default constructor.
     *
     * \note This constructor is not implemented and should not be used.
     */
    CCPoissonPointRelaxationFACOperator();

    /*!
     * \brief Copy constructor.
     *
     * \note This constructor is not implemented and should not be used.
     *
     * \param from The value to copy to this object.
     */
    CCPoissonPointRelaxationFACOperator(
        const CCPoissonPointRelaxationFACOperator& from);

    /*!
     * \brief Assignment operator.
     *
     * \note This operator is not implemented and should not be used.
     *
     * \param that The value to assign to this object.
     *
     * \return A reference to this object.
     */
    CCPoissonPointRelaxationFACOperator& operator=(
        const CCPoissonPointRelaxationFACOperator& that);

    /*!
     * \brief Initialize the hypre bottom solvers.
     */
    void
    initializeHypreLevelSolvers();

    /*!
     * \brief Initialize the PETSc bottom solver.
     */
    void
    initializePETScLevelSolver();

    /*!
     * \brief Construct a matrix corresponding to a Laplace operator restricted
     * to a single patch.
     */
    static void
    buildPatchLaplaceOperator(
        Mat& A,
        const SAMRAI::solv::PoissonSpecifications& poisson_spec,
        SAMRAI::tbox::Pointer<SAMRAI::hier::Patch<NDIM> > patch,
        const SAMRAI::hier::IntVector<NDIM>& ghost_cell_width);

    /*!
     * \brief Construct a matrix corresponding to a Laplace operator restricted
     * to a single patch with grid aligned anisotropy.
     */
    static void
    buildPatchLaplaceOperator_aligned(
        Mat& A,
        SAMRAI::tbox::Pointer<SAMRAI::pdat::CellData<NDIM,double> > C_data,
        SAMRAI::tbox::Pointer<SAMRAI::pdat::SideData<NDIM,double> > D_data,
        SAMRAI::tbox::Pointer<SAMRAI::hier::Patch<NDIM> > patch,
        const SAMRAI::hier::IntVector<NDIM>& ghost_cell_width);

    /*!
     * \brief Construct a matrix corresponding to a Laplace operator restricted
     * to a single patch with non-grid aligned anisotropy.
     */
    static void
    buildPatchLaplaceOperator_nonaligned(
        Mat& A,
        SAMRAI::tbox::Pointer<SAMRAI::pdat::CellData<NDIM,double> > C_data,
        SAMRAI::tbox::Pointer<SAMRAI::pdat::SideData<NDIM,double> > D_data,
        SAMRAI::tbox::Pointer<SAMRAI::hier::Patch<NDIM> > patch,
        const SAMRAI::hier::IntVector<NDIM>& ghost_cell_width);

    /*
     * Data depth.
     */
    int d_depth;

    /*
     * Scalar Poisson equations specifications.
     */
    SAMRAI::solv::PoissonSpecifications d_poisson_spec;

    /*
     * Boundary condition specification objects.
     */
    SAMRAI::solv::LocationIndexRobinBcCoefs<NDIM>* const d_default_bc_coef;
    std::vector<SAMRAI::solv::RobinBcCoefStrategy<NDIM>*> d_bc_coefs;

    /*
     * Coarse level solvers and solver parameters.
     */
    bool d_using_hypre;
    std::vector<SAMRAI::tbox::Pointer<CCPoissonHypreLevelSolver> > d_hypre_solvers;
    SAMRAI::tbox::Pointer<SAMRAI::tbox::Database> d_hypre_db;
    bool d_using_petsc;
    SAMRAI::tbox::Pointer<CCPoissonPETScLevelSolver> d_petsc_solver;
    SAMRAI::tbox::Pointer<SAMRAI::tbox::Database> d_petsc_db;

    /*
     * Mappings from patch indices to patch operators.
     */
    bool d_using_petsc_smoothers;
    std::vector<std::vector<Vec> > d_patch_vec_e, d_patch_vec_f;
    std::vector<std::vector<Mat> > d_patch_mat;

    /*
     * Patch overlap data.
     */
    std::vector<std::vector<SAMRAI::hier::BoxList<NDIM> > > d_patch_bc_box_overlap;
    std::vector<std::vector<std::map<int,SAMRAI::hier::Box<NDIM> > > > d_patch_smoother_bc_boxes;
};
}// namespace IBTK

/////////////////////////////// INLINE ///////////////////////////////////////

//#include <ibtk/CCPoissonPointRelaxationFACOperator.I>

//////////////////////////////////////////////////////////////////////////////

#endif //#ifndef included_CCPoissonPointRelaxationFACOperator