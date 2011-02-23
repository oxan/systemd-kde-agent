set(systemd_SRCS
    systemdkde.cpp
    main.cpp
    AuthDialog.cpp
)

kde4_add_ui_files(systemd_SRCS AuthDialog.ui)

kde4_add_executable(systemd-kde-authentication-agent-1 ${systemd_SRCS})

target_link_libraries(systemd-kde-authentication-agent-1
                                         ${KDE4_KDEUI_LIBS}
)

#configure_file(systemd-kde-authentication-agent-1.desktop.in ${CMAKE_BINARY_DIR}/systemd-kde-authentication-agent-1.desktop)

#install(TARGETS systemd-kde-authentication-agent-1 DESTINATION ${LIBEXEC_INSTALL_DIR})

# Install .desktop file to the XDG_CONFIG_DIRS if exists, to the /etc/xdg/autostart otherwise
set (DESKTOP_INSTALL_DIR $ENV{XDG_CONFIG_DIRS})
if (DESKTOP_INSTALL_DIR)
else (DESKTOP_INSTALL_DIR)
  set (DESKTOP_INSTALL_DIR /etc/xdg/autostart)
endif (DESKTOP_INSTALL_DIR)

install(FILES ${CMAKE_BINARY_DIR}/systemd-kde-authentication-agent-1.desktop DESTINATION ${DESKTOP_INSTALL_DIR})
