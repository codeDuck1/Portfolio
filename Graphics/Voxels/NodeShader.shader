Shader "Custom/InstancedCubes" {
    Properties
    {
        _Color ("Color", Color) = (1,1,1,1)
    }
    SubShader
    {
        Tags { "RenderType"="Opaque" "DisableBatching"="True" }
        LOD 100
        
        Pass
        {
            CGPROGRAM
            #pragma vertex vert
            #pragma fragment frag
            #pragma target 4.5
            #pragma multi_compile_instancing
            
            #include "UnityCG.cginc"
            
            // Mesh data we need, Unity automatically gets it for us
            struct appdata
            {
                float4 vertex : POSITION;
                float3 normal : NORMAL;
            };
            
            // Unity's way of binding vertex attributes to variables and passing data from 
            // vertex to fragment shader
            struct v2f
            {
                float4 pos : SV_POSITION;
                float3 normal : NORMAL;
                float4 color : COLOR0;
            };

            struct RenderNode
            {
                float4x4 transform;
                float4 color;
            };
            
            // Transform data created by compute shader
            StructuredBuffer<RenderNode> transformBuffer;
            
            v2f vert (appdata v, uint instanceID : SV_InstanceID)
            {
                v2f o;
                
                // Get the transformation matrix for this instance
                float4x4 objectToWorld = transformBuffer[instanceID].transform; // Model matrix
                float4 nodeColor = transformBuffer[instanceID].color;
                
                // Transform from object space to world space
                float4 worldPosition = mul(objectToWorld, float4(v.vertex.xyz, 1.0));
                // Transfrom from world space to clip space, set vertex position attribute
                o.pos = mul(UNITY_MATRIX_VP, worldPosition);
                
                // Set surface normals, extracting rotation from the transform matrix
                // to determine direction
                float3x3 normalMatrix;
                normalMatrix[0] = normalize(objectToWorld[0].xyz); // X-axis direction
                normalMatrix[1] = normalize(objectToWorld[1].xyz); // Y-axis direction
                normalMatrix[2] = normalize(objectToWorld[2].xyz); // Z-axis direction
                // Transform object space normal to world space normal
                o.normal = normalize(mul(normalMatrix, v.normal));
                // Simple lighting based on normal direction 
                float3 lightDir = normalize(float3(1, 1, -1));
                float ndotl = max(0.2, dot(o.normal, lightDir)); 
                
                // Apply lighting to the node color from compute shader
                o.color = nodeColor * float4(ndotl.xxx, 1);
                
                return o;
            }
            
            fixed4 frag (v2f i) : SV_Target
            {
                // Simply apply color data from vertex shader
                return i.color;
            }
            ENDCG
        }
    }
}