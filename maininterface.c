#include "Include/raylib.h"
#define RAYGUI_IMPLEMENTATION
#include "Include/raygui.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "ms.h"
#include "md.h"
#include "fm.h"

// Enum for different states in the application
typedef enum {
    LOADING_SCREEN,
    INIT_MEMOIRE_SCREEN,
    INIT_MEMOIRE_SCREEN2,
    MAIN_MENU,
    CREATE_FILE,
    FILE_MENU,
    MEMOIRE_SECONDAIRE_STATE
} AppState;

// Structure to hold file information
typedef struct {
    char fileName[50];     // File name (max 50 characters)
    int numberOfRecords;   // Number of records in the file
    int organizationGlobaleType;
    int organizationInterneType;  // Organization type (1 for Global, 2 for Internal)
} FileForm;

// Structure to hold ms information
typedef struct {
    int blockingfactor;   // Number of records in the file
    int disknumber;       // Organization type (1 for Global, 2 for Internal)
} msForm;
// Define a structure for a dynamic array of strings
typedef struct {
    void* data;
    size_t elementSize;
    size_t size;
    size_t capacity;
} DynamicArray;
// Function prototypes
void DrawFormStep(int step, FileForm *fileForm, bool *formSubmitted,int screenWidth);
void DrawFormStep2(int step, msForm *msForm,int screenWidth);
void DrawButton(Rectangle button, const char* text, bool isPressed);
void initArray(DynamicArray* array, size_t elementSize, size_t initialCapacity);
void insertArray(DynamicArray* array, void* element);
void freeArray(DynamicArray* array);
void* getArrayElement(DynamicArray* array, size_t index);

// Custom function definitions for menu actions
void setraylib (){






    // Convert Color to int (hexadecimal color codes)
    int darkBlue = 0x00008BFF; // Equivalent to DARKBLUE in hexadecimal
    int darkGray = 0xA9A9A9FF; // Equivalent to DARKGRAY in hexadecimal
    int rayWhite = 0xFFFFFFFF; // Equivalent to RAYWHITE in hexadecimal
    int lightGray = 0xD3D3D3FF; // Equivalent to LIGHTGRAY in hexadecimal
    int gray = 0x808080FF;
    int black = 0x000000FF; // Equivalent to GRAY in hexadecimal

    // Set custom styles



    GuiSetStyle(DEFAULT, TEXT_SIZE, 20); // Set default text size for all GUI elements

    GuiSetStyle(BUTTON, TEXT_COLOR_NORMAL, black);  // Set text color for buttons in normal state
    GuiSetStyle(BUTTON, TEXT_COLOR_FOCUSED, darkGray); // Set text color for buttons in focused state
    GuiSetStyle(BUTTON, TEXT_COLOR_PRESSED, rayWhite); // Set text color for buttons in pressed state
    GuiSetStyle(BUTTON, TEXT_COLOR_DISABLED, lightGray); // Set text color for buttons in disabled state

    GuiSetStyle(BUTTON, BASE_COLOR_NORMAL, lightGray);  // Set background color for buttons in normal state
    GuiSetStyle(BUTTON, BASE_COLOR_FOCUSED, gray);      // Set background color for buttons in focused state
    GuiSetStyle(BUTTON, BASE_COLOR_PRESSED, darkGray);  // Set background color for buttons in pressed state
    GuiSetStyle(BUTTON, BASE_COLOR_DISABLED, darkGray); // Set background color for buttons in disabled state

}
void RechercherFunction() {
    printf("Rechercher function triggered!\n");
}

void InsererFunction() {
    printf("Inserer function triggered!\n");

}

void SupprimerFunction() {
    printf("Supprimer function triggered!\n");
}

void RenommerFunction() {
    printf("Renommer function triggered!\n");
}

void BackFunction() {
    printf("Back function triggered!\n");
}

void CompacterFunction() {
    printf("Compacter function triggered!\n");
}

void ViderFunction() {
    printf("Vider function triggered!\n");
}

