#include <stdio.h>
#include <string.h>

#define MAX 100
#define FILE_NAME "students.dat"

typedef struct
{
    int roll;
    char name[50];
    float marks;

} Student;

/* ------- Function prototypes -------*/

void clearBuffer(void);
void readString(char *buf, int size);
int loadStudents(Student list[]);
void saveStudents(Student list[], int count);
int findIndex(Student list[], int count, int roll);
void addStudent(Student list[], int *count);
void displayAll(Student list[], int count);
void searchStudent(Student list[], int count);
void deleteStudent(Student list[], int *count);
float calculateAverage(Student list[], int count);

/* -------Helper functions-------*/

// Remove leftover characters (like '\n) from the input buffer
void clearBuffer(void)
{
    while (getchar() != '\n')
        ;
}

// Read a line with spaces and strips the tralling newline
void readString(char *buf, int size)
{
    fgets(buf, size, stdin);
    buf[strcspn(buf, "\n")] = '\0';
}

// Return the indes of the student with given roll, or -1 if not found
int findIndex(Student list[], int count, int roll)
{
    for (int i = 0; i < count; i++)
    {
        if (list[i].roll == roll)
            return i;
    }
    return -1;
}

/* -------File handling-----*/

// Reads all records from file into the array, returns how many were read
int loadStudents(Student list[])
{
    FILE *fp = fopen(FILE_NAME, "rb");
    if (fp == NULL)
        return 0;
    int count = fread(list, sizeof(Student), MAX, fp);
    fclose(fp);
    return count;
}

// Writes the whole array to the file
void saveStudents(Student list[], int count)
{
    FILE *fp = fopen(FILE_NAME, "wb");
    if (fp == NULL)
    {
        printf("Error: could not open file.\n");
        return;
    }
    fwrite(list, sizeof(Student), count, fp);
    fclose(fp);
}
/* ---------- Core features ---------- */

// count is passed by POINTER so the function can change it (call by reference)
void addStudent(Student list[], int *count)
{
    if (*count >= MAX)
    {
        printf("Database full!\n");
        return;
    }

    Student s;
    printf("Enter roll number: ");
    scanf("%d", &s.roll);
    clearBuffer();

    if (findIndex(list, *count, s.roll) != -1)
    {
        printf("Roll number already exists.\n");
        return;
    }

    printf("Enter name: ");
    readString(s.name, sizeof(s.name));

    printf("Enter marks: ");
    scanf("%f", &s.marks);
    clearBuffer();

    list[*count] = s;
    (*count)++;
    saveStudents(list, *count);
    printf("Student added.\n");
}

// list is an array, so it is passed by address automatically; count by value
void displayAll(Student list[], int count)
{
    if (count == 0)
    {
        printf("No records.\n");
        return;
    }
    printf("\n%-8s %-25s %-8s\n", "Roll", "Name", "Marks");
    printf("----------------------------------------\n");
    for (int i = 0; i < count; i++)
    {
        printf("%-8d %-25s %-8.2f\n", list[i].roll, list[i].name, list[i].marks);
    }
}

void searchStudent(Student list[], int count)
{
    int roll;
    printf("Enter roll number to search: ");
    scanf("%d", &roll);
    clearBuffer();

    int idx = findIndex(list, count, roll);
    if (idx == -1)
    {
        printf("Student not found.\n");
    }
    else
    {
        printf("Found -> Roll: %d, Name: %s, Marks: %.2f\n",
               list[idx].roll, list[idx].name, list[idx].marks);
    }
}

void deleteStudent(Student list[], int *count)
{
    int roll;
    printf("Enter roll number to delete: ");
    scanf("%d", &roll);
    clearBuffer();

    int idx = findIndex(list, *count, roll);
    if (idx == -1)
    {
        printf("Student not found.\n");
        return;
    }
    // shift every later record one step left to fill the gap
    for (int i = idx; i < *count - 1; i++)
    {
        list[i] = list[i + 1];
    }
    (*count)--;
    saveStudents(list, *count);
    printf("Student deleted.\n");
}

float calculateAverage(Student list[], int count)
{
    if (count == 0)
        return 0;
    float sum = 0;
    for (int i = 0; i < count; i++)
    {
        sum += list[i].marks;
    }
    return sum / count;
}

/* ---------- Main menu ---------- */

int main()
{
    Student list[MAX];
    int count = loadStudents(list);
    int choice;

    do
    {
        printf("\n===== Student Record Manager =====\n");
        printf("1. Add student\n");
        printf("2. Display all students\n");
        printf("3. Search by roll number\n");
        printf("4. Delete student\n");
        printf("5. Show class average\n");
        printf("0. Exit\n");
        printf("Choice: ");
        scanf("%d", &choice);
        clearBuffer();

        switch (choice)
        {
        case 1:
            addStudent(list, &count);
            break;
        case 2:
            displayAll(list, count);
            break;
        case 3:
            searchStudent(list, count);
            break;
        case 4:
            deleteStudent(list, &count);
            break;
        case 5:
            printf("Class average: %.2f\n", calculateAverage(list, count));
            break;
        case 0:
            printf("Goodbye!\n");
            break;
        default:
            printf("Invalid choice.\n");
        }
    } while (choice != 0);

    return 0;
}
