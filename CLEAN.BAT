@echo off
cd SOURCE
call CLEAN.BAT
cd ..
if exist *.bak del *.bak
if exist *.log del *.log
if exist *.map del *.map
