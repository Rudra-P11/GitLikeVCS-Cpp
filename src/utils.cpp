#include "utils.h"
#include <fstream>
#include <sstream>
#include <ctime>
#include <functional>

std::string hashString(const std::string& str) {
    std::hash<std::string> hasher;
    return std::to_string(hasher(str));
}

std::string getCurrentTimestamp() {
    std::time_t now = std::time(nullptr);
    char* time_str = std::ctime(&now);
    if (time_str) {
        std::string ts = time_str;
        // Remove trailing newline
        if (!ts.empty() && ts.back() == '\n') ts.pop_back();
        return ts;
    } else {
        return "Unknown time";
    }
}

std::string readFile(const std::string& path) {
    std::ifstream file(path, std::ios::binary);
    if (!file) return "";
    std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    return content;
}

bool writeFile(const std::string& path, const std::string& content) {
    std::ofstream file(path, std::ios::binary);
    if (!file) return false;
    file << content;
    return true;
}

bool fileExists(const std::string& path) {
    std::ifstream file(path);
    return file.good();
}

std::vector<std::string> getFilesInDirectory(const std::string& dir) {
    // Placeholder: not implemented without filesystem
    return {};
}
