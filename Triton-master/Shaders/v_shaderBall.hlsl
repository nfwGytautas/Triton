cbuffer persistant_Persistant
{
    matrix projectionMatrix;
};

cbuffer object_PerObject
{
    matrix transformationMatrix;
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
    float3 normal : NORMAL;
};

PixelInputType ShaderballVertexShader(VertexInputType input)
{
    PixelInputType output;
    
    // Change the position vector to be 4 units for proper matrix calculations.
    input.position.w = 1.0f;

    // Calculate the position of the vertex against the world, view, and projection matrices.
    output.position = mul(input.position, transformationMatrix);
    output.position = mul(output.position, projectionMatrix);
    
    // Store the input color for the pixel shader to use.
    output.tex = input.tex;
    
    // Calculate the normal vector against the world matrix only.
    output.normal = mul(input.normal, (float3x3) transformationMatrix);
	
    // Normalize the normal vector.
    output.normal = normalize(output.normal);
	
	// Calculate the position of the vertex in the world.
    float4 worldPosition = mul(input.position, transformationMatrix);

    return output;
}