//
//  life.c
//  homework1
//
//  Created by Nahom Molla on 29/01/2021.
//

#include <stdio.h>
#include <stdlib.h>

int validateIntInput(long int x);
int simulate(char** grid, int rows, int cols, int gen);
char** createGrid(FILE* file, int rows, int cols);
char** copyGrid(char** grid, int rows, int cols);
void freeGrid(char** grid, int rows);
void displayGrid(char** grid, int rows, int cols, int gen);
int countNeighbors(char** grid, int r, int c, int rows, int cols);

int main(int argc, const char * argv[]) {
    int rows = 10, cols = 10, generations = 10;
    FILE* file = NULL;
    char nullc = '\0';
    char *rerror=&nullc, *cerror=&nullc, *gerror=&nullc;
    
    if (argc == 1)
        file = fopen("life.txt", "r");
    
    else if (argc >= 3 && argc <= 5) {
        if(argc == 3)
            file = fopen("life.txt", "r");
        
        else { // (argc > 3)
            file = fopen(argv[3], "r");
            if (argc == 5){
                generations = validateIntInput(strtol(argv[4], &gerror, 10));
                if (generations == -1){
                    printf("Input is not a valid int!");
                    return -1;
                }
            }
        }
        rows = validateIntInput(strtol(argv[1], &rerror, 10));
        cols = validateIntInput(strtol(argv[2], &cerror, 10));
        if (rows == -1 || cols == -1){
            printf("Input is not a valid int!");
            return -1;
        }
    }
    
    else {
        printf("Bad input from console!\n");
        return -1;
    }
    
    // check to see if the inputs for row, col & gen are valid
    if (*rerror != '\0' || *cerror != '\0' || *gerror != '\0'){
        printf("Bad input from console!\n");
        return -1;
    }
    
    // check if file is empty
    if (file == NULL) {
        perror("Error");
        return -1;
    }
    
    // creates Generation 0 on the heap
    char** grid = createGrid(file, rows, cols);
    if (grid == NULL){
        printf("Could not create grid!\n");
        return -1;
    }
    
    // displays Generation 0
    displayGrid(grid, rows, cols, 0);

    // simulates & displays the specified number of generations
    if (simulate(grid, rows, cols, generations) == -1){
        printf("Could not simulate game!\n");
        return -1;
    }
}


// verifies validity of command line int inputs before casting
int validateIntInput(long int x){
    if (x < 0 || x > INT32_MAX){
        printf("Input is not a valid int!");
        return -1;
    }
    return (int)x;
}

// simulates & displays the specified number of generations
int simulate(char** grid, int rows, int cols, int gen){ // returns -1 if error occurs
    int r, c, g, neighbors;
    char** clone;
    for (g=1; g<=gen; g++){
        clone = copyGrid(grid, rows, cols);
        if (clone == NULL)
            return -1;
        
        for (r=0; r<rows; r++){
            for (c=0; c<cols; c++){
                neighbors = countNeighbors(grid, r, c, rows, cols);
                if(neighbors < 2 || neighbors > 3)
                    clone[r][c] = '-';
                
                if(neighbors == 3)
                    clone[r][c] = '*';
            }
        }
        
        // display generation: g
        displayGrid(clone, rows, cols, g);
        // free up grid before pointing to clone
        freeGrid(grid, rows);
        
        grid = clone;
    }
    return 0;
}

// creates Generation 0 on the heap
char** createGrid(FILE* file, int rows, int cols){
    int r = 0, c = 0, i = 0;
    char** grid = (char **)malloc(rows * sizeof(char *));
    if (grid == NULL){
        perror("malloc() failed");
        return NULL;
    }
    
    for (r=0; r<rows; r++){
        grid[r] = (char *)malloc(cols * sizeof(char));
        if (grid[r] == NULL){
            perror("malloc() failed");
            return NULL;
        }
    }
    
    r = 0;
    c = 0;
    i = 0;
    char curr;
    
    // reads a single char at a time & populates the grid
    while (((curr=fgetc(file)) != EOF) && r < rows) {
        if (curr == '\0' || curr == '\n' || c == cols){
            for (i=c; i<cols; i++) // fill in empty columns
                grid[r][i] = '-';
            r++;
            c=0;
        }
        else if (curr == ' '){
            grid[r][c] = '-';
            c++;
        }
        else{ // (curr == '*')
            grid[r][c] = '*';
            c++;
        }
    }
    
    fclose(file);
    
    // fill empty rows
    for (i=r; i<rows; i++)
        for (c=0; c<cols; c++)
            grid[i][c] = '-';
    
    return grid;
}

// creates a new clone of grid on the heap
char** copyGrid(char** grid, int rows, int cols){
    int r, c;
    char** clone = (char **)malloc(rows * sizeof(char *));
    if (clone == NULL){
        perror("malloc() failed");
        return NULL;
    }
    for (r=0; r<rows; r++){
        clone[r] = (char *)malloc(cols * sizeof(char));
        if (clone[r] == NULL){
            perror("malloc() failed");
            return NULL;
        }
    }

    for (r=0; r<rows; r++)
        for (c=0; c<cols; c++)
            clone[r][c] = grid[r][c];
        
    return clone;
}

// frees up malloc'd memory
void freeGrid(char** grid, int rows){
    int i;
    for (i=0; i<rows; i++)
        free(grid[i]);
    
    free(grid);
}

// prints a grid on stdout
void displayGrid(char** grid, int rows, int cols, int gen){
    printf("Generation %d:\n", gen);
    int i, j;
    for (i=0; i<rows; i++){
        for (j=0; j<cols; j++){
            printf("%c", grid[i][j]);
        }
        printf("\n");
    }
    printf("================================\n");
}

// counts the number of neighbors of grid[r][c]
int countNeighbors(char** grid, int r, int c, int rows, int cols){
    int neighbors = 0;
    
    if (r != 0 && grid[r-1][c] == '*') //top
        neighbors++;
    
    if (r < rows-1 && grid[r+1][c] == '*') //bottom
        neighbors++;
    
    if (c != 0 && grid[r][c-1] == '*') //left
        neighbors++;
    
    if (c < cols-1 && grid[r][c+1] == '*') //right
        neighbors++;
    
    if (r != 0 && c != 0 && grid[r-1][c-1] == '*') //top-left
        neighbors++;
    
    if (r != 0 && c < cols-1 && grid[r-1][c+1]  == '*') //top-right
        neighbors++;
    
    if (r < rows-1 && c != 0 && grid[r+1][c-1] == '*') //bottom-left
        neighbors++;
    
    if (r < rows-1 && c < cols-1 && grid[r+1][c+1] == '*') //bottom-right
        neighbors++;
    
    return neighbors;
}