int selectedFileIndex = -1;
int main() {

    //declaration back and :
    FILE * ms;
    Meta meta;
    Disk D;
    bool * b;
    Block buffer;
    Meta mymeta;
    ms=fopen("mydata.bin","wb+");
    // Initialization
    int screenWidth = 1280;
    const int screenHeight = 720;
    InitWindow(screenWidth, screenHeight, "PROJECT SFSD");
    setraylib();
    //interface variable
    FileForm fileForm = { "", 0, 0 };//file form(name....)
    msForm msForm = { 0, 0 };//disk form
    // Set initial state to Loading Screen
    AppState currentState = LOADING_SCREEN;

    // Initialize the dynamic arrays for storing created files and file buttons
    DynamicArray createdFiles;
    initArray(&createdFiles, sizeof(char*), 4);
    DynamicArray fileButtons;
    initArray(&fileButtons, sizeof(Rectangle), 4);

    // Timer for loading screen
    float loadingTime = 2.0f,elapsedTime = 0.0f;  // 2 seconds for loading screen


       // Form state tracking
    int i=0;//number of file buttons in one line
    int formStep = 0;
int formStep2 = 0; // To track form input step (file name, number of records, etc.)
    bool formSubmitted = false;


    // Main game loop
    while (!WindowShouldClose()) {
        // Update
        if (currentState == LOADING_SCREEN) {
            elapsedTime += GetFrameTime();
            if (elapsedTime >= loadingTime) currentState = INIT_MEMOIRE_SCREEN;
        } else if (currentState == INIT_MEMOIRE_SCREEN) {
            if (GuiButton((Rectangle){ screenWidth / 2 - 200, screenHeight / 2 - 50, 400, 100 }, "INIT MEMOIRE SECONDAIRE")) {
                printf("INIT MEMOIRE SECONDAIRE triggered!\n");
                currentState = INIT_MEMOIRE_SCREEN2;
            int formStep2 = 0; // To track form input step (file name, number of records, etc.)
            msForm.blockingfactor=0;
            msForm.disknumber=0;
            }
        } else if (currentState == INIT_MEMOIRE_SCREEN2) {
            DrawFormStep2(formStep2, &msForm, screenWidth);
            // Logic to move to the next step
            if (formStep2 == 0 && msForm.blockingfactor > 0) {
                formStep2++;
            } else if (formStep2 == 1 && msForm.disknumber > 0) {
                // Form submission logic here
                //from my var to theirs
                D.bf=msForm.blockingfactor;
                D.blocks=msForm.disknumber;
                printf("MS CREATED:\n");
                printf("BLOCKING FACTOR : %d\n", D.bf);
                printf("Number of blocks: %d\n", D.blocks);
                InitializeDisk(ms,D);
                InitializeBlock(D, &buffer);
                 for(int i=0;i<D.blocks;i++) {
        Display_Block(i,ms,D,&buffer);
        printf("the block number %d : \n",i);
        for(int j=0;j<D.bf;j++){
            printf("   name : %s  ID : %d group : %d deleted ? : %d \n",buffer.student[j].name,buffer.student[j].ID,buffer.student[j].group,buffer.student[j].deleted);
        }
        printf("the next block is %d \n",buffer.next);

    }
                formStep2=0;
                currentState = MAIN_MENU;  // Go back to main menu
            }

        } else if (currentState == MAIN_MENU) {
            if (GuiButton((Rectangle){ screenWidth / 2 - 200, 220, 400, 100 }, "MEMOIRE SECONDAIRE")) {
                currentState = MEMOIRE_SECONDAIRE_STATE;
            }
            if (GuiButton((Rectangle){ screenWidth / 2 - 200, 340, 400, 100 }, "CREER UN FICHIER")) {
                currentState = CREATE_FILE;
            }
            // Draw file buttons if any
            for (size_t i = 0; i < fileButtons.size; i++) {
                Rectangle *button = (Rectangle *)getArrayElement(&fileButtons, i);
                char *fileName = *(char **)getArrayElement(&createdFiles, i);
                if (GuiButton(*button, fileName)) {
                    currentState = FILE_MENU;
                    selectedFileIndex = i; // Track selected file
                }
            }
        } else if (currentState == MEMOIRE_SECONDAIRE_STATE) {
            if (GuiButton((Rectangle){ (screenWidth-300) / 2 , 200, 300, 50 }, "Compacter")) {
                compactage(ms,D);
                CompacterFunction();
            }
            if (GuiButton((Rectangle){ screenWidth / 2 - 150, 300, 300, 50 }, "Vider")) {
                ViderFunction();
                Empty_MS(ms,D);
                currentState =INIT_MEMOIRE_SCREEN;
            }
            if (GuiButton((Rectangle){ screenWidth / 2 - 225, 400, 450, 50 }, "ETAT DE LA MEMOIRE SECONDAIRE")) {
                printf("ETAT DE LA MEMOIRE SECONDAIRE triggered!\n");
            }
            if (GuiButton((Rectangle){ screenWidth / 2 - 150, 500, 300, 50 }, "Back")) {
                BackFunction();
                currentState = MAIN_MENU;  // Return to main menus
            }
        } else if (currentState == CREATE_FILE) {
            DrawFormStep(formStep, &fileForm, &formSubmitted , screenWidth); // Draw file creation form
             // Logic to move to the next step
            if (formStep == 0 && strlen(fileForm.fileName) > 0) {
                //filename
                formStep++;

            } else if (formStep == 1 && fileForm.numberOfRecords > 0) {
                formStep++;
            }
             else if (formStep == 2 && fileForm.organizationGlobaleType> 0) {
                formStep++;
            }else if (formStep == 3 && fileForm.organizationInterneType > 0) {
                formSubmitted = true;

        }


            // Handle form submission
            if (formSubmitted) {
               DisplayArray(&createdFiles);

                // Add new file to created files list
                char* newFileName = strdup(fileForm.fileName);
                Meta m;
                 strcpy(m.nomF, fileForm.fileName);
                m.tailleEnBlock=fileForm.numberOfRecords;
                m.orgGlobal=fileForm.organizationGlobaleType;
                m.orgInterne=fileForm.organizationInterneType;

                bool tst =creatFile(ms,&D,&m);
                if(tst){

                insertArray(&createdFiles, &newFileName);
                }

                // Add button for the new file in the file menu
                Rectangle newButton;

                if (i < 4) {
                    newButton = (Rectangle){ (screenWidth / 2 - 300) + 100 * (createdFiles.size), 450, 100.0f, 50.0f };
                    i++;
                } else {
                    newButton = (Rectangle){ (screenWidth / 2 - 700) + 100 * (createdFiles.size), 550, 100.0f, 50.0f };
                    i++;
                }
                insertArray(&fileButtons, &newButton);

                // Reset the form for next file creation
                fileForm = (FileForm){ "", 0,0 ,0 };
                formStep = 0;
                formSubmitted = false;

                currentState = MAIN_MENU;  // Go back to main menu
            }


        } else if (currentState == FILE_MENU) {
            if (GuiButton((Rectangle){ (screenWidth-200) / 2, 200, 200, 50 }, "Rechercher")) { RechercherFunction();
             int ID=1;
             printf("enter the students ID :");
        scanf("%d",&ID);
            posStudent t1 =  searchStudentID(ms,  D,  meta,  ID) ;

             printf("num block : %d",t1.numBlock);
             printf("deplacement : %d",t1.deplacement);

            }
            if (GuiButton((Rectangle){ (screenWidth-200) / 2, 260, 200, 50 }, "Inserer")) { InsererFunction();
            meta=readMeta(ms,D,1);
            Student st;
    for(int i =0;i<3;i++) {

        printf("enter the students name : ");
        scanf("%s",&st.name);
        printf("enter the students group :");
        scanf("%d",&st.group);
        printf("enter the students ID :");
        scanf("%d",&st.ID);
        st.deleted=false;

        insertStudent(ms,D,st,&meta);
        for(int i=0;i<D.blocks;i++) {
        Display_Block(i,ms,D,&buffer);
        printf("the block number %d : \n",i);
        for(int j=0;j<D.bf;j++){
            printf("   name : %s  ID : %d group : %d deleted ? : %d \n",buffer.student[j].name,buffer.student[j].ID,buffer.student[j].group,buffer.student[j].deleted);
        }
        printf("the next block is %d \n",buffer.next);

    }


    } }
            if (GuiButton((Rectangle){ (screenWidth-200) / 2, 320, 200, 50 }, "Supprimer")) {
                int pos =0;
                char name [20];
            printf("position of file ");
            scanf("%d",&pos);
            printf("enter the students name : ");
        scanf("%s",&name);

                deleteFile(ms,&D,name);
                currentState = MAIN_MENU;

                    SupprimerFunction(); }
            if (GuiButton((Rectangle){ (screenWidth-200) / 2, 380, 200, 50 }, "Renommer")) { RenommerFunction(); }
            if (GuiButton((Rectangle){ (screenWidth-230) / 2, 440, 230, 50 }, "Load file")) {
                    int pos =0;
            printf("position of file ");
            scanf("%d",&pos);
            LoadFile(ms,D,pos);
            currentState = MAIN_MENU;

              }
            if (GuiButton((Rectangle){ (screenWidth-200) / 2, 500, 200, 50 }, "Retour")) {
                BackFunction();
                currentState = MAIN_MENU;
            }
        }

        // Draw the current screen
        BeginDrawing();
        ClearBackground(LIGHTGRAY); // Windows-like background

        // Display current state screen
        if (currentState == LOADING_SCREEN) {
            DrawText("Welcome to our project", screenWidth / 2 - MeasureText("Welcome to our project", 40) / 2, screenHeight / 2 - 20, 40, DARKBLUE);
        } else if (currentState == INIT_MEMOIRE_SCREEN) {
            DrawText("INIT MEMOIRE SECONDAIRE", screenWidth / 2 - MeasureText("INIT MEMOIRE SECONDAIRE", 40) / 2, 100, 40, DARKGRAY);
        } else if (currentState == INIT_MEMOIRE_SCREEN2) {
            DrawText("CREATE MEMOIRE", screenWidth / 2 - MeasureText("CREATE MEMOIRE", 40) / 2, 100, 40, DARKGRAY);
        } else if (currentState == MAIN_MENU) {
            DrawText("MAIN MENU", screenWidth / 2 - MeasureText("MAIN MENU", 40) / 2, 100, 40, DARKGRAY);
        } else if (currentState == FILE_MENU) {
            DrawText("File Menu", screenWidth / 2 - MeasureText("File Menu", 40) / 2, 100, 40, DARKGRAY);
        }

        EndDrawing();
    }

     // Cleanup
    for (size_t i = 0; i < createdFiles.size; i++) {
        free(*(char **)getArrayElement(&createdFiles, i));
    }
    freeArray(&createdFiles);
    freeArray(&fileButtons);

    // Close the window and exit

    CloseWindow();
    return 0;
}
//display:
void DisplayArray(DynamicArray *array) {
    printf("Array elements:\n");
    for (size_t i = 0; i < array->size; i++) {
        FileForm *file = (FileForm *)getArrayElement(array, i);
        printf("File Name: %s\n", file->fileName);
        printf("Number of Records: %d\n", file->numberOfRecords);
        printf("Global Organization Globale Type: %s\n", (file->organizationGlobaleType == 1) ? "CHAINED" : "CONTIGUOUS");
        printf("Internal Organization interne Type: %s\n", (file->organizationInterneType== 1) ? "ORDONNE" : "NON ORDONNE");
        printf("\n");
    }
}

