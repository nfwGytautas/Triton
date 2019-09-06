#Returns the path to the include directory depending if the
#current application type is release or debug
def setup(type : int):
	import os
	import sys
	dir_path = os.path.dirname(os.path.realpath(__file__))
	if type == 0:
		sys.path.append(dir_path + "\\Logic")
	elif type == 1:
		sys.path.append(dir_path + "\\..\\..\\..\\PythonScripts")