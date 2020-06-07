message("**** Creating target release_deb to generate .deb for ${PROJECT_NAME}, to use it type: make release_deb ****")

set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)
include (InstallRequiredSystemLibraries)

set(CPACK_PACKAGING_INSTALL_PREFIX "/opt/xbot")
set(CPACK_GENERATOR "DEB")
set(CPACK_PACKAGE_NAME ${PROJECT_NAME})

set(CPACK_PACKAGE_VERSION_MAJOR "1")
set(CPACK_PACKAGE_VERSION_MINOR "1")
set(CPACK_PACKAGE_VERSION_PATCH "10")

set(CPACK_DEBIAN_PACKAGE_ARCHITECTURE "amd64")
set(CPACK_PACKAGE_ARCHITECTURE "amd64")

set(CPACK_DEBIAN_PACKAGE_DEPENDS "")

set(CPACK_DEBIAN_PACKAGE_SHLIBDEPS ON)
set(CPACK_DEBIAN_PACKAGE_GENERATE_SHLIBS ON)

set(CPACK_DEBIAN_PACKAGE_MAINTAINER "luca.muratore@iit.it")
set(CPACK_DEBIAN_PACKAGE_DESCRIPTION "Cross-Robot API")
include(CPack)

set(DEB_FILE_NAME ${CPACK_PACKAGE_NAME}-${CPACK_PACKAGE_VERSION}-Linux.deb)
add_custom_target(release_deb 
                  COMMAND "${CMAKE_CPACK_COMMAND}"
                  COMMENT "Generating .deb using Cpack"
                  DEPENDS ${PROJECT_NAME}
                  )