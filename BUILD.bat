@echo off
echo Building Question Paper Generator...
g++ -std=c++14 -Wall -Wextra -Iinclude -o question_generator.exe src\main.cpp src\file_manager.cpp src\preprocessor.cpp src\keyword_extractor.cpp src\line_scorer.cpp src\question_generator.cpp src\paper_exporter.cpp src\ui.cpp

if %ERRORLEVEL% EQU 0 (
    echo Build successful!
) else (
    echo Build failed!
    pause
    exit /b 1
)
