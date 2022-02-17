#!/bin/bash

echo Building SpideyZ DLL...

i686-w64-mingw32-g++ -O3 -s -static-libstdc++ -static-libgcc -static -I src -shared \
	src/SpideyZ.cpp \
	src/Patcher.cpp \
	src/Logger.cpp \
	src/CMenu.cpp \
	src/FileSystem.cpp \
	src/GameConfig.cpp \
	src/MenuSystem.cpp \
	src/Graphics.cpp \
	src/ScopedUnprotect.cpp \
	src/dllmain.cpp \
	-o "build/SpideyZ.dll"
