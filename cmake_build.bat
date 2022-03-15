rem NOTICE: This file was created by Major Gaming, LLC 
ECHO off
cls
SET ProjectName=Simple Win32 WAV Player

ECHO Building Project: %ProjectName%
cmake -version
if errorlevel 1 (
   echo Error Loading Cmake - Error Code: %errorlevel%
   echo Please install Cmake or make sure its on your path!
   PAUSE
   exit /b %errorlevel%
)

ECHO "Make sure your Cmake Version is >=v3.14 & Visual Studio 2010-2019 are installed!"
ECHO Enter Visual Studio Version [i.e. 2017, 2019, etc.]:
set /p vsVersion=
rem ECHO Answered: %vsVersion%
SET vsString="empty"
IF %vsVersion% EQU 2010 (SET vsString="Visual Studio 10 2010")
IF %vsVersion% EQU 2012 (SET vsString="Visual Studio 11 2012")
IF %vsVersion% EQU 2013 (SET vsString="Visual Studio 12 2013")
IF %vsVersion% EQU 2015 (SET vsString="Visual Studio 14 2015")
IF %vsVersion% EQU 2017 (SET vsString="Visual Studio 15 2017")
IF %vsVersion% EQU 2019 (SET vsString="Visual Studio 16 2019")
IF %vsString%=="empty" ( SET error=1 ) ELSE ( SET error=0 )
rem remove quotes
set vsString=%vsString:"=%
IF %error% EQU 0 ( ECHO Visual Studio Version: %vsString% ) ELSE ( ECHO ERROR - Invalid Visual Studio Version - %vsVersion%
    PAUSE
    exit /b 1 )
ECHO Enter Win32 or x64 for Build Config [Default: Win32]:
set /p vsConfig=
SET configError=1
IF %vsConfig%==Win32 ( SET configError=0 )
IF %vsConfig%==win32 ( SET configError=0 && SET vsConfig=Win32)
IF %vsConfig%==x64 ( SET configError=0 )
IF %configError% EQU 0 ( ECHO Platform Selected: %vsConfig% ) ELSE ( ECHO No Platform Selected Choosing Default Platform: Win32
    SET vsConfig=Win32 )
SET cur_dir="%cd%"
REM ECHO Using Current Directory For Build: %cur_dir%
SET build_dir=-S %cur_dir%

IF EXIST CMakeLists.txt ( ECHO CMakeLists.txt found ) ELSE ( ECHO Error - Missing CMakeLists.txt     
    PAUSE
    exit /b 1 )

rem https://cmake.org/cmake/help/v3.14/manual/cmake-generators.7.html#manual:cmake-generators(7)
ECHO ======================================================================================
ECHO CMAKE CONFIGURATION
ECHO ======================================================================================

ECHO VS Version: "%vsString%"
ECHO Platform: -A %vsConfig% 
ECHO Build Directory: %build_dir%
ECHO Cmake File: %build_dir%\CMakeLists.txt
ECHO About to begin building Project: %ProjectName% w/ Cmake with the following settings press enter to continue
PAUSE

cmake -G "%vsString%" -A %vsConfig% %build_dir%

ECHO Cmake Complete [Error Code: %errorlevel%]

REM Create clean.bat
echo rmdir /s /q .vs > clean.bat
echo rmdir /s /q CMakeFiles >> clean.bat
echo rmdir /s /q Debug >> clean.bat
echo rmdir /s /q Release >> clean.bat
echo rmdir /s /q "%ProjectName%.dir" >> clean.bat
echo rmdir /s /q %vsConfig% >> clean.bat
echo del ALL_BUILD.vcxproj >> clean.bat
echo del ALL_BUILD.vcxproj.filters >> clean.bat
echo del ALL_BUILD.vcxproj.user >> clean.bat
echo del cmake_install.cmake >> clean.bat
echo del CMakeCache.txt >> clean.bat
echo del "%ProjectName%.sln" >> clean.bat
echo del "%ProjectName%.vcxproj" >> clean.bat
echo del "%ProjectName%.vcxproj.filters" >> clean.bat
echo del "%ProjectName%.vcxproj.user" >> clean.bat
echo del ZERO_CHECK.vcxproj >> clean.bat
echo del ZERO_CHECK.vcxproj.filters >> clean.bat
echo del ZERO_CHECK.vcxproj.user >> clean.bat

ECHO Cleaning Script Made: clean.bat

ECHO Press enter to open Solution %ProjectName%.sln and set %ProjectName% as the startup project to build and run

PAUSE
"%ProjectName%.sln"
