#include "raylib.h"
#include "raymath.h"
#include <stdio.h>

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

	CharYaw(character, -mousePosDelt.x*character->mouseSens*(240.0f/framerate));
	CharPitch(character, -mousePosDelt.y*character->mouseSens*(240.0f/framerate));
}

void UpdateCharacterNewNew(character* character, int framerate, char grid[101][101]){
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
	movedirection = Vector3Scale(movedirection, MAX_SPEED);

		
	Vector3 bitmore = movedirection;
	bitmore = Vector3Normalize(bitmore);
	bitmore = Vector3Scale(bitmore, 1.0f);
	


	//collision with grid
	Vector2 charblock = {0};
	Vector3 newposxmov = (Vector3) {character->position.x + movedirection.x, character->position.y, character->position.z};
	newposxmov = (Vector3) {newposxmov.x + bitmore.x, newposxmov.y, newposxmov.z};
	Vector3 newposzmov = (Vector3) {character->position.x, character->position.y, character->position.z + movedirection.z};
	newposzmov = (Vector3) {newposzmov.x, newposzmov.y, newposzmov.z + bitmore.z};

	charblock.x = floor((newposxmov.x + 510.0f) / 10.0f);
	int xblock = (int)(floor(character->position.x + 510.0f) / 10.0f);
	charblock.y = floor((newposzmov.z + 510.0f) / 10.0f);
	int zblock = (int)(floor(character->position.z + 510.0f) / 10.0f);

	if(grid[(int)charblock.x][zblock] == 0){
		movedirection.x = 0.0f;
		DrawRectangle(10 + (3*(int)charblock.x), GetScreenHeight() - (105*3) + (3*zblock), 3, 3, GREEN);
	}
	if(grid[xblock][(int)charblock.y] == 0){
		movedirection.z = 0.0f;
	}
		

	character->velocity = movedirection;
	character->position = Vector3Add(character->position, movedirection);
}

void UpdateCharacterNew(character* character, int framerate){
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
	
	float currspeed = Vector3Length(character->velocity);
	
	character->acceleration = Vector3Normalize(character->acceleration);
	character->acceleration = Vector3Scale(movedirection, ACCEL_VALUE);

	if(forward == 0 && right == 0){
		Vector3 decell = Vector3Scale(character->velocity, -1.0f);
		decell = Vector3Normalize(decell);
		decell = Vector3Scale(decell, DECEL_VALUE);
		character->acceleration = decell;
	}
	

	character->velocity = Vector3Add(character->velocity, character->acceleration);
	character->velocity = Vector3Add(Vector3Scale(movedirection, currspeed), character->velocity);

	if(Vector3Length(character->velocity) < 0.0001f && forward == 0 && right == 0){
		character->velocity = (Vector3) {0.0f,0.0f,0.0f};
	}

	
	if(Vector3Length(character->velocity) > MAX_SPEED){
		character->velocity = Vector3Normalize(character->velocity);
		character->velocity = Vector3Scale(character->velocity, MAX_SPEED);
		
	}
	

	character->position = Vector3Add(character->position, character->velocity);

}

void UpdateCharacter(character* character, int framerate){
	CharLook(character, framerate);

	Vector3 rightdir = Vector3CrossProduct(character->direction, (Vector3) {0.0f,1.0f,0.0f});

	int forward = 0;
	int right = 0;

	if(IsKeyDown(KEY_W)) forward++;
	if(IsKeyDown(KEY_S)) forward--;

	if(IsKeyDown(KEY_D)) right++;
	if(IsKeyDown(KEY_A)) right--;
	

	if(IsKeyDown(KEY_SPACE)) character->position = Vector3Add(character->position, (Vector3) {0.0f,1.0f,0.0f});	
	if(IsKeyDown(KEY_LEFT_SHIFT)) character->position = Vector3Add(character->position, (Vector3) {0.0f,-1.0f,0.0f});	
	Vector3 fwd = {0};
	fwd.x = character->direction.x;
	fwd.z = character->direction.z;
	fwd = Vector3Normalize(fwd);

	Vector3 rght = {0};
	rght.x = rightdir.x;
	rght.z = rightdir.z;
	rght = Vector3Normalize(rght);

	character->acceleration = (Vector3) {0.0f,0.0f,0.0f};

	Vector3 fwdaccel = {0};
	Vector3 rghtaccel = {0};

	if(forward == 0){
		fwdaccel = Vector3Scale(fwd, -Vector3DotProduct(fwd, character->velocity));

		fwdaccel = Vector3Normalize(fwdaccel);

		fwdaccel = Vector3Scale(fwdaccel, DECEL_VALUE);
	} else {
		fwdaccel = Vector3Normalize(fwd);
		fwdaccel = Vector3Scale(fwd, (float) forward);
	}
	if(right == 0){
		rghtaccel = Vector3Scale(rght, -Vector3DotProduct(rght, character->velocity));
		rghtaccel = Vector3Normalize(rghtaccel);
		rghtaccel = Vector3Scale(rghtaccel, DECEL_VALUE);
	} else {
		rghtaccel = Vector3Normalize(rghtaccel);
		rghtaccel = Vector3Scale(rght, (float) right);
	}

	character->acceleration = Vector3Add(character->acceleration, fwdaccel);
	character->acceleration = Vector3Add(character->acceleration, rghtaccel);
	
	
	character->acceleration = Vector3Scale(character->acceleration, 0.001f);
	character->velocity = Vector3Add(character->velocity, character->acceleration);

	character->velocity = Vector3ClampValue(character->velocity, 0, MAX_SPEED);

	if(Vector3Length(character->velocity) > 0.01f || !(forward == 0 && right == 0)){
		character->position = Vector3Add(character->position, character->velocity);		
	} else {
		character->velocity = (Vector3) {0.0f,0.0f,0.0f};
	}	
	
	/*
	printf("%f %f %f\n", character->acceleration.x, character->acceleration.y, character->acceleration.z);

	printf("%f %f %f\n", character->velocity.x, character->velocity.y, character->velocity.z);

	printf("%f %f %f\n\n", character->position.x, character->position.y, character->position.z);
	*/
}
