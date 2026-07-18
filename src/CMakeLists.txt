project(soundkonverter
    VERSION 3.0.1
    LANGUAGES C CXX
    DESCRIPTION "soundKonverter is a frontend to various audio converters.")

cmake_minimum_required(VERSION 3.3.0)

# version.h.in -> version.h using project VERSION
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/version.h.in ${CMAKE_CURRENT_SOURCE_DIR}/version.h @ONLY)

set(CMAKE_CXX_STANDARD 11)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

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
    Multimedia
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
else()
    find_package(KC6)
    find_package(KCddb6 REQUIRED)  # Uses cmake/FindKCddb6.cmake
    find_package(KF6XmlGui REQUIRED)
endif()

find_package(KF${KDE_DEFAULT_MAJOR_VERSION} REQUIRED COMPONENTS
    I18n
    KCMUtils
    XmlGui
    KIO
    Solid
    Config
    ConfigWidgets
    WidgetsAddons
)


# additional libs
find_package(Cdparanoia REQUIRED)

set(TAGLIB_MIN_VERSION "1.10")
find_package(Taglib
${TAGLIB_MIN_VERSION} REQUIRED)


# plugins 
add_subdirectory(plugins)


# include directories
include_directories(${CDPARANOIA_INCLUDE_DIR})
include_directories(/usr/include BEFORE)

# translation
include(ECMOptionalAddSubdirectory)
ecm_optional_add_subdirectory( po )


# soundkonvertercore
set(${PROJECT_NAME}core_SRCS
   core/backendplugin.cpp
   core/conversionoptions.cpp
   core/codecwidget.cpp
   core/codecplugin.cpp
   core/filterplugin.cpp
   core/replaygainplugin.cpp
   core/ripperplugin.cpp
)

add_library(${PROJECT_NAME}core SHARED ${soundkonvertercore_SRCS})
target_link_libraries(${PROJECT_NAME}core
    KF${KDE_DEFAULT_MAJOR_VERSION}::WidgetsAddons
    KF${KDE_DEFAULT_MAJOR_VERSION}::KIOFileWidgets
    KF${KDE_DEFAULT_MAJOR_VERSION}::KIOCore
    KF${KDE_DEFAULT_MAJOR_VERSION}::KCMUtils
    KF${KDE_DEFAULT_MAJOR_VERSION}::XmlGui
    KF${KDE_DEFAULT_MAJOR_VERSION}::I18n
    KF${KDE_DEFAULT_MAJOR_VERSION}::Solid
    KF${KDE_DEFAULT_MAJOR_VERSION}::ConfigWidgets
)

if(${KDE_DEFAULT_MAJOR_VERSION} STREQUAL "5")
    find_package(KF${KDE_DEFAULT_MAJOR_VERSION}::KDELibs4Support)
endif()


target_link_libraries(${PROJECT_NAME}core
    Qt${QT_DEFAULT_MAJOR_VERSION}::Core
    Qt${QT_DEFAULT_MAJOR_VERSION}::Multimedia
    Qt${QT_DEFAULT_MAJOR_VERSION}::Widgets
    Qt${QT_DEFAULT_MAJOR_VERSION}::Xml
)
if(${QT_DEFAULT_MAJOR_VERSION} STREQUAL "6")
    target_link_libraries(${PROJECT_NAME}core
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

add_executable(${PROJECT_NAME} ${soundkonverter_SRCS})


target_link_libraries(${PROJECT_NAME}
    KF${KDE_DEFAULT_MAJOR_VERSION}::WidgetsAddons
    KF${KDE_DEFAULT_MAJOR_VERSION}::KIOFileWidgets
    KF${KDE_DEFAULT_MAJOR_VERSION}::KIOCore
    KF${KDE_DEFAULT_MAJOR_VERSION}::KCMUtils
    KF${KDE_DEFAULT_MAJOR_VERSION}::XmlGui
    KF${KDE_DEFAULT_MAJOR_VERSION}::I18n
    KF${KDE_DEFAULT_MAJOR_VERSION}::Solid
    KF${KDE_DEFAULT_MAJOR_VERSION}::ConfigWidgets
    Taglib::Taglib
    ${CDPARANOIA_LIBRARIES})


if(${KDE_DEFAULT_MAJOR_VERSION} STREQUAL "5")
    target_link_libraries(KF${KDE_DEFAULT_MAJOR_VERSION}::KDELibs4Support)
else()
    target_link_libraries(KF${KDE_DEFAULT_MAJOR_VERSION}::XmlGui)
    # This include_directories statement needs to be here since KCddb6 includes "kcddb6_version.h" (no prefix)
    # and the find_package() and target_link_libraries() fails to find it.
    include_directories(/usr/include/KCddb6 BEFORE)
    target_link_libraries(KCddb6)
endif()

if(${QT_DEFAULT_MAJOR_VERSION} STREQUAL "5")
    target_link_libraries(Phonon::phonon4qt5)
endif()

install(TARGETS ${PROJECT_NAME} DESTINATION ${BIN_INSTALL_DIR})


# install directories
install(FILES ${PROJECT_NAME}.desktop DESTINATION ${XDG_APPS_INSTALL_DIR})
install(FILES ${PROJECT_NAME}ui.rc DESTINATION ${KXMLGUI_INSTALL_DIR}/${PROJECT_NAME})

install(FILES ${PROJECT_NAME}_codecplugin.desktop DESTINATION ${SERVICETYPES_INSTALL_DIR})
install(FILES ${PROJECT_NAME}_filterplugin.desktop DESTINATION ${SERVICETYPES_INSTALL_DIR})
install(FILES ${PROJECT_NAME}_replaygainplugin.desktop DESTINATION ${SERVICETYPES_INSTALL_DIR})
install(FILES ${PROJECT_NAME}_ripperplugin.desktop DESTINATION ${SERVICETYPES_INSTALL_DIR})
install(FILES ${PROJECT_NAME}-rip-audiocd.desktop DESTINATION ${DATA_INSTALL_DIR}/solid/actions)

install(FILES ${PROJECT_NAME}.appdata.xml DESTINATION ${SHARE_INSTALL_PREFIX}/metainfo)

add_subdirectory(icons)
add_subdirectory(images)


# summary
feature_summary(WHAT ALL FATAL_ON_MISSING_REQUIRED_PACKAGES)
