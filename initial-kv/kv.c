#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define MAX_LINE_LENGTH 100
#define MAX_ARRAY_SIZE 3
#define MAX_ARGS 3
#define MAX_ARG_LEN 200

int getAll(const char *filePath){
    //iterate through the file printing out each line
    FILE *file;
    file = fopen(filePath, "w");
    char line[MAX_LINE_LENGTH];
    if (file == NULL) {
        perror("Error opening file");
        return 0;
    }
    while (fgets(line, sizeof(line), file)) {
          printf("%s", line);
    }
    fclose(file);
    return 1;
}

int clearFile(const char *filePath){
    FILE *file;
    file = fopen(filePath, "w");
    
    if (file == NULL) {
        perror("Error opening file");
        return 0;
    }
    fclose(file);
    return 1;
}

int deleteEntry(const char *filePath, const char *key){
FILE *file, *tempFile;
    char line[MAX_LINE_LENGTH];
    char *foundKey;
    int isDeleted = 0;
    char tempFileName[] = "tempfile.tmp";

    // Open the original file for reading
    file = fopen(filePath, "r");
    if (file == NULL) {
        perror("Error opening file");
        return 0;
    }

    // Open a temporary file for writing
    tempFile = fopen(tempFileName, "w");
    if (tempFile == NULL) {
        perror("Error opening temporary file");
        fclose(file);
        return 0;
    }

    while (fgets(line, sizeof(line), file)) {
        foundKey = strtok(line, ","); // Get the first token as the key
        if (foundKey != NULL && strcmp(foundKey, key) != 0) {
            // If the current line does not contain the key, write it to the temp file
            fputs(line, tempFile);
        } else {
            // Mark as deleted if the key matches
            isDeleted = 1;
        }
    }

    // Close both files
    fclose(file);
    fclose(tempFile);

    // Delete the original file and rename the temporary file if a line was deleted
    if (isDeleted) {
        remove(filePath); // Delete the original file
        rename(tempFileName, filePath); // Rename the temp file to the original file's name
        return 1; // Return success
    } else {
        remove(tempFileName); // No need to replace the file, just remove the temp file
    }

    return 0; // No line was deleted
}

int getEntry(const char *filePath, const char *key){
    //read file matching the key to the input string and finishing if a match is found delimiting by "," and "\n"
    //if no match is found return 0
    //if a match is found return 1 and copy the line
    FILE *file;
    char line[MAX_LINE_LENGTH];
    char *fileKey, *fileValue;
    
    file = fopen(filePath, "r");
    if (file == NULL) {
        perror("Error opening file");
        return 0;
    }
    while (fgets(line, sizeof(line), file)){
          fileKey = strtok(line, ",");
          if (fileKey != NULL && strcmp(key, fileKey) == 0)
          {
           fclose(file);
           strcpy(line, sizeof(line));
           return 1;
          }
    }
    fclose(file);
    return 0;
}

int putEntry(const char *filePath, const char *key, const char *value){
    int entryExists = getEntry(filePath, key);
    if(entryExists == 1){
        deleteEntry(filePath, key);
    }
    FILE *file;

    file = fopen(filePath, "a");
    if (file == NULL) {
        perror("Error opening file");
        return 0;
    }
         
    fprintf(file, "%s,%s\n", key, value);
    fclose(file);
    return 1;
}

int main(int argc, char *argv[]){
    const char *filePath = "map.txt";

    if (argc < 2) {
        printf("Usage: %s \"arg1,arg2,arg3\"\n", argv[0]);
        return 1;
    }

    char *args[MAX_ARGS];
    char input[MAX_ARG_LEN];
    strncpy(input, argv[1], MAX_ARG_LEN - 1);
    input[MAX_ARG_LEN - 1] = '\0'; // Ensure null-termination

    char *token = strtok(input, ",");
    int i = 0;
    while (token != NULL && i < MAX_ARGS) {
        args[i] = token;
        i++;
        token = strtok(NULL, ",");
    }
    
    char *arg = args[0];
    char *key = args[1];
    char *value = args[2];
    
    switch (arg) {
        case 'p':
             putEntry(filePath, key, value);
             break;
        case 'g':
             char result[MAX_LINE_LENGTH];
             if(filePath, getEntry(filePath, key), result){
                 printf(result);
             }
             break;
        case 'a':
             getAll(filePath);
             break;
        case 'c':
             clearFile(filePath);
             break;
        case 'd':
             deleteEntry(filePath, key);
             break;
        default:
            printf("bad command\n");
            break;
    }
    return 0;
}

