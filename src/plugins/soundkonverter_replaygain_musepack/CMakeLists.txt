project(soundkonverter_replaygain_musepackgain)

set(${PROJECT_NAME}_SRCS
   soundkonverter_replaygain_musepackgain.cpp
)

add_library(${PROJECT_NAME} MODULE ${${PROJECT_NAME}_SRCS})
target_link_libraries(${PROJECT_NAME}
   KF${KDE_DEFAULT_MAJOR_VERSION}::WidgetsAddons
   soundkonvertercore)

########### install files ###############

install(TARGETS ${PROJECT_NAME} DESTINATION ${PLUGIN_INSTALL_DIR})
install(FILES ${PROJECT_NAME}.desktop DESTINATION ${SERVICES_INSTALL_DIR})
