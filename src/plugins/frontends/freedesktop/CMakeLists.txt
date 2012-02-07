if(QT_QTDBUS_FOUND AND WITH_FREEDESKTOP_FRONTEND)
    message(STATUS "Adding freedesktop notification frontend")

    set( FREEDESKTOP_NOTIFICATION_FRONTEND_SRC
	    freedesktopnotificationfrontend.cpp
            ../../backends/freedesktop/fredesktopnotification.cpp
	)

    qt4_add_dbus_adaptor( FREEDESKTOP_NOTIFICATION_FRONTEND_SRC org.freedesktop.Notifications.xml freedesktopnotificationfrontend.h FreedesktopFrontend)

    automoc4_add_library(freedesktop MODULE  ${FREEDESKTOP_NOTIFICATION_FRONTEND_SRC} )
    target_link_libraries(freedesktop snorecore ${QT_QTDBUS_LIBRARY} ${QT_QTGUI_LIBRARY} )

    if(KDE4_FOUND)
            target_link_libraries(freedesktop   ${KDE4_KDEUI_LIBRARY})
    endif(KDE4_FOUND)


    #install the dbus interface
        if(WIN32)
            set(SNORE_LOCATION ../bin/snorenotify)
        else(WIN32)
                set(SNORE_LOCATION ${CMAKE_INSTALL_PREFIX}/bin/snorenotify)
        endif(WIN32)
    configure_file(${CMAKE_CURRENT_SOURCE_DIR}/org.freedesktop.Notifications.service.cmake ${CMAKE_CURRENT_BINARY_DIR}/org.freedesktop.Notifications.service)

    install(FILES ${CMAKE_CURRENT_BINARY_DIR}/org.freedesktop.Notifications.service
                              DESTINATION share/dbus-1/services/)

    install(TARGETS freedesktop ${SNORE_FRONTEND_INSTALL_PATH})

endif(QT_QTDBUS_FOUND AND WITH_FREEDESKTOP_FRONTEND)


