#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <vector>




// Hash a string using std::hash
std::string hashString(const std::string& str);

// Get current timestamp as string
std::string getCurrentTimestamp();

// Read file content as string
std::string readFile(const std::string& path);

// Write string to file
bool writeFile(const std::string& path, const std::string& content);

// Check if file exists
bool fileExists(const std::string& path);

// Get list of files in directory (recursive)
std::vector<std::string> getFilesInDirectory(const std::string& dir);

#endif // UTILS_H
