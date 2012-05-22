// Filename: PoissonFACPreconditionerStrategy.h
// Created on 27 Apr 2012 by Boyce Griffith
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

#ifndef included_PoissonFACPreconditionerStrategy
#define included_PoissonFACPreconditionerStrategy

/////////////////////////////// INCLUDES /////////////////////////////////////

// IBTK INCLUDES
#include <ibtk/CoarseFineBoundaryRefinePatchStrategy.h>
#include <ibtk/FACPreconditionerStrategy.h>
#include <ibtk/HierarchyMathOps.h>
#include <ibtk/RobinPhysBdryPatchStrategy.h>

// SAMRAI INCLUDES
#include <CoarsenAlgorithm.h>
#include <RefineAlgorithm.h>

/////////////////////////////// CLASS DEFINITION /////////////////////////////

namespace IBTK
{
/*!
 * \brief Class PoissonFACPreconditionerStrategy is an abstract
 * FACPreconditionerStrategy implementing many of the operations required by
 * smoothers for the Poisson equation and related problems.
 *
 * Sample parameters for initialization from database (and their default
 * values): \verbatim

 smoother_choice = "additive"                 // see setSmootherChoice()

 prolongation_method = "LINEAR_REFINE"        // see setProlongationMethod()
 restriction_method = "CONSERVATIVE_COARSEN"  // see setRestrictionMethod()

 coarse_solver_choice = "block_jacobi"        // see setCoarsestLevelSolverChoice()
 coarse_solver_tolerance = 1.0e-6             // see setCoarsestLevelSolverTolerance()
 coarse_solver_max_iterations = 10            // see setCoarsestLevelSolverMaxIterations()
 \endverbatim
*/
class PoissonFACPreconditionerStrategy
    : public FACPreconditionerStrategy
{
public:
    /*!
     * \brief Constructor.
     */
    PoissonFACPreconditionerStrategy(
        const std::string& object_name,
        SAMRAI::tbox::Pointer<SAMRAI::hier::Variable<NDIM> > scratch_var,
        int ghost_cell_width,
        SAMRAI::tbox::Pointer<SAMRAI::tbox::Database> input_db);

    /*!
     * \brief Destructor.
     */
    ~PoissonFACPreconditionerStrategy();

    /*!
     * \brief Set the hierarchy time, for use with the refinement schedules and
     * boundary condition routines employed by the object.
     */
    void
    setTime(
        double time);

    //\}

    /*!
     * \name Functions for configuring the solver.
     */
    //\{

    /*!
     * \brief Specify the levels that need to be reset the next time the
     * operator is re-initialized.
     *
     * When the operator is initialized, then only the specified range of levels
     * are reset in the operator state the next time that the operator is
     * initialized.  If the operator is not initialized, this method has no
     * effect.
     *
     * To ensure the range of levels that is reset includes all levels in the
     * patch hierarchy, use \a coarsest_ln = \a finest_ln = \p -1.
     *
     * \note This function is used to save some unnecessary computations when
     * the hierarchy is regridded.  The range of levels specified must include
     * all levels which need to be reset by
     * SAMRAI::mesh::StandardTagAndInitStrategy::resetHierarchyConfiguration().
     * Any data residing outside of this range of levels will not be reset.
     * This \b is \b not what you want to have happen if, for instance, the
     * Poisson specifications changes.
     */
    void
    setResetLevels(
        int coarsest_ln,
        int finest_ln);

    /*!
     * \brief Specify the smoother type.
     */
    virtual void
    setSmootherChoice(
        const std::string& smoother_choice) = 0;

    /*!
     * \brief Specify the coarse level solver.
     */
    virtual void
    setCoarsestLevelSolverChoice(
        const std::string& coarse_solver_choice) = 0;

    /*!
     * \brief Set tolerance for coarse level solve.
     *
     * If the coarse level solver uses a convergence tolerance parameter, the
     * specified value is used.  If the coarse level solver does not use such a
     * stopping parameter, implementations are free to ignore this value.
     */
    void
    setCoarsestLevelSolverTolerance(
        double coarse_solver_tol);

    /*!
     * \brief Set the maximum number of iterations for the coarsest level solve.
     *
     * If the coarse level solver uses a maximum number of iterations parameter,
     * the specified value is used.  If the coarse level solver does not use
     * such a stopping parameter, implementations are free to ignore this value.
     */
    void
    setCoarsestLevelSolverMaxIterations(
        int coarse_solver_max_its);

    /*!
     * \brief Set the name of the prolongation method.
     */
    void
    setProlongationMethod(
        const std::string& prolongation_method);

    /*!
     * \brief Set the name of the restriction method.
     */
    void
    setRestrictionMethod(
        const std::string& restriction_method);

    //\}

    /*!
     * \name Partial implementation of FACPreconditionerStrategy interface.
     */
    //\{

    /*!
     * \brief Restrict the residual quantity to the specified level from the
     * next finer level.
     *
     * \param src source residual
     * \param dst destination residual
     * \param dst_ln destination level number
     */
    void
    restrictResidual(
        const SAMRAI::solv::SAMRAIVectorReal<NDIM,double>& src,
        SAMRAI::solv::SAMRAIVectorReal<NDIM,double>& dst,
        int dst_ln);

    /*!
     * \brief Prolong the error quantity to the specified level from the next
     * coarser level.
     *
     * \param src source error vector
     * \param dst destination error vector
     * \param dst_ln destination level number of data transfer
     */
    void
    prolongError(
        const SAMRAI::solv::SAMRAIVectorReal<NDIM,double>& src,
        SAMRAI::solv::SAMRAIVectorReal<NDIM,double>& dst,
        int dst_ln);

    /*!
     * \brief Prolong the error quantity to the specified level from the next
     * coarser level and apply the correction to the fine-level error.
     *
     * \param src source error vector
     * \param dst destination error vector
     * \param dst_ln destination level number of data transfer
     */
    void
    prolongErrorAndCorrect(
        const SAMRAI::solv::SAMRAIVectorReal<NDIM,double>& src,
        SAMRAI::solv::SAMRAIVectorReal<NDIM,double>& dst,
        int dst_ln);

    /*!
     * \brief Compute hierarchy-dependent data.
     *
     * Note that although the vector arguments given to other methods in this
     * class may not necessarily be the same as those given to this method,
     * there will be similarities, including:
     *
     * - hierarchy configuration (hierarchy pointer and level range)
     * - number, type and alignment of vector component data
     * - ghost cell width of data in the solution (or solution-like) vector
     *
     * \param solution solution vector u
     * \param rhs right hand side vector f
     */
    void
    initializeOperatorState(
        const SAMRAI::solv::SAMRAIVectorReal<NDIM,double>& solution,
        const SAMRAI::solv::SAMRAIVectorReal<NDIM,double>& rhs);

    /*!
     * \brief Remove all hierarchy-dependent data.
     *
     * Remove all hierarchy-dependent data set by initializeOperatorState().
     *
     * \see initializeOperatorState
     */
    void
    deallocateOperatorState();

    //\}

protected:
    /*!
     * \brief Compute implementation-specific hierarchy-dependent data.
     */
    virtual void
    initializeOperatorStateSpecialized(
        const SAMRAI::solv::SAMRAIVectorReal<NDIM,double>& solution,
        const SAMRAI::solv::SAMRAIVectorReal<NDIM,double>& rhs,
        int coarsest_reset_ln,
        int finest_reset_ln) = 0;

    /*!
     * \brief Remove implementation-specific hierarchy-dependent data.
     */
    virtual void
    deallocateOperatorStateSpecialized(
        int coarsest_reset_ln,
        int finest_reset_ln) = 0;

    /*!
     * \name Methods for executing, caching, and resetting communication
     * schedules.
     */
    //\{

    /*!
     * \brief Execute a refinement schedule for prolonging data.
     */
    void
    xeqScheduleProlongation(
        int dst_idx,
        int src_idx,
        int dst_ln);

    /*!
     * \brief Execute schedule for restricting solution or residual to the
     * specified level.
     */
    void
    xeqScheduleRestriction(
        int dst_idx,
        int src_idx,
        int dst_ln);

    /*!
     * \brief Execute schedule for filling ghosts on the specified level.
     */
    void
    xeqScheduleGhostFillNoCoarse(
        int dst_idx,
        int dst_ln);

    /*!
     * \brief Execute schedule for synchronizing data on the specified level.
     */
    void
    xeqScheduleDataSynch(
        int dst_idx,
        int dst_ln);

    //\}

    /*
     * The object name is used for error reporting purposes.
     *
     * The boolean indicates whether this object has been initialized.
     */
    std::string d_object_name;
    bool d_is_initialized;

    /*
     * The current time.
     */
    double d_time;

    /*
     * Ghost cell width.
     */
    const SAMRAI::hier::IntVector<NDIM> d_gcw;

    /*!
     * \name Hierarchy-dependent objects.
     */
    //\{

    /*
     * Solution and rhs vectors.
     */
    SAMRAI::tbox::Pointer<SAMRAI::solv::SAMRAIVectorReal<NDIM,double> > d_solution, d_rhs;

    /*
     * Reference patch hierarchy and range of levels involved in the solve.
     *
     * This variable is non-null between the initializeOperatorState() and
     * deallocateOperatorState() calls.  It is not truly needed, because the
     * hierarchy is obtainable through variables in most function argument
     * lists.  We use it to enforce working on one hierarchy at a time.
     */
    SAMRAI::tbox::Pointer<SAMRAI::hier::PatchHierarchy<NDIM> > d_hierarchy;
    int d_coarsest_ln, d_finest_ln;

    /*
     * HierarchyDataOpsReal objects restricted to a single level of the patch
     * hierarchy.
     */
    std::vector<SAMRAI::tbox::Pointer<SAMRAI::math::HierarchyDataOpsReal<NDIM,double> > > d_level_data_ops;

    /*
     * Level operators, used to compute composite-grid residuals.
     */
    std::vector<SAMRAI::tbox::Pointer<IBTK::HierarchyGhostCellInterpolation> > d_hier_bdry_fill_ops;
    std::vector<SAMRAI::tbox::Pointer<IBTK::HierarchyMathOps> > d_hier_math_ops;

    /*
     * Range of levels to be reset the next time the operator is initialized.
     */
    bool d_in_initialize_operator_state;
    int d_coarsest_reset_ln, d_finest_reset_ln;

    //\}

    /*!
     * \name Solver configuration variables.
     */
    //\{

    /*
     * The kind of smoothing to perform.
     */
    std::string d_smoother_choice;

    /*
     * The names of the refinement operators used to prolong the coarse grid
     * correction.
     */
    std::string d_prolongation_method;

    /*
     * The names of the coarsening operators used to restrict the fine grid
     * error or residual.
     */
    std::string d_restriction_method;

    /*
     * Coarse level solver parameters.
     */
    std::string d_coarse_solver_choice;
    double d_coarse_solver_tol;
    int d_coarse_solver_max_its;

    //\}

    /*!
     * \name Internal context and scratch data.
     */
    //\{

    /*
     * Variable context for internally maintained hierarchy data.
     */
    SAMRAI::tbox::Pointer<SAMRAI::hier::VariableContext> d_context;

    /*
     * Patch descriptor index for scratch data.
     */
    int d_scratch_idx;

    //\}

    /*!
     * \name Various refine and coarsen objects.
     */
    //\{

    /*
     * Physical boundary operators.
     */
    SAMRAI::tbox::Pointer<RobinPhysBdryPatchStrategy> d_bc_op;

    /*
     * Coarse-fine interface interpolation objects.
     */
    SAMRAI::tbox::Pointer<CoarseFineBoundaryRefinePatchStrategy> d_cf_bdry_op;

    /*
     * Variable fill pattern object.
     */
    SAMRAI::tbox::Pointer<SAMRAI::xfer::VariableFillPattern<NDIM> > d_op_stencil_fill_pattern, d_synch_fill_pattern;

    //\}

private:
    /*!
     * \brief Default constructor.
     *
     * \note This constructor is not implemented and should not be used.
     */
    PoissonFACPreconditionerStrategy();

    /*!
     * \brief Copy constructor.
     *
     * \note This constructor is not implemented and should not be used.
     *
     * \param from The value to copy to this object.
     */
    PoissonFACPreconditionerStrategy(
        const PoissonFACPreconditionerStrategy& from);

    /*!
     * \brief Assignment operator.
     *
     * \note This operator is not implemented and should not be used.
     *
     * \param that The value to assign to this object.
     *
     * \return A reference to this object.
     */
    PoissonFACPreconditionerStrategy& operator=(
        const PoissonFACPreconditionerStrategy& that);

    /*!
     * \name Various refine and coarsen objects.
     */
    //\{

    /*
     * Error prolongation (refinement) operator.
     */
    SAMRAI::tbox::Pointer<SAMRAI::xfer::RefineOperator<NDIM> > d_prolongation_refine_operator;
    SAMRAI::tbox::Pointer<SAMRAI::xfer::RefinePatchStrategy<NDIM> > d_prolongation_refine_patch_strategy;
    SAMRAI::tbox::Pointer<SAMRAI::xfer::RefineAlgorithm<NDIM> > d_prolongation_refine_algorithm;
    std::vector<SAMRAI::tbox::Pointer<SAMRAI::xfer::RefineSchedule<NDIM> > > d_prolongation_refine_schedules;

    /*
     * Residual restriction (coarsening) operator.
     */
    SAMRAI::tbox::Pointer<SAMRAI::xfer::CoarsenOperator<NDIM> > d_restriction_coarsen_operator;
    SAMRAI::tbox::Pointer<SAMRAI::xfer::CoarsenAlgorithm<NDIM> > d_restriction_coarsen_algorithm;
    std::vector<SAMRAI::tbox::Pointer<SAMRAI::xfer::CoarsenSchedule<NDIM> > > d_restriction_coarsen_schedules;

    /*
     * Refine operator for cell data from same level.
     */
    SAMRAI::tbox::Pointer<SAMRAI::xfer::RefineAlgorithm<NDIM> > d_ghostfill_nocoarse_refine_algorithm;
    std::vector<SAMRAI::tbox::Pointer<SAMRAI::xfer::RefineSchedule<NDIM> > > d_ghostfill_nocoarse_refine_schedules;

    /*
     * Operator for data synchronization on same level.
     */
    SAMRAI::tbox::Pointer<SAMRAI::xfer::RefineAlgorithm<NDIM> > d_synch_refine_algorithm;
    std::vector<SAMRAI::tbox::Pointer<SAMRAI::xfer::RefineSchedule<NDIM> > > d_synch_refine_schedules;

    //\}
};
}// namespace IBTK

/////////////////////////////// INLINE ///////////////////////////////////////

//#include <ibtk/PoissonFACPreconditionerStrategy.I>

//////////////////////////////////////////////////////////////////////////////

#endif //#ifndef included_PoissonFACPreconditionerStrategy