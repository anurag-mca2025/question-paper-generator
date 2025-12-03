#include "../include/file_manager.h"
#include <cctype>
#include <dirent.h>
#include <sys/stat.h>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

// Objective :- Remove leading and trailing whitespace characters from a string
// Input :- str - string that may contain leading/trailing whitespace
// Output :- Returns trimmed string with whitespace removed, or empty string if input is empty or contains only whitespace
// Sideeffect :- None
// Assumption :- Input string is valid and accessible
static std::string trim(const std::string& str) {
    // Return empty string if input is already empty
    if (str.empty()) return "";
    // Find first non-whitespace character position
    size_t first = str.find_first_not_of(" \t\n\r");
    // Return empty string if only whitespace found
    if (first == std::string::npos) return "";
    // Find last non-whitespace character position
    size_t last = str.find_last_not_of(" \t\n\r");
    // Extract substring between first and last non-whitespace characters
    return str.substr(first, (last - first + 1));
}

// Objective :- Initialize FileManager with a folder or file path for document loading
// Input :- folderPath - string containing path to folder or .txt file
// Output :- None (constructor)
// Sideeffect :- Stores the provided path in member variable folderPath
// Assumption :- folderPath is a valid string (may be empty or invalid path)
FileManager::FileManager(std::string folderPath)
    : folderPath(std::move(folderPath)) {} // Move the path string to member variable

// Objective :- Load all .txt documents from a folder path or load a single .txt file. Handles both file and directory paths.
// Input :- None (uses member variable folderPath)
// Output :- Returns vector of Document objects containing loaded text files
// Sideeffect :- Reads files from filesystem, writes error messages to cerr, may open and close file streams
// Assumption :- folderPath member variable contains a valid path string, filesystem permissions allow reading
std::vector<Document> FileManager::loadDocuments() const {
    // Initialize empty vector to store documents
    std::vector<Document> docs;
    
    // Clean the path: trim whitespace and remove quotes
    std::string path = trim(folderPath);
    // Remove surrounding quotes if present
    if (!path.empty() && path.front() == '"' && path.back() == '"') {
        // Extract substring without quotes
        path = path.substr(1, path.length() - 2);
        // Trim again after removing quotes
        path = trim(path);
    }
    
    // Check if path is empty after cleaning
    if (path.empty()) {
        // Write error message to standard error
        std::cerr << "[FileManager] Empty path provided.\n";
        // Return empty vector
        return docs;
    }
    
    // First, try to open as a file directly
    std::ifstream fileTest(path);
    // Check if file opened successfully
    if (fileTest.is_open()) {
        // Close the test file stream
        fileTest.close();
        // It's a file we can open
        // Check if it's a .txt file
        if (isTextFile(path)) {
            // Create document object
            Document doc;
            // Find last directory separator to extract filename
            size_t lastSlash = path.find_last_of("/\\");
            // Extract filename from path
            doc.name = (lastSlash != std::string::npos) ? path.substr(lastSlash + 1) : path;
            // Read all lines from the file
            doc.lines = readFileLines(path);
            // Check if file had content
            if (!doc.lines.empty()) {
                // Add document to vector
                docs.push_back(std::move(doc));
                // Return documents
                return docs;
            } else {
                // Write error message for empty file
                std::cerr << "[FileManager] File is empty: " << path << "\n";
                // Return empty vector
                return docs;
            }
        } else {
            // Write error message for non-text file
            std::cerr << "[FileManager] File is not a .txt file: " << path << "\n";
            // Return empty vector
            return docs;
        }
    }
    
    // Try with stat to check if it's a file
    struct stat s{};
    // Get file status information
    if (stat(path.c_str(), &s) == 0) {
        // Check if it's a regular file
        if (s.st_mode & S_IFREG) {
            // It's a regular file
            // Check if it's a .txt file
            if (isTextFile(path)) {
                // Create document object
                Document doc;
                // Find last directory separator to extract filename
                size_t lastSlash = path.find_last_of("/\\");
                // Extract filename from path
                doc.name = (lastSlash != std::string::npos) ? path.substr(lastSlash + 1) : path;
                // Read all lines from the file
                doc.lines = readFileLines(path);
                // Check if file had content
                if (!doc.lines.empty()) {
                    // Add document to vector
                    docs.push_back(std::move(doc));
                    // Return documents
                    return docs;
                }
            }
        }
    }
    
    // Try as a directory
    DIR* dir = opendir(path.c_str());
    // Check if directory opened successfully
    if (dir) {
        struct dirent* entry;
        // Iterate through directory entries
        while ((entry = readdir(dir)) != nullptr) {
            // Get entry name
            std::string name = entry->d_name;
            // Skip current and parent directory entries
            if (name == "." || name == "..")
                continue;

            // Build full path to file
            std::string fullPath = path;
            // Add directory separator if not present
            if (path.back() != '/' && path.back() != '\\')
                fullPath += "/";
            // Append filename to path
            fullPath += name;
            
            // Check if entry is a regular file and has .txt extension
            if (isRegularFile(fullPath) && isTextFile(name)) {
                // Create document object
                Document doc;
                // Set document name to filename
                doc.name = name;
                // Read all lines from the file
                doc.lines = readFileLines(fullPath);
                // Add document if it has content
                if (!doc.lines.empty())
                    docs.push_back(std::move(doc));
            }
        }
        // Close directory stream
        closedir(dir);
        // Return loaded documents
        return docs;
    }
    
    // If we get here, path doesn't exist
    // Write error messages to standard error
    std::cerr << "[FileManager] Path not found: " << path << "\n";
    std::cerr << "[FileManager] Please check the path and try again.\n";
    // Return empty vector
    return docs;
}

