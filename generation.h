//dungeon generation algorithm
//maze generation adapted from https://pcg.wdfiles.com/local--files/pcg-algorithm%3Amaze/growingtree.py

#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define size 101 

typedef struct coord {
	char x;
	char y;
} coord;

void carve(char x, char y, char grid[size][size], coord todo[(size + 1)*(size + 1)], int* top){
	grid[x][y] = 1;

	coord new[4];
	char newcount = 0;

	for(int i = 0; i < 4; i++){
		new[i] = (coord) {0,0};
	}

	if(x > 0){
		if(grid[x-1][y] == 3){
			grid[x-1][y] = 2;
			new[newcount] = (coord) {x-1,y};
			newcount++;
		}
	}
	if(x < (size - 1)){
		if(grid[x+1][y] == 3){
			grid[x+1][y] = 2;
			new[newcount] = (coord) {x+1,y};
			newcount++;
		}
	}
	if(y > 0){
		if(grid[x][y-1] == 3){
			grid[x][y-1] = 2;
			new[newcount] = (coord) {x,y-1};
			newcount++;
		}
	}
	if(y < (size - 1)){
		if(grid[x][y+1] == 3){
			grid[x][y+1] = 2;
			new[newcount] = (coord) {x,y+1};
			newcount++;
		}
	}
	for(int j = newcount-1; j > 0; j--){
		int swap = rand() % (j + 1);
		coord temp = new[swap];
		new[swap] = new[j];
		new[j] = temp;
	}
	
	
	for(int k = 0; k < newcount; k++){


		*top = *top + 1;

		todo[*top] = new[k];

	}	
}

char check(char x, char y, char grid[size][size]){
	int count = 0;
	if (x > 0){
		if (grid[x-1][y] == 1) count++;
	}
	if (x < (size - 1)){
		if (grid[x+1][y] == 1) count++;
	}
	if (y > 0) {
		if (grid[x][y-1] == 1) count++;
	}
	if (y < (size - 1)) {
		if (grid[x][y+1] == 1) count++;
	}

	if(count == 1){
		return 1;
	} else {
		return 0;
	}
}

void printgrid(char grid[size][size]){
	for(int i = 0; i < size; i++){
		for(int j = 0; j < size; j++){
			if(grid[i][j] == 1){
				printf(" ");
			} else {
				printf("%d", (int) grid[i][j]);
			}
		}
		printf("\n");
	}
}

void makegrid(char grid[size][size]){
	//char grid[size][size] = {{0}};

	for(int i = 0; i < size; i++){
		for(int j = 0; j < (size + 1); j++){
			grid[i][j] = 3;
		}
	}
	srand(time(NULL));
	// 0 = wall
	// 1 = empty
	// 2 = exposed and undetermined
	// 3 = unexposed and undetermined

	coord todo[(size + 1)*(size + 1)] = {0};
	int top = -1;
	carve((size/2),(size/2),grid, todo, &top);



	while(top > -1){
		int choice = rand() % (top + 1);
		if(check(todo[choice].x, todo[choice].y, grid)){
			carve(todo[choice].x,todo[choice].y, grid, todo, &top);
			
		}else{
			grid[todo[choice].x][todo[choice].y] = 0;
		}
		memcpy(&todo[choice], &todo[choice+1], (top - choice) * sizeof(coord));


		top--;
	}

	int startingroomsize = ceil(size / 5);
	for(int c = (size / 2) - (startingroomsize / 2); c < (size / 2) + (startingroomsize / 2); c++){
		for(int r = (size / 2) - (startingroomsize / 2); r < (size / 2) + (startingroomsize / 2); r++){
			grid[r][c] = 1;

		}
	}
	
	int extrarooms = (rand() % 4) + 6;
	for(int w = 0; w < extrarooms; w++){
		int width = (rand() % 10) + 10;
		int height = (rand() % 10) + 10;
		
		int xpos = (rand() % (size - width - 2)) + 1;
		int ypos = (rand() % (size - height - 2)) + 1;
		for(int i = xpos; i < xpos + width; i++){
			for(int j = ypos; j < ypos + height; j++){
				grid[i][j] = 1;
			}
		}

	}
}

