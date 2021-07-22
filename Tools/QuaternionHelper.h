#ifndef QUATERNION_HELPER_H
#define QUATERNION_HELPER_H

#include <openvr_driver.h>
#include <cmath>

namespace QuaternionHelper
{
    vr::HmdQuaternion_t HmdQuaternionInit(double w, double x, double y, double z)
    {
        vr::HmdQuaternion_t q;
        q.w = w;
        q.x = x;
        q.y = y;
        q.z = z;
        return q;
    }

    vr::HmdQuaternion_t MultipleQuaternion(const vr::HmdQuaternion_t& q1, const vr::HmdQuaternion_t& q2)
    {
        vr::HmdQuaternion_t result;
        result.w = q1.w * q2.w - q1.x * q2.x - q1.y * q2.y - q1.z * q2.z;
        result.x = q1.w * q2.x + q1.x * q2.w + q1.y * q2.z - q1.z * q2.y;
        result.y = q1.w * q2.y - q1.x * q2.z + q1.y * q2.w + q1.z * q2.x;
        result.z = q1.w * q2.z + q1.x * q2.y - q1.y * q2.x + q1.z * q2.w;
        return result;
    }

    vr::HmdQuaternion_t DivideQuaternion(const vr::HmdQuaternion_t& q1, const vr::HmdQuaternion_t& q2)
    {
        vr::HmdQuaternion_t result;
        double q2dotq2 = q2.w * q2.w + q2.x * q2.x + q2.y * q2.y + q2.z * q2.z;
        result.w = (q1.w * q2.w + q1.x * q2.x + q1.y * q2.y + q1.z * q2.z) / q2dotq2;
        result.x = (q1.w * q2.x - q1.x * q2.w - q1.y * q2.z + q1.z * q2.y) / q2dotq2;
        result.y = (q1.w * q2.y + q1.x * q2.z - q1.y * q2.w - q1.z * q2.x) / q2dotq2;
        result.z = (q1.w * q2.z - q1.x * q2.y + q1.y * q2.x - q1.z * q2.w) / q2dotq2;
        return result;
    }

    vr::HmdQuaternion_t QuaternionFromMatrix(const vr::HmdMatrix34_t& m)
    {
        vr::HmdQuaternion_t result;
        result.w = std::sqrt(std::max(0.0, 1.0 + m.m[0][0] + m.m[1][1] + m.m[2][2])) / 2.0;
        result.x = (m.m[2][1] - m.m[1][2]) / (4.0 * result.w);
        result.y = (m.m[0][2] - m.m[2][0]) / (4.0 * result.w);
        result.z = (m.m[1][0] - m.m[0][1]) / (4.0 * result.w);
        return result;
    }

    vr::HmdVector3_t UnitVectorFromQuaternion(const vr::HmdQuaternion_t& q)
    {
        vr::HmdVector3_t result;
        result.v[0] = 2.0 * (q.x * q.z - q.w * q.y);
        result.v[1] = 2.0 * (q.y * q.z + q.w * q.x);
        result.v[2] = 1.0 - 2.0 * (q.x * q.x + q.y * q.y);
        return result;
    }

    double DotProductQuaternion(const vr::HmdQuaternion_t& q1, const vr::HmdQuaternion_t& q2)
    {
        return q1.x * q2.x + q1.y * q2.y + q1.z * q2.z + q1.w * q2.w;   
    }

    vr::HmdQuaternion_t Slerp(vr::HmdQuaternion_t q1, vr::HmdQuaternion_t q2, double t)
    {
        vr::HmdQuaternion_t result;
        double t_ = 1.0 - t;
        double theta = std::acos(DotProductQuaternion(q1, q2));
        double sin_theta = std::sin(theta);
        double Wa = std::sin((t_) * theta) / sin_theta;
        double Wb = std::sin((t) * theta) / sin_theta;

        result.w = q1.w * Wa + q2.w * Wb;
        result.x = q1.x * Wa + q2.x * Wb;
        result.y = q1.y * Wa + q2.y * Wb;
        result.z = q1.z * Wa + q2.z * Wb;

        return result;
    }

    vr::HmdQuaternion_t NomaralizeQuaternion(vr::HmdQuaternion_t q)
    {
        vr::HmdQuaternion_t result;
        double norm = std::sqrt(q.x * q.x + q.y * q.y + q.z * q.z + q.w * q.w);
        result.x = q.x / norm;
        result.y = q.y / norm;
        result.z = q.z / norm;
        result.w = q.w / norm;
        return result;
    }
}

#endif