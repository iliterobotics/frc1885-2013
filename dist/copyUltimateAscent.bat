REM Git push needs to be added; cd to repolocation
REM then git add config.xml then git commit -m "Changed config.xml" then git push

REM Config files also need to be broken into prod and dev sections

md %USERPROFILE%\Desktop\DisplayApp\images
md %USERPROFILE%\Desktop\DisplayApp\matchdata
md %USERPROFILE%\Desktop\DisplayApp\lib

copy /y F:\frc1885-2013.display\startUltimateAscent.bat %USERPROFILE%\Desktop\startUltimateAscent.bat
copy /y F:\frc1885-2013.display\ultimateascent.jar %USERPROFILE%\Desktop\DisplayApp\ultimateascent.jar
copy /y F:\frc1885-2013.display\images %USERPROFILE%\Desktop\DisplayApp\images
copy /y F:\frc1885-2013.display\lib %USERPROFILE%\Desktop\DisplayApp\lib
copy /y F:\frc1885-2013.display\config.xml %USERPROFILE%\Desktop\DisplayApp\config.xml
REM copy /y %USERPROFILE%\Desktop\DisplayApp\config.xml F:\frc1885-2013.display\config.xml
copy /y F:\frc1885-2013.display\modules.xml %USERPROFILE%\Desktop\DisplayApp\modules.xml
copy /y %USERPROFILE%\Desktop\DisplayApp\matchdata\ F:\

pause