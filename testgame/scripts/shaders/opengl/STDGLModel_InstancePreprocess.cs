#version 460 core

#extension GL_KHR_shader_subgroup_basic : enable

#include "STDGLModel.incl"
#include "STDGLCamera.incl"

layout(local_size_x = STDGLMODEL_INSTANCE_PREPROCESS_GROUP_SIZE, local_size_y = 1, local_size_z = 1) in;

shared uint LocalAmounts[STDGLMODEL_LOD_MAX_COUNT];
shared uint GroupIDs[4];

bool InvPlaneTest(vec3 center, float radius, vec4 plane) {
    float distance = dot(plane.xyz, center) + plane.w;
        
    if (distance < -radius) {
        return true;
    }
    return false;
}

void main() {
    if (gl_GlobalInvocationID.x == 0)
        for (int LOD = 0; LOD < STDGLMODEL_LOD_MAX_COUNT; LOD++) {
            IndirectBuffers[LOD][0].instanceCount = 0;
            LocalAmounts[LOD] = 0;
        }

    // Instances with NaN in 0 0 are "disabled"
    if (isnan(InstanceMatrices[gl_GlobalInvocationID.x][0][0])) return;
    barrier();

    vec3 position = InstanceMatrices[gl_GlobalInvocationID.x][3].xyz;
    float maxscale;

    {
        float xscale = length(InstanceMatrices[gl_GlobalInvocationID.x][0].xyz);
        float yscale = length(InstanceMatrices[gl_GlobalInvocationID.x][1].xyz);
        float zscale = length(InstanceMatrices[gl_GlobalInvocationID.x][2].xyz);

        maxscale = max(max(xscale, yscale), zscale);
    }

    float radius = ModelRadius * maxscale;
    /*
    for (int i = 0; i < 6; i++)
        if (InvPlaneTest(position, radius, CameraFrustum[i]))
            return;
     */
    int LOD = 0; //temp

    uint LocalID = atomicAdd(LocalAmounts[LOD], 1u);

    barrier();

    if (subgroupElect()) {
        for (int LOD = 0; LOD < STDGLMODEL_LOD_MAX_COUNT; LOD++) {
           GroupIDs[LOD] = atomicAdd(IndirectBuffers[LOD][0].instanceCount, LocalAmounts[LOD]);
        }
    }

    barrier();

    uint ID = GroupIDs[LOD] + LocalID;
    InstanceIndeces[LOD][ID] = gl_GlobalInvocationID.x;

    // Replicate the result into the draw buffers
    if (!subgroupElect()) return;

    uint currLock = atomicExchange(Lock, uint(false));
    barrier();
    if (currLock == 1u) {
        for (int LOD = 0; LOD < STDGLMODEL_LOD_MAX_COUNT; LOD++)
            for (int mesh = 1; mesh < STDGLMODEL_MESH_MAX_COUNT; mesh++)
                IndirectBuffers[LOD][mesh].instanceCount = IndirectBuffers[LOD][0].instanceCount;

        Lock = uint(true);
    }
}