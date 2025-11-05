#include <stdio.h>
#include <string.h>
#include "enhanced_ds.h"

// Structure for verification data
typedef struct {
    char reg_number[MAX_REG_LENGTH];
    char name[MAX_NAME_LENGTH];
    char dob[15];
    char aadhar[15];
} StudentData;

// Simple verification data with easy to remember values
StudentData verificationData[5] = {
    {"DC101", "A", "01-01-2006", "1111-1111-1111"},
    {"DC102", "B", "02-02-2006", "2222-2222-2222"},
    {"DC103", "C", "03-03-2006", "3333-3333-3333"},
    {"DC104", "D", "04-04-2006", "4444-4444-4444"},
    {"DC105", "E", "05-05-2006", "5555-5555-5555"}
};

bool verifyStudent(Student* student) {
    printf("\n%sVerification Process%s\n", COLOR_YELLOW, COLOR_RESET);
    printf("===================\n");
    printf("Enter your details for verification:\n");
    
    // Find the verification data for this registration number
    bool found = false;
    StudentData* vData = NULL;
    for (int i = 0; i < 5; i++) {
        if (strcmp(student->reg_number, verificationData[i].reg_number) == 0) {
            vData = &verificationData[i];
            found = true;
            break;
        }
    }
    
    if (!found) {
        printf("%sVerification Failed! Registration number not found.%s\n", COLOR_RED, COLOR_RESET);
        return false;
    }

    printf("Date of Birth (DD-MM-YYYY): ");
    char dob[15];
    scanf("%s", dob);
    if (strcmp(dob, vData->dob) != 0) {
        printf("%sVerification Failed! Invalid Date of Birth.%s\n", COLOR_RED, COLOR_RESET);
        return false;
    }
    
    printf("Aadhar Number (XXXX-XXXX-XXXX): ");
    char aadhar[15];
    scanf("%s", aadhar);
    if (strcmp(aadhar, vData->aadhar) != 0) {
        printf("%sVerification Failed! Invalid Aadhar Number.%s\n", COLOR_RED, COLOR_RESET);
        return false;
    }
    
    // Copy verified name to student structure
    strcpy(student->name, vData->name);
    
    printf("\n%sVerification Successful!%s\n", COLOR_GREEN, COLOR_RESET);
    return true;
}