Shader "Unlit/Rust"
{
    Properties
    {
        _MainTex("Main Texture", 2D) = "white" {}
        _BlendTex("Blend Texture", 2D) = "black" {}
        _TransitionTex ("Transition Mask", 2D) = "white" {}
        _N("Normal", Vector) = (0, 0, 0)
        _H("Distance from Origin", Float) = 0
        _BlendOffset("Blend Offset", Float) = 0.0
        _BlendSize("Blend Size", Float) = 1.0
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

            struct appdata
            {
                float4 pos    : POSITION;
                float2 uv     : TEXCOORD0;
                float3 normal : NORMAL;
            };

            struct v2f
            {
                float4 pos	   : SV_POSITION;
                float2 uv	   : TEXCOORD0;
                float3 normal  : NORMAL;
                float3 viewDir : TEXCOORD1;
                float3 wPos    : TEXCOORD2;
            };

            sampler2D _MainTex;
            float4 _MainTex_ST;

            sampler2D _BlendTex;
            sampler2D _TransitionTex;

            float3 _N;
            float _H;

            float _BlendOffset;
            float _BlendSize;

            v2f vert(appdata v)
            {
                v2f o;
                o.pos = UnityObjectToClipPos(v.pos);
                o.uv = TRANSFORM_TEX(v.uv, _MainTex);
                o.normal = UnityObjectToWorldNormal(v.normal);
                o.viewDir = WorldSpaceViewDir(v.pos);
                o.wPos = mul(unity_ObjectToWorld, v.pos);

                return o;
            }

            float overlay(float a, float b)
            {
                if (a < 0.5)
                {
                    return a * b * 2.0;
                }
                else
                {
                    return 1.0 - 2.0 * (1.0 - a) * (1.0 - b);
                }
            }

            fixed4 frag (v2f i) : SV_Target
            {
                // sample the texture
                fixed4 baseCol = tex2D(_MainTex, i.uv);
                fixed4 blendCol = tex2D(_BlendTex, i.uv);
                fixed transitionMask = tex2D(_TransitionTex, i.uv).r;

                // Signed Distance Field of Plane
                float3 dist = dot(i.wPos, _N) - _H;

                dist = max(min((dist + _BlendOffset) * _BlendSize + 0.5, 1.0), 0.0);
                float blend = overlay(dist, transitionMask);
                
                return float4(lerp(blendCol.xyz, baseCol.xyz, blend), 1.0);
            }
            ENDCG
        }
    }
}
