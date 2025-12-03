#include "../include/question_generator.h"
#include <algorithm>
#include <random>
#include <sstream>
#include <stdexcept>
#include <string>
#include <unordered_set>
#include <vector>

// Objective :- Initialize QuestionGenerator with specified counts for each question type
// Input :- mcqCount - number of MCQs to generate, fillCount - number of fill-in-blank questions, shortCount - number of short questions, longCount - number of long questions
// Output :- None (constructor)
// Sideeffect :- Stores question counts in member variables
// Assumption :- All counts are non-negative integers
QuestionGenerator::QuestionGenerator(std::size_t mcqCount,
                                     std::size_t fillCount,
                                     std::size_t shortCount,
                                     std::size_t longCount)
    : mcqCount(mcqCount),      // Store MCQ count
      fillCount(fillCount),    // Store fill-in-blank count
      shortCount(shortCount),  // Store short question count
      longCount(longCount) {}  // Store long question count

// Objective :- Generate a complete question paper with MCQs, fill-in-blanks, short questions, and long questions from scored lines and keywords
// Input :- lines - vector of ScoredLine objects (sorted by score), keywords - vector of keyword-frequency pairs
// Output :- Returns QuestionPaper object containing all generated questions
// Sideeffect :- Creates QuestionPaper object, generates random numbers, may skip questions if generation fails
// Assumption :- lines and keywords may be empty, lines are sorted by score descending, keywords are sorted by frequency
QuestionPaper QuestionGenerator::generate(const std::vector<ScoredLine>& lines,
                                           const std::vector<std::pair<std::string, std::size_t>>& keywords) const {
    // Initialize empty question paper
    QuestionPaper paper;
    // Return empty paper if inputs are empty
    if (lines.empty() || keywords.empty())
        return paper;

    // Extract keyword strings from keyword-frequency pairs
    std::vector<std::string> keywordList;
    for (auto& k : keywords)
        keywordList.push_back(k.first);

    // Initialize random number generator for shuffling options
    std::mt19937 rng(std::random_device{}());

    // Generate MCQs up to specified count or available lines
    for (size_t i = 0; i < mcqCount && i < lines.size(); i++) {
        try {
            // Build MCQ from scored line
            paper.mcqs.push_back(buildMcq(lines[i], keywordList, rng));
        } catch (...) {} // Skip MCQ if generation fails
    }

    // Generate fill-in-blank questions up to specified count or available lines
    for (size_t i = 0; i < fillCount && i < lines.size(); i++) {
        // Build fill-in-blank question
        auto q = buildFillBlank(lines[i], keywordList);
        // Add question if not empty
        if (!q.empty())
            paper.fillInBlanks.push_back(q);
    }

    // Generate short answer questions from keywords
    for (size_t i = 0; i < shortCount && i < keywordList.size(); i++)
        paper.shortQuestions.push_back(buildShortQuestion(keywordList[i]));

    // Generate long answer questions from scored lines
    for (size_t i = 0; i < longCount && i < lines.size(); i++)
        paper.longQuestions.push_back(buildLongQuestion(lines[i]));

    // Return generated question paper
    return paper;
}

// Objective :- Build a multiple choice question from a scored line by finding a keyword and creating options with distractors
// Input :- line - ScoredLine object containing processed line, keywords - vector of keyword strings, rng - random number generator reference
// Output :- Returns MCQ object with prompt, options, and correct answer
// Sideeffect :- Shuffles distractors and options, throws exception if keyword not found or insufficient distractors
// Assumption :- line contains tokens, keywords vector has at least 4 entries, rng is initialized
MCQ QuestionGenerator::buildMcq(const ScoredLine& line,
                                 const std::vector<std::string>& keywords,
                                 std::mt19937& rng) {
    // Create set of keywords for fast lookup
    std::unordered_set<std::string> kw(keywords.begin(), keywords.end());
    // Variable to store found keyword
    std::string keyword;

    // Find first keyword that appears in line tokens
    for (auto& t : line.line.tokens)
        if (kw.count(t)) {
            keyword = t;
            break;
        }

    // Throw exception if no keyword found in line
    if (keyword.empty())
        throw std::runtime_error("No keyword found");

    // Collect distractors (keywords other than the correct one)
    std::vector<std::string> distractors;
    for (auto& k : keywords)
        if (k != keyword)
            distractors.push_back(k);

    // Throw exception if not enough distractors for 4 options
    if (distractors.size() < 3)
        throw std::runtime_error("Not enough distractors");

    // Shuffle distractors for randomness
    std::shuffle(distractors.begin(), distractors.end(), rng);
    // Create options list with correct answer and 3 distractors
    std::vector<std::string> options = {
        keyword, distractors[0], distractors[1], distractors[2]
    };
    // Shuffle options so correct answer is in random position
    std::shuffle(options.begin(), options.end(), rng);

    // Find correct answer label (A, B, C, or D)
    char answer = 'A';
    for (size_t i = 0; i < options.size(); i++)
        if (options[i] == keyword)
            answer = char('A' + i);

    // Create MCQ object
    MCQ mcq;
    // Build prompt with masked keyword
    mcq.prompt = "Identify the keyword that completes the statement:\n    " +
                 maskKeyword(line.line.original, keyword);
    // Set shuffled options
    mcq.options = options;
    // Set correct answer label
    mcq.answer = answer;

    // Return completed MCQ
    return mcq;
}

