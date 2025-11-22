#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "enhanced_ds.h"

// Global college data
College colleges[MAX_COLLEGES];

// Global MAX_PREFERENCES variable
int MAX_PREFERENCES = 3; // Default value, can be changed via command line

// External function declarations
extern bool isValidDOB(const char* dob);
extern bool isValidAadhar(const char* aadhar);

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
    printf("You must select exactly %d preferences in order of priority.\n", MAX_PREFERENCES);
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
    
    // First, save original seat counts for each college
    int original_cse_seats[MAX_COLLEGES];
    int original_ece_seats[MAX_COLLEGES];
    
    for (int i = 0; i < MAX_COLLEGES; i++) {
        original_cse_seats[i] = colleges[i].seats_cse;
        original_ece_seats[i] = colleges[i].seats_ece;
    }
    
    // Count how many seats were already consumed by previous allocations
    Student* temp = q->front;
    while (temp != NULL) {
        // If student was previously allocated, we need to restore those seats
        if (strcmp(temp->allocated_college, "Not Allocated") != 0 && 
            strcmp(temp->allocated_college, "Not Eligible") != 0) {
            // Find the college index
            for (int i = 0; i < MAX_COLLEGES; i++) {
                if (strcmp(temp->allocated_college, colleges[i].name) == 0) {
                    if (strcmp(temp->allocated_branch, "CSE") == 0) {
                        original_cse_seats[i]++;
                    } else if (strcmp(temp->allocated_branch, "ECE") == 0) {
                        original_ece_seats[i]++;
                    }
                    break;
                }
            }
        }
        temp = temp->next;
    }
    
    // Reset all college seats to original values
    for (int i = 0; i < MAX_COLLEGES; i++) {
        colleges[i].seats_cse = original_cse_seats[i];
        colleges[i].seats_ece = original_ece_seats[i];
    }
    
    // Clear all previous allocations
    temp = q->front;
    while (temp != NULL) {
        strcpy(temp->allocated_college, "Not Allocated");
        strcpy(temp->allocated_branch, "NA");
        temp = temp->next;
    }
    
    // Now process allocation from the beginning based on rank order
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
    printf("\n%s=== New Student Registration ===%s\n", COLOR_YELLOW, COLOR_RESET);
    printf("%sPlease enter the following details carefully%s\n\n", COLOR_BLUE, COLOR_RESET);
    
    printf("1. Registration Number Format: DCXXX (e.g., DC101)\n");
    printf("2. Name: Maximum %d characters\n", MAX_NAME_LENGTH - 1);
    printf("3. Rank: Must be a unique number\n");
    printf("4. Date of Birth Format: DD-MM-YYYY\n");
    printf("5. Aadhar Format: XXXX-XXXX-XXXX\n\n");
    
    char dob[15];
    char aadhar[15];
    
    // Get and validate Registration Number (format: DCXXX)
    while (1) {
        printf("Enter Registration Number (format: DCXXX): ");
        scanf("%s", newStudent->reg_number);
        
        // Validate format: must be DC followed by 3 digits
        if (strlen(newStudent->reg_number) != 5 || 
            newStudent->reg_number[0] != 'D' || 
            newStudent->reg_number[1] != 'C' ||
            newStudent->reg_number[2] < '0' || newStudent->reg_number[2] > '9' ||
            newStudent->reg_number[3] < '0' || newStudent->reg_number[3] > '9' ||
            newStudent->reg_number[4] < '0' || newStudent->reg_number[4] > '9') {
            printf("%sError: Registration number must be in format DCXXX (e.g., DC101)!%s\n", COLOR_RED, COLOR_RESET);
            continue;
        }
        
        // Check if registration number already exists
        bool reg_exists = false;
        for (int i = 0; i < verificationDataCount; i++) {
            if (strcmp(verificationData[i].reg_number, newStudent->reg_number) == 0) {
                printf("%sError: Registration Number %s already exists!%s\n", COLOR_RED, newStudent->reg_number, COLOR_RESET);
                reg_exists = true;
                break;
            }
        }
        if (!reg_exists) {
            break;
        }
    }
    
    while (getchar() != '\n');  // Clear input buffer
    printf("Enter Student Name: ");
    fgets(newStudent->name, MAX_NAME_LENGTH, stdin);
    newStudent->name[strcspn(newStudent->name, "\n")] = 0;  // Remove newline
    
    int rank;
    while (1) {
        printf("Enter Rank (1 or higher): ");
        if (scanf("%d", &rank) != 1 || rank < 1) {
            printf("%sError: Please enter a valid rank number!%s\n", COLOR_RED, COLOR_RESET);
            while (getchar() != '\n');  // Clear input buffer
            continue;
        }
        if (searchBST(q->rank_tree, rank) != NULL) {
            printf("%sError: Rank %d is already taken!%s\n", COLOR_RED, rank, COLOR_RESET);
            continue;
        }
        break;
    }
    newStudent->rank = rank;
    
    while (getchar() != '\n');  // Clear input buffer
    
    // Get and validate DOB
    while (1) {
        printf("Enter Date of Birth (DD-MM-YYYY): ");
        if (scanf("%s", dob) != 1) {
            while (getchar() != '\n');  // Clear input buffer on error
            printf("%sError: Invalid input%s\n", COLOR_RED, COLOR_RESET);
            continue;
        }
        
        // Validate DOB format and date
        if (!isValidDOB(dob)) {
            printf("%sError: Invalid DOB format or invalid date. Use DD-MM-YYYY (e.g., 15-06-2000)%s\n", COLOR_RED, COLOR_RESET);
            while (getchar() != '\n');  // Clear input buffer after failed validation
            continue;
        }
        break;
    }
    
    // Get and validate Aadhar
    while (1) {
        printf("Enter Aadhar Number (XXXX-XXXX-XXXX): ");
        scanf("%s", aadhar);
        
        // Validate Aadhar format
        if (!isValidAadhar(aadhar)) {
            printf("%sError: Invalid Aadhar format. Must be XXXX-XXXX-XXXX with only numbers (e.g., 1234-5678-9012)%s\n", COLOR_RED, COLOR_RESET);
            continue;
        }
        
        // Check if Aadhar number already exists
        bool aadhar_exists = false;
        for (int i = 0; i < verificationDataCount; i++) {
            if (strcmp(verificationData[i].aadhar, aadhar) == 0) {
                printf("%sError: Aadhar Number %s already registered!%s\n", COLOR_RED, aadhar, COLOR_RESET);
                aadhar_exists = true;
                break;
            }
        }
        if (!aadhar_exists) {
            break;
        }
    }
    
    // Add to verification data
    if (!addToVerificationData(newStudent->reg_number, newStudent->name, dob, aadhar)) {
        printf("\n%sError: Could not add to verification data. Maximum limit reached.%s\n", COLOR_RED, COLOR_RESET);
        free(newStudent);
        return;
    }

    // Set as verified and continue with preferences
    newStudent->verified = true;
    inputPreferences(newStudent);
    strcpy(newStudent->allocated_college, "Not Allocated");
    strcpy(newStudent->allocated_branch, "NA");
    enqueue(q, newStudent);
    (*student_count)++;
    printf("\n%sStudent added successfully!%s\n", COLOR_GREEN, COLOR_RESET);

}

