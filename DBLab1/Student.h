#pragma once
#include "Master.h"
#include "Slave.h"
#include "OutputEntities.h"
#define STUDENT_DATA "student.fl"
#define STUDENT_GARBAGE "student_garbage.txt"
#define STUDENT_SIZE sizeof(struct Student)
int updateGroup(struct Group group, char* error, int id);
struct Student FindLastAddress(FILE *database, struct Group *group, struct Student *previous)
        {
    for (int i = 0; i < group->studentsCount; i++) {
        fread(previous, STUDENT_SIZE, 1, database);
        fseek(database, previous->nextAddress, SEEK_SET);
    }
    return (*previous);
}
void NextAddress(FILE* database, struct Group *group, struct Student *student)
        {
            fclose(database);
            database = fopen(STUDENT_DATA, "r+b");
    struct Student previous;
    fseek(database, group->studentFirstAddress, SEEK_SET);
    previous = FindLastAddress(database, group, &previous);
    previous.nextAddress = student->selfAddress;
    fwrite(&previous, STUDENT_SIZE, 1, database);
}
void overwriteGarbageAddress(int garbageCount, FILE* garbageZone, struct Student* record) {
    int * deletedIds = malloc(garbageCount * sizeof(int));
    for (int i = 0; i < garbageCount; i++) {
        fscanf(garbageZone, "%d", deletedIds + i);
    }

    record->selfAddress = deletedIds[0];
    record->nextAddress = deletedIds[0];
    fclose(garbageZone);
    garbageZone = fopen(STUDENT_GARBAGE, "wb");
    fprintf(garbageZone, "%d", garbageCount - 1);
    for (int i = 1; i < garbageCount; i++)
    {
        fprintf(garbageZone, " %d", deletedIds[i]);
    }
    free(deletedIds);
    fclose(garbageZone);
}
int insertStudent(struct Group group, struct Student student, char * error)
{
    FILE* database = fopen(STUDENT_DATA, "a+b");
    FILE* garbageZone = fopen(STUDENT_GARBAGE, "a+b");
    int garbageCount = 0;
    fscanf(garbageZone, "%d", &garbageCount);
    student.exists=1;
    struct Student stud;
    fseek(database, 0, SEEK_END);
    if (garbageCount != 0)
    {
        overwriteGarbageAddress(garbageCount, garbageZone, &student);
        fclose(database);
        database = fopen(STUDENT_DATA, "rb");
        fseek(database, student.selfAddress, SEEK_SET);
        fread(&stud, STUDENT_SIZE, 1, database);
        student.Id= stud.Id;
        fclose(database);
        database = fopen(STUDENT_DATA, "r+b");
        fseek(database, student.selfAddress, SEEK_SET);
    }
    else
    {
        if (ftell(database) != 0) {
            fseek(database, -(STUDENT_SIZE), SEEK_END);
            fread(&stud, STUDENT_SIZE, 1, database);
            student.Id = stud.Id + 1;
        }
        else
        {
            student.Id = 1;
        }
        int dbSize = ftell(database);
        student.selfAddress = dbSize;
        student.nextAddress = dbSize;
        fseek(database, 0, SEEK_END);
    }
    printf("Your student id is %d \n", student.Id);
    fwrite(&student, STUDENT_SIZE, 1, database);
    if (!group.studentsCount)
    {
        group.studentFirstAddress = student.selfAddress;
    }
    else
    {
        NextAddress(database, &group, &student);
    }
    fclose(database);
    int count = group.studentsCount + 1;
    group.studentsCount=count;
    updateGroup(group, error, group.id);
    return 1;
}
int getStudent(struct Group group, struct Student* student, int studentId, char* error) {

    FILE* database = fopen(STUDENT_DATA, "rb");
    fseek(database, group.studentFirstAddress, SEEK_SET);
    fread(student, STUDENT_SIZE, 1, database);

    for (int i = 0; i < group.studentsCount; i++) {
        if (student->Id == studentId)
        {
            fclose(database);
            return 1;
        }
        fseek(database, student->nextAddress, SEEK_SET);
        fread(student, STUDENT_SIZE, 1, database);
    }
    fclose(database);
    return 0;
}
int updateStudent(struct Student student)
        {
    FILE* database = fopen(STUDENT_DATA, "r+b");
    fseek(database, student.selfAddress, SEEK_SET);
    fwrite(&student, STUDENT_SIZE, 1, database);
    fclose(database);
    return 1;
}
void noteDeletedStudent(int address)
{
    FILE* garbageZone = fopen(STUDENT_GARBAGE, "rb");
    int garbageCount;
    fscanf(garbageZone, "%d", &garbageCount);
    int* deletedAddresses = malloc(garbageCount * sizeof(long));

    for (int i = 0; i < garbageCount; i++) {
        fscanf(garbageZone, "%d", deletedAddresses + i);
    }
    fclose(garbageZone);
    garbageZone = fopen(STUDENT_GARBAGE, "wb");
    fprintf(garbageZone, "%d", garbageCount + 1);
    for (int i = 0; i < garbageCount; i++) {
        fprintf(garbageZone, " %d", deletedAddresses[i]);
    }

    fprintf(garbageZone, " %d", address);
    free(deletedAddresses);
    fclose(garbageZone);
}
void relinkAddresses(FILE* database, struct Student previous, struct Student student, struct Group* group) {
    if (student.selfAddress == group->studentFirstAddress) {
        if (student.selfAddress == student.nextAddress) {
            group->studentFirstAddress = -1;
        } else {
            group->studentFirstAddress = student.nextAddress;
        }
    } else {
        if (student.selfAddress == student.nextAddress) {
            previous.nextAddress = previous.selfAddress;
        }
        else {
            previous.nextAddress = student.nextAddress;
        }
        fseek(database, previous.selfAddress, SEEK_SET);
        fwrite(&previous, STUDENT_SIZE, 1, database);
    }
}

void deleteStudent(struct Group group, struct Student student, char* error)
        {
    FILE* database = fopen(STUDENT_DATA, "r+b");
    struct Student previous;
    fseek(database, group.studentFirstAddress, SEEK_SET);
    do {
        fread(&previous, STUDENT_SIZE, 1, database);
        fseek(database, previous.nextAddress, SEEK_SET);
    }
    while (previous.nextAddress != student.selfAddress && student.selfAddress != group.studentFirstAddress);
    relinkAddresses(database, previous, student, &group);
            noteDeletedStudent(student.selfAddress);
            student.exists = 0;

    fseek(database, student.selfAddress, SEEK_SET);
    fwrite(&student, STUDENT_SIZE, 1, database);
    fclose(database);

    group.studentsCount--;
            updateGroup(group, error, group.id);
}
void PrintList(struct Group group)
{
    FILE* studentDB = fopen(STUDENT_DATA, "a+b");
    struct Student student;
    fseek(studentDB, group.studentFirstAddress, SEEK_SET);
    for (int i = 0; i < group.studentsCount; i++) {
        fread(&student, STUDENT_SIZE, 1, studentDB);
        if (student.exists)
        {
            OutputStudent(student, group);
            printf("<====================================>\n");
        }
        fseek(studentDB, student.nextAddress, SEEK_SET);
    }
    fclose(studentDB);
}