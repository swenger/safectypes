from distutils.core import setup

setup(
  name = 'safectypes',
  version = "0.01",
  author = "Stephan Wenger",
  author_email = "wenger@cg.cs.tu-bs.de",
  description = "A ctypes frontend to automatically generate typesafe C library wrappers",
  license = "MIT",
  py_modules = ["safectypes"],
  headers = ["safectypes.h"],
)

