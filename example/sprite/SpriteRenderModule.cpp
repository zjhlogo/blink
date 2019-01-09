#include "SpriteRenderModule.h"
#include "Sprite.h"
#include "SpriteRenderBlock.h"
#include <cameras/Camera.h>
#include <shaders/Shader.h>
#include <geometries/BufferAttributes.h>
#include <render/GlConfig.h>
#include <glad/glad.h>

SpriteRenderModule::SpriteRenderModule()
{
}

SpriteRenderModule::~SpriteRenderModule()
{
}

bool SpriteRenderModule::initialize(blink::Entity * pEntity)
{
    // depth test setup
    glDisable(GL_DEPTH_TEST);

    // cull face setup
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CW);

    // blend mode setup
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    m_spriteBatchGeometry.initialize();
    m_spriteMaterial = new SpriteMaterial();

    return true;
}

void SpriteRenderModule::terminate()
{
    SAFE_DELETE(m_spriteMaterial);
}

void SpriteRenderModule::render(blink::Object * rootObj, blink::Camera * camera)
{
    // collect all sprites
    std::vector<Sprite*> sprites;
    rootObj->collectChild<Sprite>(sprites);

    // generate render block
    SpriteRenderBlock renderBlock;
    int depth = 0;
    for (auto sprite : sprites)
    {
        depth = sprite->generateRenderBlock(renderBlock, depth);
    }

    // TODO: sort render block

    // flush render block
    for (const auto& group : renderBlock.m_spriteQuadGroupList)
    {
        blink::Shader* shader = m_spriteMaterial->getShader();
        if (!shader) continue;

        m_spriteMaterial->setTexture("tex_diffuse", group->texFile, 0);

        std::vector<blink::VertexPos3Uv2> verts;

        for (const auto& quad : group->quads)
        {
            verts.push_back({ quad.verts[0].x, quad.verts[0].y, quad.verts[0].z, quad.uvs[0].s, quad.uvs[0].t });
            verts.push_back({ quad.verts[1].x, quad.verts[1].y, quad.verts[1].z, quad.uvs[1].s, quad.uvs[1].t });
            verts.push_back({ quad.verts[2].x, quad.verts[2].y, quad.verts[2].z, quad.uvs[2].s, quad.uvs[2].t });
            verts.push_back({ quad.verts[3].x, quad.verts[3].y, quad.verts[3].z, quad.uvs[3].s, quad.uvs[3].t });
        }

        m_spriteBatchGeometry.updateVertex(verts.data(), sizeof(verts[0])*verts.size());

        glUseProgram(shader->getProgramId());
        GL_ERROR_CHECK();

        camera->setupShaderUniforms(blink::MAT4_IDENTITY, shader);
        m_spriteMaterial->setupShaderUniforms(shader);
        m_spriteMaterial->setupShaderSampler(shader);

        // TODO: apply render state

        glBindVertexArray(m_spriteBatchGeometry.getVertexArrayObjectId());
        GL_ERROR_CHECK();

        // Bind the IBO
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_spriteBatchGeometry.getIndexBufferId());
        GL_ERROR_CHECK();

        // Draws a indexed triangle array
        glDrawElements(GL_TRIANGLES, group->quads.size() * 6, GL_UNSIGNED_SHORT, 0);
        GL_ERROR_CHECK();
    }
}
