/*
   GPA & CGPA Calculator System
   United International University (UIU) grading policy

   Features:
   1. Calculate GPA of a trimester
   2. Calculate CGPA (with retake courses, or trimester-wise)
   3. Save a result to a .txt file and load it later
   4. View grading policy, calculation method and help
*/

#include <stdio.h>
#include <string.h>

#define MAX_COURSES 20
#define MAX_TRIMESTERS 12

/* ---------- Global data ---------- */
float credit[MAX_COURSES];          /* credit hour of each course      */
char  grade[MAX_COURSES][5];        /* new grade of each course        */
char  oldGrade[MAX_COURSES][5];     /* old grade (only for retakes)    */
int   retake[MAX_COURSES];          /* 1 = retake course, 0 = new      */
int   numCourses = 0;

float trimGPA[MAX_TRIMESTERS];      /* GPA of each trimester           */
float trimCredit[MAX_TRIMESTERS];   /* credits of each trimester       */
int   numTrimesters = 0;

/* ---------- Small helper functions ---------- */

void clearInput()
{
    int c;
    c = getchar();
    while (c != '\n' && c != EOF) {
        c = getchar();
    }
}

void printLine()
{
    printf("-------------------------------------------------------------\n");
}

void printHeader(char title[])
{
    printf("\n=============================================================\n");
    printf("                %s\n", title);
    printf("=============================================================\n");
}

void waitForEnter()
{
    printf("\nPress Enter to continue...");
    clearInput();
}

/* Read an integer between min and max */
int readInt(char message[], int min, int max)
{
    int x;
    int ok;

    while (1) {
        printf("%s", message);
        ok = scanf("%d", &x);
        clearInput();
        if (ok == 1 && x >= min && x <= max) {
            return x;
        }
        printf("Invalid input. Enter a number from %d to %d.\n", min, max);
    }
}

/* Read a decimal number between min and max */
float readFloat(char message[], float min, float max)
{
    float x;
    int ok;

    while (1) {
        printf("%s", message);
        ok = scanf("%f", &x);
        clearInput();
        if (ok == 1 && x >= min && x <= max) {
            return x;
        }
        printf("Invalid input. Enter a number from %.2f to %.2f.\n", min, max);
    }
}

/* Return the grade point of a letter grade, or -1 if the grade is wrong */
float gradePoint(char g[])
{
    if (strcmp(g, "A") == 0)  return 4.00;
    if (strcmp(g, "A-") == 0) return 3.67;
    if (strcmp(g, "B+") == 0) return 3.33;
    if (strcmp(g, "B") == 0)  return 3.00;
    if (strcmp(g, "B-") == 0) return 2.67;
    if (strcmp(g, "C+") == 0) return 2.33;
    if (strcmp(g, "C") == 0)  return 2.00;
    if (strcmp(g, "C-") == 0) return 1.67;
    if (strcmp(g, "D+") == 0) return 1.33;
    if (strcmp(g, "D") == 0)  return 1.00;
    if (strcmp(g, "F") == 0)  return 0.00;
    return -1;
}

/* Read a valid letter grade into g */
void readGrade(char message[], char g[])
{
    int i;

    while (1) {
        printf("%s", message);
        scanf("%4s", g);
        clearInput();

        /* change small letters to capital, e.g. b+ -> B+ */
        for (i = 0; g[i] != '\0'; i++) {
            if (g[i] >= 'a' && g[i] <= 'z') {
                g[i] = g[i] - 32;
            }
        }

        if (gradePoint(g) >= 0) {
            return;
        }
        printf("Invalid grade. Use A, A-, B+, B, B-, C+, C, C-, D+, D or F.\n");
    }
}

/* Ask a Y/N question. Returns 1 for Yes, 0 for No */
int readYesNo(char message[])
{
    char ch;

    while (1) {
        printf("%s", message);
        scanf(" %c", &ch);
        clearInput();
        if (ch == 'Y' || ch == 'y') return 1;
        if (ch == 'N' || ch == 'n') return 0;
        printf("Please enter Y or N.\n");
    }
}

