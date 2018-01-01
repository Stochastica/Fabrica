cmake_minimum_required(VERSION 3.5)

# Threading
find_package(Threads REQUIRED)

# Boost
find_package(Boost 1.61.0 COMPONENTS
	filesystem
	program_options
	system
	thread
	timer
	REQUIRED)
if (${Boost_FOUND})
	include_directories(${Boost_INCLUDE_DIRS})
	link_directories(${Boost_LIBRARY_DIR})
endif()

# OpenGL
find_package(OpenGL REQUIRED)
if (${OPENGL_FOUND})
	include_directories(${OPENGL_INCLUDE_DIR})
	link_directories(${OPENGL_LIBRARIES})
endif()

# GLFW
find_package(glfw3 REQUIRED)

# GLEW
find_package(GLEW REQUIRED)
if (${GLEW_FOUND})
	include_directories(${GLEW_INCLUDE_DIRS})
endif()

# GLM
find_package(glm REQUIRED)
if (${GLM_FOUND})
	include_directories(${GLM_INCLUDE_DIRS})
endif()

# PNG
find_package(PNG REQUIRED)
if (${PNG_FOUND})
	include_directories(${PNG_INCLUDE_DIRS})
endif()

# FreeType
find_package(Freetype REQUIRED)
if (${FREETYPE_FOUND})
	include_directories(${FREETYPE_INCLUDE_DIRS})
endif()

# Eigen3
find_package(Eigen3 REQUIRED)
include_directories(${EIGEN3_INCLUDE_DIR})

# Automatically symlink the assets folder
add_custom_target(ClientSymlinks
	COMMAND ${CMAKE_COMMAND}
	-E create_symlink
	${CMAKE_CURRENT_SOURCE_DIR}/assets
	${EXECUTABLE_OUTPUT_PATH}/assets
	)

# Sources for both the client and the server
set(SourceCommon
	main.cpp
	api/stateevents/StateEventInit.cpp
	block/Block.cpp
	common/Configuration.cpp
	common/ModuleLoader.cpp
	server/LogicRegistry.cpp
	core/Fabrica.cpp
	util/vector.cpp
	world/chunk/Chunk.cpp
	world/Universe.cpp
	world/World.cpp
	test/test.cpp
	test/common/testInit.cpp
	test/common/testUtil.cpp
	)
# Sources for the client
set(SourceClient
	client/Camera.cpp
	client/Client.cpp
	client/ClientConfig.cpp
	client/DebugScreen.cpp
	client/RenderingRegistry.cpp
	client/Window.cpp
	client/WorldRenderer.cpp
	client/gui/GUIBase.cpp
	client/renderer/block/RenderBlock.cpp
	client/renderer/Font.cpp
	client/renderer/Text.cpp
	client/renderer/TextureAtlas.cpp
	client/renderer/TextureManager.cpp
	client/renderer/PerformanceMonitor.cpp
	client/renderer/utils.cpp
	test/client/initialisation.cpp
	test/client/rendering.cpp
	test/client/testClient.cpp
	)
# Sources for the standalone server
set(SourceServer
	server/standalone/ServerStandalone.cpp
	test/server/testServer.cpp
	)

# Client part
add_executable(Client ${SourceCommon} ${SourceClient})
# Allow modules to call non-inline functions in Fabrica
set_target_properties(Client PROPERTIES
	ENABLE_EXPORTS on)
add_dependencies(Client ClientSymlinks)
target_link_libraries(Client ${CMAKE_DL_LIBS})
target_link_libraries(Client ${CMAKE_THREAD_LIBS_INIT})
target_link_libraries(Client ${Boost_LIBRARIES})
target_link_libraries(Client
	${OPENGL_LIBRARIES}
	glfw
	${GLEW_LIBRARIES}
	${FREETYPE_LIBRARIES}
	${PNG_LIBRARIES}
	)
if (UNIX)
	target_link_libraries(Client m)
endif()
target_compile_features(Client PRIVATE
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

# Server part
add_executable(Server ${SourceCommon} ${SourceServer})
target_compile_definitions(Server PRIVATE FABRICA_SERVER_STANDALONE)
set_target_properties(Server PROPERTIES
	ENABLE_EXPORTS on)
target_link_libraries(Server ${CMAKE_DL_LIBS})
target_link_libraries(Server ${CMAKE_THREAD_LIBS_INIT})
target_link_libraries(Server ${Boost_LIBRARIES})
if (UNIX)
	target_link_libraries(Server m)
endif()
target_compile_features(Server PRIVATE
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

