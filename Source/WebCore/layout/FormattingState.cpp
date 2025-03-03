/*
 * Copyright (C) 2018 Apple Inc. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY APPLE INC. AND ITS CONTRIBUTORS ``AS IS''
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL APPLE INC. OR ITS CONTRIBUTORS
 * BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
 * THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "config.h"
#include "FormattingState.h"

#include "FloatingState.h"
#include "LayoutBoxGeometry.h"
#include <wtf/IsoMallocInlines.h>

namespace WebCore {
namespace Layout {

WTF_MAKE_ISO_ALLOCATED_IMPL(FormattingState);

FormattingState::FormattingState(Ref<FloatingState>&& floatingState, Type type, LayoutState& layoutState)
    : m_layoutState(layoutState)
    , m_floatingState(WTFMove(floatingState))
    , m_type(type)
{
}

FormattingState::~FormattingState()
{
}

BoxGeometry& FormattingState::boxGeometry(const Box& layoutBox)
{
    // Should never need to mutate a display box outside of the formatting context, unless we are trying
    // to compute the static position of an out-of-flow box.
    ASSERT((layoutBox.isOutOfFlowPositioned() && isInlineFormattingState()) || &layoutState().formattingStateForFormattingContext(FormattingContext::formattingContextRoot(layoutBox)) == this);
    // Anonymous text wrappers do not need to compute box geometry. They initiate inline runs.
    ASSERT(!layoutBox.isInlineTextBox());
    return layoutState().ensureGeometryForBox(layoutBox);
}

}
}
