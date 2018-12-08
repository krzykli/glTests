@echo off

IF NOT "%VisualStudioVersion%"=="14.0" ^
call "C:\Program Files (x86)\Microsoft Visual Studio 14.0\VC\vcvarsall.bat" x86

REM 4239 - nonstandard extension, (a non-const reference may only be bount ot an lvalue)
set CompilerFlags=-MD -Od -Oi -WX -W4 -wd4267 -wd4201 -wd4996 -wd4244 -wd4101 -wd4239 -wd4189 -wd4100 -wd4530 -FC -Zi -EHs -Wv:18
REM set CompilerFlags=-MD -O2
REM 4100 - unreferenced formal parameter
REM 4101 - unreferenced local variable
REM 4189 - unused variable
REM 4201 - nameless struct/union
REM 4530 - somebody used exception handling
REM MT - statically link c runtime library
REM Oi - enable intrinsics
REM FC - display pull path passed to cl
REM Zi - generate debug info
REM EHs- - turn off exception handling
REM Wv- - no warnings after compiler version

IF NOT EXIST bin mkdir bin
pushd bin
cl %CompilerFlags% /DDEBUG ..\main.cpp kernel32.lib user32.lib gdi32.lib shell32.lib opengl32.lib ..\glew\lib\Release\Win32\glew32.lib ..\glfw\lib-vc2015\glfw3.lib /I ..\GLM /I ..\glew\include /I ..\glfw\include /link /out:gltest.exe
REM %CompilerFlags% /DDEBUG ..\main.c /IGL /link /out:gltest.exe
popd

