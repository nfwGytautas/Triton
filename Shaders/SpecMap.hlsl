#pragma pack_matrix(row_major)

#define MAX_POINT_LIGHTS 16
#define MAX_DIR_LIGHTS 1
#define MAX_SPOT_LIGHTS 4

#define SHINE 32
#define AMBIENT_INTENSITY 0.15

cbuffer Settings : register(b0)
{
    float pointLights;
    float directionalLights;
    float spotLights;

    float padding;
};

cbuffer MatrixBuffer : register(b1)
{
    matrix projection;
    matrix view;
    matrix model;
};

cbuffer PointLights : register(b2)
{
    float4 pl_Position[MAX_POINT_LIGHTS];
    float4 pl_Color[MAX_POINT_LIGHTS];
    float4 pl_AtennuationAndRange[MAX_POINT_LIGHTS];
};

cbuffer SpotLights : register(b3)
{
    float4 sl_Position[MAX_SPOT_LIGHTS];
    float4 sl_ColorAndAngle[MAX_SPOT_LIGHTS];
    float4 sl_Direction[MAX_SPOT_LIGHTS];
    float4 sl_AtennuationAndRange[MAX_SPOT_LIGHTS];
};

cbuffer DirectionalLights : register(b4)
{
    float4 dl_Position[MAX_DIR_LIGHTS];
    float4 dl_Color[MAX_DIR_LIGHTS];
    float4 dl_Direction[MAX_DIR_LIGHTS];
};

cbuffer CameraBuffer : register(b5)
{
    float4 cam_Position;
    float4 cam_Dir;
};

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

Texture2D shaderTextures[3] : register(t0);
SamplerState SampleType;

PixelInputType vertex_dbs(VertexInputType input)
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

float4 pointLightCalc(PixelInputType input, float4 specSample);
float4 spotLightCalc(PixelInputType input, float4 specSample);
float4 dirLightCalc(PixelInputType input, float4 specSample);

float4 pixel_dbs(PixelInputType input) : SV_TARGET
{
    float4 textureColor;
    float4 normalMap;
    float3 bumpNormal;
    float4 specSample;
    float4 color;

    // Sample the texture pixel at this location.
    textureColor = shaderTextures[0].Sample(SampleType, input.tex);

    // Sample the pixel in the bump map.
    normalMap = shaderTextures[1].Sample(SampleType, input.tex);

    // Sample the pixel in the spec map.
    specSample = shaderTextures[2].Sample(SampleType, input.tex);

    //Change normal map range from [0, 1] to [-1, 1]
    normalMap = (2.0f * normalMap) - 1.0f;

    //Create the "Texture Space"
    float3x3 texSpace = float3x3(input.tangent, input.bitangent, input.normal);

    //Convert normal from normal map to texture space and store in input.normal
    input.normal = normalize(mul(normalMap, texSpace));

    float4 pointLightColor = pointLightCalc(input, specSample);
    float4 spotLightColor = spotLightCalc(input, specSample);
    float4 dirLightColor = dirLightCalc(input, specSample);

    // Determine the final amount of diffuse color based on the diffuse color combined with the light intensity.
    color = pointLightColor + spotLightColor + dirLightColor;

    // Multiply the texture pixel and the final diffuse color to get the final pixel color result.
    color = color * textureColor;
	
    return color;
}

float4 pointLightCalc(PixelInputType input, float4 specSample)
{
    float4 color = float4(0, 0, 0, 0);

    for (int i = 0; i < pointLights; i++)
    {
        float3 lightPosition = pl_Position[i].xyz;
        float3 lightColor = pl_Color[i].xyz;

        float constant = pl_AtennuationAndRange[i].x;
        float _linear = pl_AtennuationAndRange[i].y;
        float quadratic = pl_AtennuationAndRange[i].z;
        float range = pl_AtennuationAndRange[i].w;

        float3 lightDir = lightPosition - input.worldPos;
        float d = length(lightDir);

        if (d > range)
        {
            break;
        }

        // Normalize light dir
        lightDir /= d;

        // diffuse shading
        float lightIntensity = saturate(dot(input.normal, lightDir));

        // specular shading
        float3 R = reflect(normalize(lightDir), normalize(input.normal));

        float3 ambient = lightColor * AMBIENT_INTENSITY;
        float3 diffuse = lightColor * lightIntensity;
        float3 specular = lightColor.xyz * pow(saturate(dot(R, input.viewDirection)), SHINE) * specSample.xyz;

        // attenuation
        float distance = length(lightPosition - input.worldPos);
        float attenuation = 1.0 / (constant + _linear * distance + quadratic * (distance * distance));

        ambient *= attenuation;
        diffuse *= attenuation;
        specular *= attenuation;

        color = float4(color.xyz + (ambient + diffuse + specular).xyz, color.w);
    }

    return color;
}

float4 spotLightCalc(PixelInputType input, float4 specSample)
{
    float4 color = float4(0, 0, 0, 0);
    
    for (int i = 0; i < spotLights; i++)
    {
        float3 lightPosition = sl_Position[i].xyz;

        float constant = sl_AtennuationAndRange[i].x;
        float _linear = sl_AtennuationAndRange[i].y;
        float quadratic = sl_AtennuationAndRange[i].z;
        float range = sl_AtennuationAndRange[i].w;

        float3 lightColor = sl_ColorAndAngle[i].xyz;
        float angle = sl_ColorAndAngle[i].w;

        float3 direction = normalize(sl_Direction[i].xyz);

        float3 lightDir = lightPosition - input.worldPos;
        float d = length(lightDir);

        if (d > range)
        {
            break;
        }

        // Normalize light dir
        lightDir /= d;

        // diffuse shading
        float lightIntensity = saturate(dot(input.normal, lightDir));

        // specular shading
        float3 R = reflect(normalize(lightDir), normalize(input.normal));

        // combine results
        float3 ambient = lightColor * AMBIENT_INTENSITY;
        float3 diffuse = lightColor * lightIntensity;
        float3 specular = lightColor.xyz * pow(saturate(dot(R, input.viewDirection)), SHINE) * specSample.xyz;
        
        // spotlight intensity
        float intensity = pow(max(dot(lightDir, normalize(direction)), 0.0f), angle);

        // attenuation
        float attenuation = 1.0 / (constant + _linear * d + quadratic * (d * d));

        // combine results
        ambient *= intensity;
        diffuse *= intensity;
        specular *= intensity;

        ambient *= attenuation;
        diffuse *= attenuation;
        specular *= attenuation;

        color = float4(color.xyz + (ambient + diffuse + specular).xyz, color.w);
    }

    return color;
}

float4 dirLightCalc(PixelInputType input, float4 specSample)
{
    float4 color = float4(0, 0, 0, 0);

    for (int i = 0; i < directionalLights; i++)
    {
        float3 lightPosition = dl_Position[i].xyz;
        float4 lightColor = float4(dl_Color[i].xyz, 1.0f);
        float3 direction = dl_Direction[i].xyz;

        float3 lightDir = normalize(-direction);

        // diffuse shading
        float lightIntensity = saturate(dot(input.normal, lightDir));

        // specular shading
        float3 R = reflect(normalize(lightDir), normalize(input.normal));

        // combine results
        float3 ambient = lightColor * AMBIENT_INTENSITY;
        float3 diffuse = lightColor * lightIntensity;
        float3 specular = lightColor.xyz * pow(saturate(dot(R, input.viewDirection)), SHINE) * specSample.xyz;

        color = float4(color.xyz + (ambient + diffuse + specular), color.w);
    }

    return color;
}