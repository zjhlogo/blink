@echo off

for /r %%f in (source/shaders/*.frag source/shaders/*.vert) do (
    glslc -o bin/assets/shaders/%%~nxf.spv source/shaders/%%~nxf
    if ERRORLEVEL 1 goto PROC_FAILED
)

echo compile complete
exit

:PROC_FAILED
echo compile failed
exit
