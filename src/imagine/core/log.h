/*
 Imagine v0.1
 [core]
 Copyright (c) 2015-present, Hugo (hrkz) Frezat
*/

#ifndef IG_CORE_LOG_H
#define IG_CORE_LOG_H

#include "imagine/core/log/mgr.h"
#include "imagine/core/log/rec.h"
#include "imagine/core/log/sink.h"

#define log__(type, ...) ig::log_mgr::get().write(type, /*__LINE__, __FILE__, __func__,*/ __VA_ARGS__)


#define log_(type, fmt, ...) \
        log__(type, fmt "\n", /*__VA_OPT__(*/__VA_ARGS__/*), 0*/)
#define upt_(type, fmt, ...) \
        log__(type, fmt "\r", /*__VA_OPT__(*/__VA_ARGS__/*), 0*/)

#endif // IG_CORE_LOG_H
