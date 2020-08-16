cmake_minimum_required(VERSION 3.12.4)

project (helloworld)

option(BUILD_BINARIES "Build the binaries in {project}/bin directory" ON)
option(BUILD_STATIC_LIBS "Build the static library" ON)
option(BUILD_SHARED_LIBS "Build the shared library" ON)
option(BUILD_TEST "Whether or not build the test cases" OFF)


set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# dir to include
set(HELLOWORLD_INCLUDE_DIRECTORIES include)

add_compile_options("-Wall" "-W" "-Wextra" "-fPIC")

# headers to install
file(GLOB_RECURSE HELLOWORLD_PUBLIC_HEADER include/*)

# source for the lib
file(GLOB_RECURSE HELLOWORLD_SOURCE src/*)
add_library(helloworld_obj OBJECT ${HELLOWORLD_SOURCE})
target_include_directories(helloworld_obj PUBLIC ${HELLOWORLD_INCLUDE_DIRECTORIES})

if (${BUILD_BINARIES})
    add_subdirectory(bin)
endif()

if (${BUILD_STATIC_LIBS})
    add_library(helloworld-static STATIC $<TARGET_OBJECTS:helloworld_obj>)

    # install instructions
    set_target_properties(helloworld-static PROPERTIES PUBLIC_HEADER ${HELLOWORLD_PUBLIC_HEADER})
    install(TARGETS helloworld-static PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_PREFIX}/include/helloworld)
endif()

if (${BUILD_SHARED_LIBS})
    add_library(helloworld-dynamic SHARED $<TARGET_OBJECTS:helloworld_obj>)

    # install instructions
    set_target_properties(helloworld-dynamic PROPERTIES PUBLIC_HEADER ${HELLOWORLD_PUBLIC_HEADER})
    install(TARGETS helloworld-dynamic PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_PREFIX}/include/helloworld)
endif()

if (${BUILD_TEST})
    # Download and unpack googletest at configure time
    configure_file(CMakeLists.txt.in googletest-download/CMakeLists.txt)
    execute_process(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
        RESULT_VARIABLE result
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/googletest-download )
    if(result)
        message(FATAL_ERROR "CMake step for googletest failed: ${result}")
    endif()
    execute_process(COMMAND ${CMAKE_COMMAND} --build .
        RESULT_VARIABLE result
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/googletest-download )
    if(result)
        message(FATAL_ERROR "Build step for googletest failed: ${result}")
    endif()

    # Prevent overriding the parent project's compiler/linker
    # settings on Windows
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

    # Add googletest directly to our build. This defines
    # the gtest and gtest_main targets.
    add_subdirectory(${CMAKE_CURRENT_BINARY_DIR}/googletest-src
                    ${CMAKE_CURRENT_BINARY_DIR}/googletest-build
                    EXCLUDE_FROM_ALL)

    # The gtest/gtest_main targets carry header search path
    # dependencies automatically when using CMake 2.8.11 or
    # later. Otherwise we have to add them here ourselves.
    if (CMAKE_VERSION VERSION_LESS 2.8.11)
        include_directories("${gtest_SOURCE_DIR}/include")
    endif()

    # Now simply link against gtest or gtest_main as needed. Eg
    add_executable(hello_test test/test.cpp)
    target_link_libraries(hello_test gtest_main helloworld_obj)
    target_include_directories(hello_test PRIVATE ${HELLOWORLD_INCLUDE_DIRECTORIES})
    add_test(NAME example_test COMMAND hello_test)
endif()