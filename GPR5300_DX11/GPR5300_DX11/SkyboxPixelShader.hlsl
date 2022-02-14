struct PS_IN
{
    float4 pos : SV_Position;
    float3 texCoords : TEXCOORD;
};

TextureCube cubeMap : register(t0);
SamplerState cubeMapSampler : register(s0);

float4 PS(PS_IN input) : SV_TARGET
{
    return cubeMap.Sample(cubeMapSampler, input.texCoords);
}