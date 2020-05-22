cmake_minimum_required(VERSION 3.17)
project(wpf_test)


add_executable(wpf_test
src/app.ico
src/app.rc
src/AssemblyInfo.cpp
src/MainWindow.h
src/MainWindow.cpp
src/main.cpp
src/pch.h
src/pch.cpp
src/pch.h
src/Resource.h
)

LIST(APPEND DOTNET_REFS "System")
LIST(APPEND DOTNET_REFS "System.Data")
LIST(APPEND DOTNET_REFS "System.Drawing")
LIST(APPEND DOTNET_REFS "System.Windows.Forms")
LIST(APPEND DOTNET_REFS "System.Xml")


set_target_properties(wpf_test PROPERTIES
    COMMON_LANGUAGE_RUNTIME ""
    VS_DOTNET_REFERENCES "${DOTNET_REFS}"

)
