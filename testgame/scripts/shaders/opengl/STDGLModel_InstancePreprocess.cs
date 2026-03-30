#version 460 core

#include "STDGLModel.incl"
#include "STDGLCamera.incl"

layout(local_size_x = STDGLMODEL_INSTANCE_PREPROCESS_GROUP_SIZE, local_size_y = 1, local_size_z = 1) in;

bool InvPlaneTest(vec3 center, float radius, vec4 plane) {
    float distance = dot(plane.xyz, center) + plane.w;
        
    if (distance < -radius) {
        return true;
    }
    return false;
}

void main() {
    if (gl_GlobalInvocationID.x == 0)
        for (int LOD = 0; LOD < STDGLMODEL_LOD_MAX_COUNT; LOD++)
            IndirectBuffers[LOD][0].instanceCount = 0;
    barrier();

    if (isnan(InstanceMatrices[gl_GlobalInvocationID.x][0][0])) return;


    vec3 position = InstanceMatrices[gl_GlobalInvocationID.x][3].xyz;
    float maxscale;

    {
        float xscale = length(InstanceMatrices[gl_GlobalInvocationID.x][0].xyz);
        float yscale = length(InstanceMatrices[gl_GlobalInvocationID.x][1].xyz);
        float zscale = length(InstanceMatrices[gl_GlobalInvocationID.x][2].xyz);

        maxscale = max(max(xscale, yscale), zscale);
    }

    float radius = ModelRadius * maxscale;

    for (int i = 0; i < 6; i++)
        if (InvPlaneTest(position, radius, CameraFrustum[i]))
            return;

    int LOD = 0; //temp

    uint ID = atomicAdd(IndirectBuffers[LOD][0].instanceCount, 1u);

    InstanceIndeces[LOD][ID] = gl_GlobalInvocationID.x;
}