/* Read the file name from the user and add .txt */
void readFileName(char name[])
{
    printf("Enter file name (without .txt): ");
    scanf("%40s", name);
    clearInput();
    strcat(name, ".txt");
}

/* ---------- GPA ---------- */

void inputCourse(int i)
{
    printf("\nCourse %d\n", i + 1);
    credit[i] = readFloat("  Credit hours (1 to 6): ", 1, 6);
    readGrade("  Grade (A, A-, B+, B, B-, C+, C, C-, D+, D, F): ", grade[i]);
    retake[i] = 0;
    strcpy(oldGrade[i], "-");
}

/* Calculate GPA of all courses now in the list */
float findGPA()
{
    int i;
    float totalCredit = 0, totalPoint = 0;

    for (i = 0; i < numCourses; i++) {
        totalPoint = totalPoint + gradePoint(grade[i]) * credit[i];
        totalCredit = totalCredit + credit[i];
    }
    if (totalCredit == 0) {
        return 0;
    }
    return totalPoint / totalCredit;
}

float findTotalCredit()
{
    int i;
    float total = 0;

    for (i = 0; i < numCourses; i++) {
        total = total + credit[i];
    }
    return total;
}

void showGPAResult()
{
    int i;

    printHeader("GPA RESULT");
    printf("Course   Credit   Grade   Grade Point\n");
    printLine();
    for (i = 0; i < numCourses; i++) {
        printf("%-9d%-9.2f%-8s%.2f\n", i + 1, credit[i], grade[i], gradePoint(grade[i]));
    }
    printLine();
    printf("Total Credits : %.2f\n", findTotalCredit());
    printf("GPA           : %.2f\n", findGPA());
    printLine();
}

void saveGPA()
{
    char name[50];
    FILE *fp;
    int i;

    readFileName(name);
    fp = fopen(name, "w");
    if (fp == NULL) {
        printf("Could not create the file.\n");
        return;
    }
    fprintf(fp, "GPA\n");
    fprintf(fp, "%d\n", numCourses);
    for (i = 0; i < numCourses; i++) {
        fprintf(fp, "%.2f %s\n", credit[i], grade[i]);
    }
    fclose(fp);
    printf("Saved in %s\n", name);
}

void calculateGPA()
{
    int i, choice, n;

    printHeader("GPA CALCULATION");
    numCourses = readInt("Number of courses: ", 1, MAX_COURSES);
    for (i = 0; i < numCourses; i++) {
        inputCourse(i);
    }
    showGPAResult();

    /* let the user change a course or the number of courses */
    while (readYesNo("Do you want to change anything? (Y/N): ") == 1) {
        printf("1. Change one course\n");
        printf("2. Change number of courses\n");
        choice = readInt("Enter your choice: ", 1, 2);

        if (choice == 1) {
            n = readInt("Which course number? ", 1, numCourses);
            inputCourse(n - 1);
        } else {
            n = readInt("New number of courses: ", 1, MAX_COURSES);
            for (i = numCourses; i < n; i++) {
                inputCourse(i);
            }
            numCourses = n;
        }
        showGPAResult();
    }

    if (readYesNo("Save this result? (Y/N): ") == 1) {
        saveGPA();
    }
}

/* ---------- CGPA with retake courses ---------- */

void inputCourseWithRetake(int i)
{
    printf("\nCourse %d\n", i + 1);
    credit[i] = readFloat("  Credit hours (1 to 6): ", 1, 6);
    retake[i] = readYesNo("  Is it a retake course? (Y/N): ");
    if (retake[i] == 1) {
        readGrade("  Old grade: ", oldGrade[i]);
        readGrade("  New grade: ", grade[i]);
    } else {
        strcpy(oldGrade[i], "-");
        readGrade("  Grade: ", grade[i]);
    }
}

/* New CGPA:
   - a new course adds its credits and points
   - a retake course does not add credits; the new grade replaces the old one */
