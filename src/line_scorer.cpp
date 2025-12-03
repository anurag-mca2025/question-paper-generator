#include "../include/line_scorer.h"
#include <algorithm>
#include <string>
#include <unordered_map>
#include <vector>

// Objective :- Score each processed line based on keyword frequency and line length, then sort by score descending
// Input :- lines - vector of ProcessedLine objects to score, keywordFreq - map of keyword frequencies
// Output :- Returns vector of ScoredLine objects sorted by score in descending order
// Sideeffect :- Creates and sorts a vector of ScoredLine objects
// Assumption :- lines and keywordFreq may be empty, keywordFreq contains at least one entry if not empty
std::vector<ScoredLine> LineScorer::score(
    const std::vector<ProcessedLine>& lines,
    const std::unordered_map<std::string, std::size_t>& keywordFreq) const {
    
    // Initialize empty vector to store scored lines
    std::vector<ScoredLine> scored;
    // Return empty vector if input is empty
    if (lines.empty() || keywordFreq.empty())
        return scored;

    // Find maximum frequency value in keyword frequency map
    const auto maxIt = std::max_element(keywordFreq.begin(), keywordFreq.end(),
                                        [](auto& a, auto& b) { return a.second < b.second; });
    // Store maximum frequency for normalization
    double maxFreq = maxIt->second;

    // Iterate through each processed line
    for (const auto& line : lines) {
        // Skip lines with no tokens
        if (line.tokens.empty())
            continue;

        // Initialize keyword score accumulator
        double keywordScore = 0.0;
        // Calculate keyword score by summing normalized frequencies
        for (const auto& token : line.tokens) {
            // Look up token in frequency map
            auto it = keywordFreq.find(token);
            // If token is a keyword, add normalized frequency to score
            if (it != keywordFreq.end())
                keywordScore += it->second / maxFreq;
        }

        // Calculate keyword density (score per token)
        double density = keywordScore / line.tokens.size();
        // Calculate length bonus (capped at 1.0 for lines with 20+ tokens)
        double lengthBonus = std::min(1.0, (double)line.tokens.size() / 20.0);

        // Create scored line object
        ScoredLine s;
        // Store original processed line
        s.line = line;
        // Calculate final score: 80% density + 20% length bonus
        s.score = density * 0.8 + lengthBonus * 0.2;
        // Add scored line to vector
        scored.push_back(std::move(s));
    }

    // Sort scored lines by score in descending order
    std::sort(scored.begin(), scored.end(),
              [](auto& a, auto& b) { return a.score > b.score; });

    // Return sorted scored lines
    return scored;
}

