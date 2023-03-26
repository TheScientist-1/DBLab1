#pragma once
#include "Master.h"

void OutputGroup(struct Group group)
{
    printf("Group's name: %s\n", group.name);
    printf("Group's specialty: %s\n", group.specialty);
    printf("Group's supervisor: %s\n", group.supervisor);
    printf("Group's year of graduation: %s\n", group.yearOfGraduation);
}

void OutputStudent(struct Student student, struct Group group)
{
    printf("Student's group id %d \n", group.id);
    printf("Group's name and specialty: %s, %s \n", group.name, group.specialty);
    printf("Student's name: %s\n", student.name);
    printf("Student's email: %s\n", student.email);
    printf("Student's year of birth: %d\n", student.birthYear);


}
