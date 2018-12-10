@ECHO OFF
SETLOCAL ENABLEDELAYEDEXPANSION

SET LOCAL_PATH_UV=C:\Keil_v5\UV4\UV4.exe
SET LOCAL_ROOT_DIR=
SET LOCAL_BUILD_LOG_DIR=

:Main
SET START_TIME=%TIME%
SET LOCAL_ROOT_DIR="%~dp0"

IF NOT EXIST "LOCAL_ROOT_DIR\build_log" (
    MKDIR build_log
)

SET LOCAL_BUILD_LOG_DIR="%~dp0\build_log"

ECHO Start %START_TIME%

FOR /R .\ %%a IN (*.uvprojx) DO (
    ECHO Build %%~pa
    CALL :BuildProc "%%~a" "%%~dpa"
)

IF NOT EXIST .\build_log\*.log (
    rd build_log
)

SET END_TIME=%TIME%
ECHO End %END_TIME%

CALL :CalcTime %START_TIME% %END_TIME%

PAUSE
EXIT /B

:BuildProc
FOR /f "tokens=3 delims=<>" %%a IN ('findstr /i "<TargetName>" ^< "%~1"') DO (
    SET QQ="building %%a...                          "
    SET /P Q="!QQ:~1,40!" < NUL
    CALL :BuildCmd %1 %2 "%%a"
)

EXIT /B

:BuildCmd
%LOCAL_PATH_UV% -b -c -j0 "%~1" -t "%~3" -o "%~2\build.log"
IF %ERRORLEVEL% NEQ 0 (
    CALL :ErrorLog %ERRORLEVEL% %2 %3
) ELSE (
    CALL :ctext 0a Pass
    REM ECHO Pass
)

IF EXIST "%~2\build.log" (
    DEL "%~2\build.log"
)
EXIT /B

:ctext
echo. >%2&findstr/a:%1 . %2*&del %2
EXIT /B

:ErrorLog
SET LOG_NAME=

IF "%1" EQU "1" (
    REM ECHO %3, Warnings Only
    CALL :ctext 0e "Warnings Only"
    FOR %%* in ("%~2\.") DO SET LOG_NAME=%%~n*
    COPY "%~2\build.log" "%LOCAL_BUILD_LOG_DIR%\!LOG_NAME!_%3_warning.log" > NUL
)
IF "%1" EQU "2" (
    REM ECHO %3, Errors
    CALL :ctext 0c Errors
    FOR %%* in ("%~2\.") DO SET LOG_NAME=%%~n*
    COPY "%~2\build.log" "%LOCAL_BUILD_LOG_DIR%\!LOG_NAME!_%3_error.log" > NUL
)
IF "%1" EQU "3" (
    ECHO %3, Device without given target
)
IF "%1" EQU "11" (
    ECHO %3, Cannot open project file for writing
)
IF "%1" EQU "12" (
    ECHO %3, Device with given name in not found in database
)
IF "%1" EQU "13" (
    ECHO %3, Error writing project file
)
IF "%1" EQU "15" (
    ECHO %3, Error reading import XML file
)

EXIT /B

:CalcTime
SET START_HOUR=%START_TIME:~0,2%
SET START_MINS=%START_TIME:~3,2%
SET START_SECS=%START_TIME:~6,2%
SET START_MSEC=%START_TIME:~9,2%

SET END_HOUR=%END_TIME:~0,2%
SET END_MINS=%END_TIME:~3,2%
SET END_SECS=%END_TIME:~6,2%
SET END_MSEC=%END_TIME:~9,2%

SET /A EXEC_MSEC=END_MSEC-START_MSEC
SET /A EXEC_SECS=END_SECS-START_SECS
SET /A EXEC_MINS=END_MINS-START_MINS
SET /A EXEC_HOUR=END_HOUR-START_HOUR

IF %EXEC_MSEC% LSS 0  SET /A EXEC_MSEC+=100 & SET /A EXEC_SECS-=1
IF %EXEC_MSEC% LSS 10 SET EXEC_MSEC=0%EXEC_MSEC%

IF %EXEC_SECS% LSS 0  SET /A EXEC_SECS+=60  & SET /A EXEC_MINS-=1
IF %EXEC_SECS% LSS 10 SET EXEC_SECS=0%EXEC_SECS%

IF %EXEC_MINS% LSS 0  SET /A EXEC_MINS+=60  & SET /A EXEC_HOUR-=1
IF %EXEC_MINS% LSS 10 SET EXEC_MINS=0%EXEC_MINS%

IF %EXEC_HOUR% LSS 0  SET /A EXEC_HOUR+=24
IF %EXEC_HOUR% LSS 10 SET EXEC_HOUR=0%EXEC_HOUR%

ECHO Build Complete, Elapse: %EXEC_HOUR%:%EXEC_MINS%:%EXEC_SECS%.%EXEC_MSEC%

EXIT /B
