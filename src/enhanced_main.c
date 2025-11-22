#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "enhanced_ds.h"

// External declarations
extern College colleges[MAX_COLLEGES];

// Function declarations
void initializeColleges(int totalStudents);
void displayMenu(void);
void addNewStudent(Queue* q, int* student_count, int totalStudents);

int main(int argc, char *argv[]) {

    // =====================================================
    //   UPDATED: Require TWO arguments
    //   argv[1] = total students
    //   argv[2] = MAX_PREFERENCES
    // =====================================================
    if (argc != 3) {
        printf("%sUsage: %s <total_students> <max_preferences>%s\n",
               COLOR_RED, argv[0], COLOR_RESET);
        return 1;
    }

    int totalStudents = atoi(argv[1]);
    if (totalStudents <= 0) {
        printf("%sError: Total students must be a positive number%s\n",
               COLOR_RED, COLOR_RESET);
        return 1;
    }

    // NEW → Set global MAX_PREFERENCES from command line
    MAX_PREFERENCES = atoi(argv[2]);
    if (MAX_PREFERENCES < 1 || MAX_PREFERENCES > 8) {
        printf("%sError: MAX_PREFERENCES must be between 1 and 8%s\n",
               COLOR_RED, COLOR_RESET);
        return 1;
    }

    printf("\n%sMAX_PREFERENCES set to: %d%s\n",
           COLOR_GREEN, MAX_PREFERENCES, COLOR_RESET);

    initializeColleges(totalStudents);
    Queue* studentQueue = createQueue();
    int choice;
    int student_count = 0;

    // Pre-register 5 verified students using verificationData[]
    for (int i = 0; i < 5; i++) {
        Student* newStudent = (Student*)malloc(sizeof(Student));
        strcpy(newStudent->reg_number, verificationData[i].reg_number);
        strcpy(newStudent->name, verificationData[i].name);
        newStudent->rank = (i + 1);
        newStudent->verified = true;
        newStudent->num_preferences = 0;
        strcpy(newStudent->allocated_college, "Not Allocated");
        strcpy(newStudent->allocated_branch, "NA");
        enqueue(studentQueue, newStudent);
        student_count++;
    }

    // =======================
    // Main menu loop
    // =======================
    while (1) {
        displayMenu();
        int scanResult = scanf("%d", &choice);
        while (getchar() != '\n');

        if (scanResult != 1) {
            printf("\n%sInvalid input! Please enter a number.%s\n",
                   COLOR_RED, COLOR_RESET);
            continue;
        }

        switch (choice) {
            case 1:
                if (student_count >= totalStudents) {
                    printf("\n%sMaximum number of students (%d) reached!%s\n",
                           COLOR_RED, totalStudents, COLOR_RESET);
                } else {
                    addNewStudent(studentQueue, &student_count, totalStudents);
                }
                break;

            case 2:
                displayStudents(studentQueue->front);
                break;

            case 3:
                displayColleges();
                break;

            case 4:
                processAllocation(studentQueue);
                break;

            case 5:
                updateStudentPreferences(studentQueue);
                break;

            case 6:
                printf("\n%sCleaning system memory...%s\n",
                       COLOR_YELLOW, COLOR_RESET);

                // =====================================
                // NEW → FREE ALL MEMORY BEFORE EXITING
                // =====================================
                cleanupQueue(studentQueue);

                printf("%sThank you for using COMEDK Admission System!%s\n",
                       COLOR_GREEN, COLOR_RESET);
                printf("%sHave a great day!%s\n\n",
                       COLOR_YELLOW, COLOR_RESET);
                return 0;

            default:
                printf("\n%sInvalid choice! Please enter a number between 1 and 6.%s\n",
                       COLOR_RED, COLOR_RESET);
        }
    }

    return 0;
}

void initializeColleges(int totalStudents) {
    strcpy(colleges[0].name, "PES University");
    strcpy(colleges[1].name, "RV College of Engineering");
    strcpy(colleges[2].name, "BMS College of Engineering");
    strcpy(colleges[3].name, "MS Ramaiah Institute of Technology");

    int seatsPerCollege = (totalStudents + MAX_COLLEGES - 1) / MAX_COLLEGES;
    if (seatsPerCollege % 2 != 0)
        seatsPerCollege++;

    for (int i = 0; i < MAX_COLLEGES; i++) {
        colleges[i].seats_cse = seatsPerCollege / 2;
        colleges[i].seats_ece = seatsPerCollege / 2;
        colleges[i].preference_count = 0;
    }

    printf("\n%sInitial Seat Distribution:%s\n", COLOR_BLUE, COLOR_RESET);
    for (int i = 0; i < MAX_COLLEGES; i++) {
        printf("%s: CSE=%d, ECE=%d\n",
               colleges[i].name,
               colleges[i].seats_cse,
               colleges[i].seats_ece);
    }
}

void displayMenu() {
    printf("\n\n%s************************************%s\n", COLOR_YELLOW, COLOR_RESET);
    printf("%s        WELCOME TO COMEDK         %s\n", COLOR_GREEN, COLOR_RESET);
    printf("%s     Admission Automation System   %s\n", COLOR_GREEN, COLOR_RESET);
    printf("%s************************************%s\n\n", COLOR_YELLOW, COLOR_RESET);
    printf("%sAvailable Options:%s\n", COLOR_BLUE, COLOR_RESET);
    printf("=================\n");
    printf("1. Add New Student Registration\n");
    printf("2. View All Registered Students\n");
    printf("3. View All Available Colleges\n");
    printf("4. Process Seat Allotment\n");
    printf("5. Update Student Preferences\n");
    printf("6. Exit\n\n");
    printf("Please enter your choice (1-6): ");
}
