from TritonCore import (EntityRegistry)
import User.SimpleTest


def reload():
    """
        Specify which modules should be reloaded if a command for reload is given
    """

    import importlib
    importlib.reload(User.SimpleTest)


def entry(registry: EntityRegistry, delta):
    """
        Entry method that is called from Triton if this function is missing than the program will not execute
        :param registry: entity registry
        :param delta: time between frame updates
        :return: none
    """

    User.SimpleTest.update(registry, delta)
    return
