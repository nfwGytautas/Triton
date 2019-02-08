#Returns the path to the include directory depending if the
#current application type is release or debug
def setup(type : int):
	import os
	dir_path = os.path.dirname(os.path.realpath(__file__))
	if type == 0:
		dir_path += "\\Logic"
		return dir_path
	elif type == 1:
		dir_path += "\\..\\..\\..\\PythonScripts"
		return dir_path
	else:
		return '.'