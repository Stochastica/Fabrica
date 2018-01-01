cmake_minimum_required(VERSION 3.5)

# Required for developing plugins

find_package(Boost 1.61.0)
if (${Boost_FOUND})
	include_directories(${Boost_INCLUDE_DIRS})
	link_directories(${Boost_LIBRARY_DIR})
endif()

# Eigen3
find_package(Eigen3 REQUIRED)
include_directories(${EIGEN3_INCLUDE_DIR})

set(Source
	Module.cpp
	)

add_library(Industria SHARED ${Source})
set_target_properties(Industria PROPERTIES
	PREFIX ""
	LIBRARY_OUTPUT_DIRECTORY ${LIBRARY_OUTPUT_PATH}/Industria)
target_compile_features(Industria PRIVATE
	cxx_range_for
	cxx_noexcept
	cxx_rvalue_references
	cxx_override
	cxx_final
	cxx_deleted_functions
	cxx_delegating_constructors
	cxx_auto_type
	cxx_constexpr
	cxx_lambdas
	cxx_relaxed_constexpr
	)
if(UNIX)
	target_compile_options(Industria PRIVATE -fvisibility=hidden)
endif()

# Link this module against the Fabrica executable
target_link_libraries(Industria PUBLIC Client Server)
