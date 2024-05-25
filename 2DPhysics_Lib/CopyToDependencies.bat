@echo off

:: Copy all header files into include directory
echo f | xcopy /f /y /s "./*.h" "../Dependencies/2DPhysics/include/"

:: Copy debug & release *.lib files into lib directory
echo f | xcopy /f /y /s "./x64/2DPhysics_Lib/%1.lib" "../Dependencies/2DPhysics/lib/"

:: Copy debug & release *.dll files into lib directory
echo f | xcopy /f /y /s "./x64/2DPhysics_Lib/%1.dll" "../Dependencies/2DPhysics/lib/"

pause