#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//Taylor Danielewski

// <unistd.h> is needed for a call to usleep which is needed for animation.
// This is not standard C, but from what is known as POSIX, which is
// supported by OSX and Cygwin
#include <unistd.h>

bool animate_or_prompt(bool animate);
bool animation_prompt(void);
bool more_generations_prompt(void);
void print_board(bool **board, int rows, int cols);
void swap(bool ***a, bool ***b);
void game_of_life(bool **current, bool **next, int rows, int cols);
bool alive(bool **grid, int row, int col);

void setup_blinker(bool **board, int row_off, int col_off);
void setup_toad(bool **board, int row_off, int col_off);
void setup_pentadecathlon(bool **board, int row_off, int col_off);
void setup_gosper_glider_gun(bool **board);
void setup_pulsar(bool **board, int row_off, int col_off);


int main() {
  const int ROWS = 33;
  const int COLS = 44;
  // Initialize the boards
  bool **current;
  bool **next;

  // Allocate memory for current and next using malloc
  current = malloc(ROWS * sizeof(bool *));
  for (int i = 0; i < ROWS; ++i) {
	  current[i] = malloc(COLS * sizeof(bool));
	  }
	  
	  next = malloc(ROWS * sizeof(bool *));
  for (int i = 0; i < ROWS; ++i) {
	  next[i] = malloc(COLS * sizeof(bool));
	  }
  // Initialize current and next so that each entry of current is false
  // and each entry of next is true

  for (int i = 0; i < ROWS; i++){
	  for (int y = 0; y < COLS; y++){
		  current[i][y] = false;
		  next[i][y] = true;
	  }
  }
  
  // Setup fancy board stuff
  // Uncomment these one at a time as you are working. Only introduce a new
  //one once the previous one is working
  setup_blinker(current, 8, 40); // Period 2
  setup_toad(current, 12, 38); // Period 2
  setup_pulsar(current, 16, 13); // Period 3
  setup_pentadecathlon(current, 13, 0); // Period 15
  setup_gosper_glider_gun(current); // Period 30

  // Ask the user if they would like to see step-by-step or an animation
  bool animate = animation_prompt();

  // Loop through until the user says otherwise
  for (int generation_count = 1; animate_or_prompt(animate);
       ++generation_count) {
    printf("Generation %d (Press CTRL+C to quit)\n", generation_count);
    // Print the board
	
	print_board(current, ROWS, COLS);
    // Call the game_of_life
	game_of_life(current, next, ROWS, COLS);
    // Swap current and next
	swap(&current, &next);
		
  }

  printf("Simulation complete\n");

  // Free up memory used by current and next
  for (int i = 0; i < ROWS; ++i) {
	free(current[i]);
	current[i] = NULL;
	}
  free(current);
  current = NULL;
  
  for (int i = 0; i < ROWS; ++i) {
	free(next[i]);
	next[i] = NULL;
  }
  free(next);
  next = NULL;

  return 0;
}

bool animate_or_prompt(bool animate) {
  if (!animate) {
    return more_generations_prompt();
  }

  // Clear screen using ANSI escape code
  // escape, clear screen, escape move cursor to 1,1
  printf("\033[2J\033[1;1H");
  // Sleep for 0.1 seconds
  usleep(100000); // 100000 ms = 0.1 seconds

  return true;
}

bool animation_prompt(void) {
char answer;
bool t = true;
bool f = false;
  // This function should prompt the user with:
  // "Would you like to see an animation (Y) Or step-by-step (N)? "
  // It should return true if the answer is "Y" or "y"
  // It should return false if the answer is "N" or "n"
  // If invalid input is entered, a message indicating invalid input
  // should be printed and the prompt should be repeated until
  // valid input is given.
  while (true){
	   printf("Would you like to see an animation (Y) Or step-by-step (N)? ");
	   scanf("%s", &answer);
	  if (answer == 'Y' || answer == 'y'){
		  return t;
		  break;
	  }
	  if (answer == 'N' || answer == 'n'){
		  return f;
		  break;
	  }
	  printf("Invalid answer: please try again:\n");
  }
  return false;
}

