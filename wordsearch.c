#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Declarations of the two functions you will implement
// Feel free to declare any helper functions or global variables

void printPuzzle(char** arr, int size);
void printPath(int *path, int wordLen, int size);

void searchPuzzle(char** arr, char* word, int size);
int searchWord(char **arr, int size, char *word, int wordLen, int row, int col, int **visited, int index, int *path);
int searchFromCell(char **arr, int size, char *word, int row, int col, int **visited, int *path);

int compareChars(char a, char b) {
    if (a >= 'A' && a <= 'Z') a = a + ('a' - 'A');  // Convert 'A'-'Z' to 'a'-'z'
    if (b >= 'A' && b <= 'Z') b = b + ('a' - 'A');  // Convert 'A'-'Z' to 'a'-'z'
    return a == b;
}





int bSize;

// Main function, DO NOT MODIFY
int main(int argc, char **argv) 
{
    if (argc != 2) 
    {
    fprintf(stderr, "Usage: %s <puzzle file name>\n", argv[0]);
    return 2;
    }

    int i, j;
    FILE *fptr;

    // Open file for reading puzzle
    fptr = fopen(argv[1], "r");

    if (fptr == NULL) 
    {
    printf("Cannot Open Puzzle File!\n");
    return 0;
    }

    // Read the size of the puzzle block
    fscanf(fptr, "%d\n", &bSize);

    // Allocate space for the puzzle block and the word to be searched
    char **block = (char**)malloc(bSize * sizeof(char*));
    char *word = (char*)malloc(20 * sizeof(char));

    // Read puzzle block into 2D arrays
    for(i = 0; i < bSize; i++) 
    {
        *(block + i) = (char*)malloc(bSize * sizeof(char));

        for (j = 0; j < bSize - 1; ++j) 
        {
            fscanf(fptr, "%c ", *(block + i) + j);
        }

        fscanf(fptr, "%c \n", *(block + i) + j);
    }

    fclose(fptr);



    printf("Enter the word to search: ");
    scanf("%s", word);

    // Print out original puzzle arr
    printPuzzle(block, bSize);

    // Call searchPuzzle to the word in the puzzle
    searchPuzzle(block, word, bSize);

return 0;
}


void printPuzzle(char** arr, int size) 
{
// This function will print out the complete puzzle arr (arr).
// It must produce the output in the SAME format as the samples
// in the instructions.
// Your implementation here...


    for (int i = 0; i < size; i++) 
    {
        for (int n = 0; n < size; n++) 
        {
            printf("%c ", *(*(arr + i) + n)); 
        }
        printf("\n");
    }

}

void searchPuzzle(char **arr, char *word, int size) 
{

    int wordLen = 0;
    for (char *ptr = word; *ptr != '\0'; ptr++) 
        wordLen++;

    // 1D array of interger pointers for path n*n
    int *path = (int *)malloc(size * size * sizeof(int));

    for (int i = 0; i < size * size; i++) 
    {
        *(path + i) = 0;
    }
    
    //2D double pointer array temp
    int **visited = (int **)malloc(size * sizeof(int *));

    for (int i = 0; i < size; i++) 
    {
        visited[i] = (int *)malloc(size * sizeof(int));

        for (int j = 0; j < size; j++) 
        {
            visited[i][j] = 0;  
        }
    }

    // Loop through each cell in the arr
    for (int i = 0; i < size; i++) 
    {
        for (int n = 0; n < size; n++) 
        {
            int rowIndex = i;
            int colIndex = n;

            if (searchFromCell(arr, size, word, rowIndex, colIndex, visited, path)) 
            {
                printf("Word found!\n");
                printPath(path, wordLen, size);  // Print the path if found

                // Free allocated memory for visited
                for (int i = 0; i < size; i++) 
                {
                    free(visited[i]);
                }

                free(visited);
                free(path);
                return;
            }
        }
    }
    printf("Word not found!\n");
    // Free allocated memory for visited
    for (int i = 0; i < size; i++) {
        free(visited[i]);
    }
    free(visited);
    free(path);
}


int searchWord(char **arr, int size, char *word, int wordLen, int row, int col, int **visited, int index, int *path) 
{
    // Check boundaries and if cell is already visited visted is 0 or 1
    if (row < 0 || row >= size || col < 0 || col >= size || visited[row][col]) 
    {
        return 0;
    }

    // Check if the current character matches
    if (!compareChars(*(*(arr + row) + col), word[index])) {
        return 0;
    }

    // Mark the cell as visited and update the path when charachter is found
    visited[row][col] = 1;
    *(path + row * size + col) = index + 1;  // Store the step number in the path

    // If all characters are found
    if (index == wordLen - 1) 
    {
        return 1;
    }

    // Search in all 8 possible directions
    int directions[8][2] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}, {-1, -1}, {-1, 1}, {1, -1}, {1, 1}};

    for (int d = 0; d < 8; d++) 
    {
        //get this in pointer notation
        int newRow = row + directions[d][0];
        int newCol = col + directions[d][1];

        if (searchWord(arr, size, word, wordLen, newRow, newCol, visited, index + 1, path)) 
        {
            return 1;
        }
    }

    // Backtrack if the word cannot be formed
    visited[row][col] = 0;
    *(path + row * size + col) = 0;

    return 0;
}

// Start search from a given cell
int searchFromCell(char **arr, int size, char *word, int row, int col, int **visited, int *path) 
{
    int wordLen = 0;
    for (char *ptr = word; *ptr != '\0'; ptr++)
    {
        wordLen++;
    }
    
    return searchWord(arr, size, word, wordLen, row, col, visited, 0, path);
}

// Function to print the search path
// Print the path array
void printPath(int *path, int wordLen, int size) 
{
    for (int i = 0; i < size; i++) 
    {
        for (int j = 0; j < size; j++) 
        {
            printf("%d ", *(path + i * size + j));  
        }
        printf("\n");
    }
}