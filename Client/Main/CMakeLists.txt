set(Bells_SRCS
  main.C
  MainWindow.C
  Settings.C
  About.C
  Help.C
 )

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)

find_package(Qt5Widgets)

qt5_wrap_ui(Bells_SRCS MainWindowBase.ui SettingsBase.ui AboutBase.ui HelpBase.ui)

qt5_add_resources(resources bells.qrc)

add_executable(bells ${Bells_SRCS} ${resources})

qt5_use_modules(bells Widgets WebEngine WebEngineWidgets Network)

target_link_libraries(bells Qt5::Widgets)



########### install files ###############

install(TARGETS bells DESTINATION /usr/local/bin)
