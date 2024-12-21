#include "raylib.h"
#include <string>
#include <cstring>
#include <iostream>
#include <vector>

// Enum for different states in the application
enum AppState {
    LOADING_SCREEN,
    INIT_MEMOIRE_SCREEN,
    INIT_MEMOIRE_SCREEN2,
    MAIN_MENU,
    CREATE_FILE,
    FILE_MENU,
    MEMOIRE_SECONDAIRE_STATE
};

// Structure to hold file information
struct FileForm {
    char fileName[50];     // File name (max 50 characters)
    int numberOfRecords;   // Number of records in the file
    int organizationType;  // Organization type (1 for Global, 2 for Internal)
};
// Structure to hold file information
struct msForm {////////////////////////////////////////////////////////////////////////////////////////////////////
       
    int blockingfactor;   // Number of records in the file
    int disknumber;  // Organization type (1 for Global, 2 for Internal)
};

// Function prototypes
void DrawButton(Rectangle button, const char* text, bool isPressed);
void DrawFormStep(int step, const FileForm& fileForm);
void DrawFormStep2(int step, const msForm& msForm);

// Custom function definitions for menu actions
void RechercherFunction() {
    std::cout << "Rechercher function triggered!" << std::endl;
}

void InsererFunction() {
    std::cout << "Inserer function triggered!" << std::endl;
}

void SupprimerFunction() {
    std::cout << "Supprimer function triggered!" << std::endl;
}

void RenommerFunction() {
    std::cout << "Renommer function triggered!" << std::endl;
}

void BackFunction() {
    std::cout << "Back function triggered!" << std::endl;
}

void CompacterFunction() {
    std::cout << "Compacter function triggered!" << std::endl;
}

void ViderFunction() {
    std::cout << "Vider function triggered!" << std::endl;
}

