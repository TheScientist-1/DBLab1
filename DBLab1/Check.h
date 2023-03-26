#pragma once


#include "Master.h"
#include "Group.h"
#include "Check.h"

#define INDEXER_SIZE sizeof(struct Indexer)
#define Group_DATA "group.fl"
#define Group_SIZE sizeof(struct Group)
#define STUDENT_DATA "student.fl"
#define STUDENT_SIZE sizeof(struct Student)
#define Group_IND "group.ind"

int getGroup(struct Group* group, int id, char* error);
int checkFileExists(FILE* indexTable, FILE* database, char* error)
{
    if (indexTable == NULL || database == NULL) {
        strcpy(error, "DB file does not exit");
        return 0;
    }
    return 1;
}
int checkIndexExists(FILE* indexTable, char* error, int id) {
    fseek(indexTable, 0, SEEK_END);
    long indexTableSize = ftell(indexTable);
    if (indexTableSize == 0 || id * INDEXER_SIZE > indexTableSize) {
        strcpy(error, "No such ID int the table");
        return 0;
    }
    return 1;
}
int checkRecordExists(struct Indexer indexer, char* error) {
    if (indexer.exists == 0 ) {
        strcpy(error, "The record was deleted");
        return 0;
    }
    return 1;
}
int checkIfRecordExist(struct Group group, int studentId, char *error ) {
    FILE* studentDB = fopen(STUDENT_DATA, "a+b");
    struct Student student;
    fseek(studentDB, group.studentFirstAddress, SEEK_SET);
    for (int i = 0; i < group.studentsCount; i++) {
        fread(&student, STUDENT_SIZE, 1, studentDB);
        fclose(studentDB);
        if (student.Id == studentId)
        {
            if(student.exists)
                return 1;
            else
            {
                strcpy(error, "This student was deleted");
                return 0;
            }
        }
        studentDB = fopen(STUDENT_DATA, "r+b");
        fseek(studentDB, student.nextAddress, SEEK_SET);
    }
    strcpy(error, "Problem occured: Either this group don't have this student or the student has been deleted/doesn't exist");
    fclose(studentDB);
    return 0;
}
void info() {
    FILE* indexTable = fopen(Group_IND, "rb");
    if (indexTable == NULL) {
        printf("Error: Database files do not exist\n");
        return;
    }
    int groupCount = 0;
    int studentCount = 0;
    fseek(indexTable, 0, SEEK_END);
    int indAmount = ftell(indexTable) / sizeof(struct Indexer);
    struct Group group;
    char error[51];
    for (int i = 1; i <= indAmount; i++)
    {
        if (getGroup(&group, i, error))
        {
            groupCount++;
            studentCount += group.studentsCount;
            printf("Group #%d has %d students\n", i, group.studentsCount);
        }
    }
    fclose(indexTable);
    printf("Total number of groups: %d\n", groupCount);
    printf("Total number of students: %d\n", studentCount);
}




