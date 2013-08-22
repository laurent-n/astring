@echo off
echo --- generation de la documentation HTML
mkdir html
REM copy images\*.* html
doxygen doxconfig.dox
echo --- generation du fichier CHM
hhc.exe html\index.hhp
copy html\AString.chm AString.chm /Y
pause

