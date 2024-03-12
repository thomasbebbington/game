#include "raylib.h"
#include "raymath.h"
#include <stdio.h>

#define MAX_SPEED 0.1f
#define DECEL_VALUE 2.0f

typedef struct character {
	Vector3 position;
	Vector3 direction;
	Vector3 velocity;
	Vector3 acceleration;
	float mouseSens;
	float height;
} character;

void CharYaw(character* character, float angle){
	Vector3 up = (Vector3) {0.0f,1.0f,0.0f};
	
	character->direction = Vector3RotateByAxisAngle(character->direction, up, angle);
}

void CharPitch(character* character, float angle){
	Vector3 right = Vector3CrossProduct(character->direction, (Vector3) {0.0f,1.0f,0.0f});

	character->direction = Vector3RotateByAxisAngle(character->direction, right, angle); 
}

void UpdateCharacter(character* character, int framerate){
	Vector2 mousePosDelt = GetMouseDelta();

	CharYaw(character, -mousePosDelt.x*character->mouseSens*(240.0f/framerate));
	CharPitch(character, -mousePosDelt.y*character->mouseSens*(240.0f/framerate));

	Vector3 rightdir = Vector3CrossProduct(character->direction, (Vector3) {0.0f,1.0f,0.0f});

	int forward = 0;
	int right = 0;

	if(IsKeyDown(KEY_W)) forward++;
	if(IsKeyDown(KEY_S)) forward--;

	if(IsKeyDown(KEY_D)) right++;
	if(IsKeyDown(KEY_A)) right--;
		
	Vector3 fwd = {0};
	fwd.x = character->direction.x;
	fwd.z = character->direction.z;

	Vector3 rght = {0};
	rght.x = rightdir.x;
	rght.z = rightdir.z;


	character->acceleration = (Vector3) {0.0f,0.0f,0.0f};

	Vector3 fwdaccel = {0};
	Vector3 rghtaccel = {0};

	if(forward == 0){
		fwdaccel = Vector3Scale(fwd, -Vector3DotProduct(fwd, character->velocity));

		fwdaccel = Vector3Normalize(fwdaccel);

		fwdaccel = Vector3Scale(fwdaccel, DECEL_VALUE);
	} else {
		fwdaccel = Vector3Scale(fwd, (float) forward);
	}
	if(right == 0){
		rghtaccel = Vector3Scale(rght, -Vector3DotProduct(rght, character->velocity));
		rghtaccel = Vector3Normalize(rghtaccel);
		rghtaccel = Vector3Scale(rghtaccel, DECEL_VALUE);
	} else {
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
