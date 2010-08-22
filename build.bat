@echo off
set numero=0
for /f "usebackq tokens=* delims=;" %%a in (`findstr ";" src\build.h`) do (
    set /a numero=%%a
)
set /a numero=%numero%+1
echo #ifndef _BUILD_H_>src\build.h
echo #define _BUILD_H_>>src\build.h
echo.>>src\build.h
echo int vmxos_build = >>src\build.h
echo %numero%;>>src\build.h
echo.>>src\build.h
echo #endif>>src\build.h
@echo on
make