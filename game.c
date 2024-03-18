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

	Mesh blockmesh = GenMeshCube(10.0f,10.0f,10.0f);
	Model blockmodel = LoadModelFromMesh(blockmesh);

	Texture2D blocktexture = LoadTexture("stone.png");
	blockmodel.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = blocktexture;

	Mesh cube = GenMeshCube(101.0f, 1.0f, 101.0f);
	Model model = LoadModelFromMesh(cube);

	Texture2D texture = LoadTexture("missing.png");
	model.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = texture;
	
	char grid[101][101] = {{0}};
	char discovered[101][101] = {{0}};
	//makegrid(grid);
	newmakegrid(grid);
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

		ClearBackground(SKYBLUE);

		BeginMode3D(camera);

		int blockcount = 0;

		for(int i = 0; i < 101; i++){
			for(int j = 0; j < 101; j++){
				if(grid[i][j] == 0){
					Vector3 blockpos = (Vector3) {-505.0f + (10*i), 0.0f, -505.0f + (10*j)};
					Vector3 dirvec = directionVector3(&(thechar.position), &blockpos);
					float prod = Vector3DotProduct(dirvec, thechar.direction);
					if(prod < 0){
						blockcount++;
						DrawModel(blockmodel, (Vector3) {-505.0f + (10*i), 0.0f, -505.0f + (10*j)}, 1.0f, WHITE);
					}

				}
				if(grid[i][j] == 4){
					DrawModel(blockmodel, (Vector3) {-505.0f + (10*i), 0.0f, -505.0f + (10*j)}, 1.0f, BLACK);

				}
			}
		}
		//printf("Block count: %d\n", blockcount);

		//DrawCube((Vector3) {0.0f,-1.0f,0.0f}, 100.0f, 1.0f, 100.0f, BROWN);
		//DrawModel(model, (Vector3) {0.0f,-10.0f,0.0f}, 10.0f, BROWN);

		//DrawModel(model, (Vector3) {0.0f,10.0f,0.0f}, 10.0f, BROWN);
		//DrawCube((Vector3) {0.0f,4.0f,0.0f}, 100.0f, 1.0f, 100.0f, GRAY);

		DrawSphere(spherePos, sphereRadius, RED); 

		EndMode3D();
		
		DrawFPS(10,10);

		int screenheight = GetScreenHeight();

		/*
		DrawLine(10, screenheight - 10, 10, screenheight - 210, WHITE);
		DrawLine(10, screenheight - 10, 210, screenheight - 10, WHITE);
		DrawLine(10, screenheight - 210, 210, screenheight - 210, WHITE);
		DrawLine(210, screenheight - 210, 210, screenheight - 10, WHITE);
		*/

		for(int i = 0; i < 101; i++){
			for(int j = 0; j < 101; j++){
				if(grid[i][j] == 0){
					DrawRectangle(10 + (2*i), screenheight - 210 + (2*j), 2, 2, WHITE);
				}
				if(grid[i][j] == 4){
					DrawRectangle(10 + (2*i), screenheight - 210 + (2*j), 2, 2, BLACK);
				}
			}
		}
		EndDrawing();
	}
	UnloadModel(blockmodel);
	UnloadTexture(blocktexture);
	UnloadModel(model);
	UnloadTexture(texture);
	CloseWindow();
}
