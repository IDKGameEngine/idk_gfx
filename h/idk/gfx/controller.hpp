#pragma once

#include <SDL3/SDL_events.h>
#include <glm/glm.hpp>


namespace idk
{
    class ICharacterController
    {
    public:
        float moveSpeed = 1.0f;
        float lookSpeed = 0.001f;

        glm::vec3 totalMove;
        float totalPitch, totalYaw;
        bool mButtonA, mButtonB, mButtonX, mButtonY;

        ICharacterController()
        :   totalMove(0.0f),
            totalPitch(0.0f), totalYaw(0.0f),
            mButtonA(false), mButtonB(false),
            mButtonX(false), mButtonY(false)
        {

        }
    
        virtual void update() = 0;
        
        void move(const glm::vec3 &v) { totalMove += moveSpeed*v; }
        void pitch(float r) { totalPitch += lookSpeed*r; }
        void yaw(float r) { totalYaw += lookSpeed*r; }
    
        void getMotion(glm::vec3 &moveDelta, float &pitchDelta, float &yawDelta)
        {
            moveDelta  = totalMove;
            pitchDelta = totalPitch;
            yawDelta   = totalYaw;

            totalMove  *= 0.0f;
            totalPitch *= 0.0f;
            totalYaw   *= 0.0f;
        }

    };

    class TestCharacterController: public ICharacterController
    {
    public:
        virtual void update() final;

    };

}

