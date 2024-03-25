#include "raylib.h"
#include "raymath.h"
#include <stdio.h>
#include <stdlib.h>

#define MAX_SPEED 0.2f
#define ACCEL_VALUE 0.005f
#define DECEL_VALUE 0.005f
#define MAX_ROTATE 0.5f

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

void UpdateCharacterNewNew(character* character, int framerate, char grid[101][101], BoundingBox bbox){
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

	Vector3 newpos = Vector3Add(character->position, movedirection);

	int charblockx = floor((newpos.x + 510.0f) / 10.0f);
	int charblockz = floor((newpos.z + 510.0f) / 10.0f);
	bool xmove = true;
	bool zmove = true;

	int xdiff;
	if(movedirection.x > 0){
		xdiff = 1;
	} else {
		xdiff = -1;
	}

	if(grid[charblockx + xdiff][charblockz] == 0){
		float blockxpos = -505.0f + (10*(charblockx+xdiff));
		float blockzpos = -505.0f + (10*charblockz);
		Vector3 blockpos = (Vector3) {blockxpos, 0.0f, blockzpos};
		BoundingBox box = (BoundingBox) {Vector3Add(bbox.min, blockpos), Vector3Add(bbox.max, blockpos)};
		if(CheckCollisionBoxSphere(box, character->position, 2.0f)){
			xmove = false;
		}
	}

	int zdiff;
	if(movedirection.z > 0){
		zdiff = 1;
	} else {
		zdiff = -1;
	}
	if(grid[charblockx][charblockz + zdiff] == 0){
		float blockxpos = -505.0f + (10*charblockx);
		float blockzpos = -505.0f + (10*(charblockz+zdiff));
		Vector3 blockpos = (Vector3) {blockxpos, 0.0f, blockzpos};
		BoundingBox box = (BoundingBox) {Vector3Add(bbox.min, blockpos), Vector3Add(bbox.max, blockpos)};
		if(CheckCollisionBoxSphere(box, character->position, 2.0f)){
			zmove = false;
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
