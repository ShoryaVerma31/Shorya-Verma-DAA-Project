
#include <stdio.h>

// Course details
struct Course {
    int courseId;
    int facultyId;
    int studentCount;
};

// Room details
struct Room {
    int roomId;
    int capacity;
};

// Timetable entry
struct Assignment {
    int courseIndex;
    int roomIndex;
    int slotIndex;
};

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
    int roomCount
) {
    if (courseIndex < 0 || courseIndex >= courseCount ||
        roomIndex < 0 || roomIndex >= roomCount) {
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

        // Try each time slot
        for (int slot = 0; slot < slotCount; slot++) {

            // Try each room
            for (int room = 0; room < roomCount; room++) {

                if (validateAssignment(
                    i, room, slot,
                    courses, rooms, assignments,
                    *assignmentCount, courseCount, roomCount
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

// Display the generated timetable
void displayTimetable(
    struct Course courses[],
    struct Room rooms[],
    struct Assignment assignments[],
    int assignmentCount
) {
    printf("\nGenerated Timetable\n");
    printf("-------------------------------\n");

    printf("Course\tRoom\tSlot\n");

    for (int i = 0; i < assignmentCount; i++) {
        int c = assignments[i].courseIndex;
        int r = assignments[i].roomIndex;

        printf("%d\t%d\t%d\n",
               courses[c].courseId,
               rooms[r].roomId,
               assignments[i].slotIndex);
    }

    printf("-------------------------------\n");
    printf("Total scheduled courses: %d\n", assignmentCount);
}

int main() {

    // Sample courses
    struct Course courses[3] = {
        {101, 1, 40},
        {102, 2, 35},
        {103, 1, 25}
    };

    // Available rooms
    struct Room rooms[3] = {
        {201, 30},
        {202, 40},
        {203, 50}
    };

    struct Assignment assignments[10];

    int courseCount = 3;
    int roomCount = 3;
    int slotCount = 3;
    int assignmentCount = 0;

    printf("University Timetable System\n\n");

    // Generate timetable using Greedy
    greedyScheduler(
        courses,
        rooms,
        assignments,
        courseCount,
        roomCount,
        slotCount,
        &assignmentCount
    );

    // Display the result
    displayTimetable(
        courses,
        rooms,
        assignments,
        assignmentCount
    );

    return 0;
}
