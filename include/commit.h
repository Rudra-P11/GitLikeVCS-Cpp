#ifndef COMMIT_H
#define COMMIT_H

#include <string>
#include <vector>
#include <ctime>

struct Commit {
    std::string hash;
    std::string message;
    std::string timestamp;
    std::vector<std::string> files;  // List of file paths changed in this commit
    Commit* parent;  // Pointer to parent commit (nullptr for root)

    // Constructor
    Commit(const std::string& msg, const std::vector<std::string>& fls, Commit* prnt = nullptr);

    // Destructor to handle linked list cleanup
    ~Commit();
};

#endif // COMMIT_H
