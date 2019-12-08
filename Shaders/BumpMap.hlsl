#pragma pack_matrix(row_major)

#include "Buffers.hlsl"
#include "Lighting.hlsl"

struct VertexInputType
{
    float4 position : POSITION;
    float2 tex : TEXCOORD0;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
    float3 bitangent : BINORMAL;
};

struct PixelInputType
{
    float4 position : SV_POSITION;
    float2 tex : TEXCOORD0;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
    float3 bitangent : BINORMAL;
    float3 viewDirection : TEXCOORD1;
    float3 worldPos : TEXCOORD2;
};

Texture2D shaderTextures[2] : register(t0);
SamplerState SampleType;

PixelInputType vertex_bump(VertexInputType input)
{
    PixelInputType output;
    
    // Change the position vector to be 4 units for proper matrix calculations.
    input.position.w = 1.0f;

    // Calculate the position of the vertex against the world, view, and projection matrices.
    output.position = mul(input.position, model);
    output.position = mul(output.position, view);
    output.position = mul(output.position, projection);
    
    // Store the texture coordinates for the pixel shader.
    output.tex = input.tex;
    
    // Calculate the normal vector against the world matrix only and then normalize the final value.
    output.normal = mul(input.normal, (float3x3) model);
    output.normal = normalize(output.normal);

    // Calculate the tangent vector against the world matrix only and then normalize the final value.
    output.tangent = mul(input.tangent, (float3x3) model);
    output.tangent = normalize(output.tangent);

    // Calculate the bi-tangent vector against the world matrix only and then normalize the final value.
    output.bitangent = mul(input.bitangent, (float3x3) model);
    output.bitangent = normalize(output.bitangent);

    float4 worldPos = mul(input.position, model);

    output.worldPos = worldPos;

    output.viewDirection = cam_Position.xyz - worldPos.xyz;

    output.viewDirection = normalize(output.viewDirection);

    return output;
}

float4 pixel_bump(PixelInputType input) : SV_TARGET
{
    float4 textureColor;
    float4 normalMap;
    float3 bumpNormal;
    float4 color;

    // Sample the texture pixel at this location.
    textureColor = shaderTextures[0].Sample(SampleType, input.tex);

    // Sample the pixel in the bump map.
    normalMap = shaderTextures[1].Sample(SampleType, input.tex);

    //Change normal map range from [0, 1] to [-1, 1]
    normalMap = (2.0f * normalMap) - 1.0f;

    //Create the "Texture Space"
    float3x3 texSpace = float3x3(input.tangent, input.bitangent, input.normal);

    //Convert normal from normal map to texture space and store in input.normal
    input.normal = normalize(mul(normalMap, texSpace));
    
    // Lighting
    LightData ld;
    ld.normal = input.normal;
    ld.worldPos = input.worldPos;
    ld.viewDirection = input.viewDirection;

    float4 pointLightColor = pointLightCalc(ld);
    float4 spotLightColor = spotLightCalc(ld);
    float4 dirLightColor = dirLightCalc(ld);

    // Determine the final amount of diffuse color based on the diffuse color combined with the light intensity.
    color = pointLightColor + spotLightColor + dirLightColor;

    // Multiply the texture pixel and the final diffuse color to get the final pixel color result.
    color = color * textureColor;
	
    return color;
}