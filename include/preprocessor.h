#ifndef PREPROCESSOR_H
#define PREPROCESSOR_H

#include "document.h"
#include <string>
#include <unordered_set>
#include <vector>

// Objective :- Class for preprocessing text: normalizing, tokenizing, and removing stopwords from documents
// Input :- None (class declaration)
// Output :- None (class declaration)
// Sideeffect :- None
class Preprocessor {
public:
    // Objective :- Initialize Preprocessor with a set of stopwords for text filtering
    // Input :- None
    // Output :- None (constructor)
    // Sideeffect :- Calls buildStopwords() to populate stopwords member variable
    // Assumption :- None
    Preprocessor();
    
    // Objective :- Process a single line of text: normalize, tokenize, and remove stopwords
    // Input :- line - string containing raw text line to process
    // Output :- Returns ProcessedLine object containing original line, cleaned text, and filtered tokens
    // Sideeffect :- Creates ProcessedLine object and processes text
    // Assumption :- line is a valid string, may be empty
    ProcessedLine process(const std::string& line) const;
    
    // Objective :- Process all lines in a document, filtering out lines with no tokens after stopword removal
    // Input :- doc - Document object containing lines to process
    // Output :- Returns vector of ProcessedLine objects (only lines with tokens)
    // Sideeffect :- Creates vector and processes each line
    // Assumption :- doc contains valid lines vector, may be empty
    std::vector<ProcessedLine> process(const Document& doc) const;
    
    // Objective :- Process all documents and combine their processed lines into a single vector
    // Input :- docs - vector of Document objects to process
    // Output :- Returns vector of ProcessedLine objects from all documents combined
    // Sideeffect :- Creates vector and processes all documents
    // Assumption :- docs vector may be empty, each document is valid
    std::vector<ProcessedLine> process(const std::vector<Document>& docs) const;
    
    // Objective :- Collect all tokens from all processed lines into a single flat vector
    // Input :- lines - vector of ProcessedLine objects containing tokens
    // Output :- Returns vector of all tokens from all lines
    // Sideeffect :- Creates new vector and copies tokens
    // Assumption :- lines vector may be empty, each ProcessedLine contains valid tokens
    std::vector<std::string> collectTokens(const std::vector<ProcessedLine>& lines) const;

private:
    std::unordered_set<std::string> stopwords;  // Set of stopwords to filter out during processing
    
    // Objective :- Build and return a set of common English stopwords to filter out during text processing
    // Input :- None
    // Output :- Returns unordered_set containing common stopwords
    // Sideeffect :- Creates and returns a set
    // Assumption :- None
    static std::unordered_set<std::string> buildStopwords();
    
    // Objective :- Normalize text by converting to lowercase and replacing non-alphanumeric characters with spaces
    // Input :- line - string containing text to normalize
    // Output :- Returns normalized string with lowercase letters/numbers and spaces only
    // Sideeffect :- Creates new string
    // Assumption :- line is a valid string, may be empty
    static std::string normalize(const std::string& line);
    
    // Objective :- Split text into individual word tokens separated by whitespace
    // Input :- text - string containing text to tokenize
    // Output :- Returns vector of strings, each string is a token (word)
    // Sideeffect :- Creates string stream and vector
    // Assumption :- text is a valid string, may be empty or contain only whitespace
    static std::vector<std::string> tokenize(const std::string& text);
    
    // Objective :- Filter out stopwords from token vector, keeping only meaningful words
    // Input :- tokens - vector of strings containing tokens to filter
    // Output :- Returns vector of tokens with stopwords removed
    // Sideeffect :- Creates new vector
    // Assumption :- tokens vector may be empty, stopwords member variable is initialized
    std::vector<std::string> removeStopWords(const std::vector<std::string>& tokens) const;
};

#endif 

