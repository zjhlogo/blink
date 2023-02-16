for /r %%f in (*.frag *.vert) do (
    glslc -o compiled/%%~nxf.spv %%~nxf
)
