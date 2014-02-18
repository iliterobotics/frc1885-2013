REM This is the config file used to copy files to a flash drive

md F:\frc1885-2013.display\images

copy /y %USERPROFILE%\workspace\frc1885-2013.display\copyUltimateAscent.bat F:\copyUltimateAscent.bat

copy /y %USERPROFILE%\workspace\frc1885-2013.display\startUltimateAscent.bat F:\frc1885-2013.display\startUltimateAscent.bat
copy /y %USERPROFILE%\workspace\frc1885-2013.display\ultimateascent.jar F:\frc1885-2013.display\ultimateascent.jar
copy /y %USERPROFILE%\workspace\frc1885-2013.display\images F:\frc1885-2013.display\images
REM copy /y %USERPROFILE%\workspace\frc1885-2013.display\config.xml F:\frc1885-2013.display\config.xml
copy /y %USERPROFILE%\workspace\frc1885-2013.display\modules.xml F:\frc1885-2013.display\modules.xml

pause