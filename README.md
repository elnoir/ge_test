# ge_test

## Build Process

By default this project works with the conan package manager and using cmake as a build system wrapper.

To build the project please follow these steps:
1. Install conan: ```pyton -m pip install conan```
2. Create a build direcotry in the checked out source code: ```mkdir build```
3. Change current directory to the build directory: ```cd build```
4. Install dependencies: ```conan install .. --build missing```
5. Build ```conan build ..```
6. Run the executable from build\bin\wpf_test.exe

## Advanced build 
