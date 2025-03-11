@echo off
echo Building backup system with Visual Studio...

:: Компилируем все cpp файлы в один исполняемый файл
cl.exe /std:c++17 /EHsc /Fe:"backup_system.exe" /I"include" src\*.cpp

echo Build complete!
if exist "backup_system.exe" (
    echo You can now run backup_system.exe
) else (
    echo Build failed!
) 