#!/usr/bin/env python3

from distutils.core import setup, Extension
from os import environ as env

version = '0.1'

extra_compile_args=[
  '-Wall',
  '-Wextra',
  '-pedantic',
  '-std=c++11',
]

setup(name='python-yamlcpp-demo',
      version=version,
      description='demo for yamlcpp converter for python',
      long_description='demo for yamlcpp converter for python.',
      author='Matthieu Bec',
      author_email='mdcb808@gmail.com',
      url='https://github.com/mdcb/python-yamlcpp',
      license='GPL',
      ext_modules=[
         Extension(
            name='yamlcpp_demo',
            sources = [
              'demo.cpp',
            ],
            include_dirs=[
              '/usr/include/boost',
              '/usr/include/yamlcpp', # pkg-config --cflags yamlcpp
            ],
            undef_macros=[],
            define_macros=[],
            libraries = [
              'boost_python3',
              'yaml-cpp', # pkg-config --libs yamlcpp
            ],
            extra_compile_args = extra_compile_args,
            ),
         ],
      )

