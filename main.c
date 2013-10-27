/* 
 * File:   main.c
 * Author: drummer
 *
 * Created on 27. Oktober 2013, 15:18
 */

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#include "archiver.h" // import my header.

// define functions section
void printHelp();
int createNewArchive(char *archiveName);
int writeMagicNumber(int fileDescriptor);
int writeDirectoryOfContents(int fileDescriptor);

/**
 * Main function for the arnew program.
 * The intention of the program is to create a new empty archive.
 * To list files contained by an archive use: ardir
 * To add files to an archive use: aradd
 * To delete files from an archive use: ardel
 * To replace files in an archive use: arrep
 * To clean up an archive removing previous deleted files use: arclean
 **/
int main(int argc, char** argv) {

    if (argc == 1) {
        printf("No archive name was given. See following help for informations\n\n");

        printHelp();
    } else {
        // TODO: check the given file name for max-lenght 255, and consisting only of characters A-Z, a-z, 0-9
        int success = createNewArchive(argv[1]);
        if (success == -1) {
            return EXIT_FAILURE;
        }

        // open archive
        int fileDescriptor = open(argv[1], O_WRONLY | O_APPEND, S_IRUSR | S_IRGRP);
        if (fileDescriptor == -1) {
            printf("The open operation for the archive failed.");
            return EXIT_FAILURE;
        }
        int result = writeMagicNumber(fileDescriptor);
        if (result == -1) {
            printf("An error occured when writing the magic number to the archive.");
            return EXIT_FAILURE;
        }

        result = writeDirectoryOfContents(fileDescriptor);
        if (result == -1) {
            printf("An error occured when writing the directory of contents to the archive.");
            return EXIT_FAILURE;
        }

        close(fileDescriptor);
    }
    return (EXIT_SUCCESS);
}

int createNewArchive(char *archiveName) {
    /*
        printf("Debug: Given archiveName: %s\n", archiveName);

        printf("ReadByOwner: %o\n", S_IRUSR);
        printf("WriteByOwner: %o\n", S_IWUSR);
        printf("ExecuteByOwner: %o\n", S_IXUSR);
        printf("ReadByGroup: %o\n", S_IRGRP);
        printf("WriteByGroup: %o\n", S_IWGRP);
        printf("ExecuteByGroup: %o\n", S_IXGRP);

        printf("Combindes: %o\n", S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP);
     */

    // Awww http://stackoverflow.com/questions/12489525/file-permission-issue-with-open-system-call-linux

    int fileDescriptor = creat(archiveName, S_IWUSR | S_IRUSR);

    if (fileDescriptor == -1) {
        printf("An error occured, when creating a new archive.");
        return fileDescriptor;
        // in case of -1 an error occured when creating the new file.
    }
    close(fileDescriptor);

    return 0;
}

void printHelp() {
    printf("The syntax for the program is: <program> <archive name> [<parameter>]");
    printf("\nFor example: ./arnew anArchive ");
}

int writeMagicNumber(int fileDescriptor) {
    char* magicNumber = MAGIC_NUMBER_ARCHIVE;

    return write(fileDescriptor, magicNumber, sizeof (magicNumber));
}

int writeDirectoryOfContents(int fileDescriptor) {

    // as default we create a directory of contents for 16 entries. The last entry is always EOA or Continue, depending on further files following or not.
    int result;

    // http://stackoverflow.com/questions/12489525/file-permission-issue-with-open-system-call-linux

    int i;
    for (i = 0; i < 16; i++) {
        Archive_Index index;
        index.bytePositionInArchive = 0;
        index.fileName = "";
        index.fileType;
        index.gid;
        index.lastAccessTime;
        index.sizeInBytes = 0;
        if (i == 15) {
            index.state = EOA;
        } else {
            index.state = FREE;
        }
        index.uid = 0;

        /*
                result = write(fileDescriptor, (Archive_Index*) &index, sizeof (index));
         */
        char *string = "SomeString"; // Teststring
        result = write(fileDescriptor, (char*) string, sizeof (string));
        if (result == -1) {
            break;
        }
    }
    return result;
}
