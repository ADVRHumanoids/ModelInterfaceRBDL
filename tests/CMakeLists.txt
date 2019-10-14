cmake_minimum_required(VERSION 2.8.11)
include(ExternalProject)

set(PROJECTNAME ModelInterfaceRBDL_tests)
project(${PROJECTNAME})

find_package(urdfdom REQUIRED)



# compite GTest
find_package(GTest)
if (NOT GTEST_FOUND)
    set(gTestSource "/usr/src/gtest")
    ExternalProject_Add(GTest-ext SOURCE_DIR ${gTestSource}
                                  PREFIX "${CMAKE_CURRENT_BINARY_DIR}/external"
                                  INSTALL_COMMAND "")
    set(GTEST_LIB_DIRS "${CMAKE_CURRENT_BINARY_DIR}/external/src/GTest-ext-build/")
    set(GTEST_INCLUDE_DIRS ${gTestSource})
    set(GTEST_BOTH_LIBRARIES gtest gtest_main)
endif()

include_directories(${GTEST_INCLUDE_DIRS})
link_directories(${GTEST_LIB_DIRS})

# add local include
include_directories(${CMAKE_CURRENT_SOURCE_DIR} )
enable_testing()

add_custom_command( TARGET ModelInterfaceRBDL POST_BUILD
                    COMMAND ${CMAKE_CTEST_COMMAND}
                    MAIN_DEPENDENCY ModelInterfaceRBDL
                    DEPENDS
                    testURDFParser
                    )

# gTests TODO add libs
SET(TestLibs ModelInterfaceRBDL ${urdfdom_LIBRARIES} ${GTEST_BOTH_LIBRARIES})

add_definitions(-DMODEL_TESTS_ROBOTS_DIR="${CMAKE_CURRENT_BINARY_DIR}/robots/")


add_executable(testURDFParser     testURDFParser.cpp)
target_link_libraries(testURDFParser ${TestLibs})
add_dependencies(testURDFParser GTest-ext ModelInterfaceRBDL )
add_test(NAME model_interface_rbdl_test_urdf_parser COMMAND testURDFParser)

