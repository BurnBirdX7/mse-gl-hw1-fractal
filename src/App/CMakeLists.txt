set(SRCS
    main.cpp
        MandelbrotWindow.cpp
        MandelbrotWindow.h
        SettingsWindow.cpp
        SettingsWindow.h

    shaders.qrc
    Shaders/mandelbrot.fs
    Shaders/mandelbrot.vs
)

find_package(Qt5 COMPONENTS Widgets REQUIRED)

add_executable(fractal ${SRCS})

target_link_libraries(fractal
    PRIVATE
        Qt5::Widgets
        FGL::Base
)