#ifndef FILE_MANAGER_H
#define FILE_MANAGER_H

#include "document.h"
#include <string>
#include <vector>

// Objective :- Class for managing file operations: loading documents from folders or individual files
// Input :- None (class declaration)
// Output :- None (class declaration)
// Sideeffect :- None
class FileManager {
public:
    // Objective :- Initialize FileManager with a folder or file path for document loading
    // Input :- folderPath - string containing path to folder or .txt file
    // Output :- None (constructor)
    // Sideeffect :- Stores the provided path in member variable folderPath
    // Assumption :- folderPath is a valid string (may be empty or invalid path)
    explicit FileManager(std::string folderPath);
    
    // Objective :- Load all .txt documents from a folder path or load a single .txt file. Handles both file and directory paths.
    // Input :- None (uses member variable folderPath)
    // Output :- Returns vector of Document objects containing loaded text files
    // Sideeffect :- Reads files from filesystem, writes error messages to cerr, may open and close file streams
    // Assumption :- folderPath member variable contains a valid path string, filesystem permissions allow reading
    std::vector<Document> loadDocuments() const;

private:
    std::string folderPath;  // Path to folder or file for document loading
    
    // Objective :- Check if the given path points to a regular file (not a directory or special file)
    // Input :- path - string containing file system path to check
    // Output :- Returns true if path is a regular file, false otherwise
    // Sideeffect :- Calls stat system call to check file status
    // Assumption :- Path string is valid, filesystem permissions allow stat call
    static bool isRegularFile(const std::string& path);
    
    // Objective :- Determine if a filename has a .txt extension (case-insensitive)
    // Input :- filename - string containing filename to check
    // Output :- Returns true if filename ends with .txt extension, false otherwise
    // Sideeffect :- Converts extension characters to lowercase
    // Assumption :- filename is a valid string, may or may not contain extension
    static bool isTextFile(const std::string& filename);
    
    // Objective :- Read all lines from a text file and return them as a vector of strings
    // Input :- filepath - string containing path to the file to read
    // Output :- Returns vector of strings, each string representing one line from the file
    // Sideeffect :- Opens file stream, reads file content, writes error message to cerr if file cannot be opened
    // Assumption :- filepath points to a valid readable file, file permissions allow reading
    static std::vector<std::string> readFileLines(const std::string& filepath);
};

#endif 

