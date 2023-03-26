#pragma once
#include <stdio.h>
#include <string.h>
#include "Master.h"
#include "time.h"
#include "Slave.h"
void inputGroup(struct Group *group)
{
    char name[20];
    char specialty[20];
    char supervisor[20];
    char yearOfGraduation[4];

    printf("Enter group's name: ");
    scanf("%s", name);
    strcpy(group->name, name);

    printf("Enter group's specialty: ");
    scanf("%s", specialty);
    strcpy(group->specialty, specialty);

    printf("Enter group's supervisor: ");
    scanf("%s", supervisor);
    strcpy(group->supervisor, supervisor);

    printf("Enter group's year of graduation: ");
    scanf("%s", yearOfGraduation);
    strcpy(group->yearOfGraduation, yearOfGraduation);

}

void InputStudent(struct Student *student)
{
    char name[10];
    char email[20];
    int birthYear;

    printf("Enter student's name: ");
    scanf("%s", &name);
    strcpy(student->name, name);

    printf("Enter student's email: ");
    scanf("%s", &email);
    strcpy(student->email, email);

    printf("Enter student's year of birth: ");
    scanf("%d", &birthYear);
    student->birthYear = birthYear;

}