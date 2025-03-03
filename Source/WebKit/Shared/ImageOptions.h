/*
 * Copyright (C) 2010 Apple Inc. All rights reserved.
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

#ifndef ImageOptions_h
#define ImageOptions_h

namespace WebKit {

enum ImageOptions {
    ImageOptionsShareable = 1 << 0,
};

enum {
    SnapshotOptionsShareable = 1 << 0,
    SnapshotOptionsExcludeSelectionHighlighting = 1 << 1,
    SnapshotOptionsInViewCoordinates = 1 << 2,
    SnapshotOptionsPaintSelectionRectangle = 1 << 3,
    SnapshotOptionsExcludeDeviceScaleFactor = 1 << 5,
    SnapshotOptionsForceBlackText = 1 << 6,
    SnapshotOptionsForceWhiteText = 1 << 7,
    SnapshotOptionsPrinting = 1 << 8,
    SnapshotOptionsUseScreenColorSpace = 1 << 9,
    SnapshotOptionsVisibleContentRect = 1 << 10,
    SnapshotOptionsFullContentRect = 1 << 11,
    SnapshotOptionsTransparentBackground = 1 << 12
};
typedef uint32_t SnapshotOptions;

inline ImageOptions snapshotOptionsToImageOptions(SnapshotOptions snapshotOptions)
{
    unsigned imageOptions = 0;

    if (snapshotOptions & SnapshotOptionsShareable)
        imageOptions |= ImageOptionsShareable;

    return static_cast<ImageOptions>(imageOptions);
}

} // namespace WebKit

#endif // ImageOptions_h
