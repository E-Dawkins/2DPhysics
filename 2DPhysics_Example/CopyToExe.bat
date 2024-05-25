@echo off

if "%~1"=="Debug" (
  set targetDll="2DPhysics_LibD"
) else (
  set targetDll="2DPhysics_Lib"
)

echo f | xcopy /f /y /s "../2DPhysics_Lib/x64/2DPhysics_Lib/%1/%targetDll%.dll" "./x64/2DPhysics_Example/%1/"

pause