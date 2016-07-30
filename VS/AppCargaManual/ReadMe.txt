========================================================================
    CONSOLE APPLICATION : AppTesteWin32 Project Overview
========================================================================

AppWizard has created this AppTesteWin32 application for you.

This file contains a summary of what you will find in each of the files that
make up your AppTesteWin32 application.


AppTesteWin32.vcxproj
    This is the main project file for VC++ projects generated using an Application Wizard.
    It contains information about the version of Visual C++ that generated the file, and
    information about the platforms, configurations, and project features selected with the
    Application Wizard.

AppTesteWin32.vcxproj.filters
    This is the filters file for VC++ projects generated using an Application Wizard. 
    It contains information about the association between the files in your project 
    and the filters. This association is used in the IDE to show grouping of files with
    similar extensions under a specific node (for e.g. ".cpp" files are associated with the
    "Source Files" filter).

AppTesteWin32.cpp
    This is the main application source file.

/////////////////////////////////////////////////////////////////////////////
Other standard files:

StdAfx.h, StdAfx.cpp
    These files are used to build a precompiled header (PCH) file
    named AppTesteWin32.pch and a precompiled types file named StdAfx.obj.

/////////////////////////////////////////////////////////////////////////////
Other notes:

AppWizard uses "TODO:" comments to indicate parts of the source code you
should add to or customize.

/////////////////////////////////////////////////////////////////////////////




Arquivos necessarios ao include:
../../chimera/include/
../../AppCargaManual/include
C:\Libs\SDL2-2.0.4\include
C:\Libs\SDL2_image-2.0.1\include
C:\Libs\SDL2_ttf-2.0.14\include
C:\libs\ftgl-2.1.3-rc5\src
C:\libs\freetype-2.5.5\include
C:\Libs\glew-1.13.0\include
C:\Libs\tinyxml2
C:\Libs\bullet3\src
C:\Libs\OculusSDK\LibOVR\Include
C:\Libs\glm

Arquivos Necessarios ao Lib:
../../Debug
C:\Libs\SDL2-2.0.4\lib\x86
C:\Libs\SDL2_image-2.0.1\lib\x86
C:\Libs\SDL2_ttf-2.0.14\lib\x86
C:\Libs\bullet3\bin
C:\Libs\tinyxml2\tinyxml2\Debug-Lib
C:\Libs\glew-1.13.0\lib\Release\Win32
C:\Libs\OculusSDK\LibOVR\Lib\Windows\Win32\Release\VS2015
%(AdditionalLibraryDirectories)

Links:
SDL2.lib
SDL2_image.lib
SDL2_ttf.lib
chimera.lib
opengl32.lib
GlU32.Lib
glew32s.lib
tinyxml2.lib
LinearMath_vs2010_debug.lib
Bullet3Common_vs2010_debug.lib
Bullet3Dynamics_vs2010_debug.lib
BulletCollision_vs2010_debug.lib
BulletDynamics_vs2010_debug.lib
Bullet3Geometry_vs2010_debug.lib
Bullet3Collision_vs2010_debug.lib
WSock32.Lib
Ws2_32.lib
winmm.lib
kernel32.lib
user32.lib
gdi32.lib
winspool.lib
comdlg32.lib
advapi32.lib
shell32.lib
ole32.lib
oleaut32.lib
uuid.lib
odbc32.lib
odbccp32.lib

OBS: libovrd.lib deve ser adicionado ao link para uso do Rift