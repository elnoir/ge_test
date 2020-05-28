# ge_test

## Build Process

By default this project works with the conan package manager and using cmake as a build system wrapper.

To build the project please follow these steps:
1. Install conan: ```python -m pip install conan```
2. Create a build direcotry in the checked out source code: ```mkdir build```
3. Change current directory to the build directory: ```cd build```
4. Install dependencies: ```conan install .. --build missing```
5. Build ```conan build ..```
6. Run the executable from build\bin\wpf_test.exe

## Advanced build

### Debug build

You can specify the build type at step 4.
Use the --s build_type="Debug" switch to debug build.
```conan install .. --build missing -s build_type=Debug```

### Build from Visual Studio

After step 4, a solution file will be generated at the ```build\ge_ai_app.sln``` path. You may open it and build from Visual Studio, but make sure that the buid configuration matches with the conan installs build configuration.
