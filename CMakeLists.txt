#
#  Copyright (C) 2016 IIT-ADVR
#  Author: Arturo Laurenzi, Luca Muratore
#  email: arturo.laurenzi@iit.it, luca.muratore@iit.it
#
#  This program is free software: you can redistribute it and/or modify
#  it under the terms of the GNU Lesser General Public License as published by
#  the Free Software Foundation, either version 3 of the License, or
#  (at your option) any later version.
#
#  This program is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
#  GNU Lesser General Public License for more details.
#
#  You should have received a copy of the GNU Lesser General Public License
#  along with this program. If not, see <http://www.gnu.org/licenses/>
#

cmake_minimum_required(VERSION 3.10)

include(ExternalProject)

project(ModelInterfaceRBDL VERSION 1.1.10)

LIST( APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake )

# C++ 11
set(CMAKE_CXX_STANDARD 20)

find_package(XBotInterface REQUIRED)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")
find_package(RBDL REQUIRED)

include_directories(include ${XBotInterface_INCLUDE_DIRS}
                            ${RBDL_INCLUDE_DIR}
                            ${RBDL_URDFReader_INCLUDE_DIR}
                            )

# for every file in INCLUDES CMake already sets the property HEADER_FILE_ONLY
file(GLOB_RECURSE ModelInterfaceRBDL_INCLUDES "${CMAKE_CURRENT_SOURCE_DIR}/include" *.h*)

add_library(ModelInterfaceRBDL SHARED ${ModelInterfaceRBDL_INCLUDES}
                                      src/ModelInterfaceRBDL.cpp)

target_link_libraries(ModelInterfaceRBDL
    PUBLIC
    ${XBotInterface_LIBRARIES}
    PRIVATE
    ${RBDL_LIBRARY}
    ${RBDL_URDFReader_LIBRARY})



########################################################################
install(TARGETS ModelInterfaceRBDL
    DESTINATION lib)

install(DIRECTORY include/
    DESTINATION include
    FILES_MATCHING PATTERN "*.h")

include(MacroInstallLib)

library_install(ModelInterfaceRBDL
    ${PROJECT_VERSION_MAJOR}
    ${PROJECT_VERSION_MINOR}
    ${PROJECT_VERSION_PATCH})

include(GenerateDeb)
