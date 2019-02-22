# coding: utf-8

from TritonMath import (Vector3)
from TritonData import (Mesh, Material)

class Transform:
    """
    Class representing a transform component
    """

    def __init__(self): ...
    def __init__(self, other : Transform): ...
    def __init__(self, position : Vector3, rotation : Vector3, scale : Vector3): ...

    Position : Vector3 = ...
    Rotation : Vector3 = ...
    Scale : Vector3 = ...

class MeshFilter:
    """
    Class representing a mesh filter component
    """

    def __init__(self): ...
    def __init__(self, other : MeshFilter): ...
    def __init__(self, mesh : Mesh): ...

    Mesh : Mesh = ...

class MeshRenderer:
    """
    Class representing a mesh renderer component
    """

    def __init__(self): ...
    def __init__(self, other : MeshRenderer): ...
    def __init__(self, material : Material): ...

    Material : Material = ...