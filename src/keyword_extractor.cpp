#include "../include/keyword_extractor.h"
#include <algorithm>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

// Objective :- Count the frequency of each token in the input vector and return a frequency map
// Input :- tokens - vector of strings containing tokens to count
// Output :- Returns unordered_map with token as key and frequency count as value
// Sideeffect :- None
// Assumption :- tokens vector may be empty, all tokens are valid strings
std::unordered_map<std::string, std::size_t> KeywordExtractor::frequency(const std::vector<std::string>& tokens) const {
    // Initialize empty map to store token frequencies
    std::unordered_map<std::string, std::size_t> counts;
    // Iterate through each token in the vector
    for (const auto& token : tokens) {
        // Increment count for current token (creates entry if doesn't exist)
        ++counts[token];
    }
    // Return frequency map
    return counts;
}

// Objective :- Extract top N keywords from frequency map, sorted by frequency (descending), then alphabetically for ties
// Input :- freq - unordered_map containing token frequencies, limit - maximum number of keywords to return
// Output :- Returns vector of pairs (keyword, frequency) sorted by frequency descending, limited to 'limit' entries
// Sideeffect :- Creates and sorts a vector, may resize the result vector
// Assumption :- freq map may be empty, limit is a positive integer
std::vector<std::pair<std::string, std::size_t>> KeywordExtractor::topKeywords(
    const std::unordered_map<std::string, std::size_t>& freq, std::size_t limit) const {
    
    // Convert frequency map to vector of pairs
    std::vector<std::pair<std::string, std::size_t>> ranked(freq.begin(), freq.end());
    // Sort vector: first by frequency (descending), then alphabetically by keyword for ties
    std::sort(ranked.begin(), ranked.end(),
              [](const auto& a, const auto& b) {
                  // If frequencies are equal, sort alphabetically by keyword
                  if (a.second == b.second)
                      return a.first < b.first;
                  // Otherwise sort by frequency descending
                  return a.second > b.second;
              });
    
    // Limit result to specified number of top keywords
    if (ranked.size() > limit)
        ranked.resize(limit);
    
    // Return sorted and limited keyword list
    return ranked;
}

