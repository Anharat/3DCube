// SimplePixelShader.hlsl

struct PixelInputType
{
    float4 position : SV_POSITION;
    float2 texcoord : TEXCOORD0;
};

cbuffer CameraBufferType
{
    float3 cameraPosition;
    float padding1; // Ensure 16-byte alignment
    float3 cameraDirection;
    float padding2; // Ensure 16-byte alignment
};


struct Ray
{
    float3 origin;
    float3 direction;
};

struct SDFResult
{
    float distance;
    float3 normal;
    float2 texCoords; // Placeholder for any extra information needed for shading
};

// Define screen resolution and other constants
static const float SCREEN_WIDTH = 700.0;
static const float SCREEN_HEIGHT = 700.0;
static const float ASPECT_RATIO = SCREEN_WIDTH / SCREEN_HEIGHT;

// Camera parameters
static const float3 CAMERA_POSITION = float3(0.0, 0.0, -1.5);
static const float FOV = 0.78539816339; // 45 degrees in radians
static const float3 CAMERA_DIRECTION = float3(0.0, -1.0, 1.0);

static const int MAX_STEPS = 100;
static const float EPSILON = 0.001;
static const float MAX_DISTANCE = 1000.0;

// Function to calculate ray origin and direction
Ray calculateRay(float2 texcoord, float3 cameraPos, float3 cameraDir, float fov, float aspectRatio)
{
    // Normalize texture coordinates to [-1, 1]
    float2 uv = (texcoord - 0.5) * 2.0;
    uv.x *= aspectRatio;

    // Camera basis vectors (assuming camera up is float3(0, 1, 0))
    float3 right = normalize(cross(float3(0, 1, 0), cameraDir));
    float3 up = normalize(cross(cameraDir, right));

    // Calculate ray direction considering aspect ratio and FOV
    float3 rayDir = normalize(cameraDir + uv.x * right + uv.y * up * (1.0 / aspectRatio));

    // Create and return the Ray
    Ray ray;
    ray.origin = cameraPos;
    ray.direction = rayDir;

    return ray;
}

// Helper function to find the maximum component of a vector
float maxComponent(float3 v)
{
    return max(max(v.x, v.y), v.z);
}

// Helper function to calculate the SDF of a cube
float sdfCube(float3 p, float cubeLength)
{
    float3 d = abs(p) - cubeLength / 2.0;
    return length(max(d, 0.0)) + min(max(d.x, max(d.y, d.z)), 0.0);
}

float3 calculateNormal(float3 p, float cubeLength, float eps)
{
    float d = sdfCube(p, cubeLength);
    float3 epsX = float3(eps, 0.0, 0.0);
    float3 epsY = float3(0.0, eps, 0.0);
    float3 epsZ = float3(0.0, 0.0, eps);

    float3 normal = normalize(float3(
        sdfCube(p + epsX, cubeLength) - d,
        sdfCube(p + epsY, cubeLength) - d,
        sdfCube(p + epsZ, cubeLength) - d
    ));

    return normal;
}

float2 calculateSurfaceCoords(float3 p, float3 normal, float cubeLength)
{
    // Normalize the point within the cube [-cubeLength/2, cubeLength/2]
    float3 normalizedPoint = (p + cubeLength / 2.0) / cubeLength;

    // Initialize texture coordinates
    float2 uv;

    // Check which face of the cube we're dealing with based on the normal
    if (abs(normal.x) > abs(normal.y) && abs(normal.x) > abs(normal.z))
    {
        // Side faces (X-axis)
        uv = normalizedPoint.yz;
    }
    else if (abs(normal.y) > abs(normal.z))
    {
        // Top and bottom faces (Y-axis)
        uv = normalizedPoint.xz;
    }
    else
    {
        // Front and back faces (Z-axis)
        uv = normalizedPoint.xy;
    }

    // Adjust UV coordinates to be within [0, 1]
    return uv;
}

SDFResult sdfWallOfCubes(float3 p)
{
    static const float CUBE_LENGTH = 0.25;
    static const float SPACING = 0.5;
    static const float3 OFFSET = float3(-10.0, -10.0, 0.0);

    // Adjust position by offset and create tiling effect
    p -= OFFSET;
    p = fmod(p + SPACING / 2.0, SPACING) - SPACING / 2.0;

    // Calculate SDF for a single cube
    float dist = sdfCube(p, CUBE_LENGTH);
    float3 normal = calculateNormal(p, CUBE_LENGTH, EPSILON);

    // Calculate texture coordinates
    float2 texCoords = calculateSurfaceCoords(p, normal, CUBE_LENGTH);

    SDFResult result;
    result.distance = dist;
    result.normal = normal;
    
    result.texCoords = texCoords; // Store the calculated texture coordinates

    return result;
}

float3 rayMarch(Ray ray)
{
    float totalDistance = 0.0;
    for (int i = 0; i < MAX_STEPS; i++)
    {
        float3 p = ray.origin + totalDistance * ray.direction;
        SDFResult result = sdfWallOfCubes(p);

        if (result.distance < EPSILON)
        {
            // Hit something
            float3 color = float3(0.0, 0.0, 1.0); // Blue for all sides
            float fadeEffect;

            // Determine which face of the cube is being hit
            if (abs(result.normal.x) > abs(result.normal.y) && abs(result.normal.x) > abs(result.normal.z))
            {
                // Sides - Fade from blue at the front to black at the back
                fadeEffect = 1.0 - result.texCoords.y; // Fade effect based on y-coordinate for front to back
            }
            else if (abs(result.normal.y) > abs(result.normal.z))
            {
                // Top/Bottom - Fade from blue to black from front to back
                fadeEffect = 1.0 - result.texCoords.y; // Fade effect based on y-coordinate
            }
            else
            {
                // Front/Back - Solid blue
                fadeEffect = 1.0; // No fade effect
            }

            // Apply the fade effect
            color *= fadeEffect;

            return color; // Return the calculated color
        }
        totalDistance += result.distance;

        if (totalDistance >= MAX_DISTANCE)
            break;
    }
    return float3(0.0, 0.0, 0.0); // Background color
}

// Main function
float4 main(PixelInputType input) : SV_TARGET
{
    Ray ray = calculateRay(input.texcoord, cameraPosition, cameraDirection, FOV, ASPECT_RATIO);

    float3 color = rayMarch(ray);
    return float4(color, 1.0);
}