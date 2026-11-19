
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#define MAX_COURSES 50
#define MAX_ROOMS 20
#define MAX_SLOTS 20

struct Course {
    int courseId;
    int facultyId;
    int studentCount;
};

struct Room {
    int roomId;
    int capacity;
};

struct Assignment {
    int courseIndex;
    int roomIndex;
    int slotIndex;
};

// Read an integer within the given range
int readInt(char message[], int min, int max) {
    char line[100];
    char *end;
    long value;

    while (1) {
        printf("%s", message);

        if (fgets(line, sizeof(line), stdin) == NULL) {
            printf("Input error. Try again.\n");
            continue;
        }

        value = strtol(line, &end, 10);

        while (isspace((unsigned char)*end)) {
            end++;
        }

        if (end == line || *end != '\0' ||
            value < min || value > max) {
            printf("Invalid input. Enter a value between %d and %d.\n",
                   min, max);
            continue;
        }

        return (int)value;
    }
}

// Check if a course can be scheduled
int validateAssignment(
    int courseIndex,
    int roomIndex,
    int slotIndex,
    struct Course courses[],
    struct Room rooms[],
    struct Assignment assignments[],
    int assignmentCount,
    int courseCount,
    int roomCount,
    int slotCount
) {
    if (courseIndex < 0 || courseIndex >= courseCount ||
        roomIndex < 0 || roomIndex >= roomCount ||
        slotIndex < 0 || slotIndex >= slotCount) {
        return 0;
    }

    // Check room capacity
    if (rooms[roomIndex].capacity <
        courses[courseIndex].studentCount) {
        return 0;
    }

    // Check previous timetable entries
    for (int i = 0; i < assignmentCount; i++) {

        if (assignments[i].slotIndex == slotIndex) {

            // Check room clash
            if (assignments[i].roomIndex == roomIndex) {
                return 0;
            }

            // Check faculty clash
            int existingCourse = assignments[i].courseIndex;

            if (courses[existingCourse].facultyId ==
                courses[courseIndex].facultyId) {
                return 0;
            }
        }
    }

    return 1;
}

// Greedy timetable scheduler
void greedyScheduler(
    struct Course courses[],
    struct Room rooms[],
    struct Assignment assignments[],
    int courseCount,
    int roomCount,
    int slotCount,
    int *assignmentCount
) {
    *assignmentCount = 0;

    for (int i = 0; i < courseCount; i++) {

        int scheduled = 0;

        for (int slot = 0; slot < slotCount; slot++) {

            for (int room = 0; room < roomCount; room++) {

                if (validateAssignment(
                    i, room, slot,
                    courses, rooms, assignments,
                    *assignmentCount, courseCount,
                    roomCount, slotCount
                )) {

                    assignments[*assignmentCount].courseIndex = i;
                    assignments[*assignmentCount].roomIndex = room;
                    assignments[*assignmentCount].slotIndex = slot;

                    (*assignmentCount)++;
                    scheduled = 1;
                    break;
                }
            }

            if (scheduled) {
                break;
            }
        }

        if (!scheduled) {
            printf("Course %d could not be scheduled.\n",
                   courses[i].courseId);
        }
    }
}

int backtrackingScheduler(
    int courseIndex,
    struct Course courses[],
    struct Room rooms[],
    struct Assignment assignments[],
    int courseCount,
    int roomCount,
    int slotCount,
    int assignmentCount
) {
    // Base case: all courses are scheduled
    if (courseIndex == courseCount) {
        return 1;
    }

    // Try every time slot
    for (int slot = 0; slot < slotCount; slot++) {

        // Try every room
        for (int room = 0; room < roomCount; room++) {

            // Check whether this assignment is valid
            if (validateAssignment(
                courseIndex, room, slot,
                courses, rooms, assignments,
                assignmentCount, courseCount,
                roomCount, slotCount
            )) {

                assignments[assignmentCount].courseIndex = courseIndex;
                assignments[assignmentCount].roomIndex = room;
                assignments[assignmentCount].slotIndex = slot;

                // Schedule the next course
                if (backtrackingScheduler(
                    courseIndex + 1,
                    courses, rooms, assignments,
                    courseCount, roomCount, slotCount,
                    assignmentCount + 1
                )) {
                    return 1;
                }

                // Undo the assignment if the next course fails
                assignments[assignmentCount].courseIndex = -1;
                assignments[assignmentCount].roomIndex = -1;
                assignments[assignmentCount].slotIndex = -1;
            }
        }
    }

    return 0;
}

