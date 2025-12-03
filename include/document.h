#ifndef DOCUMENT_H
#define DOCUMENT_H

#include <string>
#include <vector>

// Objective :- Represents a document loaded from a text file with its name and content lines
// Input :- None (data structure)
// Output :- None (data structure)
// Sideeffect :- None
struct Document {
    std::string name;                    // Name of the document file
    std::vector<std::string> lines;      // Vector containing all lines of text from the document
};

// Objective :- Represents a processed line of text with original text, cleaned text, and tokenized words
// Input :- None (data structure)
// Output :- None (data structure)
// Sideeffect :- None
struct ProcessedLine {
    std::string original;                // Original unprocessed line text
    std::string cleaned;                 // Normalized and cleaned line text
    std::vector<std::string> tokens;    // Vector of tokens (words) extracted from cleaned text
};

// Objective :- Represents a processed line with an associated importance score
// Input :- None (data structure)
// Output :- None (data structure)
// Sideeffect :- None
struct ScoredLine {
    ProcessedLine line;                  // Processed line data
    double score{};                      // Importance score (higher = more important)
};

// Objective :- Represents a multiple choice question with prompt, options, and correct answer
// Input :- None (data structure)
// Output :- None (data structure)
// Sideeffect :- None
struct MCQ {
    std::string prompt;                  // Question prompt text
    std::vector<std::string> options;   // Vector of answer options (typically 4)
    char answer{'A'};                    // Correct answer label ('A', 'B', 'C', or 'D')
};

// Objective :- Represents a complete question paper containing all types of questions
// Input :- None (data structure)
// Output :- None (data structure)
// Sideeffect :- None
struct QuestionPaper {
    std::vector<MCQ> mcqs;                           // Vector of multiple choice questions
    std::vector<std::string> fillInBlanks;           // Vector of fill-in-the-blank questions
    std::vector<std::string> shortQuestions;        // Vector of short answer questions
    std::vector<std::string> longQuestions;         // Vector of long answer questions
};

#endif 

