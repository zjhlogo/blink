#include "MapRenderSystem.h"
#include "TileAtlas.h"
#include "MapUtilities.h"
#include <glad/glad.h>
#include <render/GlConfig.h>
#include <render/shader/Shader.h>
#include <render/texture/Texture2D.h>
#include <algorithm>

MapRenderSystem::MapRenderSystem()
{
}

MapRenderSystem::~MapRenderSystem()
{
    SAFE_DELETE(m_atlas);
    SAFE_DELETE(m_material);
}

void MapRenderSystem::configure(entityx::EventManager & events)
{
    events.subscribe<MapDataUpdateEvent>(*this);
    events.subscribe<entityx::ComponentAddedEvent<blink::CameraData>>(*this);

    m_atlas = new TileAtlas();
    m_atlas->initialize();

    m_material = new DiffuseMaterial();
    m_material->setTexture("tex_diffuse", m_atlas->getTexture(), 0);
}

void MapRenderSystem::update(entityx::EntityManager & entities, entityx::EventManager & events, entityx::TimeDelta dt)
{
    // select camera
    if (!m_camera.valid()) return;
    auto cameraData = m_camera.component<blink::CameraData>().get();

    // calculate block index
    glm::ivec2 blockIndex;
    blockIndex.x = static_cast<int>((cameraData->cameraPos.x + m_mapData->origin.x * MapUtilities::TILE_SIZE) / (MapRenderBlock::BLOCK_SIZE * MapUtilities::TILE_SIZE));
    blockIndex.y = static_cast<int>((cameraData->cameraPos.y + m_mapData->origin.y * MapUtilities::TILE_SIZE) / (MapRenderBlock::BLOCK_SIZE * MapUtilities::TILE_SIZE));

    int maxX = (m_mapData->size.x + MapRenderBlock::BLOCK_SIZE - 1) / MapRenderBlock::BLOCK_SIZE;
    int maxY = (m_mapData->size.y + MapRenderBlock::BLOCK_SIZE - 1) / MapRenderBlock::BLOCK_SIZE;
    if (blockIndex.x < 0) blockIndex.x = 0;
    if (blockIndex.y < 0) blockIndex.y = 0;
    if (blockIndex.x >= maxX) blockIndex.x = maxX - 1;
    if (blockIndex.y >= maxY) blockIndex.y = maxY - 1;

    // regenerate render block
    if (m_lastBlockIndex != blockIndex)
    {
        checkInBlocksOutOfRange(blockIndex);
        generateBlocksInRange(blockIndex);

        m_lastBlockIndex = blockIndex;
    }

    auto shader = m_material->getShader();
    if (!shader) return;

    // render the geometry
    glUseProgram(shader->getProgramId());
    GL_ERROR_CHECK();

    // setup shader uniforms for material
    m_material->setupShaderUniforms(shader);
    m_material->setupShaderSampler(shader);

    // setup shader uniforms for camera
    {
        shader->setUniform("u_worldToClip", cameraData->worldToClip);
        shader->setUniform("u_localToWorld", blink::MAT4_IDENTITY);
        shader->setUniform("u_localToWorldTranInv", blink::MAT3_IDENTITY);
        shader->setUniform("u_localToClip", cameraData->worldToClip);
        shader->setUniform("u_viewPos", cameraData->cameraPos);
    }

    for (auto& renderBlock : m_renderBlockInUsed)
    {
        glBindVertexArray(renderBlock->getVertexArrayObjectId());
        GL_ERROR_CHECK();

        // Bind the IBO
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, renderBlock->getIndexBufferId());
        GL_ERROR_CHECK();

        // Draws a indexed triangle array
        glDrawElements(GL_TRIANGLES, renderBlock->getNumIndex(), GL_UNSIGNED_SHORT, 0);
        GL_ERROR_CHECK();
    }
}

void MapRenderSystem::receive(const MapDataUpdateEvent & evt)
{
    m_mapData = evt.mapData;
    // TODO: update render block
}

void MapRenderSystem::receive(const entityx::ComponentAddedEvent<blink::CameraData>& evt)
{
    m_camera = evt.entity;
}

MapRenderBlock * MapRenderSystem::checkOutBlock()
{
    if (!m_renderBlockAvailable.empty())
    {
        auto renderBlock = *m_renderBlockAvailable.rbegin();
        m_renderBlockAvailable.pop_back();

        m_renderBlockInUsed.push_back(renderBlock);
        return renderBlock;
    }

    if (m_numBlocks < MAX_RENDER_BLOCKS)
    {
        auto renderBlock = new MapRenderBlock();
        m_renderBlockInUsed.push_back(renderBlock);
        ++m_numBlocks;
        return renderBlock;
    }

    return nullptr;
}

void MapRenderSystem::checkInBlock(MapRenderBlock * renderBlock)
{
    auto it = std::find(m_renderBlockInUsed.begin(), m_renderBlockInUsed.end(), renderBlock);
    if (it != m_renderBlockInUsed.end())
    {
        m_renderBlockAvailable.push_back(*it);
        m_renderBlockInUsed.erase(it);
    }
}

void MapRenderSystem::checkInBlocksOutOfRange(const glm::ivec2& blockIndex)
{
    glm::ivec2 lb = blockIndex - m_blockRadius;
    glm::ivec2 rt = blockIndex + m_blockRadius;

    int maxX = (m_mapData->size.x + MapRenderBlock::BLOCK_SIZE - 1) / MapRenderBlock::BLOCK_SIZE;
    int maxY = (m_mapData->size.y + MapRenderBlock::BLOCK_SIZE - 1) / MapRenderBlock::BLOCK_SIZE;

    if (lb.x < 0) lb.x = 0;
    if (lb.y < 0) lb.y = 0;

    if (rt.x > maxX) rt.x = maxX;
    if (rt.y > maxY) rt.y = maxY;

    MapRenderBlockList blocksExpired;
    for (auto& block : m_renderBlockInUsed)
    {
        auto blockIndex = block->getBlockIndex();

        if (blockIndex.x < lb.x || blockIndex.x >= rt.x || blockIndex.y < lb.y || blockIndex.y >= rt.y)
        {
            blocksExpired.push_back(block);
        }
    }

    for (auto& block : blocksExpired)
    {
        checkInBlock(block);
    }
}

void MapRenderSystem::generateBlocksInRange(const glm::ivec2& blockIndex)
{
    glm::ivec2 lb = blockIndex - m_blockRadius;
    glm::ivec2 rt = blockIndex + m_blockRadius;

    int maxX = (m_mapData->size.x + MapRenderBlock::BLOCK_SIZE - 1) / MapRenderBlock::BLOCK_SIZE;
    int maxY = (m_mapData->size.y + MapRenderBlock::BLOCK_SIZE - 1) / MapRenderBlock::BLOCK_SIZE;

    if (lb.x < 0) lb.x = 0;
    if (lb.y < 0) lb.y = 0;

    if (rt.x > maxX) rt.x = maxX;
    if (rt.y > maxY) rt.y = maxY;

    for (int y = lb.y; y < rt.y; ++y)
    {
        for (int x = lb.x; x < rt.x; ++x)
        {
            glm::ivec2 currBlockIndex{ x, y };
            bool needGen = true;

            for (auto& block : m_renderBlockInUsed)
            {
                if (block->getBlockIndex() == currBlockIndex)
                {
                    needGen = false;
                    break;
                }
            }

            if (needGen)
            {
                auto block = checkOutBlock();
                block->generateGeometry(m_mapData, m_atlas, currBlockIndex);
            }
        }
    }
}
