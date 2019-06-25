#include "TRpch.h"
#include "Material.h"

void Triton::Data::Material::enable()
{
	object()->enable();
	//Shader->setUniformInt("material.matTexture", object()->Slot);
	//Shader->setUniformVector3("material.ambient", Ambient);
	//Shader->setUniformVector3("material.diffuse", Diffuse);
	//Shader->setUniformVector3("material.specular", Specular);
	Shader->setBufferValue("LightBuffer", "specularPower", &Shininess);
}

Triton::Data::Material::Material(reference<PType::Texture> obj)
	: PlatformWraper(obj), Ambient(1.0f, 1.0f, 1.0f), Diffuse(1.0f, 1.0f, 1.0f), Specular(1.0f, 1.0f, 1.0f), Shininess(32.0f)
{
}
