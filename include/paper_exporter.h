#ifndef PAPER_EXPORTER_H
#define PAPER_EXPORTER_H

#include "document.h"
#include <fstream>
#include <string>

// Objective :- Class for exporting question papers to text files with formatted output
// Input :- None (class declaration)
// Output :- None (class declaration)
// Sideeffect :- None
class PaperExporter {
public:
    // Objective :- Export question paper to a text file with formatted sections for MCQs, fill-in-blanks, short questions, and long questions. Creates new .txt file if it doesn't exist.
    // Input :- paper - QuestionPaper object containing all questions, filepath - string path where file should be written
    // Output :- Returns true if export successful, false if file cannot be opened
    // Sideeffect :- Creates new .txt file if it doesn't exist (adds .txt extension if missing), writes formatted content to file, writes error message to cerr on failure
    // Assumption :- filepath is a valid path string, directory exists and is writable, paper object contains valid question data
    bool exportToFile(const QuestionPaper& paper, const std::string& filepath) const;

private:
    // Objective :- Write a formatted section header to the output file stream
    // Input :- out - reference to output file stream, title - string containing section title
    // Output :- None (void function)
    // Sideeffect :- Writes formatted section header to file stream
    // Assumption :- out stream is open and writable, title is a valid string
    static void writeSection(std::ofstream& out, const std::string& title);
};

#endif 

