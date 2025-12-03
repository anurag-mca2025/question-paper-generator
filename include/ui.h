#ifndef UI_H
#define UI_H

#include "document.h"
#include <string>

// Objective :- Namespace containing user interface functions for console interaction
// Input :- None (namespace declaration)
// Output :- None (namespace declaration)
// Sideeffect :- None
namespace ui {
    // Objective :- Display welcome banner to the user when application starts
    // Input :- None
    // Output :- None (void function)
    // Sideeffect :- Writes banner text to standard output
    // Assumption :- Standard output is available and writable
    void printBanner();
    
    // Objective :- Display menu options to the user for application navigation
    // Input :- None
    // Output :- None (void function)
    // Sideeffect :- Writes menu text to standard output
    // Assumption :- Standard output is available and writable
    void showMenu();
    
    // Objective :- Display a prompt message and read user input from standard input
    // Input :- text - string containing prompt message to display
    // Output :- Returns string containing user input (may be empty)
    // Sideeffect :- Writes prompt to standard output, reads from standard input
    // Assumption :- Standard input/output are available, text is a valid string
    std::string prompt(const std::string& text);
    
    // Objective :- Display a summary of the generated question paper showing counts for each question type
    // Input :- p - QuestionPaper object containing generated questions
    // Output :- None (void function)
    // Sideeffect :- Writes summary information to standard output
    // Assumption :- p is a valid QuestionPaper object with accessible size() methods
    void summarize(const QuestionPaper& p);
}

#endif 

