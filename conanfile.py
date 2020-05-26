from conans import ConanFile, CMake, tools


class GeTestConan(ConanFile):
    name = "ge_test"
    version = "1.0"
    license = ""
    author = "Bittner Ede bittner.ede@gmail.com"
    url = "https://github.com/elnoir/ge_test"
    description = "Neural network on a background thread with a WinForms test application"
    topics = ("ann", "winforms", "thread")
    settings = "os", "compiler", "build_type", "arch"
    generators = "cmake"
    requires = ["gtest/1.8.1@bincrafters/stable", "boost/1.71.0@conan/stable"]


    def source(self):
        self.run("git clone https://github.com/elnoir/ge_test.git")
        # This small hack might be useful to guarantee proper /MT /MD linkage
        # in MSVC if the packaged project doesn't have variables to set it
        # properly


    def build(self):
        cmake = CMake(self)
        cmake.configure(source_folder=".")
        cmake.build()


    def package(self):
        pass

    def package_info(self):
        pass

