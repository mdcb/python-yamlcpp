#!/usr/bin/env python3

from yamlcpp_demo import demo
toto=demo()
try:
  toto.get()
except TypeError as e:
  pass

import yamlcpp
toto.get()

toto.set(toto.get()) # yaml like

toto.set([1,2,3,4]) # list
toto.set((1,2,3,4)) # tuple

toto.set([])
toto.set(())
toto.set({})

toto.set(1) # scalar
toto.set({'a':1, 'b':2}) # dict


