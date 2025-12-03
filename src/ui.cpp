#include "../include/ui.h"
#include <iostream>
#include <string>

namespace ui {

// Objective :- Display welcome banner to the user when application starts
// Input :- None
// Output :- None (void function)
// Sideeffect :- Writes banner text to standard output
// Assumption :- Standard output is available and writable
void printBanner() {
    // Print top border of banner
    std::cout << "=============================================\n";
    // Print application title
    std::cout << "           Question Paper Generator\n";
    // Print bottom border of banner
    std::cout << "=============================================\n";
}

// Objective :- Display menu options to the user for application navigation
// Input :- None
// Output :- None (void function)
// Sideeffect :- Writes menu text to standard output
// Assumption :- Standard output is available and writable
void showMenu() {
    // Display menu options with newline before menu
    std::cout << "\n1) Generate Question Paper\n2) Exit\nEnter choice: ";
}

// Objective :- Display a prompt message and read user input from standard input
// Input :- text - string containing prompt message to display
// Output :- Returns string containing user input (may be empty)
// Sideeffect :- Writes prompt to standard output, reads from standard input
// Assumption :- Standard input/output are available, text is a valid string
std::string prompt(const std::string& text) {
    // Display prompt message
    std::cout << text;
    // Variable to store user input
    std::string input;
    // Read entire line from standard input
    std::getline(std::cin, input);
    // Return user input
    return input;
}

// Objective :- Display a summary of the generated question paper showing counts for each question type
// Input :- p - QuestionPaper object containing generated questions
// Output :- None (void function)
// Sideeffect :- Writes summary information to standard output
// Assumption :- p is a valid QuestionPaper object with accessible size() methods
void summarize(const QuestionPaper& p) {
    // Print summary header
    std::cout << "\nSummary:\n";
    // Display count of multiple choice questions
    std::cout << "  MCQs: " << p.mcqs.size() << "\n";
    // Display count of fill-in-blank questions
    std::cout << "  Fill Blanks: " << p.fillInBlanks.size() << "\n";
    // Display count of short answer questions
    std::cout << "  Short Qs: " << p.shortQuestions.size() << "\n";
    // Display count of long answer questions
    std::cout << "  Long Qs: " << p.longQuestions.size() << "\n";
}

} // namespace ui