int main() {
    // Initialization
    const int screenWidth = 1280;
    const int screenHeight = 720;
    InitWindow(screenWidth, screenHeight, "raylib - File Creation Example");

    // Set initial state to Loading Screen
    AppState currentState = LOADING_SCREEN;
    FileForm fileForm = { "", 0, 0 };
    msForm msForm = { 0, 0 };// Initialize empty file form//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    std::vector<std::string> createdFiles;// Store created files

    // Timer for loading screen
    float loadingTime = 2.0f;  // 2 seconds for loading screen
    float elapsedTime = 0.0f;

    // Button for INIT MEMOIRE SECONDAIRE screen
    Rectangle initButton = { screenWidth / 2.0f - 200, screenHeight / 2.0f - 50, 400, 100 };

    // Button dimensions and positions
    float buttonWidth = 300.0f;
    float wideButtonWidth = 500.0f; // Wider buttons for 3rd and 4th options
    float buttonHeight = 100.0f;
    int i = 0;

    // Array for button positions (4 buttons for main menu)
    Rectangle buttons[4] = {
        { screenWidth / 2.0f - buttonWidth / 2.0f, 220, buttonWidth, buttonHeight },
        { screenWidth / 2.0f - buttonWidth / 2.0f, 340, buttonWidth, buttonHeight },
      
        { screenWidth / 2.0f - wideButtonWidth / 2.0f, 580, wideButtonWidth, buttonHeight }
    };

    // Button labels for main menu
    const char* buttonLabels[4] = {
        " MEMOIRE SECONDAIRE",
        "CREER UN FICHIER", 
        "AFFICHER LES METADONNEES"
    };

    // Form state tracking
    int formStep = 0;
    int formStep2 = 0;// To track form input step (file name, number of records, etc.)
    bool formSubmitted = false;
    bool form = false;// Flag to check if form has been submitted/////////////////////////////////////////////////////////////////////////
    

    // Vector for storing file buttons and selected file index
    std::vector<Rectangle> fileButtons;
    int selectedFileIndex = -1;

    // Main game loop
    while (!WindowShouldClose()) {
        // Update
        if (currentState == LOADING_SCREEN) {//////////////////////////////////////////////////////////////loadingscreen
            elapsedTime += GetFrameTime();
            if (elapsedTime >= loadingTime) currentState = INIT_MEMOIRE_SCREEN;
        }
        else if (currentState == INIT_MEMOIRE_SCREEN) {//////////////////////////////////////////////////////////initmemoir
            // Check if the INIT MEMOIRE SECONDAIRE button is clicked
            if (CheckCollisionPointRec(GetMousePosition(), initButton) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                std::cout << "INIT MEMOIRE SECONDAIRE triggered!\n";
                currentState = INIT_MEMOIRE_SCREEN2;
            }
            
        }
        else if (currentState == INIT_MEMOIRE_SCREEN2) {//////////////////////////////////////////////////////////initmemoir
           
                // Handle form input for ms creation
                if (formStep2 == 0) { // Number of blockinf factor input
                    if (IsKeyPressed(KEY_ENTER)) {
                        formStep2++;
                    }
                    else if (IsKeyPressed(KEY_BACKSPACE) && msForm.blockingfactor > 0) {
                        msForm.blockingfactor /= 10;
                    }
                    else {
                        char key = GetCharPressed();
                        if (key >= '0' && key <= '9') {
                            msForm.blockingfactor = msForm.blockingfactor * 10 + (key - '0');
                        }
                    }
                }
                if (formStep2 == 1) { // Number of disks input
                    if (IsKeyPressed(KEY_ENTER) && msForm.disknumber > 0) {
                        form = true;

                    }
                    else if (IsKeyPressed(KEY_BACKSPACE) && msForm.disknumber > 0) {
                        msForm.disknumber /= 10;
                    }
                    else {
                        char key = GetCharPressed();
                        if (key >= '0' && key <= '9') {
                            msForm.disknumber = msForm.disknumber* 10 + (key - '0');
                        }
                    }
                }

                // If form is submitted, print file info and return to main menu
                if (form) {
                    std::cout << "MS CREATED:\n";
                 
                    std::cout << "BLOCKINF FACTOR : " << msForm.blockingfactor << "\n";
                    std::cout << "number of disks: " << msForm.disknumber << "\n";


                    currentState = MAIN_MENU;  // Go back to main menu
                }
            

        }
        else if (currentState == MAIN_MENU) {/////////////////////////////////////////////////////////////////////////mainmenu
            // Handle button clicks in main menu
            for (int i = 0; i < 2; i++) {
                if (CheckCollisionPointRec(GetMousePosition(), buttons[i]) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                    switch (i) {
                    case 0:  // MEMOIRE SECONDAIRE
                       
                            currentState = MEMOIRE_SECONDAIRE_STATE; // Transition to secondary memory state
                       
                        break;
                    case 1: currentState = CREATE_FILE; break;  // Create file state
                    }
                }
            }

            // Handle file button clicks
            for (int i = 0; i < fileButtons.size(); i++) {
                if (CheckCollisionPointRec(GetMousePosition(), fileButtons[i]) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                    selectedFileIndex = i;  // Track selected file
                    currentState = FILE_MENU;  // Switch to file menu state
                }
            }
        }
        else if (currentState == MEMOIRE_SECONDAIRE_STATE) {////////////////////////////////////////////////////////////////////////////////////memsstate
            // Define buttons for secondary memory state
            Rectangle compacterButton = { screenWidth / 2 - 150, 200, 300, 50 };
            Rectangle viderButton = { screenWidth / 2 - 150, 300, 300, 50 };
            Rectangle msbutton = { screenWidth / 2 - 250, 400, 500, 100 };
            Rectangle backbutton = { screenWidth / 2 - 150, 550, 300, 50 };

            // Check button clicks for secondary memory actions
            if (CheckCollisionPointRec(GetMousePosition(), compacterButton) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                CompacterFunction();
            }
            if (CheckCollisionPointRec(GetMousePosition(), viderButton) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                ViderFunction();
            }
            if (CheckCollisionPointRec(GetMousePosition(), msbutton) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                std::cout << "ETAT DE LA MEMOIRE SECONDAIRE triggered!\n";
            }
            if (CheckCollisionPointRec(GetMousePosition(), backbutton) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                BackFunction();
                currentState = MAIN_MENU;  // Return to main menu
            }

            // Draw the secondary memory screen
            BeginDrawing();
            ClearBackground(LIGHTGRAY);
            DrawText("MEMOIRE SECONDAIRE", screenWidth / 2 - MeasureText("MEMOIRE SECONDAIRE", 40) / 2, 100, 40, DARKGRAY);
            DrawButton(compacterButton, "Compacter", false);
            DrawButton(viderButton, "Vider", false);
            DrawButton(msbutton, "ETAT DE LA MEMOIRE SECONDAIRE", false);
            DrawButton(backbutton, "Back", false);
            EndDrawing();
            continue;
        }
        else if (currentState == CREATE_FILE) {////////////////////////////////////////////////////////////////////////////////////////creatfile
            // Handle form input for file creation
            if (formStep == 0) { // File name input
                if (IsKeyPressed(KEY_BACKSPACE) && strlen(fileForm.fileName) > 0) {
                    fileForm.fileName[strlen(fileForm.fileName) - 1] = '\0';
                }
                else if (IsKeyPressed(KEY_ENTER)) {
                    formStep++;
                }
                else {
                    char key = GetCharPressed();
                    if (key >= 32 && key <= 126 && strlen(fileForm.fileName) < 49) {
                        int len = strlen(fileForm.fileName);
                        fileForm.fileName[len] = key;
                        fileForm.fileName[len + 1] = '\0';
                    }
                }
            }
            else if (formStep == 1) { // Number of records input
                if (IsKeyPressed(KEY_ENTER)) {
                    formStep++;
                }
                else if (IsKeyPressed(KEY_BACKSPACE) && fileForm.numberOfRecords > 0) {
                    fileForm.numberOfRecords /= 10;
                }
                else {
                    char key = GetCharPressed();
                    if (key >= '0' && key <= '9') {
                        fileForm.numberOfRecords = fileForm.numberOfRecords * 10 + (key - '0');
                    }
                }
            }
            else if (formStep == 2) { // Organization type input
                if (IsKeyPressed(KEY_ONE)) {
                    fileForm.organizationType = 1;
                    formSubmitted = true;
                }
                else if (IsKeyPressed(KEY_TWO)) {
                    fileForm.organizationType = 2;
                    formSubmitted = true;
                }
            }

            // If form is submitted, print file info and return to main menu
            if (formSubmitted) { 
                std::cout << "File created:\n";
                std::cout << "File Name: " << fileForm.fileName << "\n";
                std::cout << "Number of Records: " << fileForm.numberOfRecords << "\n";
                std::cout << "Organization Type: " << (fileForm.organizationType == 1 ? "Organisation Globale" : "Organisation Interne") << "\n";

                // Add new file to created files list
                createdFiles.push_back(fileForm.fileName);
                

                // Add button for the new file in the file menu///////////we need to add condition hereso when we delete a file i dosnt draw his button 
                if (i<3)
                {
                    fileButtons.push_back({ (buttons[1].x-100) + 100*  (createdFiles.size()), 450, 100.0f, buttonHeight });
                    i++;
                }
                
                else 
                {
                    fileButtons.push_back({ (buttons[1].x - 400) + 100 * (createdFiles.size()), 550, 100.0f, buttonHeight });
                    i++;
                }
                
                // Reset the form for next file creation
                fileForm = { "", 0, 0 };
                formStep = 0;
                formSubmitted = false;

                currentState = MAIN_MENU;  // Go back to main menu
            }
        }
        else if (currentState == FILE_MENU) {////////////////////////////////////////////////////filemenu
            // Define action buttons for the file menu
            Rectangle actionButtons[7] = {
                { 200, 200, 200, 50 },
                { 200, 260, 200, 50 },
                { 200, 320, 200, 50 },
                { 200, 380, 200, 50 },
                { 200, 440, 200, 50 },
                { 500, 440, 450, 150 },
                { 625, 220, 200, 200 },// Large button for file name 
            };

            // Labels for action buttons
            const char* actionLabels[6] = {
                "Rechercher", "Inserer", "Supprimer", "Renommer", "Retour","AFFICHER LES METADONNES"
            };

            // Handle button clicks for file actions
            for (int i = 0; i < 7; i++) {
                bool isPressed = CheckCollisionPointRec(GetMousePosition(), actionButtons[i]) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT);
                if (i < 6) {
                    DrawButton(actionButtons[i], actionLabels[i], isPressed);
                }
                else {
                    DrawButton(actionButtons[i], createdFiles[selectedFileIndex].c_str(), isPressed); // Display file name
                }

                // Trigger custom actions based on button clicked
                if (i == 0 && isPressed) { RechercherFunction(); }
                if (i == 1 && isPressed) { InsererFunction(); }
                if (i == 2 && isPressed) { SupprimerFunction(); }
                if (i == 3 && isPressed) { RenommerFunction(); }
                if (i == 4 && isPressed) { BackFunction(); currentState = MAIN_MENU; }
                if (i == 5 && isPressed) { std::cout << "AFFICHER LES METADONNEES triggered!\n"; }
            }
        }

        // Draw the current screen
        BeginDrawing();
        ClearBackground(LIGHTGRAY); // Windows-like background

        // Display current state screen
        if (currentState == LOADING_SCREEN) {
            DrawText("Welcome to our project", screenWidth / 2 - MeasureText("Welcome to our project", 40) / 2, screenHeight / 2 - 20, 40, DARKBLUE);
        }
        else if (currentState == INIT_MEMOIRE_SCREEN) {
            DrawText("INIT MEMOIRE SECONDAIRE", screenWidth / 2 - MeasureText("INIT MEMOIRE SECONDAIRE", 40) / 2, 100, 40, DARKGRAY);
            DrawButton(initButton, "INIT MEMOIRE SECONDAIRE", false);

        }
        else if (currentState == INIT_MEMOIRE_SCREEN2) {
            DrawText("CREATE MEMOIRE", screenWidth / 2 - MeasureText("CREATE FILE", 40) / 2, 100, 40, DARKGRAY);
            DrawFormStep2(formStep2, msForm); // Draw file creation form
        }
        else if (currentState == MAIN_MENU) {
            DrawText("MAIN MENU", screenWidth / 2 - MeasureText("MAIN MENU", 40) / 2, 100, 40, DARKGRAY);

            // Draw main menu buttons
            for (int i = 0; i < 2; i++) {
                bool isPressed = CheckCollisionPointRec(GetMousePosition(), buttons[i]) && IsMouseButtonDown(MOUSE_BUTTON_LEFT);
                DrawButton(buttons[i], buttonLabels[i], isPressed);
            }

            // Draw file buttons if any
            for (int i = 0; i < fileButtons.size(); i++) {
                bool isPressed = CheckCollisionPointRec(GetMousePosition(), fileButtons[i]) && IsMouseButtonDown(MOUSE_BUTTON_LEFT);
                DrawButton(fileButtons[i], createdFiles[i].c_str(), isPressed);
            }
        }
        else if (currentState == CREATE_FILE) {
            DrawText("CREATE FILE", screenWidth / 2 - MeasureText("CREATE FILE", 40) / 2, 100, 40, DARKGRAY);
            DrawFormStep(formStep, fileForm); // Draw file creation form
        }
        else if (currentState == FILE_MENU) {
            DrawText("File Menu", screenWidth / 2 - MeasureText("File Menu", 40) / 2, 100, 40, DARKGRAY);
        }

        EndDrawing();
    }

    // Close the window and exit
    CloseWindow();
    return 0;
}

