from TritonCore import (EntityRegistry)
from TritonData import (Texture, TextureData, MeshData, Mesh, Material)
from TritonMath import (Vector3)
import TritonStorage


def setup(registry: EntityRegistry):
    entity = create_object(registry)
    add_components(registry, entity)
    set_position(registry, entity)
    load_object(registry, entity)


def create_object(registry):
    entity = registry.create()
    TritonStorage.GameVariables['entity'] = entity
    return entity


def add_components(registry, entity):
    registry.assign_transform(entity)
    registry.assign_mesh_filter(entity)
    registry.assign_mesh_renderer(entity)


def set_position(registry, entity):
    transform = registry.get_transform(entity)
    transform.Position = Vector3(0.0, 0.0, -25.0)
    registry.set_transform(entity, transform)


def load_object(registry, entity):
    mesh_renderer = registry.get_mesh_renderer(entity)
    mesh_filter = registry.get_mesh_filter(entity)

    texture_data = TextureData()
    texture_data.fill('D:/Programming/Test files/nfw/stallTexture.png')
    material = Material(Texture.create(texture_data))
    TritonStorage.GameVariables['material'] = material
    mesh_renderer.Material = material
    registry.set_mesh_renderer(entity, mesh_renderer)

    mesh_data = MeshData()
    mesh_data.fill('D:/Programming/Test files/nfw/stall.obj')
    mesh = Mesh.create(mesh_data)
    TritonStorage.GameVariables['mesh'] = mesh
    mesh_filter.Mesh = mesh
    registry.set_mesh_filter(entity, mesh_filter)


def update(registry: EntityRegistry, delta):
    entity = TritonStorage.GameVariables['entity']
  
