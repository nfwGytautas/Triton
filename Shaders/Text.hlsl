#pragma pack_matrix(row_major)

struct VertexInputType
{
    float4 position : POSITION;
    float2 tex : TEXCOORD0;
};

struct PixelInputType
{
    float4 position : SV_POSITION;
    float2 tex : TEXCOORD0;
};

cbuffer MatrixBuffer : register(b1)
{
    matrix projection;
    matrix view;
    matrix model;
};

Texture2D shaderTexture;
SamplerState SampleType;

PixelInputType vertex_text(VertexInputType input)
{
    PixelInputType output;

    // Change the position vector to be 4 units for proper matrix calculations.
    input.position.w = 1.0f;

    // Calculate the position of the vertex against the world, view, and projection matrices.
    output.position = mul(input.position, projection);
    
    // Store the input color for the pixel shader to use.
    output.tex = input.tex;
	
    return output;
}

float4 pixel_text(PixelInputType input) : SV_TARGET
{
    float4 textureColor = shaderTexture.Sample(SampleType, input.tex);
    
    float4 color = float4(1.0, 1.0, 1.0, 1.0) * textureColor.rrra;

    // If the color is black on the texture then treat this pixel as transparent.
    if(color.r == 0.0f)
    {
        color.a = 0.0f;
    }
    else
    {
        color.a = 1.0f;
        //color = color * pixelColor;
    }

    return color;
}