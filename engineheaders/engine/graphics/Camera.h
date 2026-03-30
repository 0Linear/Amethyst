#pragma once
#include "engine/master.h"
#include <cstdint>
#include <string>
// Default camera values
const float CAMERA_DEFAULT_YAW         =  0.0f;
const float CAMERA_DEFAULT_PITCH       =  0.0f;
const float CAMERA_DEFAULT_SPEED       =  2.5f;
const float CAMERA_DEFAULT_SENSITIVITY =  0.1f;
const float CAMERA_DEFAULT_FOV         =  120.0f;
const float CAMERA_DEFAULT_NEAR        =  1.0f;
const float CAMERA_DEFAULT_FAR         =  32768.0f;

/*!
* \brief A camera interface.
*/
class Camera {
public:
    //! Name of the camera by which it can be queried
    std::string Name; 

    // Camera attributes.
    vec3 Position;
    // Euler angles.
    float Yaw   = CAMERA_DEFAULT_YAW;
    float Pitch = CAMERA_DEFAULT_PITCH;
    float MouseSensitivity = CAMERA_DEFAULT_SENSITIVITY;
    
    //! Coordinate front axis.
    vec3 Front = vec3(1, 0, 0);
    //! Coordinate up axis.
    vec3 Up = vec3(0, 0, 1);
    //! Coordinate right axis.
    vec3 Right = vec3(0, 1, 0);

    //! Processes input received from a mouse input system. Expects the offset value in both the x and y direction.
    virtual void ProcessMouseMovement(vec2 offset, bool constrainPitch = true) = 0;

    virtual uint32_t GetTexture() = 0;
    virtual uint32_t GetDepthTexture() = 0;

    virtual ~Camera() {};


    inline vec2 GetResolution() {return Resolution; };
protected:

    float FOV = CAMERA_DEFAULT_FOV;

    vec2 Resolution;

    struct frustum
    {
        struct plane{
            // unit vector + distance from origin to the nearest point in the plane
            vec4 Data = vec4(0.0f, 1.0f, 0.0f, 0.0f); 
            
            plane() = default;

	        plane(const vec3& p1, const vec3& norm)
	        	: Data(norm.norm(), -norm.norm().dot(p1)) {}

        };
    
        plane TopFace;
        plane BottomFace;

        plane RightFace;
        plane LeftFace;

        plane FarFace;
        plane NearFace;
    };

    frustum CreateFrustum() {
        frustum     Frustum;
        float halfVSide = CAMERA_DEFAULT_FAR * tanf(FOV * 0.5f);
        float halfHSide = halfVSide * Resolution.x / Resolution.y;
        vec3 frontMultFar = Front * CAMERA_DEFAULT_FAR;

        Frustum.NearFace = { Position + Front * CAMERA_DEFAULT_NEAR, Front };
        Frustum.FarFace = { Position + frontMultFar, Front * -1 };
        Frustum.RightFace = { Position,
                                (frontMultFar - Right * halfHSide).cross(Up) };
        Frustum.LeftFace = { Position,
                                Up.cross(frontMultFar + Right * halfHSide) };
        Frustum.BottomFace = { Position,
                                Right.cross(frontMultFar - Up * halfVSide) };
        Frustum.BottomFace = { Position,
                                (frontMultFar + Up * halfVSide).cross(Right) };

        return Frustum;
    }
    
};