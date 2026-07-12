project(soundkonverter
    VERSION 3.0.1
    LANGUAGES C CXX
    DESCRIPTION "soundKonverter is a frontend to various audio converters.")

cmake_minimum_required(VERSION 3.3.0)

# version.h.in -> version.h using project VERSION
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/version.h.in ${CMAKE_CURRENT_SOURCE_DIR}/version.h @ONLY)

set(CMAKE_CXX_STANDARD 11)

# show more warnings
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
# strict ISO C and ISO C++
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pedantic")
# use c++11 standard
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
# implicit conversions
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wconversion")
# implicit conversions from signed to unsigned
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wsign-conversion")

# hide deprecated declarations warnings
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-deprecated-declarations")

# kde libs
find_package(ECM REQUIRED NO_MODULE)
set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules ${ECM_MODULE_PATH})

include(KDEInstallDirs)
include(KDECompilerSettings)
include(KDECMakeSettings)
include(CMakePackageConfigHelpers)
include(ECMInstallIcons)
include(FeatureSummary)

set(QT_DEFAULT_MAJOR_VERSION "6" CACHE STRING "Qt Version defaults to 6")
set(KDE_DEFAULT_MAJOR_VERSION "6" CACHE STRING "KDE Frameworks Version defaults to 6")

if (NOT ${QT_DEFAULT_MAJOR_VERSION} STREQUAL ${KDE_DEFAULT_MAJOR_VERSION})
    message(FATAL_ERROR "Qt and KDE Major versions must match: Qt${QT_DEFAULT_MAJOR_VERSION}/KDE${KDE_DEFAULT_MAJOR_VERSION}")
endif()

find_package(Qt${QT_DEFAULT_MAJOR_VERSION} NAMES Qt5 Qt6 REQUIRED COMPONENTS
    Core
    Widgets
    Xml
)



if(${QT_DEFAULT_MAJOR_VERSION} STREQUAL "6")
    find_package(Qt6 REQUIRED COMPONENTS
        Core5Compat
    )
elseif(${QT_DEFAULT_MAJOR_VERSION} STREQUAL "5")
    find_package(Phonon4Qt5 REQUIRED)  # Else we need something else
endif()

if(${KDE_DEFAULT_MAJOR_VERSION} STREQUAL "5")
    find_package(KDELibs4Support)
endif()

find_package(KF${KDE_DEFAULT_MAJOR_VERSION} REQUIRED COMPONENTS I18n XmlGui KIO Solid Config ConfigWidgets WidgetsAddons)
find_package(KF${KDE_DEFAULT_MAJOR_VERSION}Cddb)


# additional libs
find_package(Cdparanoia REQUIRED)

set(TAGLIB_MIN_VERSION "1.10")
find_package(Taglib ${TAGLIB_MIN_VERSION} REQUIRED)


# plugins 
add_subdirectory(plugins)


# include directories
include_directories(${CDPARANOIA_INCLUDE_DIR})


# translation
include(ECMOptionalAddSubdirectory)
ecm_optional_add_subdirectory( po )


# soundkonvertercore
set(soundkonvertercore_SRCS
   core/backendplugin.cpp
   core/conversionoptions.cpp
   core/codecwidget.cpp
   core/codecplugin.cpp
   core/filterplugin.cpp
   core/replaygainplugin.cpp
   core/ripperplugin.cpp
)

add_library(soundkonvertercore SHARED ${soundkonvertercore_SRCS})
target_link_libraries(soundkonvertercore KF5::WidgetsAddons KF5::KIOFileWidgets KF5::KIOCore KF5::KDELibs4Support)
target_link_libraries(${PROJECT_NAME}
    Qt${QT_DEFAULT_MAJOR_VERSION}::Core
    Qt${QT_DEFAULT_MAJOR_VERSION}::Widgets
    Qt${QT_DEFAULT_MAJOR_VERSION}::Xml
)
if(${QT_DEFAULT_MAJOR_VERSION} STREQUAL "6")
    target_link_libraries(${PROJECT_NAME}
        Qt6::Core5Compat
    )
endif()
install(TARGETS soundkonvertercore DESTINATION ${LIB_INSTALL_DIR})


# soundkonverter
set(soundkonverter_SRCS
   configdialog/configdialog.cpp
   configdialog/configpagebase.cpp
   configdialog/configgeneralpage.cpp
   configdialog/configbackendspage.cpp
   configdialog/configadvancedpage.cpp
   configdialog/configcoverartpage.cpp
   metadata/tagengine.cpp
   metadata/MetaReplayGain.cpp
   opener/fileopener.cpp
   opener/diropener.cpp
   opener/cdopener.cpp
   opener/urlopener.cpp
   opener/playlistopener.cpp
   replaygainscanner/replaygainscanner.cpp
   replaygainscanner/replaygainfilelist.cpp
   replaygainscanner/replaygainfilelistitem.cpp
   replaygainscanner/replaygainprocessor.cpp
   global.cpp
   main.cpp
   soundkonverter.cpp
   soundkonverterapp.cpp
   soundkonverterview.cpp
   codecproblems.cpp
   codecoptimizations.cpp
   combobutton.cpp
   config.cpp
   conversionoptionsmanager.cpp
   convert.cpp
   convertitem.cpp
   filelist.cpp
   filelistitem.cpp
   logger.cpp
   logviewer.cpp
   options.cpp
   optionssimple.cpp
   optionsdetailed.cpp
   optionseditor.cpp
   optionslayer.cpp
   pluginloader.cpp
   progressindicator.cpp
   outputdirectory.cpp
   aboutplugins.cpp
)

add_executable(soundkonverter ${soundkonverter_SRCS})
target_link_libraries(soundkonverter KF5::WidgetsAddons KF5::KIOFileWidgets KF5::KIOCore KF5::Solid KF5::Cddb Phonon::phonon4qt5 Taglib::Taglib ${CDPARANOIA_LIBRARIES} KF5::KDELibs4Support soundkonvertercore)

install(TARGETS soundkonverter DESTINATION ${BIN_INSTALL_DIR})


# install directories
install(FILES soundkonverter.desktop DESTINATION ${XDG_APPS_INSTALL_DIR})
install(FILES soundkonverterui.rc DESTINATION ${KXMLGUI_INSTALL_DIR}/soundkonverter)

install(FILES soundkonverter_codecplugin.desktop DESTINATION ${SERVICETYPES_INSTALL_DIR})
install(FILES soundkonverter_filterplugin.desktop DESTINATION ${SERVICETYPES_INSTALL_DIR})
install(FILES soundkonverter_replaygainplugin.desktop DESTINATION ${SERVICETYPES_INSTALL_DIR})
install(FILES soundkonverter_ripperplugin.desktop DESTINATION ${SERVICETYPES_INSTALL_DIR})
install(FILES soundkonverter-rip-audiocd.desktop DESTINATION ${DATA_INSTALL_DIR}/solid/actions)

install(FILES soundkonverter.appdata.xml DESTINATION ${SHARE_INSTALL_PREFIX}/metainfo)

add_subdirectory(icons)
add_subdirectory(images)


# summary
feature_summary(WHAT ALL FATAL_ON_MISSING_REQUIRED_PACKAGES)
