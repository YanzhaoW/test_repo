from conan import ConanFile


class ExampleRecipe(ConanFile):
    settings = "os", "compiler", "build_type", "arch"
    generators = "CMakeDeps"
    def requirements(self):
        self.requires("asio/1.36.0")
