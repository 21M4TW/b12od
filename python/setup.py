import os
import subprocess
from setuptools import setup, Extension
from setuptools.command.build_ext import build_ext
from Cython.Build import cythonize

class CMakeBuildExt(build_ext):
    def run(self):
        # Run CMake to build the C library
        build_dir = os.path.abspath(os.path.join(os.path.dirname(__file__), "build_c"))
        os.makedirs(build_dir, exist_ok=True)
        source_dir = os.path.abspath(os.path.join(os.path.dirname(__file__), ".."))
        subprocess.check_call(["cmake", source_dir], cwd=build_dir)
        subprocess.check_call(["cmake", "--build", ".", "--target", "bolt12_offer_decode_pic"], cwd=build_dir)
        super().run()

    def build_extensions(self):
        lib_path = os.path.join(os.path.dirname(__file__), "build_c/libbolt12_offer_decode_pic.a")
        for ext in self.extensions:
          ext.extra_objects = [lib_path]   # link the static archive directly
          ext.include_dirs.append("..")
        super().build_extensions()

extensions = cythonize([
    Extension(
        "bolt12_offer_decode.bolt12_offer_decode",
        sources=["bolt12_offer_decode/bolt12_offer_decode.pyx"],
        include_dirs=["../src/lib"],   # so Cython sees bolt12_offer_decode.h
        language="c"
    )
])

setup(
    name="bolt12_offer_decode",
    version="0.1.0",
    description="Python bindings for bolt12_offer_decode C library",
    author="21M4TW",
    author_email="21M4TW@proton.me",
    url="https://github.com/21M4TW/bolt12_offer_decode",
    ext_modules=extensions,
    cmdclass={"build_ext": CMakeBuildExt},
    packages=["bolt12_offer_decode"],
    zip_safe=False,
)
