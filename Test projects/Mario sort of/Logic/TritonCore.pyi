# coding: utf-8

from TritonMath import (Vector3)
from TritonComponents import (Transform, MeshFilter, MeshRenderer)

class EntityRegistry:
    '''
    Class that stores entities and hooks components to such entities
    '''

    def __init__(self): ...
     
    def Create(self) -> int:
        '''
        Create a new entity

        :return: entity object
        :rtype: int
        '''

        ...
    
    def SetTransform(self, entity : int, value : Transform):
        '''
        Set a transform value for an entity

        :param str entity: Entity whose transform to set
        :param str value: Value of the transform component
        '''

        ...

    def GetTransform(self, entity : int) -> Transform:
        '''
        Get a transform value for an entity

        :param str entity: Entity whose transform to get
        :return: transform component
        :rtype: TritonComponents.Transform
        '''

        ...

    def AssignTransform(self, entity : int):
        '''
        Assign a transform value to an entity

        :param str entity: Entity whose transform to set
        '''

        ...


    def SetMeshFilter(self, entity : int, value : MeshFilter):
        '''
        Set a MeshFilter value for an entity

        :param str entity: Entity whose MeshFilter to set
        :param str value: Value of the MeshFilter component
        '''

        ...

    def GetMeshFilter(self, entity : int) -> MeshFilter:
        '''
        Get a MeshFilter value for an entity

        :param str entity: Entity whose MeshFilter to get
        :return: MeshFilter component
        :rtype: TritonComponents.MeshFilter
        '''

        ...

    def AssignMeshFilter(self, entity : int):
        '''
        Assign a MeshFilter value to an entity

        :param str entity: Entity whose MeshFilter to set
        '''

        ...

    def SetMeshRenderer(self, entity : int, value : MeshRenderer) -> MeshRenderer:
        '''
        Set a MeshRenderer value for an entity

        :param str entity: Entity whose MeshRenderer to set
        :param str value: Value of the MeshRenderer component
        '''

        ...

    def GetMeshRenderer(self, entity : int):
        '''
        Get a MeshRenderer value for an entity

        :param str entity: Entity whose MeshRenderer to get
        :return: MeshRenderer component
        :rtype: TritonComponents.MeshRenderer
        '''

        ...

    def AssignMeshRenderer(self, entity : int):
        '''
        Assign a MeshRenderer value to an entity

        :param str entity: Entity whose MeshRenderer to set
        '''

        ...