/*
 * Copyright (C) 2020 Apple Inc. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1.  Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 * 2.  Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 * 3.  Neither the name of Apple Inc. ("Apple") nor the names of
 *     its contributors may be used to endorse or promote products derived
 *     from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY APPLE AND ITS CONTRIBUTORS "AS IS" AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL APPLE OR ITS CONTRIBUTORS BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "config.h"
#include "AXLogger.h"

#if ENABLE(ACCESSIBILITY_ISOLATED_TREE)
#include "AXIsolatedObject.h"
#endif
#include "AXObjectCache.h"
#include "FrameView.h"
#include "LogInitialization.h"
#include "Logging.h"
#include <wtf/OptionSet.h>
#include <wtf/text/TextStream.h>

namespace WebCore {

static bool shouldLog()
{
    // Modify the initializer list below to choose what thread you want to log messages from.
    static constexpr OptionSet<AXLoggingOptions> loggingOptions { AXLoggingOptions::MainThread, AXLoggingOptions::OffMainThread };

    return (isMainThread() && loggingOptions & AXLoggingOptions::MainThread)
        || (!isMainThread() && loggingOptions & AXLoggingOptions::OffMainThread);
}

#if !LOG_DISABLED

AXLogger::AXLogger(const String& methodName)
    : m_methodName(methodName)
{
    if (auto* channel = getLogChannel("Accessibility"_s))
        channel->level = WTFLogLevel::Debug;

    if (shouldLog()) {
        if (!m_methodName.isEmpty())
            LOG_WITH_STREAM(Accessibility, stream << m_methodName << " {");
    }
}

AXLogger::~AXLogger()
{
    if (shouldLog()) {
        if (!m_methodName.isEmpty())
            LOG_WITH_STREAM(Accessibility, stream << "} " << m_methodName);
    }
}

void AXLogger::log(const String& message)
{
    if (shouldLog())
        LOG(Accessibility, "%s", message.utf8().data());
}

void AXLogger::log(const char* message)
{
    if (shouldLog())
        LOG(Accessibility, "%s", message);
}

void AXLogger::log(RefPtr<AXCoreObject> object)
{
    if (shouldLog()) {
        TextStream stream(TextStream::LineMode::MultipleLine);

        if (object)
            stream << *object;
        else
            stream << "null";

        LOG(Accessibility, "%s", stream.release().utf8().data());
    }
}

void AXLogger::log(const Vector<RefPtr<AXCoreObject>>& objects)
{
    if (shouldLog()) {
        TextStream stream(TextStream::LineMode::MultipleLine);

        stream << "[";
        for (auto object : objects) {
            if (object)
                stream << *object;
            else
                stream << "null";
        }
        stream << "]";

        LOG(Accessibility, "%s", stream.release().utf8().data());
    }
}

void AXLogger::log(const std::pair<RefPtr<AXCoreObject>, AXObjectCache::AXNotification>& notification)
{
    if (shouldLog()) {
        TextStream stream(TextStream::LineMode::MultipleLine);
        stream << "Notification " << notification.second << " for object ";
        if (notification.first)
            stream << *notification.first;
        else
            stream << "null";
        LOG(Accessibility, "%s", stream.release().utf8().data());
    }
}

void AXLogger::log(const AccessibilitySearchCriteria& criteria)
{
    TextStream stream(TextStream::LineMode::MultipleLine);
    stream << criteria;
    LOG(Accessibility, "%s", stream.release().utf8().data());
}

void AXLogger::log(AccessibilityObjectInclusion inclusion)
{
    TextStream stream(TextStream::LineMode::SingleLine);
    stream.dumpProperty("ObjectInclusion", inclusion);
    LOG(Accessibility, "%s", stream.release().utf8().data());
}

#if ENABLE(ACCESSIBILITY_ISOLATED_TREE)
void AXLogger::log(AXIsolatedTree& tree)
{
    if (shouldLog()) {
        TextStream stream(TextStream::LineMode::MultipleLine);
        stream << tree;
        LOG(Accessibility, "%s", stream.release().utf8().data());
    }
}
#endif

void AXLogger::log(AXObjectCache& axObjectCache)
{
    if (shouldLog()) {
        TextStream stream(TextStream::LineMode::MultipleLine);
        stream << axObjectCache;
        LOG(Accessibility, "%s", stream.release().utf8().data());
    }
}

#endif // !LOG_DISABLED

TextStream& operator<<(TextStream& stream, AccessibilityRole role)
{
    stream << accessibilityRoleToString(role);
    return stream;
}

TextStream& operator<<(TextStream& stream, AccessibilitySearchDirection direction)
{
    switch (direction) {
    case AccessibilitySearchDirection::Next:
        stream << "Next";
        break;
    case AccessibilitySearchDirection::Previous:
        stream << "Previous";
        break;
    };

    return stream;
}

TextStream& operator<<(TextStream& stream, AccessibilitySearchKey searchKey)
{
    switch (searchKey) {
    case AccessibilitySearchKey::AnyType:
        stream << "AnyType";
        break;
    case AccessibilitySearchKey::Article:
        stream << "Article";
        break;
    case AccessibilitySearchKey::BlockquoteSameLevel:
        stream << "BlockquoteSameLevel";
        break;
    case AccessibilitySearchKey::Blockquote:
        stream << "Blockquote";
        break;
    case AccessibilitySearchKey::BoldFont:
        stream << "BoldFont";
        break;
    case AccessibilitySearchKey::Button:
        stream << "Button";
        break;
    case AccessibilitySearchKey::CheckBox:
        stream << "CheckBox";
        break;
    case AccessibilitySearchKey::Control:
        stream << "Control";
        break;
    case AccessibilitySearchKey::DifferentType:
        stream << "DifferentType";
        break;
    case AccessibilitySearchKey::FontChange:
        stream << "FontChange";
        break;
    case AccessibilitySearchKey::FontColorChange:
        stream << "FontColorChange";
        break;
    case AccessibilitySearchKey::Frame:
        stream << "Frame";
        break;
    case AccessibilitySearchKey::Graphic:
        stream << "Graphic";
        break;
    case AccessibilitySearchKey::HeadingLevel1:
        stream << "HeadingLevel1";
        break;
    case AccessibilitySearchKey::HeadingLevel2:
        stream << "HeadingLevel2";
        break;
    case AccessibilitySearchKey::HeadingLevel3:
        stream << "HeadingLevel3";
        break;
    case AccessibilitySearchKey::HeadingLevel4:
        stream << "HeadingLevel4";
        break;
    case AccessibilitySearchKey::HeadingLevel5:
        stream << "HeadingLevel5";
        break;
    case AccessibilitySearchKey::HeadingLevel6:
        stream << "HeadingLevel6";
        break;
    case AccessibilitySearchKey::HeadingSameLevel:
        stream << "HeadingSameLevel";
        break;
    case AccessibilitySearchKey::Heading:
        stream << "Heading";
        break;
    case AccessibilitySearchKey::Highlighted:
        stream << "Highlighted";
        break;
    case AccessibilitySearchKey::ItalicFont:
        stream << "ItalicFont";
        break;
    case AccessibilitySearchKey::KeyboardFocusable:
        stream << "KeyboardFocusable";
        break;
    case AccessibilitySearchKey::Landmark:
        stream << "Landmark";
        break;
    case AccessibilitySearchKey::Link:
        stream << "Link";
        break;
    case AccessibilitySearchKey::List:
        stream << "List";
        break;
    case AccessibilitySearchKey::LiveRegion:
        stream << "LiveRegion";
        break;
    case AccessibilitySearchKey::MisspelledWord:
        stream << "MisspelledWord";
        break;
    case AccessibilitySearchKey::Outline:
        stream << "Outline";
        break;
    case AccessibilitySearchKey::PlainText:
        stream << "PlainText";
        break;
    case AccessibilitySearchKey::RadioGroup:
        stream << "RadioGroup";
        break;
    case AccessibilitySearchKey::SameType:
        stream << "SameType";
        break;
    case AccessibilitySearchKey::StaticText:
        stream << "StaticText";
        break;
    case AccessibilitySearchKey::StyleChange:
        stream << "StyleChange";
        break;
    case AccessibilitySearchKey::TableSameLevel:
        stream << "TableSameLevel";
        break;
    case AccessibilitySearchKey::Table:
        stream << "Table";
        break;
    case AccessibilitySearchKey::TextField:
        stream << "TextField";
        break;
    case AccessibilitySearchKey::Underline:
        stream << "Underline";
        break;
    case AccessibilitySearchKey::UnvisitedLink:
        stream << "UnvisitedLink";
        break;
    case AccessibilitySearchKey::VisitedLink:
        stream << "VisitedLink";
        break;
    };

    return stream;
}

TextStream& operator<<(TextStream& stream, const AccessibilitySearchCriteria& criteria)
{
    TextStream::GroupScope groupScope(stream);
    auto streamCriteriaObject = [&stream] (ASCIILiteral objectLabel, auto* axObject) {
        stream.startGroup();
        stream << objectLabel.characters() << " " << axObject << ", ID " << (axObject ? axObject->objectID() : AXID());
        stream.endGroup();
    };

    stream << "SearchCriteria " << &criteria;
    streamCriteriaObject("anchorObject"_s, criteria.anchorObject);
    streamCriteriaObject("startObject"_s, criteria.startObject);
    stream.dumpProperty("searchDirection", criteria.searchDirection);

    stream.nextLine();
    stream << "(searchKeys [";
    for (auto searchKey : criteria.searchKeys)
        stream << searchKey << ", ";
    stream << "])";

    stream.dumpProperty("searchText", criteria.searchText);
    stream.dumpProperty("resultsLimit", criteria.resultsLimit);
    stream.dumpProperty("visibleOnly", criteria.visibleOnly);
    stream.dumpProperty("immediateDescendantsOnly", criteria.immediateDescendantsOnly);

    return stream;
}

TextStream& operator<<(TextStream& stream, AccessibilityObjectInclusion inclusion)
{
    switch (inclusion) {
    case AccessibilityObjectInclusion::IncludeObject:
        stream << "IncludeObject";
        break;
    case AccessibilityObjectInclusion::IgnoreObject:
        stream << "IgnoreObject";
        break;
    case AccessibilityObjectInclusion::DefaultBehavior:
        stream << "DefaultBehavior";
        break;
    }

    return stream;
}

TextStream& operator<<(TextStream& stream, AXObjectCache::AXNotification notification)
{
    switch (notification) {
    case AXObjectCache::AXNotification::AXActiveDescendantChanged:
        stream << "AXActiveDescendantChanged";
        break;
    case AXObjectCache::AXNotification::AXAutocorrectionOccured:
        stream << "AXAutocorrectionOccured";
        break;
    case AXObjectCache::AXNotification::AXAutofillTypeChanged:
        stream << "AXAutofillTypeChanged";
        break;
    case AXObjectCache::AXNotification::AXCheckedStateChanged:
        stream << "AXCheckedStateChanged";
        break;
    case AXObjectCache::AXNotification::AXChildrenChanged:
        stream << "AXChildrenChanged";
        break;
    case AXObjectCache::AXNotification::AXColumnCountChanged:
        stream << "AXColumnCountChanged";
        break;
    case AXObjectCache::AXNotification::AXColumnIndexChanged:
        stream << "AXColumnIndexChanged";
        break;
    case AXObjectCache::AXNotification::AXColumnSpanChanged:
        stream << "AXColumnSpanChanged";
        break;
    case AXObjectCache::AXNotification::AXCurrentStateChanged:
        stream << "AXCurrentStateChanged";
        break;
    case AXObjectCache::AXNotification::AXDisabledStateChanged:
        stream << "AXDisabledStateChanged";
        break;
    case AXObjectCache::AXNotification::AXDescribedByChanged:
        stream << "AXDescribedByChanged";
        break;
    case AXObjectCache::AXNotification::AXDropEffectChanged:
        stream << "AXDropEffectChanged";
        break;
    case AXObjectCache::AXNotification::AXFlowToChanged:
        stream << "AXFlowToChanged";
        break;
    case AXObjectCache::AXNotification::AXFocusedUIElementChanged:
        stream << "AXFocusedUIElementChanged";
        break;
    case AXObjectCache::AXNotification::AXFrameLoadComplete:
        stream << "AXFrameLoadComplete";
        break;
    case AXObjectCache::AXNotification::AXGrabbedStateChanged:
        stream << "AXGrabbedStateChanged";
        break;
    case AXObjectCache::AXNotification::AXHasPopupChanged:
        stream << "AXHasPopupChanged";
        break;
    case AXObjectCache::AXNotification::AXIdAttributeChanged:
        stream << "AXIdAttributeChanged";
        break;
    case AXObjectCache::AXNotification::AXImageOverlayChanged:
        stream << "AXImageOverlayChanged";
        break;
    case AXObjectCache::AXNotification::AXIsAtomicChanged:
        stream << "AXIsAtomicChanged";
        break;
    case AXObjectCache::AXNotification::AXLanguageChanged:
        stream << "AXLanguageChanged";
        break;
    case AXObjectCache::AXNotification::AXLayoutComplete:
        stream << "AXLayoutComplete";
        break;
    case AXObjectCache::AXNotification::AXLevelChanged:
        stream << "AXLevelChanged";
        break;
    case AXObjectCache::AXNotification::AXLoadComplete:
        stream << "AXLoadComplete";
        break;
    case AXObjectCache::AXNotification::AXPlaceholderChanged:
        stream << "AXPlaceholderChanged";
        break;
    case AXObjectCache::AXNotification::AXMaximumValueChanged:
        stream << "AXMaximumValueChanged";
        break;
    case AXObjectCache::AXNotification::AXMinimumValueChanged:
        stream << "AXMinimumValueChanged";
        break;
    case AXObjectCache::AXNotification::AXMultiSelectableStateChanged:
        stream << "AXMultiSelectableStateChanged";
        break;
    case AXObjectCache::AXNotification::AXNewDocumentLoadComplete:
        stream << "AXNewDocumentLoadComplete";
        break;
    case AXObjectCache::AXNotification::AXOrientationChanged:
        stream << "AXOrientationChanged";
        break;
    case AXObjectCache::AXNotification::AXPageScrolled:
        stream << "AXPageScrolled";
        break;
    case AXObjectCache::AXNotification::AXPositionInSetChanged:
        stream << "AXPositionInSetChanged";
        break;
    case AXObjectCache::AXNotification::AXRoleChanged:
        stream << "AXRoleChanged";
        break;
    case AXObjectCache::AXNotification::AXRoleDescriptionChanged:
        stream << "AXRoleDescriptionChanged";
        break;
    case AXObjectCache::AXNotification::AXRowIndexChanged:
        stream << "AXRowIndexChanged";
        break;
    case AXObjectCache::AXNotification::AXRowSpanChanged:
        stream << "AXRowSpanChanged";
        break;
    case AXObjectCache::AXNotification::AXSelectedChildrenChanged:
        stream << "AXSelectedChildrenChanged";
        break;
    case AXObjectCache::AXNotification::AXSelectedStateChanged:
        stream << "AXSelectedStateChanged";
        break;
    case AXObjectCache::AXNotification::AXSelectedTextChanged:
        stream << "AXSelectedTextChanged";
        break;
    case AXObjectCache::AXNotification::AXSetSizeChanged:
        stream << "AXSetSizeChanged";
        break;
    case AXObjectCache::AXNotification::AXTableHeadersChanged:
        stream << "AXTableHeadersChanged";
        break;
    case AXObjectCache::AXNotification::AXURLChanged:
        stream << "AXURLChanged";
        break;
    case AXObjectCache::AXNotification::AXValueChanged:
        stream << "AXValueChanged";
        break;
    case AXObjectCache::AXNotification::AXScrolledToAnchor:
        stream << "AXScrolledToAnchor";
        break;
    case AXObjectCache::AXNotification::AXLiveRegionCreated:
        stream << "AXLiveRegionCreated";
        break;
    case AXObjectCache::AXNotification::AXLiveRegionChanged:
        stream << "AXLiveRegionChanged";
        break;
    case AXObjectCache::AXNotification::AXLiveRegionRelevantChanged:
        stream << "AXLiveRegionRelevantChanged";
        break;
    case AXObjectCache::AXNotification::AXLiveRegionStatusChanged:
        stream << "AXLiveRegionStatusChanged";
        break;
    case AXObjectCache::AXNotification::AXMenuListItemSelected:
        stream << "AXMenuListItemSelected";
        break;
    case AXObjectCache::AXNotification::AXMenuListValueChanged:
        stream << "AXMenuListValueChanged";
        break;
    case AXObjectCache::AXNotification::AXMenuClosed:
        stream << "AXMenuClosed";
        break;
    case AXObjectCache::AXNotification::AXMenuOpened:
        stream << "AXMenuOpened";
        break;
    case AXObjectCache::AXNotification::AXRowCountChanged:
        stream << "AXRowCountChanged";
        break;
    case AXObjectCache::AXNotification::AXRowCollapsed:
        stream << "AXRowCollapsed";
        break;
    case AXObjectCache::AXNotification::AXRowExpanded:
        stream << "AXRowExpanded";
        break;
    case AXObjectCache::AXNotification::AXExpandedChanged:
        stream << "AXExpandedChanged";
        break;
    case AXObjectCache::AXNotification::AXInvalidStatusChanged:
        stream << "AXInvalidStatusChanged";
        break;
    case AXObjectCache::AXNotification::AXPressDidSucceed:
        stream << "AXPressDidSucceed";
        break;
    case AXObjectCache::AXNotification::AXPressDidFail:
        stream << "AXPressDidFail";
        break;
    case AXObjectCache::AXNotification::AXPressedStateChanged:
        stream << "AXPressedStateChanged";
        break;
    case AXObjectCache::AXNotification::AXReadOnlyStatusChanged:
        stream << "AXReadOnlyStatusChanged";
        break;
    case AXObjectCache::AXNotification::AXRequiredStatusChanged:
        stream << "AXRequiredStatusChanged";
        break;
    case AXObjectCache::AXNotification::AXSortDirectionChanged:
        stream << "AXSortDirectionChanged";
        break;
    case AXObjectCache::AXNotification::AXTextChanged:
        stream << "AXTextChanged";
        break;
    case AXObjectCache::AXNotification::AXElementBusyChanged:
        stream << "AXElementBusyChanged";
        break;
    case AXObjectCache::AXNotification::AXDraggingStarted:
        stream << "AXDraggingStarted";
        break;
    case AXObjectCache::AXNotification::AXDraggingEnded:
        stream << "AXDraggingEnded";
        break;
    case AXObjectCache::AXNotification::AXDraggingEnteredDropZone:
        stream << "AXDraggingEnteredDropZone";
        break;
    case AXObjectCache::AXNotification::AXDraggingDropped:
        stream << "AXDraggingDropped";
        break;
    case AXObjectCache::AXNotification::AXDraggingExitedDropZone:
        stream << "AXDraggingExitedDropZone";
        break;
    }

    return stream;
}

TextStream& operator<<(TextStream& stream, const AXCoreObject& object)
{
    constexpr OptionSet<AXStreamOptions> options = { AXStreamOptions::ObjectID, AXStreamOptions::Role, AXStreamOptions::ParentID, AXStreamOptions::IdentifierAttribute, AXStreamOptions::OuterHTML, AXStreamOptions::DisplayContents, AXStreamOptions::Address };
    streamAXCoreObject(stream, object, options);
    return stream;
}

#if ENABLE(ACCESSIBILITY_ISOLATED_TREE)
TextStream& operator<<(TextStream& stream, AXIsolatedTree& tree)
{
    ASSERT(!isMainThread());
    TextStream::GroupScope groupScope(stream);
    stream << "treeID " << tree.treeID();
    stream.dumpProperty("rootNodeID", tree.rootNode()->objectID());
    stream.dumpProperty("focusedNodeID", tree.m_focusedNodeID);
    constexpr OptionSet<AXStreamOptions> options = { AXStreamOptions::ObjectID, AXStreamOptions::Role, AXStreamOptions::ParentID, AXStreamOptions::IdentifierAttribute, AXStreamOptions::OuterHTML, AXStreamOptions::DisplayContents, AXStreamOptions::Address };
    streamSubtree(stream, tree.rootNode(), options);
    return stream;
}

void streamIsolatedSubtreeOnMainThread(TextStream& stream, const AXIsolatedTree& tree, AXID objectID, const OptionSet<AXStreamOptions>& options)
{
    ASSERT(isMainThread());

    if (!shouldLog())
        return;

    stream.increaseIndent();
    TextStream::GroupScope groupScope(stream);

    if (options & AXStreamOptions::ObjectID)
        stream << "objectID " << objectID;

    auto ids = tree.m_nodeMap.get(objectID);
    if (options & AXStreamOptions::ParentID)
        stream.dumpProperty("parentObject", ids.parentID);

    for (auto& childID : ids.childrenIDs)
        streamIsolatedSubtreeOnMainThread(stream, tree, childID, options);

    stream.decreaseIndent();
}
#endif

TextStream& operator<<(TextStream& stream, AXObjectCache& axObjectCache)
{
    TextStream::GroupScope groupScope(stream);
    stream << "AXObjectCache " << &axObjectCache;

    if (auto* root = axObjectCache.get(axObjectCache.document().view())) {
        constexpr OptionSet<AXStreamOptions> options = { AXStreamOptions::ObjectID, AXStreamOptions::Role, AXStreamOptions::ParentID, AXStreamOptions::IdentifierAttribute, AXStreamOptions::OuterHTML, AXStreamOptions::DisplayContents, AXStreamOptions::Address };
        streamSubtree(stream, root, options);
    } else
        stream << "No root!";

    return stream;
}

void streamAXCoreObject(TextStream& stream, const AXCoreObject& object, const OptionSet<AXStreamOptions>& options)
{
    if (options & AXStreamOptions::ObjectID)
        stream << "objectID " << object.objectID();

    if (options & AXStreamOptions::Role)
        stream.dumpProperty("role", object.roleValue());

    if (options & AXStreamOptions::ParentID) {
        auto* parent = object.parentObjectUnignored();
        stream.dumpProperty("parentObject", parent ? parent->objectID() : AXID());
    }

    if (options & AXStreamOptions::IdentifierAttribute)
        stream.dumpProperty("identifierAttribute", object.identifierAttribute());

    if (options & AXStreamOptions::OuterHTML) {
        auto role = object.roleValue();
        auto* objectWithInterestingHTML = role == AccessibilityRole::Button ? // Add here other roles of interest.
            &object : nullptr;

        auto* parent = object.parentObjectUnignored();
        if (role == AccessibilityRole::StaticText && parent)
            objectWithInterestingHTML = parent;

        if (objectWithInterestingHTML)
            stream.dumpProperty("outerHTML", objectWithInterestingHTML->outerHTML().left(150));
    }

    if (options & AXStreamOptions::DisplayContents) {
        if (auto* axObject = dynamicDowncast<AccessibilityObject>(&object); axObject && axObject->hasDisplayContents())
            stream.dumpProperty("hasDisplayContents", true);
    }

    if (options & AXStreamOptions::Address) {
        stream.dumpProperty("address", &object);
        stream.dumpProperty("wrapper", object.wrapper());
    }
}

void streamSubtree(TextStream& stream, const RefPtr<AXCoreObject>& object, const OptionSet<AXStreamOptions>& options)
{
    if (!object || !shouldLog())
        return;

    stream.increaseIndent();

    TextStream::GroupScope groupScope(stream);
    streamAXCoreObject(stream, *object, options);
    for (auto& child : object->children(false))
        streamSubtree(stream, child, options);

    stream.decreaseIndent();
}

} // namespace WebCore
