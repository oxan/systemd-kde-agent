project(polkit-kde-1)

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules")

find_package(KDE4 REQUIRED)
include (KDE4Defaults)
include (MacroLibrary)
include (FindPkgConfig)


include_directories(${KDE4_INCLUDES})

add_subdirectory(agent)

#find_package(Msgfmt REQUIRED)
#find_package(Gettext REQUIRED)
#add_subdirectory( po )
#add_subdirectory( doc-translations )
