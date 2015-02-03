@Echo Off
REM =========================================================================
REM Create Package
REM   This is the bootstrapper call scripts that:
REM      - Install chocolatey
REM      - Install CoApp tools
REM      - Create nuget packages given a autopkg file
REM =========================================================================

if "%1" == "" GOTO NoArgs
if "%2" == "" GOTO NoArgs

cd %1
powershell -NoProfile -NonInteractive -ExecutionPolicy unrestricted -File CreatePackageCommon.ps1 %2

goto Finish

:NoArgs
echo ===========================================================================
echo  Package Creation Helper
echo ===========================================================================
echo.
echo Error! Parameters must be specified correctly
echo
echo     PackageCreation {AutoPkg Location} {AutoPkg Name}
echo
echo        For use in your Visual Studio build step, try calling it this way:
echo        Example:
echo           $(SolutionDir)..\PackageCreation.bat $(SolutionDir)..\ TSCUtl.autopkg
echo.
exit 1

:Finish
