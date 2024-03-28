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

void DrawTheGrid(char grid[101][101], character thechar, Model blockmodel){
	DrawCube((Vector3) {0.0f,-10.0f,0.0f}, 1000.0f, 10.0f, 1000.0f, BROWN);

	for(int i = 0; i < 101; i++){
		for(int j = 0; j < 101; j++){
			if(grid[i][j] == 0){
				Vector3 blockpos = (Vector3) {-505.0f + (10*i), 0.0f, -505.0f + (10*j)};
				Vector3 dirvec = directionVector3(&(thechar.position), &blockpos);
				float prod = Vector3DotProduct(dirvec, thechar.direction);
				if(prod < 1.0f){
					DrawModel(blockmodel, (Vector3) {-505.0f + (10*i), 0.0f, -505.0f + (10*j)}, 1.0f, WHITE);
				}

			}
			if(grid[i][j] == 4){
				DrawModel(blockmodel, (Vector3) {-505.0f + (10*i), 0.0f, -505.0f + (10*j)}, 1.0f, BLACK);

			}
		}
	}
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
	room* rooms;
	int roomcount;
	newmakegrid(grid, rooms, &roomcount);
	SetMousePosition(400,300);

	while(!WindowShouldClose()){
		sphereMovement = directionVector3(&camera.position, &spherePos);
	        sphereMovement = Vector3Scale(sphereMovement, 0.01f);	
		spherePos = Vector3Add(spherePos, sphereMovement);
		
		float ft = GetFrameTime();
		char blinded = 0;
		UpdateCharacter(&thechar, (1.0f/ft), grid, &blinded);
		
		Vector2 charblock = {0};

		charblock.x = floor((thechar.position.x + 510.0f) / 10.0f);
		charblock.y = floor((thechar.position.z + 510.0f) / 10.0f);

		moveCamera(&camera.position, &thechar.position, thechar.height);
		camera.target = Vector3Add(camera.position, thechar.direction);

		BeginDrawing();

		BeginMode3D(camera);

		ClearBackground(BLACK);

		if(!blinded){
			ClearBackground(SKYBLUE);

			DrawTheGrid(grid, thechar, blockmodel);
			
			DrawSphere(spherePos, sphereRadius, RED); 
		} else {
			ClearBackground(BLACK);
		}

		EndMode3D();
		
		DrawFPS(10,10);

		int screenheight = GetScreenHeight();

		int minimapscale = 3;

		for(int i = 0; i < 101; i++){
			for(int j = 0; j < 101; j++){
				if(grid[i][j] == 0){
					DrawRectangle(10 + (minimapscale*i), screenheight - (105*minimapscale) + (minimapscale*j), minimapscale, minimapscale, WHITE);
				}
				if(grid[i][j] == 4){
					DrawRectangle(10 + (minimapscale*i), screenheight - (105*minimapscale) + (minimapscale*j), minimapscale, minimapscale, BLACK);
				}
			}
		}
		
		//Draw char on map
		
		DrawRectangle(10 + (minimapscale*charblock.x), screenheight - (105*minimapscale) + (minimapscale*charblock.y), minimapscale, minimapscale, RED);

		EndDrawing();
	}
	free(rooms);
	UnloadModel(blockmodel);
	UnloadTexture(blocktexture);
	UnloadModel(model);
	UnloadTexture(texture);
	CloseWindow();
}
