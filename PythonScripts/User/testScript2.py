from TritonLib.TritonStorage import GameVariables
GameVariables["Testas1"] = True

import testScript
testScript.addTestVariable()

print(GameVariables)