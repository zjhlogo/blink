/**

    @file      MeshBuilder.h
    @brief
    @details   ~
    @author    zjhlogo
    @date      5.11.2021
    @copyright Copyright zjhlogo, 2021. All right reserved.

**/
#pragma once

#include "../utils/GltfUtil.h"
#include "IGeometryBuilder.h"

namespace blink
{
    class Geometry;

    class MeshBuilder : public IGeometryBuilder
    {
    public:
        MeshBuilder& loadModel(const tstring& filePath);
        void reset();

        const tinygltf::Model& getModel() const { return m_model; }

        virtual tstring getUniqueId() const;
        IGeometry* build(bool buildNormal = true, bool buildUv = true, glm::mat3* inertiaTensorOut = nullptr) const;

    private:
        tstring m_filePath;
        tinygltf::Model m_model;
    };
} // namespace blink
