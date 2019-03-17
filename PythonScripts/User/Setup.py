from TritonCore import (EntityRegistry, DataMap)
import User.SimpleTest


def reload():
    """
        Specify which modules should be reloaded if a command for reload is given
    """

    import importlib
    importlib.reload(User.SimpleTest)


def entry(registry: EntityRegistry, d_map: DataMap):
    """
        Entry method that is called from Triton if this function is missing than the program will not execute
        :param registry: entity registry
        :param d_map: data map
        :return: none
    """

    User.SimpleTest.setup(registry, d_map)
    return
