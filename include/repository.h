#ifndef REPOSITORY_H
#define REPOSITORY_H

#include "commit.h"
#include <string>
#include <vector>
#include <unordered_map>

struct StagedFile {
    std::string path;
    std::string hash;
};

class Repository {
private:
    std::string repoPath;  // Path to .vcs directory
    Commit* head;  // Current head commit
    std::vector<StagedFile> stagingArea;  // Files staged for commit with hashes
    std::string currentBranch;  // Current branch name

public:
    Repository(const std::string& path = ".");
    ~Repository();

    // Initialize repository
    bool init();

    // Add file to staging area
    bool add(const std::string& file);

    // Commit staged changes
    bool commit(const std::string& message);

    // Display commit log
    void log();

    // Checkout to a specific commit
    bool checkout(const std::string& commitHash);

    // Show status of working directory
    void status();

    // Show diff of staged changes
    void diff();

    // Branch management
    bool createBranch(const std::string& branchName);
    bool switchBranch(const std::string& branchName);
    void listBranches();
    bool mergeBranch(const std::string& branchName);

    // Handle merge conflicts (basic implementation)
    void handleMergeConflicts(const std::vector<std::string>& conflicts);

private:
    // Helper methods
    std::string generateCommitHash(const Commit* commit);
    void saveCommit(const Commit* commit);
    Commit* loadCommit(const std::string& hash);
};

#endif // REPOSITORY_H
