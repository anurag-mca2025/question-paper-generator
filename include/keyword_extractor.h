#ifndef KEYWORD_EXTRACTOR_H
#define KEYWORD_EXTRACTOR_H

#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

// Objective :- Class for extracting keywords from tokens by calculating frequencies and ranking them
// Input :- None (class declaration)
// Output :- None (class declaration)
// Sideeffect :- None
class KeywordExtractor {
public:
    // Objective :- Count the frequency of each token in the input vector and return a frequency map
    // Input :- tokens - vector of strings containing tokens to count
    // Output :- Returns unordered_map with token as key and frequency count as value
    // Sideeffect :- None
    // Assumption :- tokens vector may be empty, all tokens are valid strings
    std::unordered_map<std::string, std::size_t> frequency(const std::vector<std::string>& tokens) const;
    
    // Objective :- Extract top N keywords from frequency map, sorted by frequency (descending), then alphabetically for ties
    // Input :- freq - unordered_map containing token frequencies, limit - maximum number of keywords to return
    // Output :- Returns vector of pairs (keyword, frequency) sorted by frequency descending, limited to 'limit' entries
    // Sideeffect :- Creates and sorts a vector, may resize the result vector
    // Assumption :- freq map may be empty, limit is a positive integer
    std::vector<std::pair<std::string, std::size_t>> topKeywords(
        const std::unordered_map<std::string, std::size_t>& freq, std::size_t limit) const;
};

#endif 

