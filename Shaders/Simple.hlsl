#pragma pack_matrix(row_major)

cbuffer MatrixBuffer : register(b0)
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
};

PixelInputType vertex_Simple(VertexInputType input)
{
    PixelInputType output;
    
    // Change the position vector to be 4 units for proper matrix calculations.
    input.position.w = 1.0f;


    // Calculate the position of the vertex against the world, view, and projection matrices.
    output.position = mul(input.position, model);
    output.position = mul(output.position, view);
    output.position = mul(output.position, projection);
    
    return output;
}


float4 pixel_Simple(PixelInputType input) : SV_TARGET
{
    return float4(1.0f, 1.0f, 1.0f, 1.0f);
}