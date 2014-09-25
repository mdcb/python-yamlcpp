#!/usr/bin/env python
#
#  Copyright (C) 2012  Matthieu Bec, GMTO Corp.
#
#  This file is part of gmt-fwk-io.
#
#  This program is free software: you can redistribute it and/or modify
#  it under the terms of the GNU General Public License as published by
#  the Free Software Foundation, either version 3 of the License, or
#  (at your option) any later version.
#
#  This program is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU General Public License for more details.
#
#  You should have received a copy of the GNU General Public License
#  along with this program.  If not, see <http://www.gnu.org/licenses/>.
#


from distutils.core import setup, Extension
from os import environ as env

version = '0.1'

extra_compile_args=[
  '-Wall',
  '-std=c++11',
  '-Wno-packed-bitfield-compat',
  '-pthread',
]

setup(name='python-boostyamlcpp',
      version=version,
      description='yamlcpp converter for python-boost',
      long_description='yamlcpp converter for python-boost.',
      author='Matthieu Bec',
      author_email='mdcb808@gmail.com',
      url='https://github.com/mdcb/python-boost-yamlcpp',
      license='GPL',
      ext_modules=[
         Extension(
            name='boostyamlcpp',
            sources = [
              'boostyamlcpp.cpp',
            ],
            include_dirs=[
              '/usr/include/boost',
              '/usr/include/yamlcpp',
            ],
            undef_macros=[],
            define_macros=[],
            libraries = [
              'boost_python',
              'yaml-cpp',
            ],
            extra_compile_args = extra_compile_args,
            ),
         ],
      )

