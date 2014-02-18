REM This is the config file used to copy files to a flash drive

md E:\frc1885.display\images
set WD=%CD%

copy /y %WD%\copyUltimateAscent.bat E:\copyUltimateAscent.bat

copy /y %WD%\startUltimateAscent.bat E:\frc1885.display\startUltimateAscent.bat
copy /y %WD%\lib\ultimateascent.jar E:\frc1885.display\ultimateascent.jar
copy /y %WD%\images E:\frc1885.display\images
copy /y %WD%\config.xml E:\frc1885.display\config.xml
copy /y %WD%\modules.xml E:\frc1885.display\modules.xml

pause