void updateStudentPreferences(Queue* q) {
    printf("\n%s=== Update Student Preferences ===%s\n", COLOR_YELLOW, COLOR_RESET);
    printf("%sPlease verify your identity%s\n\n", COLOR_BLUE, COLOR_RESET);
    
    char reg_number[MAX_REG_LENGTH];
    printf("Enter Registration Number: ");
    scanf("%s", reg_number);
    
    // Find student in queue
    Student* current = q->front;
    Student* found_student = NULL;
    
    while (current != NULL) {
        if (strcmp(current->reg_number, reg_number) == 0) {
            found_student = current;
            break;
        }
        current = current->next;
    }
    
    if (!found_student) {
        printf("\n%sError: Student with registration number %s not found!%s\n", 
               COLOR_RED, reg_number, COLOR_RESET);
        return;
    }
    
    // Find verification data
    bool found_verification = false;
    StudentData* vData = NULL;
    for (int i = 0; i < verificationDataCount; i++) {
        if (strcmp(reg_number, verificationData[i].reg_number) == 0) {
            vData = &verificationData[i];
            found_verification = true;
            break;
        }
    }
    
    if (!found_verification) {
        printf("%sError: Verification data not found!%s\n", COLOR_RED, COLOR_RESET);
        return;
    }
    
    // Verify student
    printf("Enter Date of Birth (DD-MM-YYYY): ");
    char dob[15];
    scanf("%s", dob);
    if (strcmp(dob, vData->dob) != 0) {
        printf("%sVerification Failed! Invalid Date of Birth.%s\n", COLOR_RED, COLOR_RESET);
        return;
    }
    
    printf("Enter Aadhar Number (XXXX-XXXX-XXXX): ");
    char aadhar[15];
    scanf("%s", aadhar);
    if (strcmp(aadhar, vData->aadhar) != 0) {
        printf("%sVerification Failed! Invalid Aadhar Number.%s\n", COLOR_RED, COLOR_RESET);
        return;
    }
    
    // Display current student info
    printf("\n%sStudent Information:%s\n", COLOR_GREEN, COLOR_RESET);
    printf("Name: %s\n", found_student->name);
    printf("Rank: %d\n", found_student->rank);
    
    // Display current preferences if any
    if (found_student->num_preferences > 0) {
        printf("\nCurrent Preferences:\n");
        for (int i = 0; i < found_student->num_preferences; i++) {
            printf("%d. %s - %s\n", 
                   i + 1,
                   colleges[found_student->preferences[i].college_index].name,
                   found_student->preferences[i].branch);
        }
    }
    
    // Ask if user wants to update preferences
    printf("\nDo you want to update your preferences? (y/n): ");
    char choice;
    while (getchar() != '\n');  // Clear input buffer
    scanf("%c", &choice);
    
    if (choice == 'y' || choice == 'Y') {
        // Reset preferences and input new ones
        found_student->num_preferences = 0;
        inputPreferences(found_student);
        printf("\n%sPreferences updated successfully!%s\n", COLOR_GREEN, COLOR_RESET);
        
        // Log the operation
        char operation[100];
        sprintf(operation, "Updated preferences for student %s", found_student->reg_number);
        pushOperation(q->operation_log, operation);
    } else {
        printf("\n%sPreferences update cancelled.%s\n", COLOR_YELLOW, COLOR_RESET);
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

// Forward declaration for BST freeing
static void freeBSTTree(BSTNode* root);

// Recursive function to free BST nodes
static void freeBSTTree(BSTNode* root) {
    if (root == NULL) return;
    freeBSTTree(root->left);
    freeBSTTree(root->right);
    free(root);
}

// Cleanup function to free all allocated memory
void cleanupQueue(Queue* q) {
    if (q == NULL) return;
    
    // Free all students in the linked list
    Student* current = q->front;
    while (current != NULL) {
        Student* temp = current;
        current = current->next;
        free(temp);
    }
    
    // Free all BST nodes
    freeBSTTree(q->rank_tree);
    
    // Free all college graph nodes
    if (q->college_network != NULL) {
        for (int i = 0; i < MAX_COLLEGES; i++) {
            CollegeNode* node = q->college_network->adjacency_list[i];
            while (node != NULL) {
                CollegeNode* temp = node;
                node = node->next;
                free(temp);
            }
        }
        free(q->college_network->adjacency_list);
        free(q->college_network);
    }
    
    // Free all operation log nodes
    if (q->operation_log != NULL) {
        OpNode* op = q->operation_log->top;
        while (op != NULL) {
            OpNode* temp = op;
            op = op->next;
            free(temp);
        }
        free(q->operation_log);
    }
    
    // Free the queue itself
    free(q);
}