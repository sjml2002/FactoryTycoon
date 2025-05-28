@echo off
call VsMSBuildCmd.bat
call VsDevCmd.bat
nmake clean
nmake -f makefile
pause