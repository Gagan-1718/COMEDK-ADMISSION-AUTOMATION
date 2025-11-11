#include <stdio.h>
#include <string.h>
#include "enhanced_ds.h"

// Global verification data array and counter
StudentData verificationData[MAX_VERIFICATION_DATA];
int verificationDataCount;

// Array to store verification data
StudentData verificationData[MAX_VERIFICATION_DATA] = {
    {"DC101", "A", "01-01-2006", "1111-1111-1111"},
    {"DC102", "B", "02-02-2006", "2222-2222-2222"},
    {"DC103", "C", "03-03-2006", "3333-3333-3333"},
    {"DC104", "D", "04-04-2006", "4444-4444-4444"},
    {"DC105", "E", "05-05-2006", "5555-5555-5555"}
};

// Keep track of number of students in verificationData
int verificationDataCount = 5;

// Function to add new student to verification data
bool addToVerificationData(const char* reg_number, const char* name, const char* dob, const char* aadhar) {
    if (verificationDataCount >= MAX_VERIFICATION_DATA) {
        return false;
    }
    
    strcpy(verificationData[verificationDataCount].reg_number, reg_number);
    strcpy(verificationData[verificationDataCount].name, name);
    strcpy(verificationData[verificationDataCount].dob, dob);
    strcpy(verificationData[verificationDataCount].aadhar, aadhar);
    verificationDataCount++;
    return true;
}

bool verifyStudent(Student* student) {
    printf("\n%sVerification Process%s\n", COLOR_YELLOW, COLOR_RESET);
    printf("===================\n");
    printf("Enter your details for verification:\n");
    
    // Find the verification data for this registration number
    bool found = false;
    StudentData* vData = NULL;
    for (int i = 0; i < verificationDataCount; i++) {
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