/**

    @file      main.cpp
    @brief
    @details   ~
    @author    zjhlogo
    @date      2.11.2021
    @copyright © zjhlogo, 2021. All right reserved.

**/

#include "module/Crop.h"
#include "module/Step.h"
#include "noiseutils.h"

#include <noise/noise.h>

int main(int argc, char** argv)
{
    noise::module::Const srcWhite;
    srcWhite.setConstValue(1.0);

    noise::module::Const srcBlack;
    srcBlack.setConstValue(-1.0);

    Crop crop;
    crop.setSourceModule(0, srcBlack);
    crop.setSourceModule(1, srcWhite);
    crop.setRangeZ(2.5, 3.0);

    noise::module::Perlin perlin;
    perlin.setFrequency(3.0);
    Step step;
    step.setSourceModule(0, perlin);
    step.addStep(-0.2, 1.0);

    Crop crop2;
    crop2.setSourceModule(0, srcBlack);
    crop2.setSourceModule(1, step);
    crop2.setRangeZ(0.0, 2.75);

    noise::module::Max max;
    max.setSourceModule(0, crop);
    max.setSourceModule(1, crop2);

    noise::module::Turbulence turbulence(max);
    turbulence.setFrequency(4);
    turbulence.setPower(0.05);

    noise::module::Perlin perlin2;
    perlin2.setFrequency(2.0);

    noise::module::Select select(srcBlack, perlin, turbulence);
    // select.setBounds(0.9, 1.0);

    Step step2;
    step2.setSourceModule(0, select);
    step2.addStep(-0.5, 0.0);
    step2.addStep(0.5, 1.0);

    noise::utils::NoiseMap heightMap;
    noise::utils::NoiseMapBuilderPlane heightMapBuilder;
    heightMapBuilder.setSourceModule(step2);
    heightMapBuilder.setDestNoiseMap(heightMap);
    heightMapBuilder.setDestSize(512, 128);
    heightMapBuilder.setBounds(0.0, 16.0, 0.0, 4.0);
    heightMapBuilder.build();

    noise::utils::RendererImage renderer;
    noise::utils::Image image;
    renderer.setSourceNoiseMap(heightMap);
    renderer.setDestImage(image);
    renderer.buildGrayscaleGradient();
    renderer.render();

    noise::utils::WriterBMP writer;
    writer.setSourceImage(image);
    writer.setDestFilename("tutorial.bmp");
    writer.writeDestFile();

    return 0;
}
