#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/* --- Configuration --- */
#define ID_LEN    20
#define NAME_LEN  50
#define SEC_LEN   10
#define REM_LEN   50

#define FILE_LOGIN "LoginDetails.txt"
#define FILE_DATA  "StudentInfo.txt"
#define FILE_TEMP  "temp.txt"

/* --- Data Structure --- */
typedef struct {
    char id[ID_LEN];
    char name[NAME_LEN];
    char section[SEC_LEN];
    int  grade;
    char remark[REM_LEN];
} Student;

/* Prototypes */
void initialize_files(void);
int authenticate(char role_out[], char userid_out[]);
void start_app(void);
void clear_buffer(void);

/* Menus */
void admin_menu(void);
void staff_menu(void);
void student_menu(const char userid[]);

/* CRUD */
void add_record(void);
void list_records(void);
void find_record(void);
void update_full(void);
void update_limited(void);
void remove_record(void);
void view_my_record(const char userid[]);

/* Helpers */
int read_student(FILE *fp, Student *s);
void write_student(FILE *fp, const Student *s);


/* ============================
   MAIN FUNCTION
============================= */
int main(void) {
    initialize_files();
    start_app();
    return 0;
}


/* ============================
   UTILITY FUNCTIONS
============================= */

void clear_buffer(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void initialize_files(void) {
    FILE *fp;

    /* Initialize LoginDetails.txt */
    fp = fopen(FILE_LOGIN, "r");
    if (!fp) {
        printf("Creating default login file...\n");
        fp = fopen(FILE_LOGIN, "w");
        fprintf(fp, "admin admin123 admin\n");
        fprintf(fp, "staff staff123 staff\n");
        fprintf(fp, "101 pass123 student\n");
        fprintf(fp, "102 pass456 student\n");
        fclose(fp);
    } else fclose(fp);

    /* Initialize StudentInfo.txt */
    fp = fopen(FILE_DATA, "r");
    if (!fp) {
        printf("Creating empty student data file...\n");
        fp = fopen(FILE_DATA, "w");
        fclose(fp);
    } else fclose(fp);
}


/* ============================
   AUTHENTICATION
============================= */

int authenticate(char role_out[], char userid_out[]) {
    char user[ID_LEN], pass[NAME_LEN];
    char f_user[ID_LEN], f_pass[NAME_LEN], f_role[SEC_LEN];

    FILE *fp = fopen(FILE_LOGIN, "r");
    if (!fp) return 0;

    printf("Enter UserID: ");
    scanf("%s", user);
    printf("Enter Password: ");
    scanf("%s", pass);
    clear_buffer();

    while (fscanf(fp, "%s %s %s", f_user, f_pass, f_role) == 3) {
        if (strcmp(user, f_user) == 0 && strcmp(pass, f_pass) == 0) {
            strcpy(role_out, f_role);
            strcpy(userid_out, f_user);
            fclose(fp);
            return 1;
        }
    }

    fclose(fp);
    return 0;
}


/* ============================
   APP START
============================= */

void start_app(void) {
    int choice;
    char role[SEC_LEN], userid[ID_LEN];

    do {
        printf("\n=== STUDENT RECORD MANAGEMENT SYSTEM ===\n");
        printf("1. Login\n");
        printf("2. Exit\n");
        printf("Enter choice: ");
        scanf("%d", &choice);
        clear_buffer();

        if (choice == 1) {
            if (authenticate(role, userid)) {
                printf("\nLogin Successful: %s (%s)\n", userid, role);

                if (strcmp(role, "admin") == 0) admin_menu();
                else if (strcmp(role, "staff") == 0) staff_menu();
                else if (strcmp(role, "student") == 0) student_menu(userid);
            } else {
                printf("Invalid login. Try again.\n");
            }
        }

    } while (choice != 2);
}


/* ============================
   MENUS
============================= */

void admin_menu(void) {
    int ch;
    do {
        printf("\n--- ADMIN MENU ---\n");
        printf("1. Add Record\n");
        printf("2. List Records\n");
        printf("3. Find Record\n");
        printf("4. Update Record (Full)\n");
        printf("5. Delete Record\n");
        printf("6. Logout\n");
        printf("Enter choice: ");

        scanf("%d", &ch);
        clear_buffer();

        switch (ch) {
            case 1: add_record(); break;
            case 2: list_records(); break;
            case 3: find_record(); break;
            case 4: update_full(); break;
            case 5: remove_record(); break;
            case 6: printf("Logging out...\n"); break;
            default: printf("Invalid.\n");
        }
    } while (ch != 6);
}

void staff_menu(void) {
    int ch;
    do {
        printf("\n--- STAFF MENU ---\n");
        printf("1. List Records\n");
        printf("2. Find Record\n");
        printf("3. Update (Grade/Remark only)\n");
        printf("4. Logout\n");
        printf("Enter choice: ");

        scanf("%d", &ch);
        clear_buffer();

        switch (ch) {
            case 1: list_records(); break;
            case 2: find_record(); break;
            case 3: update_limited(); break;
            case 4: break;
        }
    } while (ch != 4);
}

void student_menu(const char userid[]) {
    int ch;
    do {
        printf("\n--- STUDENT MENU ---\n");
        printf("1. View My Record\n");
        printf("2. Logout\n");
        printf("Enter choice: ");

        scanf("%d", &ch);
        clear_buffer();

        if (ch == 1) view_my_record(userid);

    } while (ch != 2);
}


/* ============================
   FILE I/O HELPERS
============================= */

int read_student(FILE *fp, Student *s) {
    return fscanf(fp, "%s %s %s %d %[^\n]",
                  s->id, s->name, s->section, &s->grade, s->remark) == 5;
}

void write_student(FILE *fp, const Student *s) {
    fprintf(fp, "%s %s %s %d %s\n",
            s->id, s->name, s->section, s->grade, s->remark);
}


/* ============================
   CRUD OPERATIONS
============================= */

void add_record(void) {
    FILE *fp = fopen(FILE_DATA, "a");
    Student s;
    char temp[NAME_LEN];

    printf("Enter ID: ");
    scanf("%s", s.id);
    clear_buffer();

    printf("Enter Name (use _ instead of space): ");
    fgets(temp, NAME_LEN, stdin);
    temp[strcspn(temp, "\n")] = 0;
    strcpy(s.name, temp);

    printf("Enter Section: ");
    scanf("%s", s.section);

    printf("Enter Grade: ");
    scanf("%d", &s.grade);
    clear_buffer();

    printf("Enter Remark: ");
    fgets(s.remark, REM_LEN, stdin);
    s.remark[strcspn(s.remark, "\n")] = 0;

    write_student(fp, &s);
    fclose(fp);
    printf("Record added.\n");
}

void list_records(void) {
    FILE *fp = fopen(FILE_DATA, "r");
    Student s;

    printf("\n--- ALL STUDENT RECORDS ---\n");

    while (read_student(fp, &s)) {
        printf("%s   %s   %s   %d   %s\n",
               s.id, s.name, s.section, s.grade, s.remark);
    }

    fclose(fp);
}

void find_record(void) {
    FILE *fp = fopen(FILE_DATA, "r");
    Student s;
    char id[ID_LEN];

    printf("Enter ID to search: ");
    scanf("%s", id);
    clear_buffer();

    while (read_student(fp, &s)) {
        if (strcmp(id, s.id) == 0) {
            printf("\nFOUND:\n");
            printf("%s %s %s %d %s\n",
                   s.id, s.name, s.section, s.grade, s.remark);
            fclose(fp);
            return;
        }
    }
    printf("Record not found.\n");
    fclose(fp);
}

void update_full(void) {
    FILE *in = fopen(FILE_DATA, "r");
    FILE *out = fopen(FILE_TEMP, "w");
    Student s;
    char id[ID_LEN], temp[NAME_LEN];
    int found = 0;

    printf("Enter ID to update: ");
    scanf("%s", id);
    clear_buffer();

    while (read_student(in, &s)) {
        if (strcmp(id, s.id) == 0) {
            found = 1;

            printf("New Name (_ instead of space): ");
            fgets(temp, NAME_LEN, stdin);
            temp[strcspn(temp, "\n")] = 0;
            strcpy(s.name, temp);

            printf("New Section: ");
            scanf("%s", s.section);

            printf("New Grade: ");
            scanf("%d", &s.grade);
            clear_buffer();

            printf("New Remark: ");
            fgets(s.remark, REM_LEN, stdin);
            s.remark[strcspn(s.remark, "\n")] = 0;
        }
        write_student(out, &s);
    }

    fclose(in);
    fclose(out);

    if (found) {
        remove(FILE_DATA);
        rename(FILE_TEMP, FILE_DATA);
        printf("Record updated.\n");
    } else {
        remove(FILE_TEMP);
        printf("Record not found.\n");
    }
}

void update_limited(void) {
    FILE *in = fopen(FILE_DATA, "r");
    FILE *out = fopen(FILE_TEMP, "w");
    Student s;
    char id[ID_LEN];
    int found = 0;

    printf("Enter ID to update: ");
    scanf("%s", id);
    clear_buffer();

    while (read_student(in, &s)) {
        if (strcmp(id, s.id) == 0) {
            found = 1;

            printf("New Grade: ");
            scanf("%d", &s.grade);
            clear_buffer();

            printf("New Remark: ");
            fgets(s.remark, REM_LEN, stdin);
            s.remark[strcspn(s.remark, "\n")] = 0;
        }
        write_student(out, &s);
    }

    fclose(in);
    fclose(out);

    if (found) {
        remove(FILE_DATA);
        rename(FILE_TEMP, FILE_DATA);
        printf("Record updated.\n");
    } else {
        remove(FILE_TEMP);
        printf("Record not found.\n");
    }
}

void remove_record(void) {
    FILE *in = fopen(FILE_DATA, "r");
    FILE *out = fopen(FILE_TEMP, "w");
    Student s;
    char id[ID_LEN];
    int found = 0;

    printf("Enter ID to delete: ");
    scanf("%s", id);
    clear_buffer();

    while (read_student(in, &s)) {
        if (strcmp(id, s.id) == 0) found = 1;
        else write_student(out, &s);
    }

    fclose(in);
    fclose(out);

    if (found) {
        remove(FILE_DATA);
        rename(FILE_TEMP, FILE_DATA);
        printf("Record deleted.\n");
    } else {
        remove(FILE_TEMP);
        printf("Record not found.\n");
    }
}

void view_my_record(const char userid[]) {
    FILE *fp = fopen(FILE_DATA, "r");
    Student s;

    while (read_student(fp, &s)) {
        if (strcmp(userid, s.id) == 0) {
            printf("\nMY RECORD:\n");
            printf("%s %s %s %d %s\n",
                   s.id, s.name, s.section, s.grade, s.remark);
            fclose(fp);
            return;
        }
    }

    printf("Your record not found.\n");
    fclose(fp);
}
