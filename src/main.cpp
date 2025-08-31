#include "repository.h"
#include <iostream>
#include <string>

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cout << "Usage: vcs <command> [options]" << std::endl;
        std::cout << "English Commands: init, add <file>, commit -m <message>, log, checkout <hash>, status, diff, branch [name], merge <branch>" << std::endl;
        std::cout << "Hinglish Commands: naya repo bana, isko dekh <file>, final kar -m <message>, history dikha, idhar chal <hash>, kya haal hai, antar, naya rasta bana [name], jod de <branch>" << std::endl;
        return 1;
    }

    Repository repo;
    std::string command = argv[1];
    std::string fullCommand = command;
    if (argc >= 3) fullCommand += " " + std::string(argv[2]);
    if (argc >= 4) fullCommand += " " + std::string(argv[3]);

    if (command == "init" || fullCommand == "naya repo bana") {
        if (repo.init()) {
            std::cout << "Repository initialized." << std::endl;
        } else {
            std::cout << "Failed to initialize repository." << std::endl;
        }
    } else if (command == "add" || (fullCommand.find("isko dekh") == 0)) {
        std::string file;
        if (command == "add" && argc > 2) {
            file = argv[2];
        } else if (fullCommand.find("isko dekh") == 0 && argc > 3) {
            file = argv[3];
        } else {
            std::cout << "Usage: vcs add <file> or vcs isko dekh <file>" << std::endl;
            return 1;
        }
        if (repo.add(file)) {
            std::cout << "Added " << file << " to staging area." << std::endl;
        } else {
            std::cout << "Failed to add " << file << "." << std::endl;
        }
    } else if (command == "commit" || (fullCommand.find("final kar") == 0)) {
        std::string message;
        if (command == "commit" && argc > 3 && std::string(argv[2]) == "-m") {
            message = argv[3];
        } else if (fullCommand.find("final kar") == 0 && argc > 4 && std::string(argv[3]) == "-m") {
            message = argv[4];
        } else {
            std::cout << "Usage: vcs commit -m <message> or vcs final kar -m <message>" << std::endl;
            return 1;
        }
        if (repo.commit(message)) {
            std::cout << "Committed changes." << std::endl;
        } else {
            std::cout << "Failed to commit." << std::endl;
        }
    } else if (command == "log" || fullCommand == "history dikha") {
        repo.log();
    } else if ((command == "checkout" || command == "idhar chal") && argc > 2) {
        std::string hash = argv[2];
        if (repo.checkout(hash)) {
            std::cout << "Checked out to " << hash << "." << std::endl;
        } else {
            std::cout << "Failed to checkout." << std::endl;
        }
    } else if (command == "status" || fullCommand == "kya haal hai") {
        repo.status();
    } else if (command == "diff" || command == "antar") {
        repo.diff();
    } else if (command == "branch" || (fullCommand.find("naya rasta bana") == 0)) {
        if (argc == 2 || (fullCommand.find("naya rasta bana") == 0 && argc == 3)) {
            repo.listBranches();
        } else if (argc == 3 || (fullCommand.find("naya rasta bana") == 0 && argc == 4)) {
            std::string branchName = (argc == 3) ? argv[2] : argv[3];
            if (repo.createBranch(branchName)) {
                std::cout << "Branch created." << std::endl;
            } else {
                std::cout << "Failed to create branch." << std::endl;
            }
        } else {
            std::cout << "Usage: vcs branch [name] or vcs naya rasta bana [name]" << std::endl;
        }
    } else if ((command == "merge" || fullCommand == "jod de") && argc > 2) {
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
