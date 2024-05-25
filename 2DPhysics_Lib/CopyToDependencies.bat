@echo off

:: Clear the target directory first, before copying
cd ../Dependencies/2DPhysics/
del * /S /Q

cd ../../2DPhysics_Lib/

:: Copy all header files into include directory
echo f | xcopy /f /y /s "./*.h" "../Dependencies/2DPhysics/include/"

:: Copy *.lib file to lib directory
echo f | xcopy /f /y /s "./x64/2DPhysics_Lib/%1.lib" "../Dependencies/2DPhysics/lib/"

:: Copy *.dll file to lib directory
echo f | xcopy /f /y /s "./x64/2DPhysics_Lib/%1.dll" "../Dependencies/2DPhysics/lib/"

pause