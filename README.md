# Text-Based-Note-Taking-Application-in-c
 This application will allow users to create, view, edit, and delete notes stored in a file. It involves file handling, basic data structures, and user interaction through console input/output.


 Explanation:
Struct: struct Note is defined to represent a note with a title (title) and content (content).

File Handling: Notes are stored in a text file (notes.txt). Functions like createNote, viewNotes, editNote, and deleteNote use file operations (fopen, fprintf, fgets) to manipulate notes.

Menu System: The main function displays a menu with options (create, view, edit, delete, exit) and uses switch statements to handle user input.

Input Handling: Functions like fgets are used to handle user input for titles and content of notes, ensuring buffer overflow protection.

Error Handling: Basic error handling using perror to handle file opening errors and other potential issues.
