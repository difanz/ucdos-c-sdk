@echo off
if "%1"=="" goto usage
bcc -ml -I..\include -L..\lib -L.. %1 SDK.LIB graphics.lib
goto end
:usage
echo Usage: BUILD.BAT filename.c
:end
