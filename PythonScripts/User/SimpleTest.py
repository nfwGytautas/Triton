from TritonCore import (EntityRegistry, DataMap)
from TritonData import (Texture, TextureData, MeshData, Mesh, Material)
from TritonMath import (Vector3)
import TritonStorage


def setup(registry: EntityRegistry, d_map: DataMap):
    entity = create_object(registry)
    add_components(registry, entity)
    set_position(registry, entity)
    load_object(registry, entity, d_map)


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


def load_object(registry, entity, d_map):
    mesh_renderer = registry.get_mesh_renderer(entity)
    mesh_filter = registry.get_mesh_filter(entity)

    texture_data = TextureData()
    texture_data.Path = 'D:/Programming/Test files/nfw/stallTexture.png'
    material = Material(d_map.register_texture(texture_data))
    TritonStorage.GameVariables['material'] = material
    mesh_renderer.Material = material
    registry.set_mesh_renderer(entity, mesh_renderer)

    mesh_data = MeshData()
    mesh_data.Path = 'D:/Programming/Test files/nfw/stall.obj'
    mesh = d_map.register_mesh(mesh_data)
    TritonStorage.GameVariables['mesh'] = mesh
    mesh_filter.Mesh = mesh
    registry.set_mesh_filter(entity, mesh_filter)


def update(registry: EntityRegistry, delta):
    entity = TritonStorage.GameVariables['entity']

    transform = registry.get_transform(entity)
    transform.Rotation.y += 0.5
    transform.Rotation.x += 0.5
    transform.Rotation.z += 0.5

    registry.set_transform(entity, transform)
