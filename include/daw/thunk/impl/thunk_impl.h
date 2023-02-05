// Copyright (c) Darrell Wright
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/beached/daw_thunk
//

#pragma once

#include "thunk_arch.h"

#if DAW_THUNK_ARCH == DAW_THUNK_ARCH_C_sysv_x64
#include "thunk_sysv_x64.h"
#elif DAW_THUNK_ARCH == DAW_THUNK_ARCH_C_sysv_x86
#include "thunk_sysv_x86.h"
#elif DAW_THUNK_ARCH == DAW_THUNK_ARCH_C_win_x64
#include "thunk_win_x64.h"
#elif DAW_THUNK_ARCH == DAW_THUNK_ARCH_C_win_x86
#include "thunk_win_x86.h"
#elif DAW_THUNK_ARCH == DAW_THUNK_ARCH_C_aapcs64
#include "thunk_aapcs64.h"
#endif
