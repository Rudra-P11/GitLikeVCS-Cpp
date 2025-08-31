# GitLikeVCS-Cpp

A complete implementation of a Git-like version control system written in C++. This project demonstrates advanced C++ concepts including data structures, file handling, and system design while building a fully functional VCS from scratch.

## Features

### Core Functionality
- **Repository Management**: Initialize and manage version control repositories
- **File Tracking**: Add files to staging area and track changes
- **Commits**: Create commits with messages and timestamps
- **Commit History**: View detailed commit logs with full history
- **Branching**: Create, switch, and manage branches
- **Merging**: Merge branches with conflict detection and resolution
- **Checkout**: Switch between commits and branches

### Advanced Features
- **Status Tracking**: View current repository status and changes
- **Diff Display**: Show differences between files and commits
- **File System Integration**: Robust file handling with error checking
- **Data Persistence**: Serialize and store commit data efficiently

## Prerequisites

- C++17 compatible compiler (GCC 7+, Clang 5+, MSVC 2017+)
- CMake 3.16 or higher
- Windows/Linux/macOS

## Installation

1. Clone the repository:
```bash
git clone https://github.com/Rudra-P11/GitLikeVCS-Cpp.git
cd GitLikeVCS-Cpp
```

2. Create build directory:
```bash
mkdir build
cd build
```

3. Configure and build:
```bash
cmake ..
make
```

## Usage

### Basic Commands

Initialize a new repository:
```bash
./vcs init
```

Add files to staging:
```bash
./vcs add filename.txt
./vcs add .  # Add all files
```

Commit changes:
```bash
./vcs commit -m "Your commit message"
```

View commit history:
```bash
./vcs log
```

Create and switch branches:
```bash
./vcs branch new-feature
./vcs checkout new-feature
```

Merge branches:
```bash
./vcs merge main
```

View repository status:
```bash
./vcs status
```

Show differences:
```bash
./vcs diff
```

### Hinglish Command Aliases

The VCS also supports Hinglish (Hindi-English mix) command aliases for better accessibility:

Initialize a new repository:
```bash
./vcs naya repo bana
```

Add files to staging:
```bash
./vcs isko dekh filename.txt
```

Commit changes:
```bash
./vcs final kar -m "Your commit message"
```

View commit history:
```bash
./vcs history dikha
```

Create and switch branches:
```bash
./vcs naya rasta bana new-feature
./vcs idhar chal new-feature
```

Merge branches:
```bash
./vcs jod de main
```

View repository status:
```bash
./vcs kya haal hai
```

Show differences:
```bash
./vcs antar
```

### Example Workflow

```bash
# Initialize repository
./vcs init

# Create a file and add it
echo "Hello World" > hello.txt
./vcs add hello.txt
./vcs commit -m "Add hello.txt"

# Create a branch
./vcs branch feature
./vcs checkout feature

# Make changes and commit
echo "Updated content" > hello.txt
./vcs add hello.txt
./vcs commit -m "Update hello.txt"

# Merge back to main
./vcs checkout master
./vcs merge feature
```

## Project Structure

```
vcs_project/
├── CMakeLists.txt          # Build configuration
├── include/                # Header files
│   ├── commit.h           # Commit data structure
│   ├── repository.h       # Repository management
│   └── utils.h            # Utility functions
├── src/                   # Source files
│   ├── main.cpp          # CLI entry point
│   ├── repository.cpp    # Repository implementation
│   ├── commit.cpp        # Commit implementation
│   └── utils.cpp         # Utility implementations
├── build/                 # Build directory (generated)
└── README.md             # This file
```

## Architecture

The system uses a linked list structure to maintain commit history, with each commit containing:
- Unique hash identifier
- Commit message and timestamp
- List of modified files
- Pointer to parent commit

File changes are tracked using hashing for efficient comparison, and all data is serialized for persistence.

## Contributing

1. Fork the repository
2. Create a feature branch (`git checkout -b feature/amazing-feature`)
3. Commit your changes (`git commit -m 'Add amazing feature'`)
4. Push to the branch (`git push origin feature/amazing-feature`)
5. Open a Pull Request

## License

This project is licensed under the MIT License - see the LICENSE file for details.

## Acknowledgments

- Inspired by the Git version control system
- Built as a learning project for advanced C++ concepts
- Demonstrates practical application of data structures and algorithms

## Contact

Rudra-P11 - GitHub: https://github.com/Rudra-P11

Project Link: https://github.com/Rudra-P11/GitLikeVCS-Cpp
