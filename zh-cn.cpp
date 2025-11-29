/*
 * FAT Detection Tool (GUI Version - Fixed Parameter Parsing)
 * Uses __argc / __argv (MinGW Feature)
 * Compatible with NT3.1 ~ Win7 (including 181MB minimal version)
 */

#include <windows.h>
#include <stdio.h>
#include <string.h>

// Declare MinGW's argc/argv (available in GUI programs)
extern int __argc;
extern char** __argv;

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrev, LPSTR lpCmdLine, int nCmdShow) {
    if (__argc != 2) {
        MessageBoxA(NULL,
            "Usage: fatdetect C\n(Please specify a drive letter like C or D)",
            "Error", MB_ICONERROR | MB_OK);
        return 1;
    }

    char* arg = __argv[1];
    if (strlen(arg) == 0) {
        MessageBoxA(NULL, "Drive letter cannot be empty!", "Error", MB_ICONERROR | MB_OK);
        return 1;
    }

    char drive_char = (char)toupper((unsigned char)arg[0]);
    if (drive_char < 'A' || drive_char > 'Z') {
        MessageBoxA(NULL, "Invalid drive letter! Please enter a letter between A-Z.", "Error", MB_ICONERROR | MB_OK);
        return 1;
    }

    char rootPath[4] = { drive_char, ':', '\\', '\0' };
    char fsName[32] = {0};

    if (!GetVolumeInformationA(rootPath, NULL, 0, NULL, NULL, NULL, fsName, sizeof(fsName))) {
        MessageBoxA(NULL, "Unable to access this drive!", "Error", MB_ICONERROR | MB_OK);
        return 1;
    }

    int isFAT = 
        (strcmp(fsName, "FAT") == 0) ||
        (strcmp(fsName, "FAT12") == 0) ||
        (strcmp(fsName, "FAT16") == 0) ||
        (strcmp(fsName, "FAT32") == 0);

    char msg[256];
    if (!isFAT) {
        wsprintfA(msg, "Drive: %c:\nFile System: Non-FAT File System", drive_char);
    } else {
        DWORD sectorsPerCluster, bytesPerSector, freeClusters, totalClusters;
        if (!GetDiskFreeSpaceA(rootPath, &sectorsPerCluster, &bytesPerSector, &freeClusters, &totalClusters)) {
            wsprintfA(msg, "Drive: %c:\nFile System: FAT (Subtype undetermined)", drive_char);
        } else {
            const char* fatType;
            if (totalClusters < 4085) fatType = "FAT12";
            else if (totalClusters <= 65524) fatType = "FAT16";
            else fatType = "FAT32";
            wsprintfA(msg, "Drive: %c:\nFile System: %s", drive_char, fatType);
        }
    }

    MessageBoxA(NULL, msg, "FAT Detection Result", MB_ICONINFORMATION | MB_OK);
    return 0;
}
