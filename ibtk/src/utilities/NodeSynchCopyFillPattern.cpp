// Filename: NodeSynchCopyFillPattern.cpp
// Created on 02 Feb 2011 by Boyce Griffith
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

/////////////////////////////// INCLUDES /////////////////////////////////////

#include <ostream>

#include "BoxGeometry.h"
#include "BoxList.h"
#include "BoxOverlap.h"
#include "Index.h"
#include "NodeGeometry.h"
#include "NodeOverlap.h"
#include "NodeSynchCopyFillPattern.h"
#include "ibtk/namespaces.h" // IWYU pragma: keep
#include "tbox/Utilities.h"

/////////////////////////////// NAMESPACE ////////////////////////////////////

namespace IBTK
{
/////////////////////////////// STATIC ///////////////////////////////////////

namespace
{
static const std::string PATTERN_NAME = "NODE_SYNCH_COPY_FILL_PATTERN";
}

/////////////////////////////// PUBLIC ///////////////////////////////////////

NodeSynchCopyFillPattern::NodeSynchCopyFillPattern(const unsigned int axis)
    : d_stencil_width(1), d_axis(axis)
{
    // intentionally blank
    return;
} // NodeSynchCopyFillPattern

NodeSynchCopyFillPattern::~NodeSynchCopyFillPattern()
{
    // intentionally blank
    return;
} // NodeSynchCopyFillPattern

Pointer<BoxOverlap<NDIM> >
NodeSynchCopyFillPattern::calculateOverlap(const BoxGeometry<NDIM>& dst_geometry,
                                           const BoxGeometry<NDIM>& src_geometry,
                                           const Box<NDIM>& /*dst_patch_box*/,
                                           const Box<NDIM>& src_mask,
                                           const bool overwrite_interior,
                                           const IntVector<NDIM>& src_offset) const
{
    Pointer<NodeOverlap<NDIM> > box_geom_overlap =
        dst_geometry.calculateOverlap(src_geometry, src_mask, overwrite_interior, src_offset);
#if !defined(NDEBUG)
    TBOX_ASSERT(box_geom_overlap);
#endif
    if (box_geom_overlap->isOverlapEmpty()) return box_geom_overlap;

    const NodeGeometry<NDIM>* const t_dst_geometry =
        dynamic_cast<const NodeGeometry<NDIM>*>(&dst_geometry);
#if !defined(NDEBUG)
    TBOX_ASSERT(t_dst_geometry);
#endif
    BoxList<NDIM> dst_boxes;
    bool skip = false;
    for (unsigned int d = 0; d < NDIM && !skip; ++d)
    {
        if (d != d_axis)
        {
            skip = skip || (src_offset(d) != 0);
        }
    }
    if (!skip)
    {
        // Determine the stencil box.
        const Box<NDIM>& dst_box = t_dst_geometry->getBox();
        Box<NDIM> stencil_box = NodeGeometry<NDIM>::toNodeBox(dst_box);
        stencil_box.lower()(d_axis) = stencil_box.upper()(d_axis);

        // Intersect the original overlap boxes with the stencil box.
        const BoxList<NDIM>& box_geom_overlap_boxes =
            box_geom_overlap->getDestinationBoxList();
        for (BoxList<NDIM>::Iterator it(box_geom_overlap_boxes); it; it++)
        {
            const Box<NDIM> overlap_box = stencil_box * it();
            if (!overlap_box.empty()) dst_boxes.appendItem(overlap_box);
        }
    }
    return new NodeOverlap<NDIM>(dst_boxes, src_offset);
} // calculateOverlap

IntVector<NDIM>& NodeSynchCopyFillPattern::getStencilWidth()
{
    return d_stencil_width;
} // getStencilWidth

const std::string& NodeSynchCopyFillPattern::getPatternName() const
{
    return PATTERN_NAME;
} // getPatternName

/////////////////////////////// PROTECTED ////////////////////////////////////

/////////////////////////////// PRIVATE //////////////////////////////////////

/////////////////////////////// NAMESPACE ////////////////////////////////////

} // namespace IBTK

//////////////////////////////////////////////////////////////////////////////
