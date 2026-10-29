/* 
 * Yo ho project ma C17 language standard version use garincha.
 * Student Academic Performance & Grade Analyzer
 * Course  : Project I (BITP 141) - BIT Semester I
 * Language: C (C99)
 *
 * Purpose : Record subject marks of students, calculate total, percentage,
 *           grade, GPA and result (Pass/Fail), show class statistics and a
 *           rank list, and save/load data from a text file.
 * 
 * Aaru dherai details ko lagi, please refer to the README file.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_STUDENTS 50
#define NUM_SUBJECTS 5
#define NAME_LEN     50
#define MAX_NAME_LEN 100
#define PASS_MARK    40.0f          /* minimum marks needed in every subject chai float value 40.0 vanda mathi huncha */
#define DATA_FILE    "students.txt"

static const char *SUBJECT_NAMES[NUM_SUBJECTS] = {
    "Mathematics", "English", "Science", "Computer", "Nepali"
};

/* Every single student's record */
typedef struct {
    int   id;
    char  name[NAME_LEN];
    float marks[NUM_SUBJECTS];
    float total;
    float percentage;
    float gpa;
    char  grade[3];      /* "A+", "B", ... */
    int   passed;        /* 1 = pass, 0 = fail */
} Student;

/* ------------------------- Input helpers ------------------------- */

/* Read a line safely; returns 1 on success, 0 on end of input. */
static int readLine(const char *prompt, char *buf, int size)
{
    printf("%s", prompt);
    if (fgets(buf, size, stdin) == NULL) {
        return 0;
    }
    buf[strcspn(buf, "\n")] = '\0';
    return 1;
}

/* Keep asking until the user enters an integer within [min, max]. */
static int readInt(const char *prompt, int min, int max)
{
    char line[64], *end;
    long value;

    while (readLine(prompt, line, sizeof line)) {
        value = strtol(line, &end, 10);
        if (end != line && *end == '\0' && value >= min && value <= max) {
            return (int)value;
        }
        printf("  Invalid input. Enter a whole number from %d to %d.\n", min, max);
    }
    exit(0); /* input stream closed */
}

/* Keep asking until the user enters a number within [min, max]. */
static float readFloat(const char *prompt, float min, float max)
{
    char line[64], *end;
    double value;

    while (readLine(prompt, line, sizeof line)) {
        value = strtod(line, &end);
        if (end != line && *end == '\0' && value >= min && value <= max) {
            return (float)value;
        }
        printf("  Invalid marks. Enter a number from %.0f to %.0f.\n", min, max);
    }
    exit(0);
}

/* A valid name has only letters and spaces and is not empty. */
static int isValidName(const char *name)
{
    int i, letters = 0;
    for (i = 0; name[i] != '\0'; i++) {
        if (isalpha((unsigned char)name[i])) {
            letters++;
        } else if (name[i] != ' ') {
            return 0;
        }
    }
    return letters > 0;
}

/* ----------------------- Calculation logic ----------------------- */

/* Convert a percentage to a letter grade and grade point. */
static void gradeFromPercentage(float pct, char grade[3], float *gpa)
{
    if      (pct >= 90) { strcpy(grade, "A+"); *gpa = 4.0f; }
    else if (pct >= 80) { strcpy(grade, "A");  *gpa = 3.6f; }
    else if (pct >= 70) { strcpy(grade, "B+"); *gpa = 3.2f; }
    else if (pct >= 60) { strcpy(grade, "B");  *gpa = 2.8f; }
    else if (pct >= 50) { strcpy(grade, "C+"); *gpa = 2.4f; }
    else if (pct >= 40) { strcpy(grade, "C");  *gpa = 2.0f; }
    else                { strcpy(grade, "F");  *gpa = 0.0f; }
}

/* Fill total, percentage, grade, GPA and pass/fail for one student. */
static void calculateResult(Student *s)
{
    int i;
    s->total = 0;
    s->passed = 1;
    for (i = 0; i < NUM_SUBJECTS; i++) {
        s->total += s->marks[i];
        if (s->marks[i] < PASS_MARK) {
            s->passed = 0;          /* failing even one subject = fail */
        }
    }
    s->percentage = s->total / NUM_SUBJECTS;
    gradeFromPercentage(s->percentage, s->grade, &s->gpa);
    if (!s->passed) {               /* a failed subject overrides the grade */
        strcpy(s->grade, "F");
        s->gpa = 0.0f;
    }
}

