#include <noise/noise.h>
#include "noiseutils.h"

#include "module/Crop.h"
#include "module/Step.h"

int main(int argc, char** argv)
{
    noise::module::Const srcWhite;
    srcWhite.SetConstValue(1.0);

    noise::module::Const srcBlack;
    srcBlack.SetConstValue(-1.0);

    Crop crop;
    crop.SetSourceModule(0, srcBlack);
    crop.SetSourceModule(1, srcWhite);
    crop.SetRangeZ(2.5, 3.0);

    noise::module::Perlin perlin;
    perlin.SetFrequency(3.0);
    Step step;
    step.SetSourceModule(0, perlin);
    step.AddStep(-0.2, 1.0);

    Crop crop2;
    crop2.SetSourceModule(0, srcBlack);
    crop2.SetSourceModule(1, step);
    crop2.SetRangeZ(0.0, 2.75);

    noise::module::Max max;
    max.SetSourceModule(0, crop);
    max.SetSourceModule(1, crop2);

    noise::module::Turbulence turbulence;
    turbulence.SetSourceModule(0, max);
    turbulence.SetFrequency(4);
    turbulence.SetPower(0.05);

    noise::module::Perlin perlin2;
    perlin2.SetFrequency(2.0);

    noise::module::Select select;
    select.SetSourceModule(0, srcBlack);
    select.SetSourceModule(1, perlin2);
    select.SetControlModule(turbulence);
    select.SetBounds(0.9, 1.0);

    Step step2;
    step2.SetSourceModule(0, select);
    step2.AddStep(-0.5, 0.0);
    step2.AddStep(0.5, 1.0);

    noise::utils::NoiseMap heightMap;
    noise::utils::NoiseMapBuilderPlane heightMapBuilder;
    heightMapBuilder.SetSourceModule(step2);
    heightMapBuilder.SetDestNoiseMap(heightMap);
    heightMapBuilder.SetDestSize(512, 128);
    heightMapBuilder.SetBounds(0.0, 16.0, 0.0, 4.0);
    heightMapBuilder.Build();

    noise::utils::RendererImage renderer;
    noise::utils::Image image;
    renderer.SetSourceNoiseMap(heightMap);
    renderer.SetDestImage(image);
    renderer.BuildGrayscaleGradient();
    renderer.Render();

    noise::utils::WriterBMP writer;
    writer.SetSourceImage(image);
    writer.SetDestFilename("tutorial.bmp");
    writer.WriteDestFile();

    return 0;
}
