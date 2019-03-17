# coding: utf-8

from TritonComponents import (Transform, MeshFilter, MeshRenderer)
from TritonData import (MeshData, Mesh, TextureData, Texture)

class EntityRegistry:
    """
    Class that stores entities and hooks components to such entities
    """

    def __init__(self): ...
     
    def create(self) -> int:
        """
        Create a new entity

        :return: entity object
        :rtype: int
        """

        ...
    
    def set_transform(self, entity : int, value : Transform):
        """
        Set a transform value for an entity

        :param str entity: Entity whose transform to set
        :param str value: Value of the transform component
        """

        ...

    def get_transform(self, entity : int) -> Transform:
        """
        Get a transform value for an entity

        :param str entity: Entity whose transform to get
        :return: transform component
        :rtype: TritonComponents.Transform
        """

        ...

    def assign_transform(self, entity : int):
        """
        Assign a transform value to an entity

        :param str entity: Entity whose transform to set
        """

        ...


    def set_mesh_filter(self, entity : int, value : MeshFilter):
        """
        Set a MeshFilter value for an entity

        :param str entity: Entity whose MeshFilter to set
        :param str value: Value of the MeshFilter component
        """

        ...

    def get_mesh_filter(self, entity : int) -> MeshFilter:
        """
        Get a MeshFilter value for an entity

        :param str entity: Entity whose MeshFilter to get
        :return: MeshFilter component
        :rtype: TritonComponents.MeshFilter
        """

        ...

    def assign_mesh_filter(self, entity : int):
        """
        Assign a MeshFilter value to an entity

        :param str entity: Entity whose MeshFilter to set
        """

        ...

    def set_mesh_renderer(self, entity : int, value : MeshRenderer) -> MeshRenderer:
        """
        Set a MeshRenderer value for an entity

        :param str entity: Entity whose MeshRenderer to set
        :param str value: Value of the MeshRenderer component
        """

        ...

    def get_mesh_renderer(self, entity : int):
        """
        Get a MeshRenderer value for an entity

        :param str entity: Entity whose MeshRenderer to get
        :return: MeshRenderer component
        :rtype: TritonComponents.MeshRenderer
        """

        ...

    def assign_mesh_renderer(self, entity : int):
        """
        Assign a MeshRenderer value to an entity

        :param str entity: Entity whose MeshRenderer to set
        """

        ...

class DataMap:
    """
    Class that is responsible for keeping data alive through the
    application lifetime
    """

    def register_mesh(self, mesh_data : MeshData) -> Mesh:
        """
        Registers a mesh to the data map

        :param mesh_data: Data to register the mesh with
        :return: Mesh object
        :rtype: TritonData.Mesh
        """

        ...

    def register_texture(self, texture_data : TextureData) -> Texture:
        """
        Registers a texture to the data map

        :param texture_data: Data to register the texture with
        :return: Texture object
        :rtype: TritonData.Texture
        """

        ...