float findNewCGPA(float doneCredit, float oldCGPA)
{
    int i;
    float totalPoint = oldCGPA * doneCredit;
    float totalCredit = doneCredit;

    for (i = 0; i < numCourses; i++) {
        if (retake[i] == 1) {
            totalPoint = totalPoint + (gradePoint(grade[i]) - gradePoint(oldGrade[i])) * credit[i];
        } else {
            totalPoint = totalPoint + gradePoint(grade[i]) * credit[i];
            totalCredit = totalCredit + credit[i];
        }
    }
    if (totalCredit == 0) {
        return 0;
    }
    return totalPoint / totalCredit;
}

float findCreditAfter(float doneCredit)
{
    int i;
    float total = doneCredit;

    for (i = 0; i < numCourses; i++) {
        if (retake[i] == 0) {
            total = total + credit[i];
        }
    }
    return total;
}

void showCGPAResult(float doneCredit, float oldCGPA)
{
    int i;

    printHeader("CGPA RESULT");
    printf("Course   Credit   Grade   Retake   Old Grade\n");
    printLine();
    for (i = 0; i < numCourses; i++) {
        if (retake[i] == 1) {
            printf("%-9d%-9.2f%-8s%-9s%s\n", i + 1, credit[i], grade[i], "Yes", oldGrade[i]);
        } else {
            printf("%-9d%-9.2f%-8s%-9s%s\n", i + 1, credit[i], grade[i], "No", "-");
        }
    }
    printLine();
    printf("This trimester GPA : %.2f\n", findGPA());
    printf("Previous credits   : %.2f\n", doneCredit);
    printf("Previous CGPA      : %.2f\n", oldCGPA);
    printf("Total credits now  : %.2f\n", findCreditAfter(doneCredit));
    printf("New CGPA           : %.2f\n", findNewCGPA(doneCredit, oldCGPA));
    printLine();
}

void saveCGPA(float doneCredit, float oldCGPA)
{
    char name[50];
    FILE *fp;
    int i;

    readFileName(name);
    fp = fopen(name, "w");
    if (fp == NULL) {
        printf("Could not create the file.\n");
        return;
    }
    fprintf(fp, "CGPA\n");
    fprintf(fp, "%.2f %.2f\n", doneCredit, oldCGPA);
    fprintf(fp, "%d\n", numCourses);
    for (i = 0; i < numCourses; i++) {
        fprintf(fp, "%.2f %s %d %s\n", credit[i], grade[i], retake[i], oldGrade[i]);
    }
    fclose(fp);
    printf("Saved in %s\n", name);
}

void cgpaWithRetake()
{
    int i, choice, n;
    float doneCredit, oldCGPA;

    printHeader("CGPA WITH RETAKE COURSES");
    doneCredit = readFloat("Completed credits so far: ", 0, 160);
    oldCGPA = readFloat("Current CGPA (0 to 4): ", 0, 4);
    numCourses = readInt("Number of courses this trimester (with retakes): ", 1, MAX_COURSES);
    for (i = 0; i < numCourses; i++) {
        inputCourseWithRetake(i);
    }
    showCGPAResult(doneCredit, oldCGPA);

    while (readYesNo("Do you want to change anything? (Y/N): ") == 1) {
        printf("1. Change one course\n");
        printf("2. Change number of courses\n");
        printf("3. Change completed credits\n");
        printf("4. Change current CGPA\n");
        choice = readInt("Enter your choice: ", 1, 4);

        if (choice == 1) {
            n = readInt("Which course number? ", 1, numCourses);
            inputCourseWithRetake(n - 1);
        } else if (choice == 2) {
            n = readInt("New number of courses: ", 1, MAX_COURSES);
            for (i = numCourses; i < n; i++) {
                inputCourseWithRetake(i);
            }
            numCourses = n;
        } else if (choice == 3) {
            doneCredit = readFloat("Completed credits so far: ", 0, 160);
        } else {
            oldCGPA = readFloat("Current CGPA (0 to 4): ", 0, 4);
        }
        showCGPAResult(doneCredit, oldCGPA);
    }

    if (readYesNo("Save this result? (Y/N): ") == 1) {
        saveCGPA(doneCredit, oldCGPA);
    }
}

