#pragma once

#include "Triton\TRMacros.h"

#define TR_PYTHON_IMMUTABLE_GET_SET(funcName, valueType, object, objectVariable) \
		.def(TR_STRINGIZE("get", ##funcName), \
		[](const object& aObject) {\
			return aObject.objectVariable;\
		})\
		.def(TR_STRINGIZE("set", ##funcName), \
		[](object& aObject, valueType aValue) {\
			aObject.objectVariable = aValue;\
			return aObject.objectVariable;\
		})\

#include "Triton\Core\Math\Math.h"
PYBIND11_EMBEDDED_MODULE(TritonMath, m) {

	py::class_<Triton::Vector2, std::shared_ptr<Triton::Vector2>>(m, "Vector2")
		.def(py::init<float, float>())
		.def_readwrite("x", &Triton::Vector2::x)
		.def_readwrite("y", &Triton::Vector2::y);

	py::class_<Triton::Vector3, std::shared_ptr<Triton::Vector3>>(m, "Vector3")
		.def(py::init<float, float, float>())
		.def_readwrite("x", &Triton::Vector3::x)
		.def_readwrite("y", &Triton::Vector3::y)
		.def_readwrite("z", &Triton::Vector3::z);

	py::class_<Triton::Vector4, std::shared_ptr<Triton::Vector4>>(m, "Vector4")
		.def(py::init<float, float, float, float>())
		.def_readwrite("x", &Triton::Vector4::x)
		.def_readwrite("y", &Triton::Vector4::y)
		.def_readwrite("z", &Triton::Vector4::z)
		.def_readwrite("w", &Triton::Vector4::w);
}

#include "Triton\Core\Data\Structures\Mesh.h"
#include "Triton\Core\Data\Structures\Texture.h"
#include "Triton\Core\Data\Structures\Material.h"
PYBIND11_EMBEDDED_MODULE(TritonData, m) {
	py::class_<Triton::Data::MeshData::Vertex, std::shared_ptr<Triton::Data::MeshData::Vertex>>(m, "Vertex")
		.def(py::init<>())
		.def_readwrite("Vertice", &Triton::Data::MeshData::Vertex::Vertice)
		.def_readwrite("UV", &Triton::Data::MeshData::Vertex::UV)
		.def_readwrite("Normal", &Triton::Data::MeshData::Vertex::Normal);

	py::class_<Triton::Data::MeshData, std::shared_ptr<Triton::Data::MeshData>>(m, "MeshData")
		.def(py::init<>())
		.def(py::init<std::string>())
		.def_readwrite("Vertices", &Triton::Data::MeshData::Vertices)
		.def_readwrite("Indices", &Triton::Data::MeshData::Indices)
		.def_readwrite("Path", &Triton::Data::MeshData::Path);

	py::class_<Triton::Data::TextureData, std::shared_ptr<Triton::Data::TextureData>>(m, "TextureData")
		.def(py::init<>())
		.def(py::init<std::string>())
		.def_readwrite("Width", &Triton::Data::TextureData::Width)
		.def_readwrite("Height", &Triton::Data::TextureData::Height)
		.def_readwrite("BPP", &Triton::Data::TextureData::BPP)
		.def_readwrite("Path", &Triton::Data::TextureData::Path);

	py::class_<Triton::Data::Texture, std::shared_ptr<Triton::Data::Texture>>(m, "Texture")
		.def(py::init<>())
		.def("get_width", &Triton::Data::Texture::GetWidth)
		.def("get_height", &Triton::Data::Texture::GetHeight);

	py::class_<Triton::Data::Mesh, std::shared_ptr<Triton::Data::Mesh>>(m, "Mesh")
		.def(py::init<>())
		.def("get_width", &Triton::Data::Mesh::GetWidth)
		.def("get_height", &Triton::Data::Mesh::GetHeight);

	py::class_<Triton::Data::Material, std::shared_ptr<Triton::Data::Material>>(m, "Material")
		.def(py::init<std::shared_ptr<Triton::Data::Texture>>())
		.def(py::init<>())
		.def("set_diffuse", &Triton::Data::Material::SetDiffuse)
		.def("get_diffuse", &Triton::Data::Material::GetDiffuse)
		.def("get_texture", &Triton::Data::Material::GetTexture);
}

#include "TritonShell\Entity\Components\TritonComponents.h"
PYBIND11_EMBEDDED_MODULE(TritonComponents, m) {
	py::class_<Triton::Components::Transform, std::shared_ptr<Triton::Components::Transform>>(m, "Transform")
		.def(py::init<>())
		.def(py::init<const Triton::Components::Transform&>())
		.def(py::init<Triton::Vector3&, Triton::Vector3&, Triton::Vector3&>())
		.def_readwrite("Position", &Triton::Components::Transform::Position)
		.def_readwrite("Rotation", &Triton::Components::Transform::Rotation)
		.def_readwrite("Scale", &Triton::Components::Transform::Scale);

	py::class_<Triton::Components::MeshFilter, std::shared_ptr<Triton::Components::MeshFilter>>(m, "MeshFilter")
		.def(py::init<>())
		.def(py::init<const Triton::Components::MeshFilter&>())
		.def(py::init<std::shared_ptr<Triton::Data::Mesh>>())
		.def_readwrite("Mesh", &Triton::Components::MeshFilter::Mesh);

	py::class_<Triton::Components::MeshRenderer, std::shared_ptr<Triton::Components::MeshRenderer>>(m, "MeshRenderer")
		.def(py::init<>())
		.def(py::init<const Triton::Components::MeshRenderer&>())
		.def(py::init<std::shared_ptr<Triton::Data::Material>>())
		.def_readwrite("Material", &Triton::Components::MeshRenderer::Material);
}

#include "TritonShell\Entity\Registry.h"

#define TR_PYTHON_COMPONENT_GET(name, type) \
		.def(TR_STRINGIZE("get_", name), \
		[](Triton::ECS::Registry& aRegistry, Triton::ECS::Entity aEntity) {\
			return aRegistry.get<type>(aEntity);\
		})\

#define TR_PYTHON_COMPONENT_SET(name, type) \
		.def(TR_STRINGIZE("set_", name), \
		[](Triton::ECS::Registry& aRegistry, Triton::ECS::Entity aEntity, type& aValue) {\
			aRegistry.get<type>(aEntity) = aValue;\
		})\

#define TR_PYTHON_COMPONENT_ASSIGN(name, type) \
		.def(TR_STRINGIZE("assign_", name), \
		[](Triton::ECS::Registry& aRegistry, Triton::ECS::Entity aEntity) {\
			return aRegistry.assign<type>(aEntity);\
		})\

#define TR_PYTHON_COMPONENT_GET_ASSIGN(name, type) \
		TR_PYTHON_COMPONENT_GET(name, type)\
		TR_PYTHON_COMPONENT_SET(name, type)\
		TR_PYTHON_COMPONENT_ASSIGN(name, type)\

PYBIND11_EMBEDDED_MODULE(TritonCore, m) {
	py::class_<Triton::ECS::Registry, std::shared_ptr<Triton::ECS::Registry>>(m, "EntityRegistry")
		.def(py::init<>())
		TR_PYTHON_COMPONENT_GET_ASSIGN("transform", Triton::Components::Transform)
		TR_PYTHON_COMPONENT_GET_ASSIGN("mesh_filter", Triton::Components::MeshFilter)
		TR_PYTHON_COMPONENT_GET_ASSIGN("mesh_renderer", Triton::Components::MeshRenderer)
		.def("create",
			[](Triton::ECS::Registry& aRegistry)
	{
		return aRegistry.create();
	});

	py::class_<Triton::Core::DataMap, std::shared_ptr<Triton::Core::DataMap>>(m, "DataMap")
		.def(py::init<>())
		.def("register_mesh", &Triton::Core::DataMap::RegisterMesh)
		.def("register_texture", &Triton::Core::DataMap::RegisterTexture);
}