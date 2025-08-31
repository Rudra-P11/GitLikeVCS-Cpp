#include "commit.h"

Commit::Commit(const std::string& msg, const std::vector<std::string>& fls, Commit* prnt)
    : hash(""), message(msg), timestamp(""), files(fls), parent(prnt) {}

Commit::~Commit() {
    // No dynamic memory to clean up
}
