project(soundkonverter_codec_flac)

set(${PROJECT_NAME}_SRCS
   soundkonverter_codec_flac.cpp
   flaccodecwidget.cpp
)

add_library(${PROJECT_NAME} MODULE ${${PROJECT_NAME}_SRCS})
target_link_libraries(${PROJECT_NAME}
   KF${KDE_DEFAULT_MAJOR_VERSION}::WidgetsAddons
   Qt${QT_DEFAULT_MAJOR_VERSION}::Xml soundkonvertercore)


########### install files ###############

install(TARGETS ${PROJECT_NAME} DESTINATION ${PLUGIN_INSTALL_DIR})
install(FILES soundkonverter_codec_flac.desktop DESTINATION ${SERVICES_INSTALL_DIR})