bool more_generations_prompt() {
	char answer;
	bool t = true;
	bool f = false;
  // This function should prompt the user with:
  // "Would you like to continue? (Y/N) "
  // It should return true if the answer is "Y" or "y"
  // It should return false if the answer is "N" or "n"
  // If invalid input is entered, a message indicating invalid input
  // should be printed and the prompt should be repeated until
  // valid input is given.
    while (true){
	   printf("Would you like to continue? (Y/N) ");
	   scanf("%s", &answer);
	  if (answer == 'Y' || answer == 'y'){
		  return t;
		  break;
	  }
	  if (answer == 'N' || answer == 'n'){
		  return f;
		  break;
	  }
	  printf("Invalid answer: please try again:\n");
  }
}

void print_board(bool **board, int rows, int cols) {
  // This function should print out the 2D array board.
  // If you would like to print out a color block instead of a character,
  // use
  // printf("\033[7m \033[0m");
  // This is an ANSI escape sequence which will print out a solid block of
  // color that is the opposite color of your terminal. That is if the
  // background
  // of your terminal is normally black, it will be white, and vice versa.
	//printf("\n");
	for (int i =0; i < rows; i++){
		for (int y = 0; y < cols; y++){
			if(board[i][y]){
				printf("\033[7m \033[0m");
			}else{
				printf(" ");
			}
		}
		printf("\n");
		
	}
  }

void swap(bool ***a, bool ***b) {
  // This function should swap the contents of a and b
  bool **Temp = *a;
		*a = *b;
		*b = Temp;
		
}

void game_of_life(bool **current, bool **next, int rows, int cols) {
  // Clear out next by going through each row and column and set
  // every value to false
   for (int i = 0; i < rows; i++){
	  for (int y = 0; y < cols; y++){
		 next[i][y] = false;
	  }
  }

  // Calculate next by going through each row and column, except for the
  // borders, and set the ith, jth entry in next to the result of the
  // function alive based upon current at i, j
    for (int i = 1; i < rows-1; i++){
	  for (int y = 1; y < cols-1; y++){
		 next[i][y] = alive(current, i, y);	
		 if (next[i][y]){
		 }
	  }
  }
}

bool alive(bool **grid, int row, int col) {
  // Rules:
  

	int neighbors = 0;
bool t = true;
bool f = false;

	if (grid[row-1][col-1]){
		neighbors++;	
	}
	if (grid[row-1][col]){
		neighbors++;		
	}
	if (grid[row-1][col+1]){
		neighbors++;		
	}
	if (grid[row][col-1]){
		neighbors++;		
	}
	if (grid[row][col+1]){
		neighbors++;		
	}
	if (grid[row+1][col-1]){
		neighbors++;		
	}
	if (grid[row+1][col]){
		neighbors++;		
	}
	if (grid[row+1][col+1]){
		neighbors++;		
	}
  // * If a cell has less than 2 live neighbors, it will die (or stay dead)
	if (neighbors < 2){
		return f;
	}
	
	if (grid[row][col] && neighbors == 2){
		return t;
	}
  // * If a cell that 3 live neighbors, it will be alive
  //   (either by staying alive or coming alive)
	if (neighbors == 3){
		return t;
	}
	 // * If a cell has move than 3 live neighbors, it will die (or stay dead)
	return f;
 
}

void setup_blinker(bool **board, int row_off, int col_off) {
  board[row_off][col_off - 1] = board[row_off][col_off] =
      board[row_off][col_off + 1] = true;
}

void setup_toad(bool **board, int row_off, int col_off) {
  board[2 + row_off][2 + col_off] = board[2 + row_off][3 + col_off] =
      board[2 + row_off][4 + col_off] = true;
  board[3 + row_off][1 + col_off] = board[3 + row_off][2 + col_off] =
      board[3 + row_off][3 + col_off] = true;
}

