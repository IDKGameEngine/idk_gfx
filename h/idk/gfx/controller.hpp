#pragma once

#include <SDL3/SDL_events.h>
#include <glm/glm.hpp>


namespace idk
{
    class ICharacterController
    {
    public:
        glm::vec3 totalMove, totalLook;
        bool mButtonA, mButtonB, mButtonX, mButtonY;

        ICharacterController()
        :   totalMove(0.0f), totalLook(0.0f),
            mButtonA(false), mButtonB(false),
            mButtonX(false), mButtonY(false)
        {

        }
    
        virtual void update() = 0;
        virtual void move(const glm::vec3 &moveDelta) = 0;
        virtual void look(const glm::vec3 &lookDelta) = 0;
    
        void getMotion(glm::vec3 &moveDelta, glm::vec3 &lookDelta)
        {
            moveDelta = totalMove;
            lookDelta = totalLook;
            totalMove *= 0.0f;
            totalLook *= 0.0f;
        }
    
        // bool hasMoved(glm::vec3 &delta)
        // {
        //     if (totalMove == glm::vec3(0.0f))
        //         return false;
        //     delta = totalMove;
        //     totalMove *= 0.0f;
        //     return true;
        // }

        // bool hasLooked(glm::vec3 &delta)
        // {
        //     if (totalLook == glm::vec3(0.0f))
        //         return false;
        //     delta = totalLook;
        //     totalLook *= 0.0f;
        //     return true;
        // }
    };

    class TestCharacterController: public ICharacterController
    {
    public:
        float moveSpeed = 1.0f;
        float lookSpeed = 1.0f;

        virtual void update() final;

        virtual void move(const glm::vec3 &moveDelta) final
        {
            totalMove += moveSpeed*moveDelta;
        }

        virtual void look(const glm::vec3 &lookDelta) final
        {
            totalLook += lookSpeed*lookDelta;
        }

    };

}