static int findStudentIndex(const Student list[], int count, int id)
{
    int i;
    for (i = 0; i < count; i++) {
        if (list[i].id == id) {
            return i;
        }
    }
    return -1;
}

/* --------------------------- Features ---------------------------- */

static void printHeader(void)
{
    printf("\n%-5s %-20s %8s %8s %-5s %5s  %s\n",
           "ID", "Name", "Total", "Percent", "Grade", "GPA", "Result");
    printf("--------------------------------------------------------------------\n");
}

static void printRow(const Student *s)
{
    printf("%-5d %-20s %8.2f %7.2f%% %-5s %5.2f  %s\n",
           s->id, s->name, s->total, s->percentage, s->grade, s->gpa,
           s->passed ? "Pass" : "Fail");
}

static void addStudent(Student list[], int *count)
{
    Student s;
    char name[NAME_LEN];
    int i;

    if (*count >= MAX_STUDENTS) {
        printf("Storage full (%d students).\n", MAX_STUDENTS);
        return;
    }

    s.id = readInt("Enter student ID (1-9999): ", 1, 9999);
    if (findStudentIndex(list, *count, s.id) != -1) {
        printf("  Error: ID %d already exists.\n", s.id);
        return;
    }

    do {
        if (!readLine("Enter student name: ", name, sizeof name)) {
            exit(0);
        }
        if (!isValidName(name)) {
            printf("  Invalid name. Use letters and spaces only.\n");
        }
    } while (!isValidName(name));
    strcpy(s.name, name);

    for (i = 0; i < NUM_SUBJECTS; i++) {
        char prompt[64];
        sprintf(prompt, "Marks in %s (0-100): ", SUBJECT_NAMES[i]);
        s.marks[i] = readFloat(prompt, 0, 100);
    }

    calculateResult(&s);
    list[(*count)++] = s;
    printf("Student added successfully.\n");
}

static void displayAll(const Student list[], int count)
{
    int i;
    if (count == 0) {
        printf("No records available.\n");
        return;
    }
    printHeader();
    for (i = 0; i < count; i++) {
        printRow(&list[i]);
    }
}

static void searchStudent(const Student list[], int count)
{
    int id, idx, i;

    if (count == 0) {
        printf("No records available.\n");
        return;
    }
    id = readInt("Enter ID to search: ", 1, 9999);
    idx = findStudentIndex(list, count, id);
    if (idx == -1) {
        printf("Student with ID %d not found.\n", id);
        return;
    }
    printf("\nSubject-wise marks for %s (ID %d):\n", list[idx].name, id);
    for (i = 0; i < NUM_SUBJECTS; i++) {
        printf("  %-12s : %6.2f %s\n", SUBJECT_NAMES[i], list[idx].marks[i],
               list[idx].marks[i] < PASS_MARK ? "(Fail)" : "");
    }
    printHeader();
    printRow(&list[idx]);
}

static void showStatistics(const Student list[], int count)
{
    int i, j, passCount = 0, top = 0, low = 0;
    float sum = 0, subjectSum;

    if (count == 0) {
        printf("No records available.\n");
        return;
    }
    for (i = 0; i < count; i++) {
        sum += list[i].percentage;
        passCount += list[i].passed;
        if (list[i].percentage > list[top].percentage) top = i;
        if (list[i].percentage < list[low].percentage) low = i;
    }

    printf("\n===== Class Statistics =====\n");
    printf("Students        : %d\n", count);
    printf("Class average   : %.2f%%\n", sum / count);
    printf("Highest         : %s (%.2f%%)\n", list[top].name, list[top].percentage);
    printf("Lowest          : %s (%.2f%%)\n", list[low].name, list[low].percentage);
    printf("Passed / Failed : %d / %d\n", passCount, count - passCount);
    printf("Pass rate       : %.2f%%\n", 100.0f * passCount / count);

    printf("\nSubject averages:\n");
    for (j = 0; j < NUM_SUBJECTS; j++) {
        subjectSum = 0;
        for (i = 0; i < count; i++) {
            subjectSum += list[i].marks[j];
        }
        printf("  %-12s : %.2f\n", SUBJECT_NAMES[j], subjectSum / count);
    }
}

