
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
    // Check indexes
    if (courseIndex < 0 || courseIndex >= courseCount ||
        roomIndex < 0 || roomIndex >= roomCount) {
        printf("Invalid course or room selection.\n");
        return 0;
    }

    // Check room size
    if (rooms[roomIndex].capacity <
        courses[courseIndex].studentCount) {
        printf("Room capacity is insufficient.\n");
        return 0;
    }

    // Check previous timetable entries
    for (int i = 0; i < assignmentCount; i++) {

        // Same time slot?
        if (assignments[i].slotIndex == slotIndex) {

            // Is the room already occupied?
            if (assignments[i].roomIndex == roomIndex) {
                printf("Room clash detected.\n");
                return 0;
            }

            // Is the faculty member already teaching?
            int existingCourse = assignments[i].courseIndex;

            if (courses[existingCourse].facultyId ==
                courses[courseIndex].facultyId) {
                printf("Faculty clash detected.\n");
                return 0;
            }
        }
    }

    return 1;
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

    // Store scheduled classes
    struct Assignment assignments[10];

    int courseCount = 3;
    int roomCount = 3;
    int assignmentCount = 0;

    printf("       UNIVERSITY TIMETABLE SYSTEM\n");

    // Test an assignment
    int courseIndex = 1;
    int roomIndex = 1;
    int slotIndex = 0;

    printf("\nChecking proposed assignment...\n");

    if (validateAssignment(
        courseIndex,
        roomIndex,
        slotIndex,
        courses,
        rooms,
        assignments,
        assignmentCount,
        courseCount,
        roomCount
    )) {

        assignments[assignmentCount].courseIndex = courseIndex;
        assignments[assignmentCount].roomIndex = roomIndex;
        assignments[assignmentCount].slotIndex = slotIndex;

        assignmentCount++;

        printf("Assignment accepted successfully.\n");

    } else {
        printf("Assignment rejected.\n");
    }

    printf("\nTotal scheduled courses: %d\n", assignmentCount);

    return 0;
}
