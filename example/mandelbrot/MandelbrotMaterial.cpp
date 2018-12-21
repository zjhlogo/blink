#include "MandelbrotMaterial.h"
#include <shaders/Shader.h>

static const char* MANDELBROT_VS =
#include "Mandelbrot.vs"
;

static const char* MANDELBROT_FS =
#include "Mandelbrot.fs"
;

MandelbrotMaterial::MandelbrotMaterial()
{
    m_shader = blink::Shader::fromBuffer("mandelbrot", MANDELBROT_VS, MANDELBROT_FS);
}

MandelbrotMaterial::~MandelbrotMaterial()
{
    SAFE_RELEASE(m_shader);
}

void MandelbrotMaterial::setupShaderUniforms(blink::Shader * shader)
{
    shader->setUniform("u_maxIterations", m_maxIterations);
    shader->setUniform("u_zoom", m_zoom);
    shader->setUniform("u_escapeRadius", m_escapeRadius);
    shader->setUniform("u_centerPos", m_centerPos);
    shader->setUniform("u_colorScale", m_colorScale);
}
