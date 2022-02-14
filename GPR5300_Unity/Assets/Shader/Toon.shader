Shader "Unlit/Toon"
{
    Properties
    {
        _MainTex ("Texture", 2D) = "white" {}
        _Tint ("Tint", Color) = (1., 1., 1., 1.)
        _EdgeSmoothness("Edge Smoothness", Range(0.0, 0.25)) = 0.05
        _Glossiness ("Glossiness", Float) = 32
        _SpecularColor("Specular Color", Color) = (1., 1., 1., 1.)
        _AmbientColor ("Ambient Color", Color) = (0.4, 0.4, 0.4)

        _RimThreshold("Rim Threshold", Range(0.0, 1.0)) = 0.1
        _RimAmount ("Rim Amount", Range(0.0, 1.0)) = 0.7
        _RimColor ("Rim Color", Color) = (1., 1., 1., 1.)
    }
    SubShader
    {
        Tags { "RenderType"="Opaque" }
        LOD 100

        Pass
        {
            CGPROGRAM
            #pragma vertex vert
            #pragma fragment frag
            // make fog work
            #pragma multi_compile_fog

            #include "UnityCG.cginc"
            #include "UnityLightingCommon.cginc"
            
            struct appdata
            {
                float4 pos    : POSITION;
                float2 uv     : TEXCOORD0;
                float3 normal : NORMAL;
            };

            struct v2f
            {
                float4 pos	  : SV_POSITION;
                float2 uv	  : TEXCOORD0;
                float3 normal : NORMAL;
                float3 viewDir : TEXCOORD1;
            };
            sampler2D _MainTex;
            float4 _MainTex_ST;

            float _EdgeSmoothness;

            float _Glossiness;

            float4 _SpecularColor;

            float4 _Tint;
            float4 _AmbientColor;

            float _RimThreshold;
            float _RimAmount;
            float4 _RimColor;

            v2f vert (appdata v)
            {
                v2f o;
                o.pos = UnityObjectToClipPos(v.pos);
                o.uv = TRANSFORM_TEX(v.uv, _MainTex);

                o.normal = UnityObjectToWorldNormal(v.normal);

                o.viewDir = WorldSpaceViewDir(v.pos);

                return o;
            }

            fixed4 frag (v2f i) : SV_Target
            {
                // sample the texture
                fixed4 col = tex2D(_MainTex, i.uv);
                col *= _Tint;

                float3 normal = normalize(i.normal);

                // Lambert
                float NdotL = dot(normal, _WorldSpaceLightPos0);

                // Wie smooth der Übergang zwischen Licht und Schatten ist
                float lightIntensity = smoothstep(0, _EdgeSmoothness, NdotL);
            
                float4 light = lightIntensity * _LightColor0;

                // Specular Highlights
                float3 viewDir = normalize(i.viewDir);

                float3 halfVector = normalize(_WorldSpaceLightPos0 + viewDir);
                float NdotH = dot(normal, halfVector);

                float specularIntensity = pow(NdotH * lightIntensity, _Glossiness * _Glossiness);

                float specularIntensitySmooth = smoothstep(0.005, 0.01, specularIntensity);
                float4 specular = specularIntensitySmooth * _SpecularColor;

                // Rim Light
                float4 rimDot = 1 - dot(viewDir, normal);

                float rimIntensity = rimDot * pow(NdotL, _RimThreshold);

                rimIntensity = smoothstep(_RimAmount - 0.01, _RimAmount + 0.01, rimIntensity);

                float4 rim = rimIntensity * _RimColor;


                return col * (_AmbientColor + light + specular + rim);
            }
            ENDCG
        }
    }
}
