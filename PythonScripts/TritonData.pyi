# coding: utf-8

from TritonMath import (Vector2, Vector3)

from typing import List

class Vertex:
    """
    Class representing the data inside a vertex
    """

    def __init__(self): ...

    Vertice : Vector3 = ...
    UV : Vector2 = ...
    Normal : Vector3 = ...

class MeshData:
    """
    Class representing the data of a mesh which can be used to create a mesh object
    """

    def __init__(self): ...

    Vertices : List[Vertex] = ...
    Indices : List[int] = ...

    def fill(self, path : str):
        """
        Fill object with data from file

        :param str path: path to file
        """

        ...


class TextureData:
    """
    Class representing the data of a texture which can be used to create a texture object
    """

    def __init__(self): ...

    Width : float = ...
    Height : float = ...
    BPP : int = ...

    def fill(self, path : str):
        """
        Fill object with data from file

        :param str path: path to file
        """

        ...


class Texture:
    """
    Class representing an image which is used for creation of a material
    """

    def __init__(self): ...

    def get_width(self) -> float:
        """
        Get width of the texture

        :return: width of texture
        :rtype: float
        """

        ...

    def get_height(self) -> float:
        """
        Get height of the texture

        :return: Height of texture
        :rtype: float
        """

        ...

    @staticmethod
    def create(data : TextureData) -> Texture:
        """
        Create the texture object from a data object

        :return: Height of mesh
        :rtype: float
        """

        ...


class Mesh:
    """
    Class representing a mesh which is used for rendering objects
    """

    def __init__(self): ...

    def get_width(self) -> float:
        """
        Get width of the mesh object

        :return: Width of mesh
        :rtype: float
        """

        ...

    def get_height(self) -> float:
        """
        Get height of the mesh object

        :return: Height of mesh
        :rtype: float
        """

        ...

    @staticmethod
    def create(data : MeshData) -> Mesh:
        """
        Create the a mesh object from a mesh data object

        :param str data: Data object
        :return: Created mesh object
        :rtype: TritonData.Mesh
        """

        ...


class Material:
    """
    Class representing a material object which is used for shading meshes
    """

    def __init__(self): ...
    def __init__(self, texture : Texture): ...


    def get_diffuse(self) -> Vector3:
        """
        Get material diffuse value

        :return: diffuse value
        :rtype: TritonMath.Vector3
        """

        ...

    def set_diffuse(self, value : Vector3):
        """
        Set material diffuse value

        :param str value: Diffuse value
        """

        ...

    def get_texture(self) -> Texture:
        """
        Get the underlying texture of this material

        :return: Texture
        :rtype: TritonData.Texture
        """

        ...