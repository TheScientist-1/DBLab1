#pragma once
struct Group
{
    int id;
    char name[20];
    char specialty[20];
    char supervisor[20];
    char yearOfGraduation[4];
    int studentsCount;
    int studentFirstAddress;
};
struct Indexer
{
    int id;
    int address;
    int exists;
};
