@echo off
for /f "usebackq tokens=1 delims=r|- skip=1" %%a in (`svn log -r head -q file:///C:/repository/vmxos_bk`) do (
    echo #ifndef _VERSION_H_>src\version.h
    echo #define _VERSION_H_>>src\version.h
    echo.>>src\version.h
    echo char* vmxos_version = "v0.1.%%a%DATE% %TIME:~0,5%";>>src\version.h
    echo.>>src\version.h
    echo #endif>>src\version.h
    echo.>>src\version.h
)
@echo on