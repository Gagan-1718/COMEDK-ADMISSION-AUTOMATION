#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include "enhanced_ds.h"

// ==========================================================
//        GLOBAL VERIFICATION DATA (FIXED DUPLICATES)
// ==========================================================

StudentData verificationData[MAX_VERIFICATION_DATA] = {
    {"DC101", "A", "01-01-2006", "1111-1111-1111"},
    {"DC102", "B", "02-02-2006", "2222-2222-2222"},
    {"DC103", "C", "03-03-2006", "3333-3333-3333"},
    {"DC104", "D", "04-04-2006", "4444-4444-4444"},
    {"DC105", "E", "05-05-2006", "5555-5555-5555"}
};

int verificationDataCount = 5;


// ==========================================================
//         STRICT & CORRECT DOB VALIDATION
// ==========================================================
bool isValidDOB(const char* dob) {
    if (strlen(dob) != 10) return false;
    if (dob[2] != '-' || dob[5] != '-') return false;

    // All other characters must be digits
    for (int i = 0; i < 10; i++) {
        if (i == 2 || i == 5) continue;
        if (!isdigit(dob[i])) return false;
    }

    int day  = (dob[0] - '0') * 10 + (dob[1] - '0');
    int month = (dob[3] - '0') * 10 + (dob[4] - '0');
    int year = (dob[6] - '0') * 1000 + (dob[7] - '0') * 100 +
               (dob[8] - '0') * 10 + (dob[9] - '0');

    if (year < 1900) return false;
    if (month < 1 || month > 12) return false;

    int daysInMonth[] = {31,28,31,30,31,30,31,31,30,31,30,31};

    bool isLeap = (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
    if (isLeap) daysInMonth[1] = 29;

    if (day < 1 || day > daysInMonth[month - 1]) return false;

    // Check against today's date
    time_t t = time(NULL);
    struct tm now = *localtime(&t);

    int cDay = now.tm_mday;
    int cMonth = now.tm_mon + 1;
    int cYear = now.tm_year + 1900;

    // DOB must be strictly *before* today
    if (year > cYear) return false;
    if (year == cYear && month > cMonth) return false;
    if (year == cYear && month == cMonth && day >= cDay) return false;

    return true;
}


// ==========================================================
//         STRICT AADHAR VALIDATION (XXXX-XXXX-XXXX)
// ==========================================================
bool isValidAadhar(const char* aadhar) {
    if (strlen(aadhar) != 14) return false;
    if (aadhar[4] != '-' || aadhar[9] != '-') return false;

    for (int i = 0; i < 14; i++) {
        if (i == 4 || i == 9) continue;
        if (!isdigit(aadhar[i])) return false;
    }
    return true;
}


// ==========================================================
//        ADD TO VERIFICATION DATA (UNCHANGED)
// ==========================================================
bool addToVerificationData(const char* reg_number,
                           const char* name,
                           const char* dob,
                           const char* aadhar)
{
    if (verificationDataCount >= MAX_VERIFICATION_DATA)
        return false;

    strcpy(verificationData[verificationDataCount].reg_number, reg_number);
    strcpy(verificationData[verificationDataCount].name, name);
    strcpy(verificationData[verificationDataCount].dob, dob);
    strcpy(verificationData[verificationDataCount].aadhar, aadhar);

    verificationDataCount++;
    return true;
}


// ==========================================================
//                STUDENT VERIFICATION PROCESS
// ==========================================================
bool verifyStudent(Student* student) {
    printf("\n%sVerification Process%s\n", COLOR_YELLOW, COLOR_RESET);
    printf("===================\n");

    StudentData* vData = NULL;

    for (int i = 0; i < verificationDataCount; i++) {
        if (strcmp(student->reg_number, verificationData[i].reg_number) == 0) {
            vData = &verificationData[i];
            break;
        }
    }

    if (!vData) {
        printf("%sVerification Failed! Registration number not found.%s\n",
               COLOR_RED, COLOR_RESET);
        return false;
    }

    char dob[15];
    printf("Date of Birth (DD-MM-YYYY): ");
    scanf("%s", dob);

    if (!isValidDOB(dob) || strcmp(dob, vData->dob) != 0) {
        printf("%sVerification Failed! Invalid Date of Birth.%s\n",
               COLOR_RED, COLOR_RESET);
        return false;
    }

    char aadhar[20];
    printf("Aadhar Number (XXXX-XXXX-XXXX): ");
    scanf("%s", aadhar);

    if (!isValidAadhar(aadhar) || strcmp(aadhar, vData->aadhar) != 0) {
        printf("%sVerification Failed! Invalid Aadhar.%s\n",
               COLOR_RED, COLOR_RESET);
        return false;
    }

    // Verification success
    strcpy(student->name, vData->name);

    printf("\n%sVerification Successful!%s\n", COLOR_GREEN, COLOR_RESET);
    return true;
}
