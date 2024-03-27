#include "raylib.h"
#include "raymath.h"
#include <stdio.h>
#include <stdlib.h>

#define MAX_SPEED 0.2f
#define ACCEL_VALUE 0.005f
#define DECEL_VALUE 0.005f
#define MAX_ROTATE 0.5f
#define HITBOX_SIZE 1.0f

typedef struct character {
	Vector3 position;
	Vector3 direction;
	Vector3 velocity;
	Vector3 acceleration;
	float mouseSens;
	float height;
} character;

static void CharYaw(character* character, float angle){
	Vector3 up = (Vector3) {0.0f,1.0f,0.0f};
	
	character->direction = Vector3RotateByAxisAngle(character->direction, up, angle);
}

static void CharPitch(character* character, float angle){
	Vector3 right = Vector3CrossProduct(character->direction, (Vector3) {0.0f,1.0f,0.0f});

	character->direction = Vector3RotateByAxisAngle(character->direction, right, angle); 
}

static void CharLook(character* character, int framerate){
	Vector2 mousePosDelt = GetMouseDelta();

	CharYaw(character, -mousePosDelt.x*character->mouseSens);
	CharPitch(character, -mousePosDelt.y*character->mouseSens);
}

void UpdateCharacterNewNew(character* character, int framerate, char grid[101][101], BoundingBox bbox, char* blinded){
	CharLook(character, framerate);
	
	Vector3 forwarddir = (Vector3) {character->direction.x, 0.0f, character->direction.z};
	Vector3 rightdir = Vector3CrossProduct(character->direction, (Vector3) {0.0f,1.0f,0.0f});

	forwarddir = Vector3Normalize(forwarddir);
	rightdir = Vector3Normalize(rightdir);

	int forward = 0;
	int right = 0;

	if(IsKeyDown(KEY_W)) forward++;
	if(IsKeyDown(KEY_S)) forward--;

	if(IsKeyDown(KEY_D)) right++;
	if(IsKeyDown(KEY_A)) right--;

	Vector3 movedirection = Vector3Add(Vector3Scale(forwarddir, forward), Vector3Scale(rightdir, right));
	movedirection.y = 0;
	movedirection = Vector3Normalize(movedirection);
	movedirection = Vector3Scale(movedirection, MAX_SPEED*(240.0f/framerate));

	int charblockx = floor((character->position.x + 510.0f) / 10.0f);
	int charblockz = floor((character->position.z + 510.0f) / 10.0f);
	bool xmove = true;
	bool zmove = true;
	
	*blinded = 0;
	if(grid[charblockx][charblockz] == 4){
		*blinded = 1;
	}

	int xdiff;
	if(movedirection.x > 0){
		xdiff = 1;
	} else {
		xdiff = -1;
	}
	
	for(int zch = -1; zch < 2; zch = zch + 2){
		int blockx = floor((character->position.x + (xdiff * HITBOX_SIZE) + 510.0f) / 10.0f);
		int blockz = floor((character->position.z + (zch * HITBOX_SIZE * 0.6f) + 510.0f) / 10.0f);
		if(grid[blockx][blockz] == 0){
			xmove = false;
		}
		blockz = floor((character->position.z + (0.5f * zch) + 510.0f) / 10.0f);
		blockx = floor((character->position.x + 510.0f) / 10.0f);
		if(grid[blockx][blockz] == 4){
			*blinded = 1;
		}
	}		

	int zdiff;
	if(movedirection.z > 0){
		zdiff = 1;
	} else {
		zdiff = -1;
	}

	for(int xch = -1; xch < 2; xch = xch + 2){
		int blockx = floor((character->position.x + (xch * HITBOX_SIZE * 0.6f) + 510.0f) / 10.0f);
		int blockz = floor((character->position.z + (zdiff * HITBOX_SIZE) + 510.0f) / 10.0f);
		if(grid[blockx][blockz] == 0){
			zmove = false;
		}
		blockx = floor((character->position.x + (0.5f * xch) + 510.0f) / 10.0f);
	        blockz = floor((character->position.z + 510.0f) / 10.0f);
		if(grid[blockx][blockz] == 4){
			*blinded = 1;
		}	
	}	


	if(xmove == false){
		movedirection.x = 0.0f;
	}
	if(zmove == false){
		movedirection.z = 0.0f;
	}

	character->velocity = movedirection;
	character->position = Vector3Add(character->position, movedirection);
}
