# Copyright (c) Darrell Wright
#
# Distributed under the Boost Software License, Version 1.0. (See accompanying
# file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt)
#
# Official repository: https://github.com/beached/daw_thunk
#

include( FetchContent )

set( DAW_ENABLE_TESTING OFF )

FetchContent_Declare(
        header_libraries
        GIT_REPOSITORY https://github.com/beached/header_libraries.git
        GIT_TAG v2
)

FetchContent_MakeAvailable( header_libraries )

