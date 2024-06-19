#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NOTES 100
#define MAX_TITLE_LENGTH 50
#define MAX_CONTENT_LENGTH 1000
#define FILENAME "notes.txt"

// Structure to represent a note
struct Note {
    char title[MAX_TITLE_LENGTH];
    char content[MAX_CONTENT_LENGTH];
};

// Function prototypes
void displayMenu();
void createNote();
void viewNotes();
void editNote();
void deleteNote();
void clearBuffer();

int main() {
    int choice;

    while (1) {
        displayMenu();
        printf("Enter your choice: ");
        scanf("%d", &choice);
        clearBuffer(); // Clear stdin buffer

        switch (choice) {
            case 1:
                createNote();
                break;
            case 2:
                viewNotes();
                break;
            case 3:
                editNote();
                break;
            case 4:
                deleteNote();
                break;
            case 5:
                printf("Exiting program.\n");
                exit(EXIT_SUCCESS);
            default:
                printf("Invalid choice. Please enter again.\n");
                break;
        }
    }

    return 0;
}

// Display menu options
void displayMenu() {
    printf("\n1. Create a new note\n");
    printf("2. View all notes\n");
    printf("3. Edit a note\n");
    printf("4. Delete a note\n");
    printf("5. Exit\n");
}

// Create a new note and save to file
void createNote() {
    struct Note note;
    FILE *file = fopen(FILENAME, "a");
    if (file == NULL) {
        perror("Error opening file");
        return;
    }

    printf("\nEnter title (max %d characters): ", MAX_TITLE_LENGTH - 1);
    fgets(note.title, MAX_TITLE_LENGTH, stdin);
    note.title[strcspn(note.title, "\n")] = '\0'; // Remove newline character

    printf("Enter content (max %d characters):\n", MAX_CONTENT_LENGTH - 1);
    fgets(note.content, MAX_CONTENT_LENGTH, stdin);
    note.content[strcspn(note.content, "\n")] = '\0'; // Remove newline character

    fprintf(file, "%s\n%s\n", note.title, note.content);
    fclose(file);

    printf("Note created successfully.\n");
}

// View all saved notes
void viewNotes() {
    struct Note notes[MAX_NOTES];
    FILE *file = fopen(FILENAME, "r");
    if (file == NULL) {
        perror("Error opening file");
        return;
    }

    int count = 0;
    while (count < MAX_NOTES && fgets(notes[count].title, MAX_TITLE_LENGTH, file) != NULL) {
        notes[count].title[strcspn(notes[count].title, "\n")] = '\0'; // Remove newline character
        fgets(notes[count].content, MAX_CONTENT_LENGTH, file);
        notes[count].content[strcspn(notes[count].content, "\n")] = '\0'; // Remove newline character
        count++;
    }

    fclose(file);

    if (count == 0) {
        printf("No notes found.\n");
    } else {
        printf("\n--- All Notes ---\n");
        for (int i = 0; i < count; ++i) {
            printf("Title: %s\n", notes[i].title);
            printf("Content: %s\n", notes[i].content);
            printf("-----------------\n");
        }
    }
}

// Edit an existing note
void editNote() {
    struct Note notes[MAX_NOTES];
    FILE *file = fopen(FILENAME, "r+");
    if (file == NULL) {
        perror("Error opening file");
        return;
    }

    int count = 0;
    while (count < MAX_NOTES && fgets(notes[count].title, MAX_TITLE_LENGTH, file) != NULL) {
        notes[count].title[strcspn(notes[count].title, "\n")] = '\0'; // Remove newline character
        fgets(notes[count].content, MAX_CONTENT_LENGTH, file);
        notes[count].content[strcspn(notes[count].content, "\n")] = '\0'; // Remove newline character
        count++;
    }

    if (count == 0) {
        printf("No notes found.\n");
        fclose(file);
        return;
    }

    char titleToFind[MAX_TITLE_LENGTH];
    printf("\nEnter the title of the note to edit: ");
    fgets(titleToFind, MAX_TITLE_LENGTH, stdin);
    titleToFind[strcspn(titleToFind, "\n")] = '\0'; // Remove newline character

    int found = 0;
    for (int i = 0; i < count; ++i) {
        if (strcmp(notes[i].title, titleToFind) == 0) {
            printf("Enter new content for the note:\n");
            fgets(notes[i].content, MAX_CONTENT_LENGTH, stdin);
            notes[i].content[strcspn(notes[i].content, "\n")] = '\0'; // Remove newline character

            fseek(file, 0, SEEK_SET);
            for (int j = 0; j < count; ++j) {
                fprintf(file, "%s\n%s\n", notes[j].title, notes[j].content);
            }
            printf("Note edited successfully.\n");
            found = 1;
            break;
        }
    }

    if (!found) {
        printf("Note with title '%s' not found.\n", titleToFind);
    }

    fclose(file);
}

// Delete a note
void deleteNote() {
    struct Note notes[MAX_NOTES];
    FILE *file = fopen(FILENAME, "r+");
    if (file == NULL) {
        perror("Error opening file");
        return;
    }

    int count = 0;
    while (count < MAX_NOTES && fgets(notes[count].title, MAX_TITLE_LENGTH, file) != NULL) {
        notes[count].title[strcspn(notes[count].title, "\n")] = '\0'; // Remove newline character
        fgets(notes[count].content, MAX_CONTENT_LENGTH, file);
        notes[count].content[strcspn(notes[count].content, "\n")] = '\0'; // Remove newline character
        count++;
    }

    if (count == 0) {
        printf("No notes found.\n");
        fclose(file);
        return;
    }

    char titleToFind[MAX_TITLE_LENGTH];
    printf("\nEnter the title of the note to delete: ");
    fgets(titleToFind, MAX_TITLE_LENGTH, stdin);
    titleToFind[strcspn(titleToFind, "\n")] = '\0'; // Remove newline character

    int found = 0;
    FILE *tempFile = fopen("temp.txt", "w");
    if (tempFile == NULL) {
        perror("Error opening temp file");
        fclose(file);
        return;
    }

    for (int i = 0; i < count; ++i) {
        if (strcmp(notes[i].title, titleToFind) == 0) {
            printf("Note with title '%s' deleted.\n", titleToFind);
            found = 1;
        } else {
            fprintf(tempFile, "%s\n%s\n", notes[i].title, notes[i].content);
        }
    }

    fclose(file);
    fclose(tempFile);

    // Rename temp file to original file
    if (remove(FILENAME) != 0) {
        perror("Error deleting original file");
        return;
    }
    if (rename("temp.txt", FILENAME) != 0) {
        perror("Error renaming temp file");
        return;
    }

    if (!found) {
        printf("Note with title '%s' not found.\n", titleToFind);
    }
}

// Utility function to clear input buffer
void clearBuffer() {
    while (getchar() != '\n');
}
