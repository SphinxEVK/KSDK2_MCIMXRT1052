@echo off

::-----::
set iar_path=%~dp0\iar\
::-----::

echo Cleanning up repository...
for /f "delims=" %%i in ('dir iar_path /b /a-d /s "*.browse" and "*.linf" and "*.o" and "*.pbi" and "*.pbd" and "*.xcl" and "*.out" and "*.hex" and "*.bin"') do (del %%i)
echo Done!

pause