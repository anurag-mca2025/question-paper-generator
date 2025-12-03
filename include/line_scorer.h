#ifndef LINE_SCORER_H
#define LINE_SCORER_H

#include "document.h"
#include <string>
#include <unordered_map>
#include <vector>

// Objective :- Class for scoring processed lines based on keyword frequency and line length
// Input :- None (class declaration)
// Output :- None (class declaration)
// Sideeffect :- None
class LineScorer {
public:
    // Objective :- Score each processed line based on keyword frequency and line length, then sort by score descending
    // Input :- lines - vector of ProcessedLine objects to score, keywordFreq - map of keyword frequencies
    // Output :- Returns vector of ScoredLine objects sorted by score in descending order
    // Sideeffect :- Creates and sorts a vector of ScoredLine objects
    // Assumption :- lines and keywordFreq may be empty, keywordFreq contains at least one entry if not empty
    std::vector<ScoredLine> score(
        const std::vector<ProcessedLine>& lines,
        const std::unordered_map<std::string, std::size_t>& keywordFreq) const;
};

#endif 

