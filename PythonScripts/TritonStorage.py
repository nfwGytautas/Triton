GameVariables = {

}

Resources = {

}


def change_variable(name, value):
    GameVariables[name] = value


def change_resource(name, value):
    Resources[name] = value


def get_variable(name):
    if name in GameVariables:
        return GameVariables[name]
    else:
        raise ValueError(('GameVariable does not contain {0}}!', name))
