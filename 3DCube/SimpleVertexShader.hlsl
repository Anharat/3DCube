// SimpleVertexShader.hlsl

// Define the input vertex structure
struct VertexInputType
{
    float3 position : POSITION;
    float2 texcoord : TEXCOORD0;
};

// Define the output structure of the vertex shader
struct PixelInputType
{
    float4 position : SV_POSITION;
    float2 texcoord : TEXCOORD0;
};

// Vertex Shader
PixelInputType main(VertexInputType input)
{
    PixelInputType output;
    // Pass the position directly, as it is already in clip space
    output.position = float4(input.position, 1.0f);
    // Pass the texture coordinate
    output.texcoord = input.texcoord;

    return output;
}