void setup_pentadecathlon(bool **board, int row_off, int col_off) {
  board[4 + row_off][5 + col_off] = board[5 + row_off][5 + col_off] = true;
  board[7 + row_off][5 + col_off] = board[8 + row_off][5 + col_off] =
      board[9 + row_off][5 + col_off] = board[10 + row_off][5 + col_off] = true;
  board[12 + row_off][5 + col_off] = board[13 + row_off][5 + col_off] = true;

  board[6 + row_off][4 + col_off] = board[6 + row_off][6 + col_off] = true;
  board[11 + row_off][4 + col_off] = board[11 + row_off][6 + col_off] = true;
}

void setup_gosper_glider_gun(bool **board) {
  board[6][1] = board[7][1] = board[6][2] = board[7][2] = true;

  board[4][35] = board[5][35] = board[4][36] = board[5][36] = true;

  board[4][13] = board[4][14] = true;
  board[5][12] = board[5][16] = true;
  board[6][11] = board[6][17] = true;
  board[7][11] = board[7][15] = board[7][17] = board[7][18] = true;
  board[8][11] = board[8][17] = true;
  board[9][12] = board[9][16] = true;
  board[10][13] = board[10][14] = true;

  board[2][25] = true;
  board[3][23] = board[3][25] = true;
  board[4][21] = board[4][22] = true;
  board[5][21] = board[5][22] = true;
  board[6][21] = board[6][22] = true;
  board[7][23] = board[7][25] = true;
  board[8][25] = true;
}

void setup_pulsar(bool **board, int row_off, int col_off) {
  board[2 + row_off][4 + col_off] = board[2 + row_off][5 + col_off] =
      board[2 + row_off][6 + col_off] = true;
  board[2 + row_off][10 + col_off] = board[2 + row_off][11 + col_off] =
      board[2 + row_off][12 + col_off] = true;

  board[7 + row_off][4 + col_off] = board[7 + row_off][5 + col_off] =
      board[7 + row_off][6 + col_off] = true;
  board[7 + row_off][10 + col_off] = board[7 + row_off][11 + col_off] =
      board[7 + row_off][12 + col_off] = true;

  board[9 + row_off][4 + col_off] = board[9 + row_off][5 + col_off] =
      board[9 + row_off][6 + col_off] = true;
  board[9 + row_off][10 + col_off] = board[9 + row_off][11 + col_off] =
      board[9 + row_off][12 + col_off] = true;

  board[14 + row_off][4 + col_off] = board[14 + row_off][5 + col_off] =
      board[14 + row_off][6 + col_off] = true;
  board[14 + row_off][10 + col_off] = board[14 + row_off][11 + col_off] =
      board[14 + row_off][12 + col_off] = true;

  board[4 + row_off][2 + col_off] = board[5 + row_off][2 + col_off] =
      board[6 + row_off][2 + col_off] = true;
  board[4 + row_off][7 + col_off] = board[5 + row_off][7 + col_off] =
      board[6 + row_off][7 + col_off] = true;
  board[4 + row_off][9 + col_off] = board[5 + row_off][9 + col_off] =
      board[6 + row_off][9 + col_off] = true;
  board[4 + row_off][14 + col_off] = board[5 + row_off][14 + col_off] =
      board[6 + row_off][14 + col_off] = true;

  board[10 + row_off][2 + col_off] = board[11 + row_off][2 + col_off] =
      board[12 + row_off][2 + col_off] = true;
  board[10 + row_off][7 + col_off] = board[11 + row_off][7 + col_off] =
      board[12 + row_off][7 + col_off] = true;
  board[10 + row_off][9 + col_off] = board[11 + row_off][9 + col_off] =
      board[12 + row_off][9 + col_off] = true;
  board[10 + row_off][14 + col_off] = board[11 + row_off][14 + col_off] =
      board[12 + row_off][14 + col_off] = true;
}