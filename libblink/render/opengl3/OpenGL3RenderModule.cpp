#include "OpenGL3RenderModule.h"
#include "../GlConfig.h"
#include "../../materials/Material.h"
#include "../../shaders/Shader.h"
#include "../../geometries/BufferAttributes.h"
#include "../../geometries/BufferGeometry.h"
#include "../../objects/Mesh.h"
#include "../../cameras/Camera.h"
#include "../../lights/Light.h"
#include <glad/glad.h>
#include <StringBuilder.h>

namespace blink
{
    OpenGL3RenderModule::OpenGL3RenderModule()
    {

    }

    OpenGL3RenderModule::~OpenGL3RenderModule()
    {

    }

    bool OpenGL3RenderModule::initialize(Entity* pEntity)
    {
        //LOGI("Version: {0}", glGetString(GL_VERSION));
        //LOGI("Vendor: {0}", glGetString(GL_VENDOR));
        //LOGI("Renderer: {0}", glGetString(GL_RENDERER));

        //const GLubyte* pszExtensions = glGetString(GL_EXTENSIONS);
        //if (pszExtensions)
        //{
        //    StringList extensionsArray;
        //    StringBuilder::split(extensionsArray, reinterpret_cast<const char*>(pszExtensions), " ");
        //    for (const auto& ext : extensionsArray)
        //    {
        //        LOGI("Extensions: {0}", ext);
        //    }
        //}

        //LOG_GL_INT_CAPACITY(GL_DEPTH_BITS);
        //LOG_GL_INT_CAPACITY(GL_STENCIL_BITS);

        //LOG_GL_INT_CAPACITY(GL_MAX_VERTEX_ATTRIBS);               // The value must be at least 8
        //LOG_GL_INT_CAPACITY(GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS);   // The value may be 0
        //LOG_GL_INT_CAPACITY(GL_MAX_TEXTURE_IMAGE_UNITS);			// The value must be at least 8

        //LOG_GL_INT_CAPACITY(GL_MAX_TEXTURE_SIZE);					// The value must be at least 64
        //LOG_GL_INT_CAPACITY(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS);	// The value must be at least 8
        //LOG_GL_INT_CAPACITY(GL_MAX_CUBE_MAP_TEXTURE_SIZE);        // The value must be at least 16
        //LOG_GL_INT_CAPACITY(GL_MAX_RENDERBUFFER_SIZE);            // The value must be at least 1

        //GL_ERROR_CHECK();

        // depth test setup
        glEnable(GL_DEPTH_TEST);

        // cull face setup
        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);
        glFrontFace(GL_CW);

        // blend mode setup
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        return true;
    }

    void OpenGL3RenderModule::terminate()
    {

    }

    void OpenGL3RenderModule::render(Object * rootObj, Camera * camera)
    {
        if (!rootObj || !camera) return;

        glClearColor(0.1f, 0.3f, 0.7f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // collect lights
        std::vector<Light*> lights;
        rootObj->collectChild<Light>(lights, false);

        // collect renderable objects
        std::vector<Mesh*> meshes;
        rootObj->collectChild<Mesh>(meshes);

        for (auto mesh : meshes)
        {
            renderObject(mesh, camera, lights);
        }
    }

    void OpenGL3RenderModule::renderObject(Mesh* mesh, Camera* camera, const std::vector<Light*>& lights)
    {
        Material* material = mesh->getMaterial();
        if (!material) return;

        BufferGeometry* geometry = dynamic_cast<BufferGeometry*>(mesh->getGeometry());
        if (!geometry) return;

        Shader* shader = material->getShader();
        if (!shader) return;

        glUseProgram(shader->getProgramId());
        GL_ERROR_CHECK();

        camera->setupShaderUniforms(mesh->getLocalToWorldTransform(), shader);
        // TODO: support multiply lights
        for (auto light : lights)
        {
            light->setupShaderUniforms(shader);
        }
        material->setupShaderUniforms(shader);
        material->setupShaderSampler(shader);

        // TODO: apply render state

        glBindVertexArray(geometry->getVertexArrayObjectId());
        GL_ERROR_CHECK();

        // Bind the IBO
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, geometry->getIndexBufferId());
        GL_ERROR_CHECK();

        // Draws a indexed triangle array
        glDrawElements(GL_TRIANGLES, geometry->getNumIndex(), GL_UNSIGNED_SHORT, 0);
        GL_ERROR_CHECK();
    }
}
