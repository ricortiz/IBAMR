// Filename: LNode-inl.h
// Created on 05 May 2011 by Boyce Griffith
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

#ifndef included_LNode_inl_h
#define included_LNode_inl_h

/////////////////////////////// INCLUDES /////////////////////////////////////

#include "ibtk/LNode.h"
#include "ibtk/StreamableManager.h"
#include "ibtk/compiler_hints.h"
#include "SAMRAI/tbox/MessageStream.h"

/////////////////////////////// NAMESPACE ////////////////////////////////////

namespace IBTK
{
/////////////////////////////// STATIC ///////////////////////////////////////

/////////////////////////////// PUBLIC ///////////////////////////////////////

inline
LNode::LNode(
    const int lagrangian_nidx,
    const int global_petsc_nidx,
    const int local_petsc_nidx,
    const SAMRAI::hier::IntVector& periodic_offset,
    const Vector& periodic_displacement,
    const std::vector<boost::shared_ptr<Streamable> >& node_data)
    : LNodeIndex(lagrangian_nidx, global_petsc_nidx, local_petsc_nidx, periodic_offset, periodic_displacement),
      d_node_data(node_data)
{
    setupNodeDataTypeArray();
    return;
}// LNode

inline
LNode::LNode(
    const LNode& from)
    : LNodeIndex(from),
      d_node_data(from.d_node_data)
{
    setupNodeDataTypeArray();
    return;
}// LNode

inline
LNode::LNode(
    SAMRAI::tbox::MessageStream& stream,
    const SAMRAI::hier::IntVector& offset)
    : LNodeIndex(),
      d_node_data()
{
    unpackStream(stream,offset);
    return;
}// LNode

inline
LNode::~LNode()
{
    // intentionally blank
    return;
}// ~LNode

inline LNode&
LNode::operator=(
    const LNode& that)
{
    if (this != &that)
    {
        LNodeIndex::operator=(that);
        assignThatToThis(that);
    }
    return *this;
}// operator=

inline void
LNode::registerPeriodicShift(
    const SAMRAI::hier::IntVector& offset,
    const Vector& displacement)
{
    LNodeIndex::registerPeriodicShift(offset, displacement);
    for (std::vector<boost::shared_ptr<Streamable> >::iterator it = d_node_data.begin(); it != d_node_data.end(); ++it)
    {
        (*it)->registerPeriodicShift(offset, displacement);
    }
    return;
}// registerPeriodicShift

inline const std::vector<boost::shared_ptr<Streamable> >&
LNode::getNodeData() const
{
    return d_node_data;
}// getNodeData

inline void
LNode::setNodeData(
    const std::vector<boost::shared_ptr<Streamable> >& node_data)
{
    d_node_data = node_data;
    setupNodeDataTypeArray();
    return;
}// setNodeData

template<typename T>
inline T*
LNode::getNodeDataItem() const
{
    if (LIKELY(T::STREAMABLE_CLASS_ID < MAX_SIZE))
    {
        return static_cast<T*>(d_node_data_type_arr[T::STREAMABLE_CLASS_ID]);
    }
    else
    {
        const unsigned int node_data_sz = d_node_data.size();
        T* ret_val = NULL;
        Streamable* it_val;
        unsigned int k;
        for (k = 0; k < node_data_sz && !ret_val; ++k)
        {
            it_val = d_node_data[k].get();
            if (it_val->getStreamableClassID() == T::STREAMABLE_CLASS_ID)
            {
                ret_val = static_cast<T*>(it_val);
            }
        }
        return ret_val;
    }
}// getNodeDataItem

template<typename T>
inline std::vector<T*>
LNode::getNodeDataVector() const
{
    const unsigned int node_data_sz = d_node_data.size();
    std::vector<T*> ret_val;
    ret_val.reserve(node_data_sz);
    Streamable* it_val;
    for (unsigned int k = 0; k < node_data_sz; ++k)
    {
        it_val = d_node_data_type_arr[k];
        if (it_val->getStreamableClassID() == T::STREAMABLE_CLASS_ID)
        {
            ret_val.push_back(static_cast<T*>(it_val));
        }
    }
    return ret_val;
}// getNodeDataVector

inline void
LNode::copySourceItem(
    const SAMRAI::hier::Index& src_index,
    const SAMRAI::hier::IntVector& src_offset,
    const LNodeIndex& src_item)
{
    LNodeIndex::copySourceItem(src_index, src_offset, src_item);
    const LNode* const p_src_item = dynamic_cast<const LNode*>(&src_item);
#if !defined(NDEBUG)
    TBOX_ASSERT(p_src_item);
#endif
    assignThatToThis(*p_src_item);
    return;
}// copySourceItem

inline size_t
LNode::getDataStreamSize() const
{
    return LNodeIndex::getDataStreamSize() + StreamableManager::getManager()->getDataStreamSize(d_node_data);
}// getDataStreamSize

inline void
LNode::packStream(
    SAMRAI::tbox::MessageStream& stream)
{
    LNodeIndex::packStream(stream);
    StreamableManager::getManager()->packStream(stream, d_node_data);
    return;
}// packStream

inline void
LNode::unpackStream(
    SAMRAI::tbox::MessageStream& stream,
    const SAMRAI::hier::IntVector& offset)
{
    LNodeIndex::unpackStream(stream, offset);
    d_node_data.clear();
    StreamableManager::getManager()->unpackStream(stream, offset, d_node_data);
    setupNodeDataTypeArray();
    return;
}// unpackStream

/////////////////////////////// PRIVATE //////////////////////////////////////

inline void
LNode::assignThatToThis(
    const LNode& that)
{
    d_node_data = that.d_node_data;
    setupNodeDataTypeArray();
    return;
}// assignThatToThis

inline void
LNode::setupNodeDataTypeArray()
{
    std::fill(d_node_data_type_arr,d_node_data_type_arr+MAX_SIZE,nullptr);
    int class_id;
    for (const auto& data_item : d_node_data)
    {
        class_id = data_item->getStreamableClassID();
        if (LIKELY(class_id < MAX_SIZE && !d_node_data_type_arr[class_id]))
        {
            d_node_data_type_arr[class_id] = data_item.get();
        }
    }
    return;
}// setupNodeDataTypeArray

//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////

}// namespace IBTK

//////////////////////////////////////////////////////////////////////////////

#endif //#ifndef included_LNode_inl_h
