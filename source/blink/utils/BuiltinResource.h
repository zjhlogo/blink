//
// Created by zjhlogo on 2023/3/4.
//
#pragma once

namespace blink
{
    class IGeometry;
    class IMaterial;

    class BuiltinResource
    {
    public:
        static bool initialize();
        static void terminate();

        static IGeometry* getSphereGeometry() { return m_sphereGeometry; }
        static IMaterial* getUnlitMaterial() { return m_unlitMaterial; }
    private:
        static IGeometry* m_sphereGeometry;
        static IMaterial* m_unlitMaterial;

    };

} // namespace blink
