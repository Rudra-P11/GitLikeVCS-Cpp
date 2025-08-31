#include "repository.h"
#include "utils.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <direct.h>

Repository::Repository(const std::string& path) : repoPath(path + "/.vcs"), head(nullptr), currentBranch("master") {
    // Load staging area with hashes
    std::string stagingContent = readFile(repoPath + "/staging");
    std::istringstream iss(stagingContent);
    std::string line;
    while (std::getline(iss, line)) {
        if (!line.empty()) {
            size_t colonPos = line.find(':');
            if (colonPos != std::string::npos) {
                std::string filePath = line.substr(0, colonPos);
                std::string fileHash = line.substr(colonPos + 1);
                stagingArea.push_back({filePath, fileHash});
            }
        }
    }
    // Load head
    std::string headHash = readFile(repoPath + "/HEAD");
    if (!headHash.empty()) {
        head = loadCommit(headHash);
    }
    // Load current branch
    std::string branchContent = readFile(repoPath + "/CURRENT_BRANCH");
    if (!branchContent.empty()) {
        currentBranch = branchContent;
    }
}

Repository::~Repository() {
    // Cleanup commits (simple: just delete head chain)
    Commit* current = head;
    while (current) {
        Commit* next = current->parent;
        delete current;
        current = next;
    }
}

bool Repository::init() {
    _mkdir(repoPath.c_str());
    _mkdir((repoPath + "/objects").c_str());
    _mkdir((repoPath + "/refs").c_str());
    _mkdir((repoPath + "/branches").c_str());
    // Save initial head as null
    writeFile(repoPath + "/HEAD", "");
    writeFile(repoPath + "/staging", "");
    // Create master branch
    writeFile(repoPath + "/branches/master", "");
    writeFile(repoPath + "/CURRENT_BRANCH", "master");
    return true;
}

bool Repository::add(const std::string& file) {
    if (!fileExists(file)) {
        std::cerr << "File does not exist: " << file << std::endl;
        return false;
    }
    // Compute file hash
    std::string fileContent = readFile(file);
    std::string fileHash = hashString(fileContent);
    // Check if already staged
    for (auto& staged : stagingArea) {
        if (staged.path == file) {
            staged.hash = fileHash;  // Update hash if modified
            return true;
        }
    }
    stagingArea.push_back({file, fileHash});
    // Save staging area with hashes
    std::string content;
    for (const auto& f : stagingArea) {
        content += f.path + ":" + f.hash + "\n";
    }
    writeFile(repoPath + "/staging", content);
    return true;
}

bool Repository::commit(const std::string& message) {
    if (stagingArea.empty()) {
        std::cerr << "No files staged for commit." << std::endl;
        return false;
    }
    // Extract file paths from staging area
    std::vector<std::string> files;
    for (const auto& staged : stagingArea) {
        files.push_back(staged.path);
    }
    Commit* newCommit = new Commit(message, files, head);
    newCommit->hash = generateCommitHash(newCommit);
    newCommit->timestamp = getCurrentTimestamp();
    head = newCommit;
    saveCommit(newCommit);
    stagingArea.clear();
    writeFile(repoPath + "/HEAD", newCommit->hash);
    writeFile(repoPath + "/staging", "");
    // Update current branch
    writeFile(repoPath + "/branches/" + currentBranch, newCommit->hash);
    return true;
}

void Repository::log() {
    Commit* current = head;
    while (current) {
        std::cout << "Commit: " << current->hash << std::endl;
        std::cout << "Message: " << current->message << std::endl;
        std::cout << "Timestamp: " << current->timestamp << std::endl;
        std::cout << "Files: ";
        for (const auto& file : current->files) {
            std::cout << file << " ";
        }
        std::cout << std::endl << std::endl;
        current = current->parent;
    }
}

bool Repository::checkout(const std::string& commitHash) {
    // For simplicity, assume hash is valid and restore files from commit
    // In real Git, this would restore working directory
    Commit* commit = loadCommit(commitHash);
    if (!commit) {
        std::cerr << "Commit not found: " << commitHash << std::endl;
        return false;
    }
    // Restore files (placeholder: just print)
    for (const auto& file : commit->files) {
        std::cout << "Restoring " << file << std::endl;
        // In real impl, copy from .vcs/objects
    }
    head = commit;
    writeFile(repoPath + "/HEAD", commitHash);
    return true;
}

std::string Repository::generateCommitHash(const Commit* commit) {
    std::string data = commit->message + commit->timestamp;
    for (const auto& file : commit->files) {
        data += file;
    }
    return hashString(data);
}

void Repository::saveCommit(const Commit* commit) {
    std::string path = repoPath + "/objects/" + commit->hash;
    std::ostringstream oss;
    oss << commit->hash << "\n";
    oss << commit->message << "\n";
    oss << commit->timestamp << "\n";
    for (const auto& file : commit->files) {
        oss << file << "\n";
    }
    if (commit->parent) {
        oss << commit->parent->hash << "\n";
    } else {
        oss << "\n";
    }
    writeFile(path, oss.str());
}