// Draw the current step of the file creation form
void DrawFormStep(int step, FileForm *fileForm, bool *formSubmitted,int screenWidth) {
    static char fileName[100] = {0};
    static char numRecords[100] = {0};
    static bool nameEditMode = false;
    static bool numRecordsEditMode = false;

    if (step == 0) {
        GuiLabel((Rectangle){ (screenWidth-400) / 2, 200, 400, 20 }, "Enter file name:");
        if (GuiTextBox((Rectangle){ (screenWidth-500) / 2, 230, 600, 40 }, fileName, 100, nameEditMode)) {
            nameEditMode = !nameEditMode;
        }
        if (GuiButton((Rectangle){ screenWidth/2+300, 230, 100, 40 }, "Submit")) {
            strncpy(fileForm->fileName, fileName, sizeof(fileForm->fileName) - 1);
            fileForm->fileName[sizeof(fileForm->fileName) - 1] = '\0'; // Ensure null termination
            nameEditMode = false;
            memset(fileName, 0, sizeof(fileName)); // Clear the input string
        }
    } else if (step == 1) {
        GuiLabel((Rectangle){ (screenWidth-400) / 2, 200, 400, 20 }, "Enter number of blocks:");
        if (GuiTextBox((Rectangle){ (screenWidth-500) / 2, 230, 600, 40 }, numRecords, 100, numRecordsEditMode)) {
            numRecordsEditMode = !numRecordsEditMode;
        }
        if (GuiButton((Rectangle){ screenWidth/2+300, 230, 100, 40 }, "Submit")) {
            fileForm->numberOfRecords = atoi(numRecords);
            numRecordsEditMode = false;
            memset(numRecords, 0, sizeof(numRecords)); // Clear the input string
        }
    } else if (step == 2) {
        GuiLabel((Rectangle){ (screenWidth-300) / 2, 200, 400, 20 }, "Choose organization Global type:");
        if (GuiButton((Rectangle){ (screenWidth-200) / 2, 230, 200, 40 }, "1. CHAINED")) {
            fileForm->organizationGlobaleType = 1;
        }
        if (GuiButton((Rectangle){ (screenWidth-200) / 2, 280, 200, 40 }, "2. CONTIGUOUS")) {
            fileForm->organizationGlobaleType  = 2;
        }
    } else if (step == 3) {
        GuiLabel((Rectangle){ (screenWidth-300) / 2, 200, 400, 20 }, "Choose organization Internal type:");
        if (GuiButton((Rectangle){ (screenWidth-200) / 2, 230, 200, 40 }, "1. ORDONNE")) {
            fileForm->organizationInterneType = 1;
            *formSubmitted = true;
        }
        if (GuiButton((Rectangle){ (screenWidth-200) / 2, 280, 200, 40 }, "2. NON ORDONNE")) {
            fileForm->organizationInterneType = 2;
            *formSubmitted = true;
        }
    }
}

