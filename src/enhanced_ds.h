#ifndef ENHANCED_DS_H
#define ENHANCED_DS_H

#include <stdbool.h>

// ANSI color codes
#define COLOR_RED     "\x1b[31m"
#define COLOR_GREEN   "\x1b[32m"
#define COLOR_YELLOW  "\x1b[33m"
#define COLOR_BLUE    "\x1b[34m"
#define COLOR_RESET   "\x1b[0m"

#define MAX_COLLEGES 4
#define MAX_PREFERENCES 3
#define MAX_NAME_LENGTH 50
#define MAX_REG_LENGTH 10

// BST for rank tracking
typedef struct BSTNode {
    int rank;
    struct Student* student;
    struct BSTNode* left;
    struct BSTNode* right;
} BSTNode;

// Graph for college network
typedef struct CollegeNode {
    int college_id;
    int preference_weight;
    struct CollegeNode* next;
} CollegeNode;

typedef struct {
    CollegeNode** adjacency_list;
} CollegeGraph;

// Stack for operation logging
typedef struct OpNode {
    char operation[100];
    struct OpNode* next;
} OpNode;

typedef struct {
    OpNode* top;
} OpStack;

// Original structures enhanced with internal tracking
typedef struct {
    int college_index;
    char branch[4];  // CSE or ECE
    int preference_weight;  // Added for preference tracking
} Preference;

typedef struct Student {
    char reg_number[MAX_REG_LENGTH];
    char name[MAX_NAME_LENGTH];
    int rank;
    bool verified;
    Preference preferences[MAX_PREFERENCES];
    int num_preferences;
    char allocated_college[MAX_NAME_LENGTH];
    char allocated_branch[4];
    struct Student* next;
    BSTNode* rank_node;  // Link to BST node
} Student;

typedef struct {
    char name[MAX_NAME_LENGTH];
    int seats_cse;
    int seats_ece;
    int preference_count;  // Track how often this college is preferred
} College;

// Queue structure (Priority Queue)
typedef struct {
    Student* front;
    BSTNode* rank_tree;  // BST root
    CollegeGraph* college_network;  // College preference graph
    OpStack* operation_log;  // Operation history
} Queue;

// Function declarations
Queue* createQueue(void);
void enqueue(Queue* q, Student* newStudent);
void displayStudents(Student* head);
bool verifyStudent(Student* student);
void inputPreferences(Student* student);
void displayColleges(void);
void processAllocation(Queue* q);
void displaySystemStatus(Queue* q);  // New function to show internal state

#endif