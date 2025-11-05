#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "enhanced_ds.h"

// Global college data
College colleges[MAX_COLLEGES];

// Internal function declarations
static BSTNode* createBSTNode(int rank, Student* student);
static void insertBST(BSTNode** root, int rank, Student* student);
static BSTNode* searchBST(BSTNode* root, int rank);
static void pushOperation(OpStack* stack, const char* operation);
static void updateCollegeNetwork(CollegeGraph* graph, int src, int dest, int weight);

// BST functions
static BSTNode* createBSTNode(int rank, Student* student) {
    BSTNode* node = (BSTNode*)malloc(sizeof(BSTNode));
    node->rank = rank;
    node->student = student;
    node->left = node->right = NULL;
    return node;
}

static void insertBST(BSTNode** root, int rank, Student* student) {
    if (*root == NULL) {
        *root = createBSTNode(rank, student);
        return;
    }
    if (rank < (*root)->rank)
        insertBST(&(*root)->left, rank, student);
    else
        insertBST(&(*root)->right, rank, student);
}

static BSTNode* searchBST(BSTNode* root, int rank) {
    if (root == NULL || root->rank == rank)
        return root;
    if (rank < root->rank)
        return searchBST(root->left, rank);
    return searchBST(root->right, rank);
}

// Queue operations
Queue* createQueue() {
    Queue* q = (Queue*)malloc(sizeof(Queue));
    q->front = NULL;
    q->rank_tree = NULL;
    q->college_network = (CollegeGraph*)malloc(sizeof(CollegeGraph));
    q->college_network->adjacency_list = (CollegeNode**)calloc(MAX_COLLEGES, sizeof(CollegeNode*));
    q->operation_log = (OpStack*)malloc(sizeof(OpStack));
    q->operation_log->top = NULL;
    pushOperation(q->operation_log, "Queue initialized");
    return q;
}

void enqueue(Queue* q, Student* newStudent) {
    // Add to BST first
    insertBST(&q->rank_tree, newStudent->rank, newStudent);
    newStudent->rank_node = searchBST(q->rank_tree, newStudent->rank);
    
    // Update college network based on preferences
    for (int i = 0; i < newStudent->num_preferences; i++) {
        int college_idx = newStudent->preferences[i].college_index;
        colleges[college_idx].preference_count++;
        if (i > 0) {
            int prev_college = newStudent->preferences[i-1].college_index;
            updateCollegeNetwork(q->college_network, prev_college, college_idx, 1);
        }
    }
    
    // Regular queue insertion based on rank
    if (q->front == NULL || newStudent->rank < q->front->rank) {
        newStudent->next = q->front;
        q->front = newStudent;
    } else {
        Student* current = q->front;
        while (current->next != NULL && current->next->rank < newStudent->rank) {
            current = current->next;
        }
        newStudent->next = current->next;
        current->next = newStudent;
    }
    
    char operation[100];
    sprintf(operation, "Enqueued student with rank %d", newStudent->rank);
    pushOperation(q->operation_log, operation);
}

void displayStudents(Student* head) {
    printf("\n%sStudent List%s\n", COLOR_BLUE, COLOR_RESET);
    printf("============\n");
    printf("%-10s %-20s %-6s %-30s %-10s\n", 
           "Reg No", "Name", "Rank", "College", "Branch");
    printf("------------------------------------------------------------\n");
    
    Student* current = head;
    while (current != NULL) {
        printf("%-10s %-20s %-6d %-30s %-10s\n",
               current->reg_number,
               current->name,
               current->rank,
               current->allocated_college,
               current->allocated_branch);
        current = current->next;
    }
}

// Operation Stack functions
static void pushOperation(OpStack* stack, const char* operation) {
    OpNode* newNode = (OpNode*)malloc(sizeof(OpNode));
    strcpy(newNode->operation, operation);
    newNode->next = stack->top;
    stack->top = newNode;
}

static void updateCollegeNetwork(CollegeGraph* graph, int src, int dest, int weight) {
    CollegeNode* newNode = (CollegeNode*)malloc(sizeof(CollegeNode));
    newNode->college_id = dest;
    newNode->preference_weight = weight;
    newNode->next = graph->adjacency_list[src];
    graph->adjacency_list[src] = newNode;
}

void displayColleges() {
    printf("\n%s================================================================%s\n", COLOR_BLUE, COLOR_RESET);
    printf("%s                  COMEDK College Information                      %s\n", COLOR_BLUE, COLOR_RESET);
    printf("%s================================================================%s\n", COLOR_BLUE, COLOR_RESET);
    printf("%-4s %-35s %-15s %-10s\n", 
           "No.", "College Name", "Branch", "Seats");
    printf("----------------------------------------------------------------\n");
    
    for (int i = 0; i < MAX_COLLEGES; i++) {
        // Display CSE information
        printf("%-4d %-35s %-15s %-10d\n", 
               (2*i + 1), 
               colleges[i].name, 
               "CSE", 
               colleges[i].seats_cse);
               
        // Display ECE information
        printf("%-4d %-35s %-15s %-10d\n", 
               (2*i + 2), 
               "", 
               "ECE", 
               colleges[i].seats_ece);
               
        if (i < MAX_COLLEGES - 1) {
            printf("----------------------------------------------------------------\n");
        }
    }
    printf("================================================================\n");
}