/* Rank list: bubble sort on an index array (original data stays unchanged). */
static void showRankList(const Student list[], int count)
{
    int order[MAX_STUDENTS], i, j, tmp;

    if (count == 0) {
        printf("No records available.\n");
        return;
    }
    for (i = 0; i < count; i++) {
        order[i] = i;
    }
    for (i = 0; i < count - 1; i++) {
        for (j = 0; j < count - 1 - i; j++) {
            if (list[order[j]].percentage < list[order[j + 1]].percentage) {
                tmp = order[j];
                order[j] = order[j + 1];
                order[j + 1] = tmp;
            }
        }
    }
    printf("\n%-5s %-5s %-20s %8s %8s %-5s %5s  %s\n", "Rank", "ID", "Name",
           "Total", "Percent", "Grade", "GPA", "Result");
    printf("------------------------------------------------------------------------\n");
    for (i = 0; i < count; i++) {
        printf("%-5d ", i + 1);
        printRow(&list[order[i]]);
    }
}

/* ------------------------- File handling ------------------------- */

static int saveToFile(const Student list[], int count)
{
    FILE *fp = fopen(DATA_FILE, "w");
    int i, j;

    if (fp == NULL) {
        printf("Error: could not open %s for writing.\n", DATA_FILE);
        return 0;
    }
    for (i = 0; i < count; i++) {
        fprintf(fp, "%d,%s", list[i].id, list[i].name);
        for (j = 0; j < NUM_SUBJECTS; j++) {
            fprintf(fp, ",%.2f", list[i].marks[j]);
        }
        fprintf(fp, "\n");
    }
    fclose(fp);
    printf("Saved %d record(s) to %s.\n", count, DATA_FILE);
    return 1;
}

static int loadFromFile(Student list[])
{
    FILE *fp = fopen(DATA_FILE, "r");
    char line[256];
    int count = 0;

    if (fp == NULL) {
        return 0;                   /* first run: no file yet */
    }
    while (count < MAX_STUDENTS && fgets(line, sizeof line, fp) != NULL) {
        Student s;
        if (sscanf(line, "%d,%49[^,],%f,%f,%f,%f,%f", &s.id, s.name,
                   &s.marks[0], &s.marks[1], &s.marks[2],
                   &s.marks[3], &s.marks[4]) == 7) {
            calculateResult(&s);
            list[count++] = s;
        }                           /* corrupted lines haru skipped */
    }
    fclose(fp);
    return count;
}

/* ------------------------------ Main ----------------------------- */

static void showMenu(void)
{
    printf("\n====== Student Grade Analyzer ======\n");
    printf("1. Add student\n");
    printf("2. Display all students\n");
    printf("3. Search student by ID\n");
    printf("4. Class statistics\n");
    printf("5. Rank list\n");
    printf("6. Save records\n");
    printf("0. Exit\n");
}

int main(void)
{
    Student students[MAX_STUDENTS];
    int count = loadFromFile(students);
    int choice;

    if (count > 0) {
        printf("Loaded %d record(s) from %s.\n", count, DATA_FILE);
    }

    do {
        showMenu();
        choice = readInt("Enter choice (0-6): ", 0, 6);
        switch (choice) {
            case 1: addStudent(students, &count);    break;
            case 2: displayAll(students, count);     break;
            case 3: searchStudent(students, count);  break;
            case 4: showStatistics(students, count); break;
            case 5: showRankList(students, count);   break;
            case 6: saveToFile(students, count);     break;
            case 0:
                saveToFile(students, count);         /* auto-save hunxha on exit */
                printf("For know Good Luck Sathi!\n");
                break;
        }
    } while (choice != 0);

    return 0;
}
