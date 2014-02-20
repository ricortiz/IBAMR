// Filename: KrylovLinearSolver.h
// Created on 08 Sep 2003 by Boyce Griffith
//
// Copyright (c) 2002-2013, Boyce Griffith
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

#ifndef included_KrylovLinearSolver
#define included_KrylovLinearSolver

/////////////////////////////// INCLUDES /////////////////////////////////////

#include <stddef.h>

#include "SAMRAI/solv/SAMRAIVectorReal.h"
#include "ibtk/LinearOperator.h"
#include "ibtk/LinearSolver.h"


namespace IBTK {
class HierarchyMathOps;
}  // namespace IBTK

/////////////////////////////// CLASS DEFINITION /////////////////////////////

namespace IBTK
{
/*!
 * \brief Class KrylovLinearSolver provides an abstract interface for the
 * implementation of Krylov subspace solvers for linear problems of the form
 * \f$Ax=b\f$.
 */
class KrylovLinearSolver
    : public LinearSolver
{
public:
    /*!
     * \brief Default constructor.
     */
    KrylovLinearSolver();

    /*!
     * \brief Empty destructor.
     */
    ~KrylovLinearSolver();

    /*!
     * \brief Set the HierarchyMathOps object used by the solver.
     */
    void
    setHierarchyMathOps(
        boost::shared_ptr<HierarchyMathOps> hier_math_ops);

    /*!
     * \name General-purpose solver functionality.
     */
    //\{

    /*!
     * \brief Set whether the solver should use homogeneous boundary conditions.
     */
    void
    setHomogeneousBc(
        bool homogeneous_bc);

    /*!
     * \brief Set the time at which the solution is to be evaluated.
     */
    void
    setSolutionTime(
        double solution_time);

    /*!
     * \brief Set the current time interval.
     */
    void
    setTimeInterval(
        double current_time,
        double new_time);

    //\}

    /*!
     * \name Krylov solver functionality.
     */
    //\{

    /*!
     * \brief Set the linear operator used when solving \f$Ax=b\f$.
     */
    virtual void
    setOperator(
        boost::shared_ptr<LinearOperator> A);

    /*!
     * \brief Retrieve the linear operator used when solving \f$Ax=b\f$.
     */
    virtual boost::shared_ptr<LinearOperator>
    getOperator() const;

    /*!
     * \brief Set the preconditioner used by the Krylov subspace method when
     * solving \f$Ax=b\f$.
     *
     * \note If the preconditioner is NULL, no preconditioning is performed.
     */
    virtual void
    setPreconditioner(
        boost::shared_ptr<LinearSolver> pc_solver=NULL);

    /*!
     * \brief Retrieve the preconditioner used by the Krylov subspace method
     * when solving \f$Ax=b\f$.
     */
    virtual boost::shared_ptr<LinearSolver>
    getPreconditioner() const;

    //\}

protected:
    // Solver components.
    boost::shared_ptr<LinearOperator> d_A;
    boost::shared_ptr<LinearSolver> d_pc_solver;
    boost::shared_ptr<SAMRAI::solv::SAMRAIVectorReal<double> > d_x, d_b;

private:
    /*!
     * \brief Copy constructor.
     *
     * \note This constructor is not implemented and should not be used.
     *
     * \param from The value to copy to this object.
     */
    KrylovLinearSolver(
        const KrylovLinearSolver& from);

    /*!
     * \brief Assignment operator.
     *
     * \note This operator is not implemented and should not be used.
     *
     * \param that The value to assign to this object.
     *
     * \return A reference to this object.
     */
    KrylovLinearSolver&
    operator=(
        const KrylovLinearSolver& that);
};
}// namespace IBTK

//////////////////////////////////////////////////////////////////////////////

#endif //#ifndef included_KrylovLinearSolver
