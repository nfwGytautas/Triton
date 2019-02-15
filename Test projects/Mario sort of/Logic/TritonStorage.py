GameVariables = {

}

Resources = {

}

def ChangeVariable(name, value):
    GameVariables[name] = value

def ChangeResource(name, value):
    Resources[name] = value


def GetVariable(name):
    if name in GameVariables:
        return GameVariables[name]
    else:
        raise ValueError(('GameVariable does not contain {0}}!', name))