// Objective :- Build a fill-in-the-blank question by finding a keyword in the line and masking it
// Input :- line - ScoredLine object containing processed line, keywords - vector of keyword strings to search for
// Output :- Returns string containing question with masked keyword, or empty string if no keyword found
// Sideeffect :- None
// Assumption :- line contains valid tokens, keywords vector may be empty
std::string QuestionGenerator::buildFillBlank(const ScoredLine& line,
                                               const std::vector<std::string>& keywords) {
    // Search for first keyword that appears in line
    for (auto& k : keywords)
        if (containsKeyword(line.line, k))
            // Return line with keyword masked
            return maskKeyword(line.line.original, k);
    // Return empty string if no keyword found
    return {};
}

// Objective :- Generate a short answer question prompt asking to explain a keyword concept
// Input :- keyword - string containing keyword to ask about
// Output :- Returns formatted question string
// Sideeffect :- None
// Assumption :- keyword is a valid non-empty string
std::string QuestionGenerator::buildShortQuestion(const std::string& keyword) {
    // Return formatted question asking for explanation
    return "Explain the concept of \"" + keyword + "\" in 2–3 sentences.";
}

// Objective :- Generate a long answer question prompt based on the original line text
// Input :- line - ScoredLine object containing original line text
// Output :- Returns formatted long question string
// Sideeffect :- Creates string stream
// Assumption :- line.line.original contains valid text
std::string QuestionGenerator::buildLongQuestion(const ScoredLine& line) {
    // Create output string stream
    std::ostringstream ss;
    // Build question prompt with original line text
    ss << "Discuss in detail: \"" << line.line.original
       << "\". Include definitions, examples, and implications.";
    // Return formatted string
    return ss.str();
}

// Objective :- Check if a processed line contains a specific keyword in its tokens
// Input :- line - ProcessedLine object containing tokens, kw - string keyword to search for
// Output :- Returns true if keyword found in tokens, false otherwise
// Sideeffect :- None
// Assumption :- line.tokens vector is valid, kw is a valid string
bool QuestionGenerator::containsKeyword(const ProcessedLine& line, const std::string& kw) {
    // Search for keyword in tokens vector
    return std::find(line.tokens.begin(), line.tokens.end(), kw) != line.tokens.end();
}

// Objective :- Remove (mask) a keyword from text by finding it case-insensitively and deleting it
// Input :- text - string containing original text, keyword - string keyword to remove
// Output :- Returns text string with keyword removed, or original text if keyword not found
// Sideeffect :- Creates lowercase copies of strings
// Assumption :- text and keyword are valid strings, keyword may appear in text
std::string QuestionGenerator::maskKeyword(const std::string& text, const std::string& keyword) {
    // Lambda function to convert string to lowercase
    auto lower = [](const std::string& s) {
        std::string out;
        // Convert each character to lowercase
        for (unsigned char c : s)
            out.push_back(std::tolower(c));
        return out;
    };

    // Convert text and keyword to lowercase for case-insensitive search
    std::string lowText = lower(text);
    std::string lowKey = lower(keyword);
    // Find keyword position in lowercase text
    size_t pos = lowText.find(lowKey);

    // Check if keyword was found
    if (pos != std::string::npos) {
        // Create copy of original text
        std::string masked = text;
        // Replace keyword with empty string (remove it)
        masked.replace(pos, keyword.size(), "");
        // Return masked text
        return masked;
    }

    // Return original text if keyword not found
    return text;
}

