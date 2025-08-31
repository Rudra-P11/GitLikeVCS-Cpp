# Git-Like Version Control System in C++

## Introduction
Welcome to my Git-like VCS project built in C++. This project simulates core Git functionalities using linked lists and file handling, demonstrating advanced C++ concepts and system design. The goal is to create a fully functional version control system from scratch, teaching the internals of how Git works while applying C++ best practices.

---

## Project Overview
This VCS allows users to:
- Track file changes using hashing for efficient comparison
- Save snapshots (commits) with metadata
- View commit history through a linked list structure
- Checkout previous versions by restoring files
- Manage branches for parallel development
- Merge branches with basic conflict detection

The system uses a linked list for commits (similar to Git's DAG), file hashing for change detection, and persistent storage via file serialization.

---

## Development Timeline

| Day | Tasks & Thoughts |
|------|------------------|
| 1 | Planning: Decided on core features, chose linked lists for commits, file-based storage for simplicity. Set up project structure with separate headers/sources. |
| 2 | Defined Commit struct: Thought about what data to store (hash for uniqueness, message for description, timestamp for ordering, files for changes, parent for history). Added constructor/destructor for memory management. |
| 3 | Utility functions: Needed hashing for change detection, timestamps for commit ordering, file I/O for persistence. Chose std::hash for simplicity, std::time for timestamps. |
| 4 | Repository logic: Designed Repository class to manage all VCS operations. Thought about state persistence - decided to save HEAD, staging area, and commits to files. |
| 5 | CLI implementation: Created command parsing in main.cpp. Considered user experience - made commands intuitive like Git. |
| 6 | Enhanced features: Added staging with hashes, status/diff commands, branching. Thought about how to represent branches as pointers to commits. |
| 7 | Testing and debugging: Found Windows-specific issues with directory creation and timestamps. Added error handling and validation. |

---

## File Structure & Purpose

### Headers (`include/`)
- **commit.h**: Defines the Commit struct - the core data structure representing a snapshot of changes
- **repository.h**: Defines the Repository class - manages all VCS operations and state
- **utils.h**: Declares utility functions for common operations like hashing and file I/O

### Sources (`src/`)
- **main.cpp**: Entry point with command-line argument parsing and routing to Repository methods
- **repository.cpp**: Implements all Repository class methods - the heart of the VCS logic
- **utils.cpp**: Implements utility functions - handles low-level operations
- **commit.cpp**: Implements Commit struct methods - constructor, destructor, etc.

### Build System
- **CMakeLists.txt**: Configures compilation with C++17 standard, includes directories, and links necessary libraries

---

## Detailed Code Explanation

### Commit Structure (commit.h & commit.cpp)
The Commit struct represents a single snapshot in the version history:

```cpp
struct Commit {
    std::string hash;           // Unique identifier (SHA-like hash)
    std::string message;        // User-provided commit message
    std::string timestamp;      // When commit was created
    std::vector<std::string> files;  // List of files changed in this commit
    Commit* parent;             // Pointer to previous commit (nullptr for root)
};
```

**Why this design?**
- Linked list structure allows efficient traversal of history
- Hash ensures uniqueness and integrity checking
- Files vector tracks what changed (simplified from Git's tree objects)
- Parent pointer creates the commit graph

**Constructor**: Takes message, files, and optional parent. Generates hash and timestamp.
**Destructor**: Cleans up the linked list to prevent memory leaks.

### Repository Class (repository.h & repository.cpp)
The Repository class manages the entire VCS state and operations:

**Key Members:**
- `repoPath`: Path to .vcs directory
- `head`: Pointer to current commit
- `stagingArea`: Vector of StagedFile structs (path + hash)
- `currentBranch`: Name of active branch

**Core Methods:**

1. **init()**: Creates .vcs directory structure
   - Why? Provides isolated storage for VCS metadata
   - How: Uses _mkdir on Windows, creates subdirs for objects, branches, etc.
   - Returns: bool indicating success

2. **add(const std::string& file)**: Stages a file for commit
   - Why? Allows selective commits, like Git's staging area
   - How: Computes file hash, adds/updates in stagingArea vector
   - Returns: bool for success (file exists, etc.)

3. **commit(const std::string& message)**: Creates new commit from staged files
   - Why? Saves current state as permanent snapshot
   - How: Creates Commit object, links to head, saves to disk, clears staging
   - Returns: bool if staging area wasn't empty

4. **log()**: Displays commit history
   - Why? Allows users to see project evolution
   - How: Traverses linked list from head to root, prints details
   - Returns: void (prints to console)

5. **checkout(const std::string& hash)**: Restores files from specific commit
   - Why? Allows switching to different project states
   - How: Loads commit by hash, updates head pointer
   - Returns: bool if commit found

**Phase 2 Enhancements:**
- **status()**: Shows staged vs unstaged changes
  - Compares file hashes to detect modifications
  - Prints staged files and modified files not staged
- **diff()**: Shows staged changes
  - Displays file paths and their hashes
  - Placeholder for full line-by-line diff

**Phase 3 Enhancements:**
- **createBranch(const std::string& name)**: Creates new branch
  - Saves current HEAD hash to branches/name file
- **switchBranch(const std::string& name)**: Switches to branch
  - Loads branch's commit hash, updates head and currentBranch
- **listBranches()**: Shows all branches
  - Lists files in .vcs/branches/, marks current with *
- **mergeBranch(const std::string& name)**: Merges branch into current
  - Loads branch commit, updates head (basic implementation)
- **handleMergeConflicts()**: Basic conflict resolution
  - Adds conflict markers to files, prompts manual resolution

### Utility Functions (utils.h & utils.cpp)
Helper functions for common operations:

1. **hashString(const std::string& str)**: Generates hash using std::hash
   - Why? Efficient change detection without storing full file contents
   - How: Uses C++ standard library hash function
   - Returns: string representation of hash

2. **getCurrentTimestamp()**: Gets current time as string
   - Why? For commit ordering and history
   - How: Uses std::time and std::ctime with null checks
   - Returns: Formatted timestamp string

3. **readFile/writeFile**: File I/O operations
   - Why? Persistent storage for commits, staging, branches
   - How: Uses ifstream/ofstream with error handling
   - Returns: string content or bool success

4. **fileExists()**: Checks if file exists
   - Why? Validation before operations
   - How: Uses std::filesystem::exists
   - Returns: bool

### CLI Interface (main.cpp)
Command-line parsing and routing:

- Parses argv[] for command and arguments
- Creates Repository instance
- Routes to appropriate method based on command
- Handles different argument patterns (e.g., commit -m "message")

**Command Mapping:**
- init → repo.init()
- add <file> → repo.add(file)
- commit -m <msg> → repo.commit(msg)
- log → repo.log()
- checkout <hash> → repo.checkout(hash)
- status → repo.status()
- diff → repo.diff()
- branch [name] → repo.createBranch(name) or repo.listBranches()
- merge <branch> → repo.mergeBranch(branch)

### Hinglish Command Aliases

The VCS also supports Hinglish (Hindi-English mix) command aliases for better accessibility and user experience:

**Hinglish Command Mapping:**
- naya repo bana → repo.init() (Initialize repository)
- isko dekh <file> → repo.add(file) (Add file to staging)
- final kar -m <msg> → repo.commit(msg) (Commit changes)
- history dikha → repo.log() (View commit history)
- idhar chal <hash> → repo.checkout(hash) (Checkout commit)
- kya haal hai → repo.status() (View repository status)
- antar → repo.diff() (Show differences)
- naya rasta bana [name] → repo.createBranch(name) or repo.listBranches() (Create/list branches)
- jod de <branch> → repo.mergeBranch(branch) (Merge branch)

**Example Hinglish Workflow:**
```bash
# Initialize repository
./vcs naya repo bana

# Add files to staging
./vcs isko dekh file1.txt
./vcs isko dekh file2.txt

# Check status
./vcs kya haal hai

# Commit changes
./vcs final kar -m "Initial commit"

# View history
./vcs history dikha

# Create and switch branches
./vcs naya rasta bana feature-x
./vcs idhar chal feature-x

# Make changes and commit
./vcs isko dekh modified.txt
./vcs final kar -m "Added feature X"

# Merge back
./vcs idhar chal master
./vcs jod de feature-x
```

---

## Key Features Implemented

### Phase 1: Core System
- **Init**: Creates .vcs directory with objects/, branches/, refs/ subdirs
- **Add**: Stages files by computing hashes and storing in vector
- **Commit**: Creates Commit objects, generates unique hashes, saves to disk
- **Log**: Traverses commit linked list and displays history
- **Checkout**: Loads commit by hash and updates head

### Phase 2: Enhanced Staging Area
- **File Hash Tracking**: Uses SHA-like hashing for change detection
- **Status Command**: Shows staged files and modified unstaged files
- **Diff Command**: Displays staged changes with hashes
- **Improved Add**: Updates hashes for modified staged files

### Phase 3: Branching & Merging
- **Branch Creation**: Creates branch files pointing to current commit
- **Branch Switching**: Updates HEAD and currentBranch, loads branch commit
- **Branch Listing**: Displays all branches with current indicator
- **Branch Merging**: Basic merge by updating head to branch commit
- **Conflict Handling**: Adds merge markers to conflicting files

---

## Challenges & Solutions

| Problem | Solution | Why It Works |
|---------|----------|--------------|
| Timestamp empty on Windows | Added nullptr check for std::ctime | Prevents crashes on systems where ctime fails |
| Staging area not persistent | Save/load staging vector to/from file | Maintains state across program runs |
| Directory creation on Windows | Used _mkdir with <direct.h> include | Windows-specific API for directory creation |
| Hash uniqueness | Combined message, timestamp, files for hash input | Ensures different commits have different hashes |
| Memory leaks in linked list | Added destructors and careful pointer management | Prevents memory leaks in long-running programs |
| File path differences | Used relative paths, handled in code | Works across different operating systems |
| Branch persistence | Store branch names and hashes in files | Maintains branch state across sessions |
| Merge conflicts | Basic marker insertion, manual resolution | Provides foundation for advanced conflict resolution |

---

## Step-by-Step Development Guide

1. **Setup Project Structure**
   - Create vcs_project/ with src/, include/, build/ subdirs
   - Write CMakeLists.txt for compilation
   - Why: Organized structure prevents confusion, CMake enables easy building

2. **Define Core Data Structures**
   - Implement Commit struct with all necessary fields
   - Add constructor/destructor for proper memory management
   - Why: Commit is the fundamental unit, needs careful design

3. **Create Repository Class Skeleton**
   - Define all public methods in header
   - Implement basic structure in source file
   - Why: Repository manages all operations, needs clear interface

4. **Implement Utility Functions**
   - Start with hashing and file I/O
   - Add timestamp and path handling
   - Why: These are used everywhere, implement once

5. **Build CLI Interface**
   - Parse command-line arguments
   - Route to Repository methods
   - Why: Users need easy interaction with the system

6. **Implement Core VCS Operations**
   - Start with init (easiest)
   - Add file staging and committing
   - Why: These are the most fundamental features

7. **Add History and Navigation**
   - Implement log for history viewing
   - Add checkout for version switching
   - Why: Users need to see and navigate project history

8. **Enhance with Advanced Features**
   - Add status and diff commands
   - Implement branching and merging
   - Why: Makes the VCS more powerful and Git-like

9. **Test Thoroughly**
   - Test each command individually
   - Test combinations and edge cases
   - Why: Ensures reliability and catches bugs

10. **Document and Polish**
    - Write comprehensive documentation
    - Add error handling and user-friendly messages
    - Why: Makes the project complete and professional

---

## Usage Guide

### Basic Workflow
```bash
# Initialize repository
./vcs init

# Add files to staging
./vcs add file1.txt
./vcs add file2.txt

# Check status
./vcs status

# Commit changes
./vcs commit -m "Initial commit"

# View history
./vcs log

# Create and switch branches
./vcs branch feature-x
./vcs checkout feature-x

# Make changes and commit
./vcs add modified.txt
./vcs commit -m "Added feature X"

# Merge back
./vcs checkout master
./vcs merge feature-x
```

### Advanced Usage
```bash
# See staged changes
./vcs diff

# List all branches
./vcs branch

# Checkout specific commit
./vcs checkout abc123def456
```

---

## Creative Use Cases

- **Personal Note Versioning**: Track changes to personal documents or notes
- **Configuration File Tracking**: Version control system configurations
- **Collaborative Writing**: Track changes in shared documents
- **Code Experiments**: Save different versions of experimental code
- **Backup System**: Automatic snapshots of important files
- **Learning Tool**: Understand how version control works internally
- **Portfolio Showcase**: Demonstrate C++ and systems programming skills

---

## Phase 2: Enhanced Staging Area - Detailed Implementation

### New Features Implemented
- **File Hash Tracking**: Each staged file now stores its content hash for efficient change detection
- **Status Command**: Provides clear view of repository state - what files are staged, what files are modified but not staged
- **Diff Command**: Shows which files have been staged for commit with their hashes
- **Improved Add**: If a file is already staged and modified, updates its hash instead of duplicating

### Technical Enhancements
- **StagedFile Struct**: Added to repository.h
  ```cpp
  struct StagedFile {
      std::string path;  // File path relative to repo root
      std::string hash;  // SHA-like hash of file content
  };
  ```
  Why? Separates file path from content hash for efficient comparison

- **Enhanced Repository Constructor**: Now loads staging area with hashes from .vcs/staging file
  - Reads file line by line, parses "path:hash" format
  - Reconstructs stagingArea vector on startup
  - Why? Maintains staging state across program runs

- **Improved add() Method**:
  - Computes file hash using hashString(readFile(file))
  - Checks if file already staged, updates hash if so
  - Saves updated staging area to disk
  - Why? Prevents duplicate staging, enables change detection

- **status() Method**:
  - Iterates stagingArea, prints staged files with hashes
  - For each file in last commit, checks if current content differs
  - Prints modified files not staged
  - Why? Gives users clear view of repository state

- **diff() Method**:
  - Iterates stagingArea, shows file paths and hashes
  - Placeholder for full diff (line-by-line changes)
  - Why? Shows what will be committed

### Usage Examples
```bash
# After modifying a staged file
./vcs status
# Output:
# Staged files:
#   file.txt (hash: abc123...)
#
# Modified files (not staged):
#   (none)

# After modifying an unstaged file
./vcs status
# Output:
# Staged files:
#   file.txt (hash: abc123...)
#
# Modified files (not staged):
#   other.txt (modified)
```

## Phase 3: Branching & Merging - Detailed Implementation

### New Features Implemented
- **Branch Creation**: Create named branches pointing to current commit
- **Branch Switching**: Change active branch, updating working directory
- **Branch Listing**: Show all branches with current branch indicator
- **Branch Merging**: Basic merge implementation with conflict detection
- **Conflict Handling**: Basic merge conflict resolution framework

### Technical Enhancements
- **currentBranch Member**: Added to Repository class
  - Tracks active branch name
  - Loaded from .vcs/CURRENT_BRANCH file
  - Why? Enables branch-aware operations

- **Branch Storage**: Uses .vcs/branches/ directory
  - Each branch is a file containing commit hash
  - master branch created automatically in init()
  - Why? Simple file-based storage, easy to implement

- **createBranch() Method**:
  - Checks if branch already exists
  - Saves current HEAD hash to branches/branchName file
  - Why? Creates branch pointing to current state

- **switchBranch() Method**:
  - Validates branch exists
  - Loads branch's commit hash
  - Updates head pointer and currentBranch
  - Saves new state to files
  - Why? Changes active branch and working state

- **listBranches() Method**:
  - Lists all files in branches/ directory
  - Marks current branch with *
  - Why? Shows available branches clearly

- **mergeBranch() Method**:
  - Loads target branch's commit
  - Updates head to that commit (basic fast-forward)
  - Updates current branch file
  - Why? Brings changes from other branch into current

- **handleMergeConflicts() Method**:
  - Takes vector of conflicting files
  - For each conflict, reads file and adds markers:
    ```
    <<<<<<< HEAD
    current content
    =======
    incoming content
    >>>>>>> branch
    ```
  - Prompts user to resolve manually
  - Why? Provides foundation for conflict resolution

### Usage Examples
```bash
# Create and switch to new branch
./vcs branch feature-login
./vcs checkout feature-login

# Make changes and commit
./vcs add login.cpp
./vcs commit -m "Add login feature"

# Switch back and merge
./vcs checkout master
./vcs merge feature-login
```

### Branch File Structure
```
.vcs/branches/
├── master (contains: commit-hash-of-last-commit)
└── feature-x (contains: commit-hash-when-branch-created)
```

---

## Next Steps: Phase 4 and Beyond

### CLI Improvements
- Add help command with detailed usage
- Implement command aliases (e.g., 'co' for checkout)
- Add colored output for better readability
- Implement interactive mode for complex operations

### Advanced Features
- **Remote Simulation**: Add push/pull commands using local directories
- **Advanced Merging**: Implement 3-way merge algorithm
- **Conflict Resolution**: Interactive conflict resolver
- **Stashing**: Save/restore uncommitted changes
- **Tagging**: Create named references to commits

### Performance Enhancements
- Implement commit graph visualization
- Add garbage collection for unreachable commits
- Optimize file storage with compression
- Add indexing for faster log operations

### Testing & Quality
- Expand unit test coverage
- Add integration tests for complex workflows
- Implement continuous integration
- Add performance benchmarking

---

## Conclusion

This Git-like VCS project demonstrates comprehensive C++ programming skills including:
- Data structure design (linked lists, vectors, maps)
- Memory management (pointers, destructors, RAII)
- File I/O and serialization
- Error handling and validation
- Command-line interface design
- System-level programming (directory operations)
- Algorithm implementation (hashing, change detection)

The project provides a solid foundation for understanding version control internals while serving as a portfolio piece showcasing advanced C++ development capabilities. Each phase builds upon the previous, demonstrating iterative development and feature enhancement.

---
