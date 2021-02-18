# OpenGL_Pathfinding
A* in C++ with OpenGL visualization

# Completed Things
  - A* algorithm complete
  - checking for valid grid
  - terminal visualization
  - Interactivity (choosing start/end node, wall placement)
  - Windows Compatibilty

# TODO
  - OpenGL integration

# Building
### Mac/Linux
From the root directory, run the following:
    
    ./scripts/build-clang.sh

Run the main executable:
    
    ./build/exe/A_Star.cc


### 64-bit Windows
Install mingw-w64 Version 7.3.0: http://mingw-w64.org/doku.php

Compile from the base directory using something like this:

    g++ -std=c++1z -g main.cc -g *.cpp -o A_Star.exe -I src -I win32_dep\curses -L win32_dep\curses -lpdcurses -lstdc++fs

If using VSCode, you can set up a build task like this, and build main.cc:

    {
      "tasks": [
        {
          "type": "cppbuild",
          "label": "C/C++: g++.exe build active file",
          "command": "C:\\Program Files\\mingw-w64\\x86_64-7.3.0-posix-seh-rt_v5-rev0\\mingw64\\bin\\g++.exe",
          "args": [
            "-std=c++1z",
            "-g",
            "${file}",
            "-g",
            "${fileDirname}\\*.cpp",
            "-o",
            "${fileDirname}\\A_Star.exe",
            "-I",
            "${fileDirname}",
            "-I",
            "${workspaceFolder}\\win32_dep\\curses",
            "-L",
            "${workspaceFolder}\\win32_dep\\curses",
            "-lpdcurses",
            "-lstdc++fs"
          ],
          "options": {
            "cwd": "C:\\Program Files\\mingw-w64\\x86_64-8.1.0-posix-seh-rt_v6-rev0\\mingw64\\bin"
          },
          "problemMatcher": [
            "$gcc"
          ],
          "group": {
            "kind": "build",
            "isDefault": true
          },
          "detail": "Task generated by Debugger."
        }
      ],
      "version": "2.0.0"
    }

Run the main program (./ may not be necessary depending on your terminal emulator):
    
    ./A_Star.exe
