#!/usr/bin/env python2

from coolfluid import *
import sys

# This test runs another python script using the ScriptEngine

root = Core.root()
engine = root.create_component('PythonEngine', 'cf3.python.ScriptEngine')

script_file = open(sys.argv[1])
engine.execute_script(script_file.read())