/* ---------- CGPA trimester-wise ---------- */

void inputTrimester(int i)
{
    printf("\nTrimester %d\n", i + 1);
    trimGPA[i] = readFloat("  GPA (0 to 4): ", 0, 4);
    trimCredit[i] = readFloat("  Credits (1 to 16): ", 1, 16);
}

float findTrimesterCGPA()
{
    int i;
    float totalPoint = 0, totalCredit = 0;

    for (i = 0; i < numTrimesters; i++) {
        totalPoint = totalPoint + trimGPA[i] * trimCredit[i];
        totalCredit = totalCredit + trimCredit[i];
    }
    if (totalCredit == 0) {
        return 0;
    }
    return totalPoint / totalCredit;
}

void showTrimesterResult()
{
    int i;
    float totalCredit = 0;

    printHeader("TRIMESTER-WISE CGPA");
    printf("Trimester   GPA     Credits\n");
    printLine();
    for (i = 0; i < numTrimesters; i++) {
        printf("%-12d%-8.2f%.2f\n", i + 1, trimGPA[i], trimCredit[i]);
        totalCredit = totalCredit + trimCredit[i];
    }
    printLine();
    printf("Total credits : %.2f\n", totalCredit);
    printf("CGPA          : %.2f\n", findTrimesterCGPA());
    printLine();
}

void saveTrimesters()
{
    char name[50];
    FILE *fp;
    int i;

    readFileName(name);
    fp = fopen(name, "w");
    if (fp == NULL) {
        printf("Could not create the file.\n");
        return;
    }
    fprintf(fp, "TRIMESTER\n");
    fprintf(fp, "%d\n", numTrimesters);
    for (i = 0; i < numTrimesters; i++) {
        fprintf(fp, "%.2f %.2f\n", trimGPA[i], trimCredit[i]);
    }
    fclose(fp);
    printf("Saved in %s\n", name);
}

void cgpaTrimesterWise()
{
    int i, choice, n;

    printHeader("CGPA TRIMESTER-WISE");
    numTrimesters = readInt("Number of trimesters: ", 1, MAX_TRIMESTERS);
    for (i = 0; i < numTrimesters; i++) {
        inputTrimester(i);
    }
    showTrimesterResult();

    while (readYesNo("Do you want to change anything? (Y/N): ") == 1) {
        printf("1. Change one trimester\n");
        printf("2. Change number of trimesters\n");
        choice = readInt("Enter your choice: ", 1, 2);

        if (choice == 1) {
            n = readInt("Which trimester? ", 1, numTrimesters);
            inputTrimester(n - 1);
        } else {
            n = readInt("New number of trimesters: ", 1, MAX_TRIMESTERS);
            for (i = numTrimesters; i < n; i++) {
                inputTrimester(i);
            }
            numTrimesters = n;
        }
        showTrimesterResult();
    }

    if (readYesNo("Save this result? (Y/N): ") == 1) {
        saveTrimesters();
    }
}

void cgpaMenu()
{
    int choice;

    printHeader("CGPA CALCULATION");
    printf("1. CGPA with retake courses\n");
    printf("2. CGPA trimester-wise\n");
    printf("3. Back to main menu\n");
    choice = readInt("Enter your choice: ", 1, 3);

    if (choice == 1) {
        cgpaWithRetake();
    } else if (choice == 2) {
        cgpaTrimesterWise();
    }
}

/* ---------- Load a saved record ---------- */

