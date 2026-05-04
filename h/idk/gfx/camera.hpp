#pragma once

#include "idk/core/transform.hpp"


namespace idk::gfx
{
    class Camera
    {
    private:
        idk::Transform mT;
        glm::mat4 mV, mP, mPV;
        float mVfov, mAspect, mZnear, mZfar;
        bool  mDirty;

        glm::mat4 _compute_view()
        {
            return glm::lookAt(mT.getPos(), mT.getPos() + mT.getFront(), coordinate_system::UP);
        }
    
        glm::mat4 _compute_proj()
        {
            return glm::perspective(glm::radians(mVfov), mAspect, mZnear, mZfar);
        }
    

    public:
        Camera(float vfov, float aspect, float znear, float zfar)
        :   mT(glm::vec3(0.0f)),
            mV(glm::mat4(1.0f)), mP(glm::mat4(1.0f)), mPV(glm::mat4(1.0f)),
            mVfov(vfov), mAspect(aspect), mZnear(znear), mZfar(zfar),
            mDirty(false)
        {
            mV = _compute_view();
            mP = _compute_proj();
            mPV = mP * mV;
        }

        bool isDirty() { return mDirty; }
        void unDirty() { mDirty = false; }

        void setVerticalFov(float vfov)   { mDirty=true; mVfov=vfov; }
        void setAspectRatio(float aspect) { mDirty=true; mAspect=aspect; }
        void setNearPlane  (float znear)  { mDirty=true; mZnear=znear; }
        void setFarPlane   (float zfar)   { mDirty=true; mZfar=zfar; }

        idk::Transform &getTransform()
        {
            return mT;
        }

        const glm::mat4 &getView()
        {
            if (mDirty) { mV = _compute_view(); }
            return mV;
        }

        const glm::mat4 &getProj()
        {
            if (mDirty) { mP = _compute_proj(); }
            return mP;
        }

        const glm::mat4 &getProjView()
        {
            if (mDirty) { mPV = getProj() * getView(); }
            return mPV;
        }

    };
}