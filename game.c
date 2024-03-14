#include "raylib.h"
#include "raymath.h"
#include "character.h"
#include "generation.h"

#include <stdio.h>
#include <math.h>

Vector3 directionVector3(Vector3* v1, Vector3* v2){
	Vector3 diff = (Vector3) {v1->x - v2->x, v1->y - v2->y, v1->z - v2->z};
	diff = Vector3Normalize(diff);
	return diff;
}

void moveCamera(Vector3* camPos, Vector3* charPos, float charHeight){
	camPos->x = charPos->x;
	camPos->y = charPos->y + charHeight;
	camPos->z = charPos->z;
}

void main(){
	const int w = 800;
	const int h = 600;

	const int framerate = 240;

	InitWindow(w, h, "The Game!");

	SetTargetFPS(framerate);

	character thechar = {0};
	thechar.position = (Vector3) {0.0f,0.0f,0.0f};
	thechar.direction = (Vector3) {1.0f,0.0f,0.0f};
	thechar.velocity = (Vector3) {0.0f,0.0f,0.0f};
	thechar.acceleration = (Vector3) {0.0f,0.0f,0.0f};
	thechar.mouseSens = 0.003f;
	thechar.height = 2.0f;

	Camera camera = {0};
	camera.position = (Vector3){thechar.position.x, thechar.position.y + thechar.height, thechar.position.z};
	
	camera.fovy = 90.0f;
	camera.up = (Vector3) {0.0f, 1.0f, 0.0f};
	camera.projection = CAMERA_PERSPECTIVE;

	DisableCursor();

	Vector3 spherePos = (Vector3) {10.0f, 2.0f, 0.0f};
	Vector3 sphereMovement = (Vector3) {0.0f,0.0f,0.0f};
	float sphereRadius = 1.0f;


	camera.target = spherePos;

	int cameraMode = CAMERA_FIRST_PERSON;

	Mesh blockmesh = GenMeshCube(1.0f,6.0f,1.0f);
	Model blockmodel = LoadModelFromMesh(blockmesh);

	Texture2D blocktexture = LoadTexture("stone.png");
	blockmodel.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = blocktexture;

	Mesh cube = GenMeshCube(101.0f, 1.0f, 101.0f);
	Model model = LoadModelFromMesh(cube);

	Texture2D texture = LoadTexture("missing.png");
	model.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = texture;
	
	char grid[101][101] = {{0}};
	makegrid(grid);
	printgrid(grid);

	while(!WindowShouldClose()){
		//UpdateCamera(&camera, cameraMode);
		//Move Sphere
		sphereMovement = directionVector3(&camera.position, &spherePos);
	        sphereMovement = Vector3Scale(sphereMovement, 0.01f);	
		//spherePos = Vector3Add(spherePos, sphereMovement);

		/*
		if(CheckCollisionSpheres(spherePos, sphereRadius, camera.position, 0)){
			break;
		}
		*/

		UpdateCharacter(&thechar, framerate);	
		moveCamera(&camera.position, &thechar.position, thechar.height);
		camera.target = Vector3Add(camera.position, thechar.direction);

		BeginDrawing();

		ClearBackground(RAYWHITE);

		BeginMode3D(camera);

		for(int i = 0; i < 101; i++){
			for(int j = 0; j < 101; j++){
				if(grid[i][j] == 0 || grid[i][j] == 3){
					DrawModel(blockmodel, (Vector3) {-50.5f + i, 0.0f, -50.5f + j}, 1.0f, WHITE);
				}
			}
		}

		//DrawCube((Vector3) {0.0f,-1.0f,0.0f}, 100.0f, 1.0f, 100.0f, BROWN);
		DrawModel(model, (Vector3) {0.0f,-1.0f,0.0f}, 1.0f, WHITE);
		//DrawCube((Vector3) {0.0f,4.0f,0.0f}, 100.0f, 1.0f, 100.0f, GRAY);

		DrawSphere(spherePos, sphereRadius, RED); 

		EndMode3D();
		
		DrawFPS(10,10);
		EndDrawing();
	}
	UnloadModel(model);
	UnloadTexture(texture);
	CloseWindow();
}
