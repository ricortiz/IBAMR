// Filename: CartGridFunctionSet.h
// Created on 05 Sep 2012 by Boyce Griffith
//
// Copyright (c) 2002-2014, Boyce Griffith
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

#ifndef included_CartGridFunctionSet
#define included_CartGridFunctionSet

/////////////////////////////// INCLUDES /////////////////////////////////////

#include <stddef.h>
#include <string>
#include <vector>

#include "SAMRAI/hier/PatchLevel.h"
#include "ibtk/CartGridFunction.h"


namespace SAMRAI {
namespace hier {
class Patch;
class PatchHierarchy;
class Variable;
}  // namespace hier
}  // namespace SAMRAI

/////////////////////////////// CLASS DEFINITION /////////////////////////////

namespace IBTK
{
/*!
 * \brief Class CartGridFunctionSet is a concrete CartGridFunction that is used
 * to allow multiple CartGridFunction objects to act as a single function.
 */
class CartGridFunctionSet
    : public CartGridFunction
{
public:
    /*!
     * \brief The default constructor sets the name of the strategy object and
     * sets the collection of functions to be empty.
     */
    CartGridFunctionSet(
        const std::string& object_name="");

    /*!
     * \brief Empty virtual destructor.
     */
    virtual
    ~CartGridFunctionSet();

    /*!
     * \brief Add a CartGridFunction to the set of functions grouped together by
     * this object.
     */
    void
    addFunction(
        boost::shared_ptr<CartGridFunction> fcn);

    /*!
     * \name Methods to set patch interior data.
     */
    //\{

    /*!
     * \brief Indicates whether the concrete CartGridFunctionSet object is
     * time-dependent.
     */
    bool
    isTimeDependent() const;

    /*!
     * \brief Evaluate the function on the patch interiors on the specified
     * levels of the patch hierarchy using the implementations of
     * setDataOnPatchHierarchy() provided by the component function objects.
     */
    void
    setDataOnPatchHierarchy(
        int data_idx,
        boost::shared_ptr<SAMRAI::hier::Variable > var,
        boost::shared_ptr<SAMRAI::hier::PatchHierarchy > hierarchy,
        double data_time,
        bool initial_time=false,
        int coarsest_ln=-1,
        int finest_ln=-1);

    /*!
     * \brief Evaluate the function on the patch interiors on the specified
     * level of the patch hierarchy using the implementations of
     * setDataOnPatchLevel() provided by the component function objects.
     */
    void
    setDataOnPatchLevel(
        int data_idx,
        boost::shared_ptr<SAMRAI::hier::Variable > var,
        boost::shared_ptr<SAMRAI::hier::PatchLevel > patch_level,
        double data_time,
        bool initial_time=false);

    /*!
     * \brief Evaluate the function on the patch interior using the
     * implementations of setDataOnPatch() provided by the component function
     * objects.
     */
    void
    setDataOnPatch(
        int data_idx,
        boost::shared_ptr<SAMRAI::hier::Variable > var,
        boost::shared_ptr<SAMRAI::hier::Patch > patch,
        double data_time,
        bool initial_time=false,
        boost::shared_ptr<SAMRAI::hier::PatchLevel > patch_level=boost::shared_ptr<SAMRAI::hier::PatchLevel >(NULL));

    //\}

protected:
    /*
     * The collection of function objects.
     */
    std::vector<boost::shared_ptr<CartGridFunction> > d_fcns;

private:
    /*!
     * \brief Copy constructor.
     *
     * \note This constructor is not implemented and should not be used.
     *
     * \param from The value to copy to this object.
     */
    CartGridFunctionSet(
        const CartGridFunctionSet& from);

    /*!
     * \brief Assignment operator.
     *
     * \note This operator is not implemented and should not be used.
     *
     * \param that The value to assign to this object.
     *
     * \return A reference to this object.
     */
    CartGridFunctionSet&
    operator=(
        const CartGridFunctionSet& that);
};
}// namespace IBTK

//////////////////////////////////////////////////////////////////////////////

#endif //#ifndef included_CartGridFunctionSet
