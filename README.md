Introduction
============
This is a simple OpenGL demo showing dragons carrying letters and generally having fun!

Building
=======
This demo can be used at least on Windows and Linux. The CMake build system is used to build this project on these platforms using the same build scripts.

Dependencies
------------

* CMake 2.8+
* Qt 4.6+
* GLEW
* TIFF
* OpenGL 2.0 drivers

Building on Windows
-------------------
Microsoft Visual Stdio 2008/2010 (Expression editions should work fine too) is required to build the demo on Windows. CMake is used to generate Visual Studio solution and project files. It can be downloaded at http://www.cmake.org/cmake/resources/software.html.

Other dependencies to install:

* Qt at http://qt-project.org/downloads (choose the online installer, make sure the Qt libraries match your version of Visual Studio).
* GLEW at http://downloads.sourceforge.net/project/glew/glew/1.9.0/glew-1.9.0-win32.zip (save in the 'dragon-demo/depends' folder).
* TIFF at http://sourceforge.net/projects/gnuwin32/files/tiff/3.8.2-1/tiff-3.8.2-1.exe/download (save in the 'dragon-demo/depends' folder and run the installer).

Generating the Visual Studio solution for the demo takes a little while but only needs to be done once.  First, start cmake-gui which can be found in the Start menu. Click on the Browse Source button and select the 'dragon-demo' folder. Then, click on Browse build, make a 'build' folder in the 'dragon-demo' folder and select it. Finally, click on the Configure button.

A window will pop up asking which generator to use. Visual Studio 9 or 10 should be selected here, then click Finish. This may take a minute and will end up in an error pop-up.

Click on the Grouped check box to reduce the number of list entries. Then click on Ungrouped Entries and click on the value to the right of QT_MAKE_EXECUTABLE. Use the '...' button to the right and locate the qmake.exe file (mine is installed at: C:\QtSDK\Desktop\Qt\4.8.1\msvc2010\bin). Once this is done, click Configure again.

Finally, click configure. After this last step there should not be any error remaining. Click Generate to create the Visual Studio solution in your build foler.

In Visual Studio, set DragonDemo as startup project and build the solution. To run it, you will need to copy several .dll files to the output folder (dragon-demo\build\bin\Debug):

* QtCore4d.dll, QtGui4d.dll, QtOpenGL4d.dll and QtNetwork4d.dll from C:\QtSDK\Desktop\Qt\4.8.1\msvc2010\bin\

At this point you can finally hit F5 and run the demo!

Building on Linux
-----------------
Install CMake 2.8+, Qt 4.6+, glew and tiff using your distribution's package manager. Then, in a terminal:

    $ cd /path/to/dragon-demo
    $ make build
    $ cd build
    $ cmake-gui ..

Select the default project generator (Makefiles), click Finish, Configure and Generate. Close cmake-gui.

    $ make -j4
    $ bin/DragonDemo

If you want to look at the source or develop it on Linux I suggest using Qt Creator which has native support for CMake projects (File -> Open File/Project and select the top-level CMakeLists.txt file).
