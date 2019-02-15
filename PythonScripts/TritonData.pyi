# coding: utf-8

from TritonMath import (Vector3)

from typing import List

class MeshData:
    '''
    Class representing the data of a mesh which can be used to create a mesh object
    '''

    def __init__(self): ...

    Vertices : List[float] = ...
    UVs : List[float] = ...
    
    def Fill(self, path : str):
        '''
        Fill object with data from file

        :param str path: path to file
        '''

        ...


class TextureData:
    '''
    Class representing the data of a texture which can be used to create a texture object
    '''

    def __init__(self): ...

    Width : float = ...
    Height : float = ...
    BPP : int = ...

    def Fill(self, path : str):
        '''
        Fill object with data from file

        :param str path: path to file
        '''

        ...


class Texture:
    '''
    Class representing an image which is used for creation of a material
    '''

    def __init__(self): ...

    def GetWidth(self) -> float:
        '''
        Get width of the texture

        :return: width of texture
        :rtype: float
        '''

        ...

    def GetHeight(self) -> float:
        '''
        Get height of the texture

        :return: Height of texture
        :rtype: float
        '''

        ...

    @staticmethod
    def Create(data : TextureData) -> Texture:
        '''
        Create the texture object from a data object

        :return: Height of mesh
        :rtype: float
        '''

        ...


class Mesh:
    '''
    Class representing a mesh which is used for rendering objects
    '''

    def __init__(self): ...

    def GetWidth(self) -> float:
        '''
        Get width of the mesh object

        :return: Width of mesh
        :rtype: float
        '''

        ...
    
    def GetHeight(self) -> float:
        '''
        Get height of the mesh object

        :return: Height of mesh
        :rtype: float
        '''

        ...

    @staticmethod
    def Create(data : MeshData, a3D : bool = True) -> Mesh:
        '''
        Create the a mesh object from a meshdata object

        :param str data: Data object
        :param str a3D: Should the data be interpreted as 3D model data
        :return: Created mesh object
        :rtype: TritonData.Mesh
        '''

        ...


class Material:
    '''
    Class representing a material object which is used for shading meshes
    '''

    def __init__(self): ...
    def __init__(self, texture : Texture): ...


    def GetDiffuse(self) -> Vector3:
        '''
        Get material diffuse value

        :return: diffuse value
        :rtype: TritonMath.Vector3
        '''

        ...

    def SetDiffuse(self, value : Vector3):
        '''
        Set material diffuse value

        :param str value: Diffuse value
        '''

        ...

    def GetTexture(self) -> Texture:
        '''
        Get the underlying texture of this material

        :return: Texture
        :rtype: TritonData.Texture
        '''

        ...