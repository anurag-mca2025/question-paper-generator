#ifndef QUESTION_GENERATOR_H
#define QUESTION_GENERATOR_H

#include "document.h"
#include <random>
#include <string>
#include <utility>
#include <vector>

// Objective :- Class for generating different types of questions (MCQs, fill-in-blanks, short, long) from processed text
// Input :- None (class declaration)
// Output :- None (class declaration)
// Sideeffect :- None
class QuestionGenerator {
public:
    // Objective :- Initialize QuestionGenerator with specified counts for each question type
    // Input :- mcqCount - number of MCQs to generate (default 5), fillCount - number of fill-in-blank questions (default 5), shortCount - number of short questions (default 5), longCount - number of long questions (default 3)
    // Output :- None (constructor)
    // Sideeffect :- Stores question counts in member variables
    // Assumption :- All counts are non-negative integers
    QuestionGenerator(std::size_t mcqCount = 5,
                      std::size_t fillCount = 5,
                      std::size_t shortCount = 5,
                      std::size_t longCount = 3);
    
    // Objective :- Generate a complete question paper with MCQs, fill-in-blanks, short questions, and long questions from scored lines and keywords
    // Input :- lines - vector of ScoredLine objects (sorted by score), keywords - vector of keyword-frequency pairs
    // Output :- Returns QuestionPaper object containing all generated questions
    // Sideeffect :- Creates QuestionPaper object, generates random numbers, may skip questions if generation fails
    // Assumption :- lines and keywords may be empty, lines are sorted by score descending, keywords are sorted by frequency
    QuestionPaper generate(const std::vector<ScoredLine>& lines,
                           const std::vector<std::pair<std::string, std::size_t>>& keywords) const;

private:
    std::size_t mcqCount;      // Number of multiple choice questions to generate
    std::size_t fillCount;     // Number of fill-in-blank questions to generate
    std::size_t shortCount;    // Number of short answer questions to generate
    std::size_t longCount;     // Number of long answer questions to generate
    
    // Objective :- Build a multiple choice question from a scored line by finding a keyword and creating options with distractors
    // Input :- line - ScoredLine object containing processed line, keywords - vector of keyword strings, rng - random number generator reference
    // Output :- Returns MCQ object with prompt, options, and correct answer
    // Sideeffect :- Shuffles distractors and options, throws exception if keyword not found or insufficient distractors
    // Assumption :- line contains tokens, keywords vector has at least 4 entries, rng is initialized
    static MCQ buildMcq(const ScoredLine& line,
                        const std::vector<std::string>& keywords,
                        std::mt19937& rng);
    
    // Objective :- Build a fill-in-the-blank question by finding a keyword in the line and masking it
    // Input :- line - ScoredLine object containing processed line, keywords - vector of keyword strings to search for
    // Output :- Returns string containing question with masked keyword, or empty string if no keyword found
    // Sideeffect :- None
    // Assumption :- line contains valid tokens, keywords vector may be empty
    static std::string buildFillBlank(const ScoredLine& line,
                                      const std::vector<std::string>& keywords);
    
    // Objective :- Generate a short answer question prompt asking to explain a keyword concept
    // Input :- keyword - string containing keyword to ask about
    // Output :- Returns formatted question string
    // Sideeffect :- None
    // Assumption :- keyword is a valid non-empty string
    static std::string buildShortQuestion(const std::string& keyword);
    
    // Objective :- Generate a long answer question prompt based on the original line text
    // Input :- line - ScoredLine object containing original line text
    // Output :- Returns formatted long question string
    // Sideeffect :- Creates string stream
    // Assumption :- line.line.original contains valid text
    static std::string buildLongQuestion(const ScoredLine& line);
    
    // Objective :- Check if a processed line contains a specific keyword in its tokens
    // Input :- line - ProcessedLine object containing tokens, kw - string keyword to search for
    // Output :- Returns true if keyword found in tokens, false otherwise
    // Sideeffect :- None
    // Assumption :- line.tokens vector is valid, kw is a valid string
    static bool containsKeyword(const ProcessedLine& line, const std::string& kw);
    
    // Objective :- Remove (mask) a keyword from text by finding it case-insensitively and deleting it
    // Input :- text - string containing original text, keyword - string keyword to remove
    // Output :- Returns text string with keyword removed, or original text if keyword not found
    // Sideeffect :- Creates lowercase copies of strings
    // Assumption :- text and keyword are valid strings, keyword may appear in text
    static std::string maskKeyword(const std::string& text, const std::string& keyword);
};

#endif 

