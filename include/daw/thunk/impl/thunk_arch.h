// Copyright (c) Darrell Wright
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/beached/daw_thunk
//

#pragma once

/// \brief Theses constants will be the value of DAW_THUNK_ARCH
#define DAW_THUNK_ARCH_C_sysv_x86 0
#define DAW_THUNK_ARCH_C_sysv_x64 1
#define DAW_THUNK_ARCH_C_win_x86 2
#define DAW_THUNK_ARCH_C_win_x64 3
#define DAW_THUNK_ARCH_C_aapcs64 4

#if not defined( DAW_THUNK_ARCH )
#if defined( _WIN64 )
#if defined( __MINGW64__ )
#define DAW_THUNK_ARCH DAW_THUNK_ARCH_C_sysv_x64
#define DAW_THUNK_ARCH_sysv_x64
#else
#define DAW_THUNK_ARCH DAW_THUNK_ARCH_C_win_x64
#define DAW_THUNK_ARCH_win_x64
#endif
#elif defined( _WIN32 )
#if defined( __MINGW32__ )
#define DAW_THUNK_ARCH DAW_THUNK_ARCH_C_sysv_x86
#define DAW_THUNK_ARCH_sysv_x86
#else
#define DAW_THUNK_ARCH DAW_THUNK_ARCH_C_win_x86
#define DAW_THUNK_ARCH_win_x86
#endif
#elif defined( _M_ARM64 )
// Windows on Arm64
#error Architecture unsupported
#elif defined( __GNUC__ )
#if defined( __x86_64__ )
#define DAW_THUNK_ARCH DAW_THUNK_ARCH_C_sysv_x64
#define DAW_THUNK_ARCH_sysv_x64
#elif defined( __i386__ )
#define DAW_THUNK_ARCH DAW_THUNK_ARCH_C_sysv_x86
#define DAW_THUNK_ARCH_sysv_x86
#elif defined( __aarch64__ )
#define DAW_THUNK_ARCH DAW_THUNK_ARCH_C_aapcs64
#define DAW_THUNK_ARCH_aapcs64
#endif
#endif
#endif

#if not defined( DAW_THUNK_ARCH )
#error Unsupported Architecture
#endif
