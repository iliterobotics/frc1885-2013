REM This is the config file used to copy files to the current user's desktop

md %USERPROFILE%\Desktop\DisplayApp\images
md %USERPROFILE%\Desktop\DisplayApp\matchdata
md %USERPROFILE%\Desktop\DisplayApp\lib

copy /y c:\Users\Robot\workspace\frc1885-2013.display\startUltimateAscent.bat %USERPROFILE%\Desktop\startUltimateAscent.bat
copy /y c:\Users\Robot\workspace\frc1885-2013.display\ultimateascent.jar %USERPROFILE%\Desktop\DisplayApp\ultimateascent.jar
copy /y c:\Users\Robot\workspace\frc1885-2013.display\lib %USERPROFILE%\Desktop\DisplayApp\lib
copy /y c:\Users\Robot\workspace\frc1885-2013.display\images %USERPROFILE%\Desktop\DisplayApp\images
copy /y c:\Users\Robot\workspace\frc1885-2013.display\config.xml %USERPROFILE%\Desktop\DisplayApp\config.xml
copy /y c:\Users\Robot\workspace\frc1885-2013.display\modules.xml %USERPROFILE%\Desktop\DisplayApp\modules.xml

pause