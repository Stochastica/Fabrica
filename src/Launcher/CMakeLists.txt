cmake_minimum_required(VERSION 3.5)

set(StdFeatures
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
	)

# Do not remove the following auto-generated signatures

# Auto-generated. Do not edit. All changes will be undone
set(SourceFiles
	main.cpp
	)
# Auto-generated end

add_executable(Launcher ${SourceFiles})
target_compile_features(Launcher PRIVATE ${StdFeatures})

