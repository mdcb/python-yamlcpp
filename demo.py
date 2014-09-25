#!/usr/bin/env python

from boostyamlcpp_demo import demo
toto=demo()
try:
  toto.get()
except TypeError, e:
  pass

import boostyamlcpp
toto.get()
toto.set(toto.get())


