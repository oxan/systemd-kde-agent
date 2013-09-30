set(systemd_agent_SRCS
    main.cpp
    SystemdKDEAgent.cpp
    AuthDialog.cpp
)

kde4_add_ui_files(systemd_agent_SRCS AuthDialog.ui)

kde4_add_executable(systemd-kde-agent ${systemd_agent_SRCS})

target_link_libraries(systemd-kde-agent ${KDE4_KDEUI_LIBS})

configure_file(systemd-kde-agent.desktop.in ${CMAKE_BINARY_DIR}/systemd-kde-agent.desktop)

install(TARGETS systemd-kde-agent DESTINATION ${LIBEXEC_INSTALL_DIR})

install(FILES ${CMAKE_BINARY_DIR}/systemd-kde-agent DESTINATION ${AUTOSTART_INSTALL_DIR})