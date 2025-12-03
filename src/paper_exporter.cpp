#include "../include/paper_exporter.h"
#include <fstream>
#include <iostream>
#include <string>
#include <cctype>

// Objective :- Ensure filename has .txt extension, adding it if missing
// Input :- filepath - string containing original file path
// Output :- Returns string with .txt extension guaranteed
// Sideeffect :- None
// Assumption :- filepath is a valid string
static std::string ensureTxtExtension(const std::string& filepath) {
    // Find last dot in filename
    size_t lastDot = filepath.find_last_of('.');
    // Find last directory separator
    size_t lastSlash = filepath.find_last_of("/\\");
    
    // Check if dot exists and is after the last directory separator (i.e., it's part of filename, not path)
    if (lastDot != std::string::npos && (lastSlash == std::string::npos || lastDot > lastSlash)) {
        // Extract extension
        std::string ext = filepath.substr(lastDot + 1);
        // Convert extension to lowercase for comparison
        for (char &c : ext) {
            c = std::tolower(static_cast<unsigned char>(c));
        }
        // If extension is already .txt, return original path
        if (ext == "txt") {
            return filepath;
        }
    }
    // Add .txt extension if missing or different extension
    return filepath + ".txt";
}

// Objective :- Export question paper to a text file with formatted sections for MCQs, fill-in-blanks, short questions, and long questions. Creates new .txt file if it doesn't exist.
// Input :- paper - QuestionPaper object containing all questions, filepath - string path where file should be written
// Output :- Returns true if export successful, false if file cannot be opened
// Sideeffect :- Creates new .txt file if it doesn't exist, writes formatted content to file, writes error message to cerr on failure
// Assumption :- filepath is a valid path string, directory exists and is writable, paper object contains valid question data
bool PaperExporter::exportToFile(const QuestionPaper& paper, const std::string& filepath) const {
    // Ensure filename has .txt extension
    std::string finalPath = ensureTxtExtension(filepath);
    // Open output file stream (creates file if it doesn't exist)
    std::ofstream out(finalPath);
    // Check if file opened successfully
    if (!out) {
        // Write error message to standard error
        std::cerr << "[PaperExporter] Cannot write to: " << finalPath << "\n";
        // Return failure status
        return false;
    }

    // Write Multiple Choice Questions section header
    writeSection(out, "Multiple Choice Questions");
    // Iterate through each MCQ
    for (size_t i = 0; i < paper.mcqs.size(); i++) {
        // Write question number and prompt
        out << "Q" << (i + 1) << ". " << paper.mcqs[i].prompt << "\n";
        // Initialize option label to 'A'
        char label = 'A';
        // Write each option with label
        for (auto& opt : paper.mcqs[i].options)
            out << "   " << label++ << ") " << opt << "\n";
        // Write correct answer
        out << "   Answer: " << paper.mcqs[i].answer << "\n\n";
    }

    // Write Fill in the Blanks section header
    writeSection(out, "Fill in the Blanks");
    // Iterate through each fill-in-blank question
    for (size_t i = 0; i < paper.fillInBlanks.size(); i++)
        // Write question number and text
        out << "Q" << (i + 1) << ". " << paper.fillInBlanks[i] << "\n";
    // Add blank line after section
    out << "\n";

    // Write Short Answer Questions section header
    writeSection(out, "Short Answer Questions");
    // Iterate through each short question
    for (size_t i = 0; i < paper.shortQuestions.size(); i++)
        // Write question number and text
        out << "Q" << (i + 1) << ". " << paper.shortQuestions[i] << "\n";
    // Add blank line after section
    out << "\n";

    // Write Long Answer Questions section header
    writeSection(out, "Long Answer Questions");
    // Iterate through each long question
    for (size_t i = 0; i < paper.longQuestions.size(); i++)
        // Write question number and text
        out << "Q" << (i + 1) << ". " << paper.longQuestions[i] << "\n\n";

    // Return success status
    return true;
}

// Objective :- Write a formatted section header to the output file stream
// Input :- out - reference to output file stream, title - string containing section title
// Output :- None (void function)
// Sideeffect :- Writes formatted section header to file stream
// Assumption :- out stream is open and writable, title is a valid string
void PaperExporter::writeSection(std::ofstream& out, const std::string& title) {
    // Write section header with title surrounded by equals signs
    out << "==== " << title << " ====\n\n";
}

