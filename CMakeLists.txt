cmake_minimum_required(VERSION 4.0)
project(ArchiViz)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)


find_package(Qt6 COMPONENTS
        Core
        Gui
        OpenGL
        OpenGLWidgets
        Widgets
        REQUIRED)

add_executable(ArchiViz main.cpp
        Cube.cpp
        Cube.h
        DrawingArea.cpp
        DrawingArea.h
        Shape.cpp
        Shape.h
        Sphere.cpp
        Sphere.h
        Pyramid.cpp
        Pyramid.h
        Cylinder.cpp
        Cylinder.h
        CompositeObject.cpp
        CompositeObject.h)
target_link_libraries(ArchiViz
        Qt::Core
        Qt::Gui
        Qt::Widgets
        Qt::OpenGL
        Qt::OpenGLWidgets
)

