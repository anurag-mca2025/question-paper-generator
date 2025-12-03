#include "../include/preprocessor.h"
#include <cctype>
#include <sstream>
#include <string>
#include <unordered_set>
#include <vector>

// Objective :- Initialize Preprocessor with a set of stopwords for text filtering
// Input :- None
// Output :- None (constructor)
// Sideeffect :- Calls buildStopwords() to populate stopwords member variable
// Assumption :- None
Preprocessor::Preprocessor() : stopwords(buildStopwords()) {} // Initialize stopwords set

// Objective :- Process a single line of text: normalize, tokenize, and remove stopwords
// Input :- line - string containing raw text line to process
// Output :- Returns ProcessedLine object containing original line, cleaned text, and filtered tokens
// Sideeffect :- Creates ProcessedLine object and processes text
// Assumption :- line is a valid string, may be empty
ProcessedLine Preprocessor::process(const std::string& line) const {
    // Create ProcessedLine object to store results
    ProcessedLine result;
    // Store original line text
    result.original = line;
    // Normalize line (lowercase, remove special chars)
    result.cleaned = normalize(line);
    // Tokenize cleaned text into words
    auto tokens = tokenize(result.cleaned);
    // Remove stopwords from tokens
    result.tokens = removeStopWords(tokens);
    // Return processed line
    return result;
}

// Objective :- Process all lines in a document, filtering out lines with no tokens after stopword removal
// Input :- doc - Document object containing lines to process
// Output :- Returns vector of ProcessedLine objects (only lines with tokens)
// Sideeffect :- Creates vector and processes each line
// Assumption :- doc contains valid lines vector, may be empty
std::vector<ProcessedLine> Preprocessor::process(const Document& doc) const {
    // Initialize vector to store processed lines
    std::vector<ProcessedLine> processed;
    // Reserve space for efficiency
    processed.reserve(doc.lines.size());
    
    // Iterate through each line in document
    for (const auto& line : doc.lines) {
        // Process current line
        auto processedLine = process(line);
        // Only add lines that have tokens after processing
        if (!processedLine.tokens.empty()) {
            // Add processed line to vector
            processed.push_back(std::move(processedLine));
        }
    }
    
    // Return processed lines
    return processed;
}

// Objective :- Process all documents and combine their processed lines into a single vector
// Input :- docs - vector of Document objects to process
// Output :- Returns vector of ProcessedLine objects from all documents combined
// Sideeffect :- Creates vector and processes all documents
// Assumption :- docs vector may be empty, each document is valid
std::vector<ProcessedLine> Preprocessor::process(const std::vector<Document>& docs) const {
    // Initialize vector to store all processed lines
    std::vector<ProcessedLine> all;
    // Iterate through each document
    for (const auto& doc : docs) {
        // Process current document
        auto processed = process(doc);
        // Append processed lines to combined vector
        all.insert(all.end(), processed.begin(), processed.end());
    }
    // Return all processed lines
    return all;
}

// Objective :- Collect all tokens from all processed lines into a single flat vector
// Input :- lines - vector of ProcessedLine objects containing tokens
// Output :- Returns vector of all tokens from all lines
// Sideeffect :- Creates new vector and copies tokens
// Assumption :- lines vector may be empty, each ProcessedLine contains valid tokens
std::vector<std::string> Preprocessor::collectTokens(const std::vector<ProcessedLine>& lines) const {
    // Initialize vector to store all tokens
    std::vector<std::string> tokens;
    // Iterate through each processed line
    for (const auto& line : lines) {
        // Append all tokens from current line to token vector
        tokens.insert(tokens.end(), line.tokens.begin(), line.tokens.end());
    }
    // Return collected tokens
    return tokens;
}

// Objective :- Build and return a set of common English stopwords to filter out during text processing
// Input :- None
// Output :- Returns unordered_set containing common stopwords
// Sideeffect :- Creates and returns a set
// Assumption :- None
std::unordered_set<std::string> Preprocessor::buildStopwords() {
    // Return set initialized with common English stopwords
    return {"a", "an", "the", "and", "or", "but", "if", "then", "else", "for", "with",
            "about", "into", "onto", "from", "that", "this", "those", "these", "to", "of",
            "in", "on", "at", "as", "by", "it", "is", "are", "was", "were", "be", "been",
            "being", "so", "we", "you", "they", "he", "she", "them", "their", "there",
            "here", "can", "could", "should", "would", "may", "might", "will", "just",
            "than", "also", "such", "not", "no", "do", "does", "did", "done", "its"};
}

// Objective :- Normalize text by converting to lowercase and replacing non-alphanumeric characters with spaces
// Input :- line - string containing text to normalize
// Output :- Returns normalized string with lowercase letters/numbers and spaces only
// Sideeffect :- Creates new string
// Assumption :- line is a valid string, may be empty
std::string Preprocessor::normalize(const std::string& line) {
    // Initialize string to store normalized result
    std::string lower;
    // Reserve space for efficiency
    lower.reserve(line.size());
    
    // Iterate through each character in input
    for (unsigned char ch : line) {
        // Check if character is alphanumeric or whitespace
        if (std::isalnum(ch) || std::isspace(ch)) {
            // Convert to lowercase and add to result
            lower.push_back(std::tolower(ch));
        } else {
            // Replace special characters with space
            lower.push_back(' ');
        }
    }
    
    // Return normalized string
    return lower;
}

// Objective :- Split text into individual word tokens separated by whitespace
// Input :- text - string containing text to tokenize
// Output :- Returns vector of strings, each string is a token (word)
// Sideeffect :- Creates string stream and vector
// Assumption :- text is a valid string, may be empty or contain only whitespace
std::vector<std::string> Preprocessor::tokenize(const std::string& text) {
    // Create input string stream from text
    std::istringstream stream(text);
    // Initialize vector to store tokens
    std::vector<std::string> tokens;
    // Variable to hold each token read from stream
    std::string token;
    
    // Extract tokens separated by whitespace
    while (stream >> token) {
        // Add token to vector
        tokens.push_back(token);
    }
    
    // Return vector of tokens
    return tokens;
}

// Objective :- Filter out stopwords from token vector, keeping only meaningful words
// Input :- tokens - vector of strings containing tokens to filter
// Output :- Returns vector of tokens with stopwords removed
// Sideeffect :- Creates new vector
// Assumption :- tokens vector may be empty, stopwords member variable is initialized
std::vector<std::string> Preprocessor::removeStopWords(const std::vector<std::string>& tokens) const {
    // Initialize vector to store filtered tokens
    std::vector<std::string> filtered;
    // Reserve space for efficiency
    filtered.reserve(tokens.size());
    
    // Iterate through each token
    for (const auto& token : tokens) {
        // Check if token is not a stopword
        if (stopwords.find(token) == stopwords.end())
            // Add token to filtered vector
            filtered.push_back(token);
    }
    
    // Return filtered tokens
    return filtered;
}

