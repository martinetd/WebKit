/*
 * Copyright (C) 2020 Apple Inc. All rights reserved.
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

#pragma once

#include "LayoutInitialContainingBlock.h"
#include <wtf/HashMap.h>
#include <wtf/UniqueRef.h>
#include <wtf/Vector.h>

namespace WebCore {

class RenderBlock;
class RenderBoxModelObject;
class RenderInline;

namespace LayoutIntegration {

#if ENABLE(TREE_DEBUGGING)
struct InlineContent;
#endif

class BoxTree {
public:
    BoxTree(RenderBlock&);
    ~BoxTree();

    void updateStyle(const RenderBoxModelObject&);

    const RenderBlock& rootRenderer() const { return m_rootRenderer; }
    RenderBlock& rootRenderer() { return m_rootRenderer; }

    const Layout::ContainerBox& rootLayoutBox() const;
    Layout::ContainerBox& rootLayoutBox();

    const Layout::Box& layoutBoxForRenderer(const RenderObject&) const;
    Layout::Box& layoutBoxForRenderer(const RenderObject&);

    const Layout::ContainerBox& layoutBoxForRenderer(const RenderElement&) const;
    Layout::ContainerBox& layoutBoxForRenderer(const RenderElement&);

    const RenderObject& rendererForLayoutBox(const Layout::Box&) const;
    RenderObject& rendererForLayoutBox(const Layout::Box&);

    size_t boxCount() const { return m_renderers.size(); }

    const auto& renderers() const { return m_renderers; }

private:
    Layout::InitialContainingBlock& initialContainingBlock();

    void buildTreeForInlineContent();
    void buildTreeForFlexContent();
    void appendChild(UniqueRef<Layout::Box>, RenderObject&);

    RenderBlock& m_rootRenderer;
    Vector<WeakPtr<RenderObject>, 1> m_renderers;

    HashMap<CheckedRef<const Layout::Box>, WeakPtr<RenderObject>> m_boxToRendererMap;
};

#if ENABLE(TREE_DEBUGGING)
void showInlineContent(TextStream&, const InlineContent&, size_t depth);
#endif
}
}

