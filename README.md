quiGLy - Quickly prototype OpenGL
=================================

About
-----

quiGLy was initiated as a students project at Westfälische Wilhelms-Universität Münster in 2015.
The application allows for any kind of prototyping related to OpenGL, be it shader programming or context configuration.
It might actually be quite useful for students or people who want to or need to learn OpenGL and related concepts like:
* Pipeline
* Context state
* Objects (Buffers, Textures, ..)
* Binding
* etc..

Currently, no documentation is provided but will be added as soon as possible.
Meanwhile, you might have a look at the provided project wizard, available from the toolbar.
It might help you configuring your own pipelines.

### NOTE: quiGLy is not well-tested and is still very much in development stage.

## Usage

Once quiGLy has been started, the application shows a single view with several components.
Application images and documentation will follow soon.

## Build

Required dependencies:
* Qt 5.5 or higher, modules: Core, Gui, Widgets, Xml (https://www.qt.io/)
* libassimp (http://assimp.org/)
* zlib (https://zlib.net/)

Included libraries:
* GLI (https://gli.g-truc.net/0.8.2/index.html)
* GLM (https://glm.g-truc.net/0.9.9/index.html)
* minizip (http://www.winimage.com/zLibDll/minizip.html)

The project was developed on and therefore should work on GNU/Linux, Windows as well as macOS.
Simply install the required dependencies and run the cmake script.
