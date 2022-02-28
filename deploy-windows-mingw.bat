@echo off

cd ..
mkdir tmp
mkdir distrib

cd tmp
qmake ..\partyzone_launcher
mingw32-make

copy release\partyzone_launcher.exe ..\distrib\

cd ..
rd /S /Q tmp\

cd distrib

windeployqt --webkit2 --qmldir C:\Qt\5.15.2\msvc2015_64\qml --force --release partyzone_launcher.exe
