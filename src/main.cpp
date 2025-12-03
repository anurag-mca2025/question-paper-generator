#include "../include/document.h"
#include "../include/file_manager.h"
#include "../include/preprocessor.h"
#include "../include/keyword_extractor.h"
#include "../include/line_scorer.h"
#include "../include/question_generator.h"
#include "../include/paper_exporter.h"
#include "../include/ui.h"
#include <iostream>
#include <string>

// Objective :- Main entry point of the question paper generator application. Manages the application flow, user interaction, and coordinates all components to generate question papers from text documents.
// Input :- None (reads from standard input)
// Output :- Returns 0 on successful program termination
// Sideeffect :- Reads user input from console, writes output to console and files, creates FileManager, Preprocessor, KeywordExtractor, LineScorer, QuestionGenerator, and PaperExporter objects
int main() {

    // Display welcome banner to the user
    ui::printBanner();

    // Initialize preprocessor to clean and tokenize text
    Preprocessor pre;

    // Initialize keyword extractor to find important terms
    KeywordExtractor ex;

    // Initialize line scorer to rank lines by importance
    LineScorer scorer;

    // Initialize question generator to create questions
    QuestionGenerator gen;

    // Initialize paper exporter to save questions to file
    PaperExporter exporter;

    // Main application loop - continues until user chooses to exit
    while (true) {

        // Display menu options to user
        ui::showMenu();

        // Variable to store user's menu choice
        std::string choice;

        // Read user's choice from standard input
        std::getline(std::cin, choice);

        // Check if user wants to exit
        if (choice == "2") {

            // Display goodbye message
            std::cout << "Goodbye!\n";

            // Exit the main loop
            break;

        }

        // Validate user input - must be "1" to proceed
        if (choice != "1") {

            // Display error message for invalid choice
            std::cout << "Invalid choice!\n";

            // Continue to next iteration of loop
            continue;

        }

        // Prompt user for input folder or file path
        std::string folder = ui::prompt("Enter folder path or .txt file path: ");

        // Create file manager with user-provided path
        FileManager fm(folder);

        // Load all text documents from the specified path
        auto docs = fm.loadDocuments();

        // Check if any documents were loaded
        if (docs.empty()) {

            // Display error message if no files found
            std::cout << "No .txt files found!\n";

            // Continue to next iteration of loop
            continue;

        }

        // Process all documents to clean and tokenize text
        auto processed = pre.process(docs);

        // Collect all tokens from processed lines
        auto tokens = pre.collectTokens(processed);

        // Calculate frequency of each token
        auto freq = ex.frequency(tokens);

        // Extract top 100 keywords based on frequency
        auto keywords = ex.topKeywords(freq, 100);

        // Score each processed line based on keyword frequency
        auto scored = scorer.score(processed, freq);

        // Check if scoring was successful
        if (scored.empty()) {

            // Display error message if scoring failed
            std::cout << "Unable to score lines!\n";

            // Continue to next iteration of loop
            continue;

        }

        // Generate question paper from scored lines and keywords
        auto paper = gen.generate(scored, keywords);

        // Display summary of generated questions
        ui::summarize(paper);

        // Prompt user for output file path (will automatically add .txt extension if missing)
        std::string out = ui::prompt("Enter output file path: ");

        // Attempt to export question paper to file (creates new .txt file if it doesn't exist)
        if (exporter.exportToFile(paper, out))
            // Display success message
            std::cout << "Saved successfully.\n";
        else
            // Display failure message
            std::cout << "Save failed.\n";

    }

    // Return success status
    return 0;

}