Commit* Repository::loadCommit(const std::string& hash) {
    std::string path = repoPath + "/objects/" + hash;
    if (!fileExists(path)) return nullptr;
    std::string content = readFile(path);
    std::istringstream iss(content);
    std::string line;
    std::getline(iss, line); // hash
    std::getline(iss, line); // message
    std::string message = line;
    std::getline(iss, line); // timestamp
    std::string timestamp = line;
    std::vector<std::string> files;
    while (std::getline(iss, line) && !line.empty()) {
        files.push_back(line);
    }
    std::string parentHash;
    if (std::getline(iss, parentHash)) {
        Commit* parent = loadCommit(parentHash);
        return new Commit(message, files, parent);
    }
    return new Commit(message, files);
}

void Repository::status() {
    std::cout << "Staged files:" << std::endl;
    if (stagingArea.empty()) {
        std::cout << "  (none)" << std::endl;
    } else {
        for (const auto& staged : stagingArea) {
            std::cout << "  " << staged.path << " (" << staged.hash << ")" << std::endl;
        }
    }
    std::cout << std::endl;

    // Check for modified files (simple check: compare with head commit)
    std::cout << "Modified files (not staged):" << std::endl;
    if (head) {
        for (const auto& file : head->files) {
            if (fileExists(file)) {
                std::string currentContent = readFile(file);
                std::string currentHash = hashString(currentContent);
                bool isStaged = false;
                for (const auto& staged : stagingArea) {
                    if (staged.path == file) {
                        isStaged = true;
                        break;
                    }
                }
                if (!isStaged) {
                    // In real Git, compare with committed version
                    std::cout << "  " << file << " (modified)" << std::endl;
                }
            }
        }
    } else {
        std::cout << "  (none - no commits yet)" << std::endl;
    }
}

void Repository::diff() {
    std::cout << "Staged changes:" << std::endl;
    if (stagingArea.empty()) {
        std::cout << "  (no staged changes)" << std::endl;
    } else {
        for (const auto& staged : stagingArea) {
            std::cout << "  " << staged.path << ":" << std::endl;
            std::cout << "    Hash: " << staged.hash << std::endl;
            // In real diff, show line-by-line changes
            std::cout << "    (full diff not implemented - showing hash only)" << std::endl;
        }
    }
}

bool Repository::createBranch(const std::string& branchName) {
    if (fileExists(repoPath + "/branches/" + branchName)) {
        std::cerr << "Branch '" << branchName << "' already exists." << std::endl;
        return false;
    }
    std::string headHash = readFile(repoPath + "/HEAD");
    writeFile(repoPath + "/branches/" + branchName, headHash);
    std::cout << "Created branch '" << branchName << "'." << std::endl;
    return true;
}

bool Repository::switchBranch(const std::string& branchName) {
    if (!fileExists(repoPath + "/branches/" + branchName)) {
        std::cerr << "Branch '" << branchName << "' does not exist." << std::endl;
        return false;
    }
    std::string branchHash = readFile(repoPath + "/branches/" + branchName);
    if (!branchHash.empty()) {
        head = loadCommit(branchHash);
        writeFile(repoPath + "/HEAD", branchHash);
    } else {
        head = nullptr;
        writeFile(repoPath + "/HEAD", "");
    }
    currentBranch = branchName;
    writeFile(repoPath + "/CURRENT_BRANCH", branchName);
    std::cout << "Switched to branch '" << branchName << "'." << std::endl;
    return true;
}

void Repository::listBranches() {
    std::cout << "Branches:" << std::endl;
    // List all branch files
    // For simplicity, assume we have master and check for others
    if (fileExists(repoPath + "/branches/master")) {
        std::cout << (currentBranch == "master" ? "* " : "  ") << "master" << std::endl;
    }
    // In real impl, iterate through branches directory
    // For now, just master
}

bool Repository::mergeBranch(const std::string& branchName) {
    if (!fileExists(repoPath + "/branches/" + branchName)) {
        std::cerr << "Branch '" << branchName << "' does not exist." << std::endl;
        return false;
    }
    std::string branchHash = readFile(repoPath + "/branches/" + branchName);
    if (branchHash.empty()) {
        std::cout << "Nothing to merge from '" << branchName << "'." << std::endl;
        return true;
    }
    // Simple merge: just switch head to branch commit
    // In real Git, this would be more complex
    head = loadCommit(branchHash);
    writeFile(repoPath + "/HEAD", branchHash);
    writeFile(repoPath + "/branches/" + currentBranch, branchHash);
    std::cout << "Merged '" << branchName << "' into '" << currentBranch << "'." << std::endl;
    return true;
}

void Repository::handleMergeConflicts(const std::vector<std::string>& conflicts) {
    if (conflicts.empty()) {
        std::cout << "No merge conflicts." << std::endl;
        return;
    }
    std::cout << "Merge conflicts detected in the following files:" << std::endl;
    for (const auto& file : conflicts) {
        std::cout << "  " << file << std::endl;
        // Basic handling: append conflict markers to file
        std::string content = readFile(file);
        content = "<<<<<<< HEAD\n" + content + "\n=======\n" + content + "\n>>>>>>> " + currentBranch + "\n";
        writeFile(file, content);
    }
    std::cout << "Please resolve conflicts manually and commit." << std::endl;
}
