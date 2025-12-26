# COMEDK Admission Automation System

A comprehensive C-based student admission management system for COMEDK (Consortium of Medical, Engineering and Dental Colleges of Karnataka) that automates seat allocation based on student ranks, preferences, and college availability.

## 📋 Overview

This system implements an advanced data structure-based approach to manage student admissions, featuring automated seat allocation, student verification, preference management, and real-time seat tracking across multiple colleges and branches.

## ✨ Key Features

### 🎓 Student Management
- **Registration & Verification**: Secure student registration with multi-field validation
  - Registration number format validation (DCXXX)
  - Aadhar number verification (XXXX-XXXX-XXXX format)
  - Date of birth validation (DD-MM-YYYY)
  - Duplicate detection for registration numbers, Aadhar, and ranks
  
- **Priority-Based Queue**: Students are automatically sorted by rank for fair allocation
- **Preference System**: Students can select up to 8 college-branch combinations
- **Flexible Preference Count**: Configurable maximum preferences (1-8) via command-line argument

### 🏛️ College Management
- **Multi-College Support**: Currently supports 4 premier institutions:
  1. PES University
  2. RV College of Engineering
  3. BMS College of Engineering
  4. MS Ramaiah Institute of Technology

- **Branch Support**: Computer Science Engineering (CSE) and Electronics & Communication Engineering (ECE)
- **Dynamic Seat Distribution**: Automatic seat allocation based on total student count
- **Real-time Seat Tracking**: Live updates of available seats after each allocation

### 🔧 Advanced Features
- **Smart Allocation Algorithm**: Rank-based seat allocation respecting student preferences
- **Preference Updates**: Modify student preferences and re-run allocation
- **Memory Management**: Comprehensive cleanup functions to prevent memory leaks
- **Operation Logging**: Stack-based operation history for audit trails
- **BST Rank Tracking**: Binary Search Tree for efficient rank-based queries
- **College Network Graph**: Graph-based preference pattern analysis

## 🏗️ System Architecture

### Data Structures Used

1. **Queue (Priority Queue)**: Student management sorted by rank
2. **Binary Search Tree (BST)**: Fast rank lookup and validation
3. **Stack**: Operation history and logging
4. **Graph**: College preference network analysis
5. **Hash-based Verification**: Student credential verification

### File Structure

```
src/
├── enhanced_main.c        # Main program logic and menu system
├── enhanced_comedk.c      # Core allocation and processing functions
├── enhanced_ds.h          # Data structure definitions and declarations
└── verification.c         # Student verification and validation logic
```

## 🚀 Getting Started

### Prerequisites

- GCC compiler (or any C compiler with C99 support)
- Windows, Linux, or macOS
- Terminal with ANSI color support (for colored output)

### Compilation

```bash
gcc -o admission src/enhanced_main.c src/enhanced_comedk.c src/verification.c -I src
```

### Running the Program

The program requires two command-line arguments:

```bash
./admission <total_students> <max_preferences>
```

**Parameters:**
- `total_students`: Total number of students to accommodate (positive integer)
- `max_preferences`: Maximum preferences per student (1-8)

**Example:**

```bash
./admission 100 5
```

This will:
- Set total capacity to 100 students
- Allow each student to select up to 5 preferences
- Distribute seats proportionally across 4 colleges (25 per college, split between CSE/ECE)

## 📖 Usage Guide

### Main Menu Options

```
1. Add New Student        - Register a new student
2. Display All Students   - View all registered students and allocations
3. Display Colleges       - Show college information and available seats
4. Process Allocation     - Run the seat allocation algorithm
5. Update Preferences     - Modify student preferences
6. Exit                   - Clean up and exit the system
```

### Student Registration Flow

1. **Registration Number**: Enter in format DCXXX (e.g., DC101)
2. **Name**: Enter student's full name
3. **Rank**: Enter unique rank (lower is better)
4. **Date of Birth**: Format DD-MM-YYYY (e.g., 15-06-2006)
5. **Aadhar Number**: Format XXXX-XXXX-XXXX (e.g., 1234-5678-9012)
6. **Preferences**: Select up to MAX_PREFERENCES college-branch combinations

### Preference Selection

Students select from 8 available options:

```
1. PES University - CSE
2. PES University - ECE
3. RV College of Engineering - CSE
4. RV College of Engineering - ECE
5. BMS College of Engineering - CSE
6. BMS College of Engineering - ECE
7. MS Ramaiah Institute of Technology - CSE
8. MS Ramaiah Institute of Technology - ECE
```

## 🔐 Validation Rules

### Registration Number
- Format: DCXXX (DC followed by 3 digits)
- Must be unique across all students
- Example: DC101, DC250

### Date of Birth
- Format: DD-MM-YYYY
- Must be a valid calendar date
- Must be in the past (before today)
- Handles leap years correctly

### Aadhar Number
- Format: XXXX-XXXX-XXXX
- Must contain exactly 12 digits
- Hyphens required at correct positions
- Must be unique across all students

### Rank
- Must be a positive integer
- Must be unique (no two students can have same rank)
- Determines allocation priority (lower rank = higher priority)

## 🎯 Allocation Algorithm

The seat allocation follows these rules:

1. **Pre-registered Students**: System includes 5 verified students (DC101-DC105) by default
2. **Rank Order**: Students are processed in ascending order of rank
3. **Preference Priority**: For each student, preferences are checked in order
4. **First Available**: Student is allocated to first available preference with seats
5. **No Allocation**: If no preferences have seats, student is marked "Not Allocated"
6. **Re-allocation**: Running allocation again resets all assignments and reallocates from scratch

## 💻 Code Examples

### Example Session

```bash
$ ./admission 100 5

MAX_PREFERENCES set to: 5

Initial Seat Distribution:
PES University: CSE=25, ECE=25
RV College of Engineering: CSE=25, ECE=25
BMS College of Engineering: CSE=25, ECE=25
MS Ramaiah Institute of Technology: CSE=25, ECE=25

================================
COMEDK Admission System
================================
1. Add New Student
2. Display All Students
3. Display Colleges
4. Process Allocation
5. Update Student Preferences
6. Exit
================================
Enter your choice: 1
```

## 🧪 Testing

The system comes with 5 pre-registered verified students for testing:

| Reg No | Name | DOB        | Aadhar          |
|--------|------|------------|-----------------|
| DC101  | A    | 01-01-2006 | 1111-1111-1111  |
| DC102  | B    | 02-02-2006 | 2222-2222-2222  |
| DC103  | C    | 03-03-2006 | 3333-3333-3333  |
| DC104  | D    | 04-04-2006 | 4444-4444-4444  |
| DC105  | E    | 05-05-2006 | 5555-5555-5555  |

## 🛠️ Technical Details

### Memory Management
- Dynamic memory allocation for all data structures
- Comprehensive cleanup function (`cleanupQueue`) to prevent memory leaks
- Proper deallocation of BST nodes, graphs, and stacks

### Color Coding
- 🔴 Red: Errors and warnings
- 🟢 Green: Success messages
- 🟡 Yellow: Information and prompts
- 🔵 Blue: Headers and titles

### Complexity Analysis
- **Student Insertion**: O(log n) - BST insertion
- **Rank Search**: O(log n) - BST search
- **Allocation**: O(n × m) where n = students, m = max preferences
- **Display**: O(n) - Linear traversal

## 🔄 Future Enhancements

Possible improvements for the system:

- [ ] Database integration for persistence
- [ ] Web-based GUI interface
- [ ] Multiple allocation rounds
- [ ] Waitlist management
- [ ] Category-based reservations (SC/ST/OBC)
- [ ] Merit list generation
- [ ] PDF report generation
- [ ] Email notification system
- [ ] More colleges and branches
- [ ] Seat matrix customization per college

## 🤝 Contributing

Contributions are welcome! Please feel free to submit pull requests or open issues for bugs and feature requests.

### Areas for Contribution
- Additional validation rules
- Enhanced reporting features
- Performance optimizations
- GUI development
- Documentation improvements
- Test case development

## 📝 License

This project is open source and available for educational purposes.

## 👥 Authors

Created as part of a Data Structures and Algorithms project for college admission automation.

## 📞 Support

For issues, questions, or suggestions, please open an issue on GitHub.

## 🙏 Acknowledgments

- COMEDK for the inspiration
- Data Structures and Algorithms course
- Open source community

---

**Note**: This is an educational project and should not be used for actual admission processes without proper testing and certification.
