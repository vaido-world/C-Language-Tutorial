@ECHO OFF
TITLE Compile Tiny C with itself AND run WinSocket example

ECHO Downloading "Old Precompiled Tiny C Compiler" in .zip archive format
curl -L "http://download.savannah.gnu.org/releases/tinycc/tcc-0.9.27-win64-bin.zip" -O

ECHO Extracting "Old Precompiled Tiny C Compiler" .zip Archive
tar xf "tcc-0.9.27-win64-bin.zip"

ECHO Setting "Old Precompiled Tiny C Compiler" to a PATH variable
SET "PATH=%PATH%;%CD%\tcc"

ECHO Downloading "Latest Tiny C Compiler Source Code"
curl "https://repo.or.cz/tinycc.git/snapshot/HEAD.tar.gz" -O

ECHO Extracting "Latest Tiny C Compiler Source Code" .tar.gz Archive
tar xf "HEAD.tar.gz"

ECHO Opening "Latest Tiny C Compiler Source Code" extraction directory
CD "tinycc-HEAD-*"

ECHO Opening "win32" directory of "Latest Tiny C Compiler Source Code"
CD "win32"

ECHO Launching "Tiny C Compiler Building Script"
CALL build-tcc.bat -c tcc -i "..\..\latest-built"
ECHO Tiny C Compiler Successfully Compiled from the latest Source Code.

ECHO Trying to Unstuck from weirdly written "Tiny C Compiler Building Script"
ping 127.0.0.1 -n 6 > nul

ECHO If you see this, this script is still going, that means this script is successfully Unstuck, else something might have gone wrong

ECHO Opening the Directory of the "latest compiled Tiny C Compiler"
CD "..\..\latest-built"

ECHO Downloading "Source code for Winsocket test program"
curl "https://raw.githubusercontent.com/vaido-world/C-Language-Tutorial/master/Tiny%%20C%%20compiler/winsocketsResearch/WinSocktest.c" -O

ECHO Compiling "Source code for Winsocket test program"
tcc "WinSocktest.c" -lws2_32

ECHO Launching the compiled Winsocket Test Program.
WinSocktest.exe

REM If this Error happen, there is some kind of problem in the above: Launching "Tiny C Compiler Building Script"
REM CALL %~0 is simply a statement to start this Batch script again.
IF NOT EXIST "i386-win32-tcc.exe" ECHO Something went wrong with the Building Tiny C Compiler, restart might help.
IF NOT EXIST "i386-win32-tcc.exe" CD "%~DP0" 
IF NOT EXIST "i386-win32-tcc.exe" CALL "%~0" 
IF     EXIST "i386-win32-tcc.exe" ECHO Recompilation has been successfull, you now have a latest Tiny C Compiler compiled from Source Code
IF     EXIST "i386-win32-tcc.exe" ECHO The New Tiny C Compiler can be Found in %CD%

pause

REM taskkill /F /FI "WindowTitle eq  build-tcc.bat" /T
REM little hack to prevent this script from being stuck after build-tcc.bat Exit 
