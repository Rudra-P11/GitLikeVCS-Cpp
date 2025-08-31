#include "repository.h"
#include <iostream>
#include <string>

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cout << "Usage: vcs <command> [options]" << std::endl;
        std::cout << "Commands: init, add <file>, commit -m <message>, log, checkout <hash>, status, diff, branch [name], merge <branch>" << std::endl;
        return 1;
    }

    Repository repo;
    std::string command = argv[1];

    if (command == "init") {
        if (repo.init()) {
            std::cout << "Repository initialized." << std::endl;
        } else {
            std::cout << "Failed to initialize repository." << std::endl;
        }
    } else if (command == "add" && argc > 2) {
        std::string file = argv[2];
        if (repo.add(file)) {
            std::cout << "Added " << file << " to staging area." << std::endl;
        } else {
            std::cout << "Failed to add " << file << "." << std::endl;
        }
    } else if (command == "commit" && argc > 3 && std::string(argv[2]) == "-m") {
        std::string message = argv[3];
        if (repo.commit(message)) {
            std::cout << "Committed changes." << std::endl;
        } else {
            std::cout << "Failed to commit." << std::endl;
        }
    } else if (command == "log") {
        repo.log();
    } else if (command == "checkout" && argc > 2) {
        std::string hash = argv[2];
        if (repo.checkout(hash)) {
            std::cout << "Checked out to " << hash << "." << std::endl;
        } else {
            std::cout << "Failed to checkout." << std::endl;
        }
    } else if (command == "status") {
        repo.status();
    } else if (command == "diff") {
        repo.diff();
    } else if (command == "branch") {
        if (argc == 2) {
            repo.listBranches();
        } else if (argc == 3) {
            std::string branchName = argv[2];
            if (repo.createBranch(branchName)) {
                std::cout << "Branch created." << std::endl;
            } else {
                std::cout << "Failed to create branch." << std::endl;
            }
        } else {
            std::cout << "Usage: vcs branch [name]" << std::endl;
        }
    } else if (command == "merge" && argc > 2) {
        std::string branchName = argv[2];
        if (repo.mergeBranch(branchName)) {
            std::cout << "Merge completed." << std::endl;
        } else {
            std::cout << "Failed to merge." << std::endl;
        }
    } else {
        std::cout << "Unknown command or invalid arguments." << std::endl;
    }

    return 0;
}
