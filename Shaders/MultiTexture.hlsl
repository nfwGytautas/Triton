#pragma pack_matrix(row_major)

#define MAX_POINT_LIGHTS 16
#define MAX_DIR_LIGHTS 1
#define MAX_SPOT_LIGHTS 4

#define SHINE 32
#define AMBIENT_INTENSITY 0.15

#define GAMMA 2.0

cbuffer MatrixBuffer : register(b1)
{
    matrix projection;
    matrix view;
    matrix model;
};

struct VertexInputType
{
    float4 position : POSITION;
    float2 tex : TEXCOORD0;
    float3 normal : NORMAL;
};

struct PixelInputType
{
    float4 position : SV_POSITION;
    float2 tex : TEXCOORD0;
};

Texture2D shaderTextures[2] : register(t0);
SamplerState SampleType;

PixelInputType vertex_multi(VertexInputType input)
{
    PixelInputType output;

    // Change the position vector to be 4 units for proper matrix calculations.
    input.position.w = 1.0f;

    // Calculate the position of the vertex against the world, view, and projection matrices.
    output.position = mul(input.position, model);
    output.position = mul(output.position, view);
    output.position = mul(output.position, projection);
    
    // Store the input color for the pixel shader to use.
    output.tex = input.tex;
    
    return output;
}

float4 pixel_multi(PixelInputType input) : SV_TARGET
{
    float4 color1;
    float4 color2;
    float4 blendColor;

    // Get the pixel color from the first texture.
    color1 = shaderTextures[0].Sample(SampleType, input.tex);

    // Get the pixel color from the second texture.
    color2 = shaderTextures[1].Sample(SampleType, input.tex);

    // Blend the two pixels together and multiply by the gamma value.
    blendColor = color1 * color2 * GAMMA;
    
    // Saturate the final color.
    blendColor = saturate(blendColor);

    return blendColor;
}