// Draw the current step of the ms creation form
void DrawFormStep2(int step, msForm *msForm,int screenWidth) {
    static char blockingFactorStr[12] = {0};
    static char diskNumberStr[12] = {0};
    static bool blockingFactorEditMode = false;
    static bool diskNumberEditMode = false;

    if (step == 0) {
        GuiLabel((Rectangle){ (screenWidth-400) / 2, 200, 400, 20 }, "Enter BLOCKING FACTOR:");
        if (GuiTextBox((Rectangle){(screenWidth-500) / 2, 230, 600, 40 }, blockingFactorStr, 12, blockingFactorEditMode)) {
            blockingFactorEditMode = !blockingFactorEditMode;
        }
        if (GuiButton((Rectangle){ screenWidth/2+300, 230, 100, 40 }, "Submit")) {
            msForm->blockingfactor = atoi(blockingFactorStr);
            blockingFactorEditMode = false;
            memset(blockingFactorStr, 0, sizeof(blockingFactorStr)); // Clear the input string
        }
    } else if (step == 1) {
        GuiLabel((Rectangle){ (screenWidth-400) / 2, 200, 400, 20 }, "Enter number of Blocks:");
        if (GuiTextBox((Rectangle){(screenWidth-500) / 2, 230, 600, 40 }, diskNumberStr, 12, diskNumberEditMode)) {
            diskNumberEditMode = !diskNumberEditMode;
        }
        if (GuiButton((Rectangle){ screenWidth/2+300, 230, 100, 40 }, "Submit")) {
            msForm->disknumber = atoi(diskNumberStr);
            diskNumberEditMode = false;
            memset(diskNumberStr, 0, sizeof(diskNumberStr)); // Clear the input string
        }
    }
}

// Initialize a dynamic array
void initArray(DynamicArray* array, size_t elementSize, size_t initialCapacity) {
    array->data = malloc(initialCapacity * elementSize);
    array->elementSize = elementSize;
    array->size = 0;
    array->capacity = initialCapacity;
}

// Insert an element into the dynamic array
void insertArray(DynamicArray* array, void* element) {
    if (array->size == array->capacity) {
        array->capacity *= 2;
        array->data = realloc(array->data, array->capacity * array->elementSize);
    }
    memcpy((char*)array->data + array->size * array->elementSize, element, array->elementSize);
    array->size++;
}

// Free the dynamic array
void freeArray(DynamicArray* array) {
    free(array->data);
    array->data = NULL;
    array->size = 0;
    array->capacity = 0;
}

// Retrieve an element from the dynamic array at a specified index
void* getArrayElement(DynamicArray* array, size_t index) {
    if (index >= array->size) {
        return NULL; // Index out of bounds
    }
    return (char*)array->data + index * array->elementSize;
}