// Draw a button with a label and pressed state
void DrawButton(Rectangle button, const char* text, bool isPressed) {
    Color shadowColor = Fade(DARKGRAY, 0.3f);  // Button shadow
    Color borderColor = GRAY;  // Button border color
    Color fillColor = isPressed ? DARKBLUE : (CheckCollisionPointRec(GetMousePosition(), button) ? SKYBLUE : WHITE);

    // Draw the shadow, main fill, and border of the button
    DrawRectangle((int)button.x + 3, (int)button.y + 3, (int)button.width, (int)button.height, shadowColor);
    DrawRectangleRec(button, fillColor);
    DrawRectangleLinesEx(button, 2, borderColor);

    // Draw the button text in the center
    int textWidth = MeasureText(text, 20);
    DrawText(text, (int)(button.x + button.width / 2 - textWidth / 2), (int)(button.y + button.height / 2 - 10), 20, DARKGRAY);
}

// Draw the current step of the file creation form
void DrawFormStep(int step, const FileForm& fileForm) {
    if (step == 0) {
        DrawText("Enter file name:", 200, 200, 20, DARKGRAY);
        DrawRectangle(200, 250, 600, 40, WHITE);
        DrawText(fileForm.fileName, 210, 260, 20, BLACK);
    }
    else if (step == 1) {
        DrawText("Enter number of records:", 200, 200, 20, DARKGRAY);
        DrawRectangle(200, 250, 600, 40, WHITE);
        DrawText(std::to_string(fileForm.numberOfRecords).c_str(), 210, 260, 20, BLACK);
    }
    else if (step == 2) {
        DrawText("Choose organization type:", 200, 200, 20, DARKGRAY);
        DrawText("1. Organisation Globale", 210, 260, 20, DARKGRAY);
        DrawText("2. Organisation Interne", 210, 290, 20, DARKGRAY);
    }
}
void DrawFormStep2(int step, const msForm& msForm) {
    if (step == 0) {
        DrawText("Enter BLOCKING FACTOR:", 200, 200, 20, DARKGRAY);
        DrawRectangle(200, 250, 600, 40, WHITE);
        DrawText(std::to_string(msForm.blockingfactor).c_str(), 210, 260, 20, BLACK);
    }
    else if (step == 1) {
        DrawText("Enter number of disk:", 200, 200, 20, DARKGRAY);
        DrawRectangle(200, 250, 600, 40, WHITE);
        DrawText(std::to_string(msForm.disknumber).c_str(), 210, 260, 20, BLACK);
    }
    
}
