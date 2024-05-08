# Minimum cmake verison 3.1 required for the variable CMAKE_CXX_STANDARD
cmake_minimum_required(VERSION 3.1)


set(PROJECT_NAME GHOST-WCSim)

list(APPEND CMAKE_MODULE_PATH
        $ENV{HK_PILOT_DIR}/cmake
        )
include(GetVersionGit)
MESSAGE("Preparing project ${PROJECT_NAME} version ${PROJECT_VERSION}" )
project(${PROJECT_NAME} VERSION ${PROJECT_VERSION})
include(HKPackageBuilder)
hkbuilder_prepare_project()
hk_check_dependencies()

#add common functions directory
include_directories(src BEFORE)
add_subdirectory(src)

#add ABC tools
# This should really be improved to only be included for the inheriting tools, rather than all tools
include_directories(HKGFileWriterBase BEFORE)

# Add your tools here
hk_add_tool(WCSim_exe)
hk_add_tool(HKG4TrackingAction)
hk_add_tool(HKG4StackingAction)
hk_add_tool(HKG4PhysicsList)
hk_add_tool(HKG4SteppingAction)
hk_add_tool(HKG4PrimaryGeneratorAction)
hk_add_tool(HKG4EventAction)
hk_add_tool(HKG4RunAction)
hk_add_tool(HKGFileWriterBase)
hk_add_tool(HKGFileWriterRootWCSim)

option(WCSIM_Check_Geometry_Overlaps
  "Toggle WCSim to save photon scattering and reflection history"
  OFF
)
if(WCSIM_Check_Geometry_Overlaps STREQUAL ON)
  add_definitions(-DWCSIM_CHECK_GEOMETRY_OVERLAPS=1)
else()
  add_definitions(-DWCSIM_CHECK_GEOMETRY_OVERLAPS=0)  
endif()

hk_finalize_project()