void inputPreferences(Student* student) {
    printf("\n%s============ College Preference Selection ============%s\n", COLOR_YELLOW, COLOR_RESET);
    printf("Student: %s (Rank: %d)\n", student->name, student->rank);
    printf("You must select exactly 3 preferences in order of priority.\n");
    printf("Each preference should be a combination of college and branch.\n\n");
    
    displayColleges();
    student->num_preferences = 0;
    
    for (int i = 0; i < MAX_PREFERENCES; i++) {
        printf("\n------------------------------------------------\n");
        printf("Enter Priority %d Choice (1-8): ", i + 1);
        int choice;
        scanf("%d", &choice);
        
        if (choice < 1 || choice > 8) {
            printf("%sInvalid choice! Please enter a number between 1 and 8.%s\n", COLOR_RED, COLOR_RESET);
            i--;
            continue;
        }
        
        student->preferences[i].college_index = (choice - 1) / 2;
        strcpy(student->preferences[i].branch, 
               (choice % 2 == 1) ? "CSE" : "ECE");
        
        printf("Selected: %s - %s\n", 
               colleges[student->preferences[i].college_index].name,
               student->preferences[i].branch);
        
        student->num_preferences++;
    }
}

void processAllocation(Queue* q) {
    printf("\n%sProcessing seat allocation...%s\n", COLOR_YELLOW, COLOR_RESET);
    
    Student* current = q->front;
    while (current != NULL) {
        if (!current->verified) {
            strcpy(current->allocated_college, "Not Eligible");
            strcpy(current->allocated_branch, "NA");
            current = current->next;
            continue;
        }
        
        bool allocated = false;
        
        for (int i = 0; i < current->num_preferences && !allocated; i++) {
            int college_idx = current->preferences[i].college_index;
            char* branch = current->preferences[i].branch;
            
            if (strcmp(branch, "CSE") == 0 && colleges[college_idx].seats_cse > 0) {
                strcpy(current->allocated_college, colleges[college_idx].name);
                strcpy(current->allocated_branch, "CSE");
                colleges[college_idx].seats_cse--;
                allocated = true;
                
                char operation[100];
                sprintf(operation, "Allocated %s to %s CSE", current->name, colleges[college_idx].name);
                pushOperation(q->operation_log, operation);
            }
            else if (strcmp(branch, "ECE") == 0 && colleges[college_idx].seats_ece > 0) {
                strcpy(current->allocated_college, colleges[college_idx].name);
                strcpy(current->allocated_branch, "ECE");
                colleges[college_idx].seats_ece--;
                allocated = true;
                
                char operation[100];
                sprintf(operation, "Allocated %s to %s ECE", current->name, colleges[college_idx].name);
                pushOperation(q->operation_log, operation);
            }
        }
        
        if (!allocated) {
            strcpy(current->allocated_college, "Not Allocated");
            strcpy(current->allocated_branch, "NA");
            pushOperation(q->operation_log, "Student could not be allocated");
        }
        
        current = current->next;
    }
    
    printf("\n%sAllocation complete! View all students to see results.%s\n", COLOR_GREEN, COLOR_RESET);
}

void addNewStudent(Queue* q, int* student_count, int totalStudents) {
    Student* newStudent = (Student*)malloc(sizeof(Student));
    printf("\n%sEnter Student Details%s\n", COLOR_YELLOW, COLOR_RESET);
    printf("=====================\n");
    
    printf("Enter Registration Number (e.g., DC101): ");
    scanf("%s", newStudent->reg_number);
    
    printf("Enter Name: ");
    scanf("%s", newStudent->name);
    
    int rank;
    do {
        printf("Enter Rank: ");
        scanf("%d", &rank);
        if (searchBST(q->rank_tree, rank) != NULL) {
            printf("%sRank %d is already taken!%s\n", COLOR_RED, rank, COLOR_RESET);
        }
    } while (searchBST(q->rank_tree, rank) != NULL);
    newStudent->rank = rank;
    
    if (verifyStudent(newStudent)) {
        inputPreferences(newStudent);
        strcpy(newStudent->allocated_college, "Not Allocated");
        strcpy(newStudent->allocated_branch, "NA");
        enqueue(q, newStudent);
        (*student_count)++;
        printf("\n%sStudent added successfully!%s\n", COLOR_GREEN, COLOR_RESET);
    } else {
        free(newStudent);
    }
}

void displaySystemStatus(Queue* q) {
    printf("\n%sSystem Status%s\n", COLOR_BLUE, COLOR_RESET);
    printf("=============\n\n");
    
    // Display college preference counts
    printf("College Preferences:\n");
    printf("-------------------\n");
    for (int i = 0; i < MAX_COLLEGES; i++) {
        printf("%s: %d preferences\n", 
               colleges[i].name, 
               colleges[i].preference_count);
    }
    
    // Display recent operations
    printf("\nRecent Operations:\n");
    printf("------------------\n");
    OpNode* current = q->operation_log->top;
    int count = 0;
    while (current != NULL && count < 5) {
        printf("- %s\n", current->operation);
        current = current->next;
        count++;
    }
}