#include "raylib.h"
#include "raymath.h"
#include <stdio.h>

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
}
