/*
 * Copyright 2006 The Android Open Source Project
 *
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */


#include "SkEventSink.h"
// #include "SkMutex.h"
#include <mutex>

class SkEventSink_Globals {
public:
    SkEventSink_Globals() {
        fNextSinkID = 0;
        fSinkHead = nullptr;
    }

    // SkMutex         fSinkMutex;
    std::recursive_mutex    fSinkMutex;
    SkEventSinkID   fNextSinkID;
    SkEventSink*    fSinkHead;
};

static SkEventSink_Globals& getGlobals() {
    // leak this, so we don't incur any shutdown perf hit
    static SkEventSink_Globals* gGlobals = new SkEventSink_Globals;
    return *gGlobals;
}

SkEventSink::SkEventSink() {
    SkEventSink_Globals& globals = getGlobals();

    //globals.fSinkMutex.acquire();
    globals.fSinkMutex.lock();

    fID = ++globals.fNextSinkID;
    fNextSink = globals.fSinkHead;
    globals.fSinkHead = this;

    // globals.fSinkMutex.release();
    globals.fSinkMutex.unlock();
}

SkEventSink::~SkEventSink() {
    SkEventSink_Globals& globals = getGlobals();

    // globals.fSinkMutex.acquire();
    globals.fSinkMutex.lock();

    SkEventSink* sink = globals.fSinkHead;
    SkEventSink* prev = nullptr;

    for (;;) {
        SkEventSink* next = sink->fNextSink;
        if (sink == this) {
            if (prev) {
                prev->fNextSink = next;
            } else {
                globals.fSinkHead = next;
            }
            break;
        }
        prev = sink;
        sink = next;
    }
    // globals.fSinkMutex.release();
    globals.fSinkMutex.unlock();
}

bool SkEventSink::doEvent(const SkEvent& evt) {
    return this->onEvent(evt);
}

bool SkEventSink::doQuery(SkEvent* evt) {
    SkASSERT(evt);
    return this->onQuery(evt);
}

bool SkEventSink::onEvent(const SkEvent&) {
    return false;
}

bool SkEventSink::onQuery(SkEvent*) {
    return false;
}

///////////////////////////////////////////////////////////////////////////////

SkEventSink* SkEventSink::FindSink(SkEventSinkID sinkID) {
    if (sinkID == 0)
        return nullptr;

    SkEventSink_Globals&    globals = getGlobals();

    std::lock_guard<std::recursive_mutex> ac(globals.fSinkMutex);
    // SkAutoMutexAcquire      ac(globals.fSinkMutex);
    SkEventSink*            sink = globals.fSinkHead;

    while (sink) {
        if (sink->getSinkID() == sinkID)
            return sink;
        sink = sink->fNextSink;
    }
    return nullptr;
}
