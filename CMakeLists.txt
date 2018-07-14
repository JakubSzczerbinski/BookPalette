
project (FCL)
cmake_minimum_required(VERSION 3.0.0)

find_package(PkgConfig REQUIRED)

find_package (Boost 1.58 REQUIRED)
pkg_check_modules (cairo cairo REQUIRED)

include_directories(
       ${Boost_INCLUDE_DIRS}
       ${cairo_INCLUDE_DIRS}
       $ENV{HOME}/podofo/include
)

link_directories($ENV{HOME}/podofo/lib)

link_libraries(${Boost_LIBRARIES} ${cairo_LIBRARIES} podofo pthread freetype fontconfig jpeg z)

set(CMAKE_CXX_FLAGS "-std=c++14 -Wall -Wextra -Og -ggdb")

add_subdirectory(src)