// Objective :- Check if the given path points to a regular file (not a directory or special file)
// Input :- path - string containing file system path to check
// Output :- Returns true if path is a regular file, false otherwise
// Sideeffect :- Calls stat system call to check file status
// Assumption :- Path string is valid, filesystem permissions allow stat call
bool FileManager::isRegularFile(const std::string& path) {
    // Structure to hold file status information
    struct stat s{};
    // Get file status - returns 0 on success
    if (stat(path.c_str(), &s) == 0)
        // Check if file mode indicates regular file
        return (s.st_mode & S_IFREG);
    // Return false if stat failed or not a regular file
    return false;
}

// Objective :- Determine if a filename has a .txt extension (case-insensitive)
// Input :- filename - string containing filename to check
// Output :- Returns true if filename ends with .txt extension, false otherwise
// Sideeffect :- Converts extension characters to lowercase
// Assumption :- filename is a valid string, may or may not contain extension
bool FileManager::isTextFile(const std::string& filename) {
    // Find last occurrence of dot character
    auto dot = filename.rfind('.');
    // Return false if no dot found
    if (dot == std::string::npos)
        return false;

    // Extract substring after the dot (extension)
    std::string ext = filename.substr(dot + 1);
    // Convert extension to lowercase for case-insensitive comparison
    for (char &c : ext)
        c = std::tolower(static_cast<unsigned char>(c));
    // Check if extension equals "txt"
    return ext == "txt";
}

// Objective :- Read all lines from a text file and return them as a vector of strings
// Input :- filepath - string containing path to the file to read
// Output :- Returns vector of strings, each string representing one line from the file
// Sideeffect :- Opens file stream, reads file content, writes error message to cerr if file cannot be opened
// Assumption :- filepath points to a valid readable file, file permissions allow reading
std::vector<std::string> FileManager::readFileLines(const std::string& filepath) {
    // Open input file stream
    std::ifstream input(filepath);
    // Initialize vector to store lines
    std::vector<std::string> lines;
    
    // Check if file opened successfully
    if (!input.is_open()) {
        // Write error message to standard error
        std::cerr << "[FileManager] Failed to open: " << filepath << '\n';
        // Return empty vector
        return lines;
    }

    // Variable to hold each line read from file
    std::string line;
    // Read file line by line until end of file
    while (std::getline(input, line))
        // Add line to vector
        lines.push_back(line);
    
    // Return vector containing all lines
    return lines;
}
