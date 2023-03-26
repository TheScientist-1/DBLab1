#include <stdio.h>
#include "Master.h"
#include "Input.h"
#include "OutputEntities.h"
#include "Slave.h"
#include "Group.h"
#include "Student.h"

#define INDEXER_SIZE sizeof(struct Indexer)
int main()
{
    struct Group group;
    struct Student student;
    while (1)
    {
        int choice;
        int id;
        char error[20];
        printf("Choose option:\n0 Insert group\n1 Get group\n2 Update group\n3 Delete group\n4 Output list of groups\n5 Insert student\n6 Get student\n7 Update student\n8 Delete student \n9 Output list of students\n10 Information\n11 Quit\n");
        printf("Your choice: ");
        scanf("%d", &choice);
        switch (choice) {
            case 0:
                inputGroup(&group);
                insertGroup(group);
                break;
            case 1:
                printf("Enter group ID: \n");
                scanf("%d", &id);
                if(getGroup(&group, id, error) != 0)
                {
                    OutputGroup(group);
                }
                else
                {
                    printf("Error: %s\n", error);
                }
                break;
            case 2:
                printf("Enter group ID: \n");
                scanf("%d", &id);
                getGroup(&group, id, error);
                inputGroup(&group);
                if(updateGroup(group, error, id) == 0)
                    printf("%s", error);
                else
                    printf("Updated successfully!\n");
                break;
            case 3:
                printf("Enter ID: \n");
                scanf("%d", &id);
                if(deleteGroup(id, error) != 0)
                   printf("Deleted successfully!\n");
                else
                    printf("Error: %s\n", error);
                break;
            case 4:
                PrintListOfGroup(error);
                break;
            case 5:
                printf("Enter group ID: \n");
                scanf("%d", &id);
                if(getGroup(&group, id, error) != 0)
                {
                    InputStudent(&student);
                    insertStudent(group, student, error);
                    printf("Inserted successfully! \n");
                }
                else
                {
                    printf("Error %s\n", error);
                }
                break;
            case 6:
                printf("Enter group ID: \n");
                scanf("%d", &id);
                if(getGroup(&group, id, error))
                {
                    printf("Enter student's ID: \n");
                    scanf("%d", &id);
                    if(checkIfRecordExist(group, id, error))
                    {
                        getStudent(group, &student, id, error);
                        OutputStudent(student, group);
                    }
                    else
                    {
                        printf("Error %s\n", error);
                    }
                }
                else
                {
                    printf("Error %s\n", error);
                }
                  break;
            case 7:
                printf("Enter group ID: \n");
                scanf("%d", &id);
                if(getGroup(&group, id, error))
                {
                    printf("Enter student's ID:\n ");
                    scanf("%d", &id);
                    if(checkIfRecordExist(group, id, error))
                    {
                        getStudent(group, &student, id, error);
                        InputStudent(&student);
                        updateStudent(student);
                        printf("Updated successfully!\n ");
                    }
                    else
                    {
                        printf("Error %s\n", error);
                    }
                }
                else
                {
                    printf("Error %s \n", error);
                }

                break;
            case 8:
                printf("Enter group ID: \n");
                scanf("%d", &id);
                if(getGroup(&group, id, error))
                {
                    printf("Enter student's ID:\n");
                    scanf("%d", &id);
                    if(checkIfRecordExist(group, id, error))
                    {
                        getStudent(group, &student, id, error);
                        deleteStudent(group, student, error);
                        printf("Deleted successfully! \n");
                    }
                    else
                    {
                        printf("Error %s \n ", error);
                    }
                }
                else
                {
                    printf("Error %s \n ", error);
                }
                break;
            case 9:
                printf("Enter group ID: \n");
                scanf("%d", &id);
                if(getGroup(&group, id, error))
                {
                   if(group.studentsCount != 0)
                   {
                       PrintList(group);
                   }
                   else
                   {
                       printf("This group has no students\n");
                   }
                }
                else
                {
                    printf("Error %s \n ", error);
                }
                break;
            case 10:
                info();
                break;
            case 11:
                return 0;
            default:
                printf("No command chosen. \n");
        }
        printf("\n");
    }
}
