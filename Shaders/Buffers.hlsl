#pragma pack_matrix(row_major)

#ifndef __TRITON_HLSL_BUFFERS___
#define __TRITON_HLSL_BUFFERS___

#define MAX_POINT_LIGHTS 16
#define MAX_DIR_LIGHTS 1
#define MAX_SPOT_LIGHTS 4

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

#endif // __TRITON_HLSL_BUFFERS___