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

typedef struct room{
	int xpos;
	int ypos;
	int width;
	int height;
} room;

static void carve(char x, char y, char grid[size][size], coord todo[(size + 1)*(size + 1)], int* top){
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

static char check(char x, char y, char grid[size][size]){
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

static void domaze(char grid[size][size], char startx, char starty){
	coord todo[(size + 1)*(size + 1)] = {0};
	int top = -1;
	carve(startx, starty, grid, todo, &top);

	while(top > -1){
		int choice = rand() % (top + 1);
		if(check(todo[choice].x, todo[choice].y, grid)){
			carve(todo[choice].x,todo[choice].y, grid, todo, &top);
			
		}else{
			grid[todo[choice].x][todo[choice].y] = 0;
		}
		memmove(&todo[choice], &todo[choice+1], (top - choice) * sizeof(coord));


		top--;
	}
}

/*
2 to 5
5 - 2 = 3

rand() % 3 is 0 to 2
0 to 2 + 3 is 3 to 5
*/

static int randint(int min, int max){
	return (rand() % (max - min)) + min;
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
		memmove(&todo[choice], &todo[choice+1], (top - choice) * sizeof(coord));


		top--;
	}

	room startingroom = {0};
	int startingroomsize = ceil(size / 5);
	startingroom.width = startingroomsize;
	startingroom.height = startingroomsize;
	startingroom.xpos = (size / 2) - (startingroomsize/2);
	startingroom.ypos = startingroom.xpos;
	for(int c = (size / 2) - (startingroomsize / 2); c < (size / 2) + (startingroomsize / 2); c++){
		for(int r = (size / 2) - (startingroomsize / 2); r < (size / 2) + (startingroomsize / 2); r++){
			grid[r][c] = 1;

		}
	}
	
	int extrarooms = (rand() % 4) + 6;

	const int roomcount = extrarooms+1;
	room rooms[roomcount];
	rooms[0] = startingroom;

	for(int w = 0; w < extrarooms; w++){
		int width = (rand() % 10) + 10;
		int height = (rand() % 10) + 10;
		
		int xpos = (rand() % (size - width - 2)) + 1;
		int ypos = (rand() % (size - height - 2)) + 1;
		rooms[w + 1].xpos = xpos;
		rooms[w + 1].ypos = ypos;
		rooms[w + 1].width = width;
		rooms[w + 1].height = height;

		for(int i = xpos; i < xpos + width; i++){
			for(int j = ypos; j < ypos + height; j++){
				grid[i][j] = 1;
			}
		}

	}

}

static char checkroomcollision(room* rooms, int roomtocheck){
	for(int i = 0; i < roomtocheck; i++){
		if(rooms[i].xpos + rooms[i].width + 4 > rooms[roomtocheck].xpos
				&& rooms[i].xpos < rooms[roomtocheck].xpos + rooms[roomtocheck].width + 4
				&& rooms[i].ypos + rooms[i].height + 4> rooms[roomtocheck].ypos
				&& rooms[i].ypos < rooms[roomtocheck].ypos + rooms[roomtocheck].height + 4){
			return 0;
		}
	}
	return 1;
}

static void removedeadends(char grid[size][size]){
goagain:
	for(int i = 1; i < size - 1; i++){
		for(int j = 1; j < size - 1; j++){
			if(grid[i][j] == 1){
				int adjcount = 0;
				if(grid[i-1][j] == 1 || grid[i-1][j] == 4) adjcount++;
				if(grid[i][j-1] == 1 || grid[i][j-1] == 4) adjcount++;
				if(grid[i+1][j] == 1 || grid[i+1][j] == 4) adjcount++;
				if(grid[i][j+1] == 1 || grid[i][j+1] == 4) adjcount++;

				if(adjcount == 1){
					grid[i][j] = 0;
					goto goagain;
				}
			}
		}
	}
}

static void cleanmaze(char grid[size][size]){
	for(int i = 1; i < size - 1; i++){
		for(int j = 1; j < size - 1; j++){
			if(grid[i][j] == 0){
				int adjcount = 0;
				if(grid[i-1][j] == 0 || grid[i-1][j] == 3) adjcount++;
				if(grid[i][j-1] == 0 || grid[i][j-1] == 3) adjcount++;
				if(grid[i+1][j] == 0 || grid[i+1][j] == 3) adjcount++;
				if(grid[i][j+1] == 0 || grid[i][j+1] == 3) adjcount++;

				if(adjcount == 4){
					grid[i][j] = 3;
				}
			}
		}
	}
}

void newmakegrid(char grid[size][size]){
	srand(time(NULL));

	room startingroom = {0};
	startingroom.width = ceil(size / 5);
	startingroom.height = startingroom.width;
	startingroom.xpos = (size / 2) - (startingroom.width / 2);
	startingroom.ypos = (size / 2) - (startingroom.height / 2);

	int extrarooms = (rand() % 4) + 6;

	int roomcount = extrarooms + 1;
	room rooms[roomcount];
	
	rooms[0] = startingroom;
	
	for(int j = 1; j < roomcount; j++){
		int roomwidth = (rand() % 10) + 10;
		int roomheight = (rand() % 10) + 10;

		rooms[j].width = roomwidth;
		rooms[j].height = roomheight;
	}

	for(int i = 1; i < roomcount; i++){
		int attemptsremaining = 4;

		while(1){
			int xpos = randint(2, size - rooms[i].width - 1);
			int ypos = randint(2, size - rooms[i].height - 1);
			
			rooms[i].xpos = xpos;
			rooms[i].ypos = ypos;

			if(checkroomcollision(rooms, i)){
				break;
			}

			if(attemptsremaining>0){
				attemptsremaining--;
			} else {
				rooms[i].xpos = -1;
				rooms[i].ypos = -1;
				break;
			}
		}
	}
	
	for(int i = 0; i < size; i++){
		for(int j = 1; j < size - 1; j++){
			grid[i][j] = 3;
		}
	}

	for(int i = 0; i < size; i++){
		grid[i][0] = 0;
		grid[i][size - 1] = 0;
	}
	for(int j = 0; j < size; j++){
		grid[0][j] = 0;
		grid[size - 1][j] = 0;
	}

	for(int i = 0; i < roomcount; i++){
		//grid[rooms[i].xpos - 1][rooms[i].ypos - 1] = 0;
		if(rooms[i].xpos > 0){
			for(int r = rooms[i].xpos - 1; r < rooms[i].xpos + rooms[i].width + 2; r++){
				grid[r][rooms[i].ypos - 1] = 0;
				grid[r][rooms[i].ypos + rooms[i].height + 1] = 0;
			}
			for(int c = rooms[i].ypos - 1; c < rooms[i].ypos + rooms[i].height + 2; c++){
				grid[rooms[i].xpos - 1][c] = 0;
				grid[rooms[i].xpos + rooms[i].width + 1][c] = 0;
			}	
			for(int r = rooms[i].xpos; r < rooms[i].xpos + rooms[i].width + 1; r++){
				for(int c = rooms[i].ypos; c < rooms[i].ypos + rooms[i].height + 1; c++){
					grid[r][c] = 1;
				}
			}
		}
	}	



	domaze(grid, rooms[0].xpos - 2, rooms[0].ypos - 2);			
	int possdoors[20];
	int countpossdoors = 0;

	for(int i = 0; i < roomcount; i++){
		if(rooms[i].ypos > 2){
			countpossdoors = 0;

			for(int r = rooms[i].xpos; r < rooms[i].xpos + rooms[i].width + 1; r++){
				if(grid[r][rooms[i].ypos - 2] == 1){
					possdoors[countpossdoors] = r;
					countpossdoors++;
				}
			}

			if(countpossdoors > 0){
				int door = randint(0, countpossdoors - 1);
				grid[possdoors[door]][rooms[i].ypos - 1] = 4;
			}

		}
		if(rooms[i].ypos < size - 2 && rooms[i].ypos > -1){
			countpossdoors = 0;

			for(int r = rooms[i].xpos; r < rooms[i].xpos + rooms[i].width + 1; r++){
				if(grid[r][rooms[i].ypos + rooms[i].height + 2] == 1){
					possdoors[countpossdoors] = r;
					countpossdoors++;
				}
			}

			if(countpossdoors > 0){
				int door = randint(0, countpossdoors - 1);
				grid[possdoors[door]][rooms[i].ypos + rooms[i].height + 1] = 4; 
			}
		}
		if(rooms[i].xpos > 2){
			countpossdoors = 0;

			for(int c = rooms[i].ypos; c < rooms[i].ypos + rooms[i].height + 1; c++){
				if(grid[rooms[i].xpos - 2][c] == 1){
					possdoors[countpossdoors] = c;
					countpossdoors++;
				}
			}

			if(countpossdoors > 0){
				int door = randint(0, countpossdoors - 1);
				grid[rooms[i].xpos - 1][possdoors[door]] = 4;
			}
					}
		if(rooms[i].xpos < size - 2 && rooms[i].xpos > -1){
			countpossdoors = 0;

			for(int c = rooms[i].ypos; c < rooms[i].ypos + rooms[i].height + 1; c++){
				if(grid[rooms[i].xpos + rooms[i].width + 2][c] == 1){
					possdoors[countpossdoors] = c;
					countpossdoors++;
				}
			}
		
			if(countpossdoors > 0){
				int door = randint(0, countpossdoors - 1);
				grid[rooms[i].xpos + rooms[i].width + 1][possdoors[door]] = 4;
			}
		}
	}

	removedeadends(grid);
	cleanmaze(grid);
}

