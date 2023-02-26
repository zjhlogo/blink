@echo off

for /r %%f in (*.frag *.vert) do (
    glslc -o compiled/%%~nxf.spv %%~nxf
    if ERRORLEVEL 1 goto PROC_FAILED
)

echo compile complete
exit

:PROC_FAILED
echo compile failed
exit
