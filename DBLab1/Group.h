#pragma once
#include <malloc.h>
#include "Master.h"
#include "Input.h"
#include "Check.h"
#include "OutputEntities.h"
#include "Slave.h"
#include "Student.h"


#define Group_GARBAGE "group_garbage.txt"
#define INDEXER_SIZE sizeof(struct Indexer)
int PrintListOfGroup(char *error);
void overwriteGarbageGroupElement(int garbageCount, FILE* garbageZone, struct Group* group)
        {
    int* deleteIdsFile = malloc(garbageCount * sizeof(int));
    for (int i = 0; i < garbageCount; i++)
    {
        fscanf(garbageZone, "%d", deleteIdsFile + i);
    }
    fclose(garbageZone);
            group->id = deleteIdsFile[0];
    fopen(Group_GARBAGE, "wb");
    fprintf(garbageZone, "%d", garbageCount - 1);
    for (int i = 1; i < garbageCount; i++)
    {
        fprintf(garbageZone, " %d", deleteIdsFile[i]);
    }
    fclose(garbageZone);
    free(deleteIdsFile);
}
int insertGroup(struct Group group){
    FILE* garbageZone = fopen(Group_GARBAGE, "a+b");
    FILE* indexTable = fopen(Group_IND, "a+b");
    FILE* database = fopen(Group_DATA, "a+b");
    int garbageCount = 0;
    fscanf(garbageZone, "%d", &garbageCount);
    struct Indexer indexer;
    if (garbageCount !=0 )
    {
        overwriteGarbageGroupElement(garbageCount, garbageZone, &group);
        fclose(database);
        fclose(indexTable);
        indexTable = fopen(Group_IND, "r+b");
        database = fopen(Group_DATA, "r+b");
        fseek(indexTable, (group.id - 1) * INDEXER_SIZE, SEEK_SET);
        fread(&indexer, INDEXER_SIZE, 1, indexTable);
        fseek(database, indexer.address, SEEK_SET);
    }
    else {
        fseek(indexTable, 0, SEEK_END);
        if (ftell(indexTable) != 0) {
            fseek(indexTable, -(INDEXER_SIZE), SEEK_END);
            fread(&indexer, INDEXER_SIZE, 1, indexTable);
            group.id = indexer.id + 1;
        } else {
            group.id = 1;
        }
    }
    group.studentFirstAddress = -1;
    group.studentsCount = 0;
    fwrite(&group, Group_SIZE, 1, database);
    indexer.id = group.id;
    indexer.exists = 1;
    indexer.address = (group.id - 1) * Group_SIZE;
    printf("Your group id is %d\n", group.id);
    fseek(indexTable, (group.id - 1) * INDEXER_SIZE, SEEK_SET);
    fwrite(&indexer, INDEXER_SIZE, 1, indexTable);
    fclose(database);
    fclose(indexTable);
    return 1;
}
int getGroup(struct Group* group, int id, char* error)
        {
    FILE* indexTable = fopen(Group_IND, "rb");
    FILE* database = fopen(Group_DATA, "rb");
    if(checkFileExists(indexTable, database, error) == 0)
        return 0;
    struct Indexer indexer;
    if(checkIndexExists(indexTable, error, id) ==0)
        return 0;
    fseek(indexTable, (id - 1) * INDEXER_SIZE, SEEK_SET);
    fread(&indexer, INDEXER_SIZE, 1, indexTable);
    if (checkRecordExists(indexer, error)==0)
        return 0;
    fseek(database, indexer.address, SEEK_SET);
    fread(group, Group_SIZE, 1, database);
    fclose(indexTable);
    fclose(database);
    return 1;
}

