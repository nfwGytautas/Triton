#pragma pack_matrix(row_major)

#ifndef __TRITON_HLSL_LIGHTING___
#define __TRITON_HLSL_LIGHTING___

#include "Buffers.hlsl"

#define SHINE 32
#define AMBIENT_INTENSITY 0.15

struct LightData
{
    float3 normal : NORMAL;
    float3 viewDirection : TEXCOORD1;
    float3 worldPos : TEXCOORD2;
};

float4 pointLightCalc(LightData input)
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

        float3 halfway = lightDir + input.viewDirection;

        // Normalize light dir
        lightDir /= d;

        // diffuse shading
        float lightIntensity = saturate(dot(input.normal, lightDir));

        // specular shading
        float3 R = reflect(normalize(lightDir), normalize(input.normal));

        float3 ambient = lightColor * AMBIENT_INTENSITY;
        float3 diffuse = lightColor * lightIntensity;
        float3 specular = lightColor.xyz * pow(saturate(dot(R, halfway)), SHINE);

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

float4 spotLightCalc(LightData input)
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
        
        float3 halfway = lightDir + input.viewDirection;

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
        float3 specular = lightColor.xyz * pow(saturate(dot(R, halfway)), SHINE);
        
        // spotlight intensity
        float intensity = pow(max(dot(lightDir, normalize(halfway)), 0.0f), angle);

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

float4 dirLightCalc(LightData input)
{
    float4 color = float4(0, 0, 0, 0);

    for (int i = 0; i < directionalLights; i++)
    {
        float3 lightPosition = dl_Position[i].xyz;
        float4 lightColor = float4(dl_Color[i].xyz, 1.0f);
        float3 direction = dl_Direction[i].xyz;

        float3 lightDir = normalize(-direction);

        float3 halfway = lightDir + input.viewDirection;
        
        // diffuse shading
        float lightIntensity = saturate(dot(input.normal, lightDir));

        // specular shading
        float3 R = reflect(normalize(lightDir), normalize(input.normal));

        // combine results
        float3 ambient = lightColor * AMBIENT_INTENSITY;
        float3 diffuse = lightColor * lightIntensity;
        float3 specular = lightColor.xyz * pow(saturate(dot(R, halfway)), SHINE);

        color = float4(color.xyz + (ambient + diffuse + specular), color.w);
    }

    return color;
}

#endif // __TRITON_HLSL_LIGHTING___