// Display the generated timetable
void displayTimetable(
    struct Course courses[],
    struct Room rooms[],
    struct Assignment assignments[],
    int assignmentCount
) {
    printf("\nGenerated Timetable\n");
    printf("--------------------------------------------------\n");
    printf("Course\tFaculty\tStudents\tRoom\tSlot\n");

    for (int i = 0; i < assignmentCount; i++) {
        int c = assignments[i].courseIndex;
        int r = assignments[i].roomIndex;

        printf("%d\t%d\t%d\t\t%d\t%d\n",
               courses[c].courseId,
               courses[c].facultyId,
               courses[c].studentCount,
               rooms[r].roomId,
               assignments[i].slotIndex + 1);
    }

    printf("--------------------------------------------------\n");
    printf("Total scheduled courses: %d\n", assignmentCount);
}

// Enter course and room details
void enterData(
    struct Course courses[],
    struct Room rooms[],
    int *courseCount,
    int *roomCount,
    int *slotCount
) {
    printf("\nEnter Course Details\n");

    *courseCount = readInt("Number of courses: ", 1, MAX_COURSES);

    for (int i = 0; i < *courseCount; i++) {
        printf("\nCourse %d\n", i + 1);

        courses[i].courseId =
            readInt("Course ID: ", 1, 99999);

        courses[i].facultyId =
            readInt("Faculty ID: ", 1, 99999);

        courses[i].studentCount =
            readInt("Number of students: ", 1, 10000);
    }

    printf("\nEnter Room Details\n");

    *roomCount = readInt("Number of rooms: ", 1, MAX_ROOMS);

    for (int i = 0; i < *roomCount; i++) {
        printf("\nRoom %d\n", i + 1);

        rooms[i].roomId =
            readInt("Room ID: ", 1, 99999);

        rooms[i].capacity =
            readInt("Room capacity: ", 1, 10000);
    }

    *slotCount = readInt("\nNumber of time slots: ", 1, MAX_SLOTS);

    printf("\nData entered successfully.\n");
}

int main() {
    struct Course courses[MAX_COURSES];
    struct Room rooms[MAX_ROOMS];
    struct Assignment assignments[MAX_COURSES];

    int courseCount = 0;
    int roomCount = 0;
    int slotCount = 0;
    int assignmentCount = 0;

    int dataEntered = 0;
    int timetableGenerated = 0;
    int choice;

    while (1) {
        printf("\nUniversity Timetable System\n\n");
        printf("1. Enter course and room details\n");
        printf("2. Generate timetable (Greedy)\n");
        printf("3. View timetable\n");
        printf("4. Exit\n");

        choice = readInt("\nEnter your choice: ", 1, 4);

        switch (choice) {

            case 1:
                enterData(
                    courses, rooms,
                    &courseCount, &roomCount, &slotCount
                );

                dataEntered = 1;
                timetableGenerated = 0;
                break;

            case 2:
                if (!dataEntered) {
                    printf("\nPlease enter course and room details first.\n");
                    break;
                }

                greedyScheduler(
                    courses, rooms, assignments,
                    courseCount, roomCount, slotCount,
                    &assignmentCount
                );

                timetableGenerated = 1;
                printf("\nTimetable generation completed.\n");
                break;

            case 3:
                if (!timetableGenerated) {
                    printf("\nPlease generate the timetable first.\n");
                    break;
                }

                displayTimetable(
                    courses, rooms, assignments, assignmentCount
                );
                break;

            case 4:
                printf("\nExiting the system.\n");
                return 0;
        }
    }
}