int updateGroup(struct Group group, char* error, int id) {
    FILE* indexTable = fopen(Group_IND, "r+b");
    FILE* database = fopen(Group_DATA, "r+b");
    struct Indexer indexer;
    if (checkFileExists(indexTable, database, error)==0) {
        return 0;
    }
    if (checkIndexExists(indexTable, error, id)==0) {
        return 0;
    }
    fseek(indexTable, (id - 1) * INDEXER_SIZE, SEEK_SET);
    fread(&indexer, INDEXER_SIZE, 1, indexTable);
    if(checkRecordExists(indexer, error)==0)
        return 0;
    group.id = id;
    fseek(database, indexer.address, SEEK_SET);
    fwrite(&group, Group_SIZE, 1, database);
    fclose(indexTable);
    fclose(database);
    return 1;
}
void listOfDeletedGroup(int id)
{
    FILE* garbage = fopen(Group_GARBAGE, "rb");
    if (garbage == NULL)
    {
        printf("Error occured during opening file \n");
        return;
    }
    int garbageCount;
    fscanf(garbage, "%d", &garbageCount);
    int* deleteIds = malloc(garbageCount * sizeof(int));

    for (int i = 0; i < garbageCount; i++)
    {
        fscanf(garbage, "%d", deleteIds + i);
    }
    fclose(garbage);
    garbage = fopen(Group_GARBAGE, "wb");
    fprintf(garbage, "%d", garbageCount + 1);
    for (int i = 0; i < garbageCount; i++) {
        fprintf(garbage, " %d", deleteIds[i]);
    }
    fprintf(garbage, " %d", id);
    free(deleteIds);
    fclose(garbage);
}
int deleteGroup(int id, char* error) {
    FILE* indexTable = fopen(Group_IND, "r+b");
    struct Indexer indexer;
    if (indexTable == NULL) {
        strcpy(error, "Database files are not created yet");
        return 0;
    }
    if (checkIndexExists(indexTable, error, id)==0) {
        return 0;
    }
    fseek(indexTable, (id - 1) * INDEXER_SIZE, SEEK_SET);
    fread(&indexer, INDEXER_SIZE, 1, indexTable);
    if(checkRecordExists(indexer, error)==0)
        return 0;
    struct Group group;
    getGroup(&group, id, error);
    indexer.exists = 0;
    fseek(indexTable, (id - 1) * INDEXER_SIZE, SEEK_SET);
    fwrite(&indexer, INDEXER_SIZE, 1, indexTable);
    fclose(indexTable);
    listOfDeletedGroup(id);
    if (group.studentsCount != 0) {
        FILE* studentDB = fopen(STUDENT_DATA, "r+b");
        struct Student student;
        fseek(studentDB, group.studentFirstAddress, SEEK_SET);
        for (int i = 0; i < group.studentsCount; i++)
        {
            fread(&student, STUDENT_SIZE, 1, studentDB);
            fclose(studentDB);
            deleteStudent(group, student, error);
            studentDB = fopen(STUDENT_DATA, "r+b");
            fseek(studentDB, student.nextAddress, SEEK_SET);
        }
        fclose(studentDB);
    }
    return 1;
}
 int PrintListOfGroup(char *error)
 {
     FILE* database = fopen(Group_DATA, "rb");
     FILE* indexTable = fopen(Group_IND, "rb");
     struct Group group;
     struct Indexer indexer;
     if (checkFileExists(indexTable, database, error)==0)
     {
         return 0;
     }
     fseek(indexTable, -INDEXER_SIZE, SEEK_END);
     fread(&indexer, INDEXER_SIZE, 1, indexTable);
     rewind(indexTable);
     int endIndex =0;
     getGroup(&group, indexer.id, error);
     endIndex = indexer.id;
     int id = 0;
     int count =0;
     while(id < endIndex)
     {
         fseek(indexTable, id *INDEXER_SIZE, SEEK_SET);
         fread(&indexer, INDEXER_SIZE, 1, indexTable);
         if(indexer.exists)
         {
             printf("<=============================>\n");
             getGroup(&group, indexer.id, error);
             printf("Id %d\n", group.id);
             OutputGroup(group);
             count++;
         }
         id++;
     }
     if(count == 0)
     {
         printf("No groups\n");
     }
     return 1;
 }


