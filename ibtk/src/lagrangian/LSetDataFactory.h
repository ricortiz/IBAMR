// Filename: LSetDataFactory.h
// Created on 04 Jun 2007 by Boyce Griffith
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

#ifndef included_LSetDataFactory
#define included_LSetDataFactory

/////////////////////////////// INCLUDES /////////////////////////////////////

#include <stddef.h>

#include "SAMRAI/hier/Box.h"
#include "SAMRAI/pdat/CellGeometry.h"  // IWYU pragma: keep
#include "SAMRAI/pdat/IndexDataFactory.h"
#include "SAMRAI/hier/IntVector.h"
#include "ibtk/FixedSizedStream-inl.h"
#include "ibtk/LMarker.h"
#include "ibtk/LMarker-inl.h"
#include "ibtk/LNode.h"
#include "ibtk/LNodeIndex.h"
#include "ibtk/LNodeIndex-inl.h"
#include "ibtk/LNode-inl.h"
#include "ibtk/LSet-inl.h"
#include "SAMRAI/tbox/Arena.h"


namespace IBTK {
template <class T> class LSet;
}  // namespace IBTK
namespace SAMRAI {
namespace hier {
class Patch;
class PatchData;
class PatchDataFactory;
}  // namespace hier
}  // namespace SAMRAI

/////////////////////////////// CLASS DEFINITION /////////////////////////////

namespace IBTK
{
/*!
 * \brief Class LSetPatchDataFactory provides a SAMRAI::hier::PatchDataFactory
 * class corresponding to patch data of type LSetData.
 */
template<class T>
class LSetDataFactory
    : public SAMRAI::pdat::IndexDataFactory<LSet<T>,SAMRAI::pdat::CellGeometry >
{
public:
    /*!
     * The default constructor for the LSetDataFactory class.  The ghost cell
     * width argument gives the default width for all data objects created with
     * this factory.
     */
    LSetDataFactory(
        const SAMRAI::hier::IntVector& ghosts);

    /*!
     * Virtual destructor for the data factory class.
     */
    virtual
    ~LSetDataFactory();

    /*!
     * Virtual factory function to allocate a concrete data object.  The default
     * information about the object (e.g., ghost cell width) is taken from the
     * factory.  If no memory pool is provided, the allocation routine assumes
     * some default memory pool.
     */
    boost::shared_ptr<SAMRAI::hier::PatchData >
    allocate(
        const SAMRAI::hier::Box& box,
        boost::shared_ptr<SAMRAI::tbox::Arena> pool=NULL) const;

    /*!
     * Virtual factory function to allocate a concrete data object.  The default
     * information about the object (e.g., ghost cell width) is taken from the
     * factory.  If no memory pool is provided, the allocation routine assumes
     * some default memory pool.
     */
    boost::shared_ptr<SAMRAI::hier::PatchData >
    allocate(
        const SAMRAI::hier::Patch& patch,
        boost::shared_ptr<SAMRAI::tbox::Arena> pool=NULL) const;

    /*!
     * Calculate the amount of memory needed to store the data object, including
     * object data but not dynamically allocated data.
     */
    size_t
    getSizeOfMemory(
        const SAMRAI::hier::Box& box) const;

    /*!
     * Virtual function to clone the data factory.  This will return a new
     * instantiation of the factory with the same properties (e.g., same type).
     * The properties of the cloned factory can then be changed without
     * modifying the original.
     */
    boost::shared_ptr<SAMRAI::hier::PatchDataFactory >
    cloneFactory(
        const SAMRAI::hier::IntVector& ghosts);

    /*!
     * Return whether it is valid to copy this LSetDataFactory to the supplied
     * destination patch data factory. It will return true if dst_pdf is a
     * LSetDataFactory, false otherwise.
     */
    bool
    validCopyTo(
        const boost::shared_ptr<SAMRAI::hier::PatchDataFactory >& dst_pdf) const;

private:
    /*!
     * \brief Default constructor.
     *
     * \note This constructor is not implemented and should not be used.
     */
    LSetDataFactory();

    /*!
     * \brief Copy constructor.
     *
     * \note This constructor is not implemented and should not be used.
     *
     * \param from The value to copy to this object.
     */
    LSetDataFactory(
        const LSetDataFactory<T>& from);

    /*!
     * \brief Assignment operator.
     *
     * \note This operator is not implemented and should not be used.
     *
     * \param that The value to assign to this object.
     *
     * \return A reference to this object.
     */
    LSetDataFactory&
    operator=(
        const LSetDataFactory<T>& that);
};
}// namespace IBTK

//////////////////////////////////////////////////////////////////////////////

#endif //#ifndef included_LSetDataFactory
