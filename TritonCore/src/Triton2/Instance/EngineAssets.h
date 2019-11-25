#pragma once

#include <string>

namespace Triton
{
	namespace Predefined
	{
		std::string _2dshaderSource = "#pragma pack_matrix(row_major)\n"
			"\n"
			"struct VertexInputType\n"
			"{\n"
			"    float4 position : POSITION;\n"
			"    float2 tex : TEXCOORD0;\n"
			"};\n"
			"\n"
			"struct PixelInputType\n"
			"{\n"
			"    float4 position : SV_POSITION;\n"
			"    float2 tex : TEXCOORD0;\n"
			"};\n"
			"\n"
			"cbuffer MatrixBuffer : register(b1)\n"
			"{\n"
			"    matrix projection;\n"
			"    matrix view;\n"
			"    matrix model;\n"
			"};\n"
			"\n"
			"Texture2D shaderTexture;\n"
			"SamplerState SampleType;\n"
			"\n"
			"PixelInputType vertex_quad(VertexInputType input)\n"
			"{\n"
			"    PixelInputType output;\n"
			"\n"
			"    // Change the position vector to be 4 units for proper matrix calculations.\n"
			"    input.position.w = 1.0f;\n"
			"\n"
			"    // Calculate the position of the vertex against the world, view, and projection matrices.\n"
			"    output.position = mul(input.position, model);\n"
			"    output.position = mul(output.position, projection);\n"
			"\n"
			"    // Store the input color for the pixel shader to use.\n"
			"    output.tex = input.tex;\n"
			"\n"
			"    return output;\n"
			"}\n"
			"\n"
			"float4 pixel_quad(PixelInputType input) : SV_TARGET\n"
			"{\n"
			"    return shaderTexture.Sample(SampleType, input.tex);\n"
			"}\n";
	}
}