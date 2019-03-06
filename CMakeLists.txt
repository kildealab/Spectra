#-------------------------------------------------------------------------------
# Setup the project
cmake_minimum_required(VERSION 2.6 FATAL_ERROR)
project(spectra)

#----------------------------------------------------------------------------
# Build plot program if ROOT built with GUI via CMake is found
#
find_package(ROOT COMPONENTS Gui QUIET)
#message(STATUS ROOT_USE_FILE " ${ROOT_USE_FILE}")
if(ROOT_FOUND AND ROOT_USE_FILE)
  add_subdirectory(plot)
else()
  message("-- This application requires ROOT built with GUI via CMake, plot is disabled")
endif()

# Find Geant4 package, activating all available Vis drivers by default
# You can set WITH_GEANT4_VIS to OFF via the command line or ccmake/cmake-gui
# to build a batch mode only executable
#
option(WITH_GEANT4_VIS "Build example with Geant4 Vis drivers" ON)
if(WITH_GEANT4_VIS)
  find_package(Geant4 REQUIRED vis_all)
else()
  find_package(Geant4 REQUIRED)
endif()

#-------------------------------------------------------------------------------
# Setup Geant4 include directories and compile definitions
#
include(${Geant4_USE_FILE})
include_directories(${PROJECT_SOURCE_DIR}/include)

#-------------------------------------------------------------------------------
# Locate sources and headers for this project
#
file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cc)
file(GLOB headers ${PROJECT_SOURCE_DIR}/include/*.hh)

#-------------------------------------------------------------------------------
# Add the executable, and link it to the Geant4 libraries
#
add_executable(${PROJECT_NAME} ${PROJECT_NAME}.cc ${sources} ${headers})
target_link_libraries(${PROJECT_NAME} ${Geant4_LIBRARIES} )

#-------------------------------------------------------------------------------
# Install the executable to 'bin' directory under CMAKE_INSTALL_PREFIX
#
install(TARGETS ${PROJECT_NAME} DESTINATION bin)

# -----------------------------------------------------------------------------
# --- Copy MAC files 
# ------------------------------------------------------------------------------
file(GLOB MAC_FILES ${PROJECT_SOURCE_DIR}/*.mac ${PROJECT_SOURCE_DIR}/*.in
    ${PROJECT_SOURCE_DIR}/*.sh ${PROJECT_SOURCE_DIR}/*.csv ${PROJECT_SOURCE_DIR}/*.py)
#${PROJECT_SOURCE_DIR}/*.sh
#   ${PROJECT_SOURCE_DIR}/*.csh)

foreach(_script ${MAC_FILES})
  configure_file(
    ${_script}
    ${PROJECT_BINARY_DIR}/.
    COPYONLY
    )
endforeach()



