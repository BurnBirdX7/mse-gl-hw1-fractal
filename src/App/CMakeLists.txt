set(SRCS
    main.cpp
        MandelbrotWindow.cpp
        MandelbrotWindow.h

    shaders.qrc
    Shaders/mandelbrot.fs
    Shaders/mandelbrot.vs
        SettingsWindow.cpp
        SettingsWindow.h
)

find_package(Qt5 COMPONENTS Widgets REQUIRED)

add_executable(demo-app ${SRCS})

target_link_libraries(demo-app
    PRIVATE
        Qt5::Widgets
        FGL::Base
)