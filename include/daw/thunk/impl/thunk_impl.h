// Copyright (c) Darrell Wright
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/beached/daw_thunk
//

#pragma once

#if defined( __GNUC__ )
#if defined( __x86_64__ )
#include "thunk_sysv_x64.h"
#elif not defined( __i386__ )
#include "thunk_sysv_x86.h"
#else
#error Architecture unsupported
#endif
#elif defined( _WIN64 )
#include "thunk_win_x64.h"
#else
#error Architecture unsupported
#endif
