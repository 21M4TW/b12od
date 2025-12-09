import os
import subprocess
from setuptools import setup, Extension
from setuptools.command.build_ext import build_ext
from Cython.Build import cythonize

class CMakeBuildExt(build_ext):
    def run(self):
        # Run CMake to build the C library
        build_dir = os.path.abspath(os.path.join(os.path.dirname(__file__), "b12od-core/build"))
        os.makedirs(build_dir, exist_ok=True)
        source_dir = os.path.abspath(os.path.join(os.path.dirname(__file__), "b12od-core"))
        build_type = os.environ.get("BUILD_TYPE", "")

        if build_type == "":
          subprocess.check_call(["cmake", source_dir], cwd=build_dir)
        else:
          subprocess.check_call(["cmake", "-DCMAKE_BUILD_TYPE="+build_type, source_dir], cwd=build_dir)
        subprocess.check_call(["cmake", "--build", ".", "--target", "libb12od_pic"], cwd=build_dir)
        super().run()

    def build_extensions(self):
        lib_path = os.path.join(os.path.dirname(__file__), "b12od-core/build/libb12od_pic.a")

        for ext in self.extensions:
          ext.extra_objects = [lib_path]   # link the static archive directly
          ext.include_dirs.append("b12od-core")
        super().build_extensions()

c_flags = os.environ.get("CMAKE_C_FLAGS", "").split()

extensions = cythonize([
  Extension(
    "b12od.b12od",
    sources=["b12od/b12od.pyx"],
    include_dirs=["b12od-core/include"],   # so Cython sees bolt12_offer_decode.h
    language="c",
    extra_compile_args=c_flags
  )
])

setup(
    ext_modules=extensions,
    cmdclass={"build_ext": CMakeBuildExt},
    packages=["b12od"],
    zip_safe=False,
    install_requires=["pytest"],
)