void loadRecord()
{
    char name[50];
    char type[20];
    FILE *fp;
    int i;
    float doneCredit, oldCGPA;

    printHeader("LOAD SAVED RECORD");
    readFileName(name);
    fp = fopen(name, "r");
    if (fp == NULL) {
        printf("File not found.\n");
        waitForEnter();
        return;
    }

    fscanf(fp, "%19s", type);

    if (strcmp(type, "GPA") == 0) {
        fscanf(fp, "%d", &numCourses);
        for (i = 0; i < numCourses; i++) {
            fscanf(fp, "%f %4s", &credit[i], grade[i]);
        }
        showGPAResult();
    } else if (strcmp(type, "CGPA") == 0) {
        fscanf(fp, "%f %f", &doneCredit, &oldCGPA);
        fscanf(fp, "%d", &numCourses);
        for (i = 0; i < numCourses; i++) {
            fscanf(fp, "%f %4s %d %4s", &credit[i], grade[i], &retake[i], oldGrade[i]);
        }
        showCGPAResult(doneCredit, oldCGPA);
    } else if (strcmp(type, "TRIMESTER") == 0) {
        fscanf(fp, "%d", &numTrimesters);
        for (i = 0; i < numTrimesters; i++) {
            fscanf(fp, "%f %f", &trimGPA[i], &trimCredit[i]);
        }
        showTrimesterResult();
    } else {
        printf("This file is not a saved record.\n");
    }

    fclose(fp);
    waitForEnter();
}

/* ---------- Information pages ---------- */

void showGradingPolicy()
{
    printHeader("UIU GRADING POLICY");
    printf("Grade   Grade Point   Marks\n");
    printLine();
    printf("A       4.00          90-100\n");
    printf("A-      3.67          86-89\n");
    printf("B+      3.33          82-85\n");
    printf("B       3.00          78-81\n");
    printf("B-      2.67          74-77\n");
    printf("C+      2.33          70-73\n");
    printf("C       2.00          66-69\n");
    printf("C-      1.67          62-65\n");
    printf("D+      1.33          58-61\n");
    printf("D       1.00          55-57\n");
    printf("F       0.00          0-54\n");
    waitForEnter();
}

void showMethod()
{
    printHeader("HOW GPA AND CGPA ARE CALCULATED");
    printf("GPA  = Sum of (grade point x credit) / total credits\n\n");
    printf("CGPA (trimester-wise)\n");
    printf("     = Sum of (trimester GPA x trimester credits) / total credits\n\n");
    printf("CGPA (with retake)\n");
    printf("     = (old CGPA x old credits + new points) / total credits\n");
    printf("     For a retake course, the new grade replaces the old grade,\n");
    printf("     and its credits are not counted again.\n");
    waitForEnter();
}

void showHelp()
{
    printHeader("HELP");
    printf("1. Calculate GPA   : enter each course's credit and grade.\n");
    printf("2. Calculate CGPA  : with retake courses, or trimester-wise.\n");
    printf("3. Load Record     : open a result you saved before.\n");
    printf("4. Grading Policy  : see the grade points and marks.\n");
    printf("5. Method          : see the formulas used.\n");
    printf("\nTips:\n");
    printf("- Grades can be typed in small or capital letters (b+ or B+).\n");
    printf("- After every result you can change any input and see it again.\n");
    printf("- Saved files are normal .txt files in the program folder.\n");
    waitForEnter();
}

/* ---------- Main menu ---------- */

int main()
{
    int choice;

    while (1) {
        printHeader("UIU GPA & CGPA CALCULATOR");
        printf("1. Calculate GPA\n");
        printf("2. Calculate CGPA\n");
        printf("3. Load saved record\n");
        printf("4. View grading policy\n");
        printf("5. Method of calculation\n");
        printf("6. Help\n");
        printf("7. Exit\n");
        choice = readInt("Enter your choice: ", 1, 7);

        if (choice == 1) {
            calculateGPA();
        } else if (choice == 2) {
            cgpaMenu();
        } else if (choice == 3) {
            loadRecord();
        } else if (choice == 4) {
            showGradingPolicy();
        } else if (choice == 5) {
            showMethod();
        } else if (choice == 6) {
            showHelp();
        } else {
            printf("\nThank you for using the calculator!\n");
            break;
        }
    }
    return 0;
}
