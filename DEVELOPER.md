# Update CTest Results

1. Open 'Developer PowerShell for VS 2022' using Windows Terminal

    Copy/Modify the Target: from 'Developer PowerShell for VS 2022' Start menu item

    ```
    Command line:
    pwsh.exe -NoExit -Command "&{Import-Module """C:\Program Files\Microsoft Visual Studio\2022\Community\Common7\Tools\Microsoft.VisualStudio.DevShell.dll"""; Enter-VsDevShell 0d2a9cd0 -SkipAutomaticLocation -DevCmdArguments """-arch=x64 -host_arch=x64"""}"
    ```

    Note the addition of the -DevCmdArguments """-arch=x64 -host_arch=x64"""

2. Run the regression tests

    ```
    ctest -S ctest.regression.cmake -DCTEST_TEST_TIMEOUT=1500
    ```

    This will submit the results to https://my.cdash.org/index.php?project=phreeqc3

3. Once the results have been verified, copy results from the _ctest _regression directory

    ```
    cp .\_ctest_regression\examples\* .\examples\. -Exclude CMakeFiles, cmake_install.cmake, CTestTestfile.cmake
    cp .\_ctest_regression\mytest\* .\mytest\. -Exclude CMakeFiles, cmake_install.cmake, CTestTestfile.cmake
    ```