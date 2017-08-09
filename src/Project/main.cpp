#include <cstdio>
#include <cstdlib>
#include <vector>
#include <iostream>
#include <cmath>
#include <ctime>

#include <GL/glew.h>
#include <GL/glfw3.h>

#include <GL/freeglut.h>

#include "shader.h"
#include "transform.h"
#include "camera.h"
#include "projection.h"
#include "mathematics.h"
#include "vertex.h"
#include "texture.h"
#include "mesh.h"
#include "drawing.h"
#include "shadow.h"

const int INIT_WIDTH = 1024, INIT_HEIGHT = 768;
const double TIME_STEP = 0.001;
float DELTA = 0.095;
const int BOXSIZE = 10;

vec3 camPos(9.04906f, 8.6675f, 3.28771f), camTarget(-0.625726f, -0.709427f, 0.324315f), camUp(-0.541865f, 0.781103f, 0.310259f);

Camera cam(camPos, camTarget, camUp, INIT_WIDTH, INIT_HEIGHT);
Projection proj(PI / 3, INIT_WIDTH, INIT_HEIGHT, 0.1f, 100.0f);

vec3 lightPosition(BOXSIZE / 2.0, BOXSIZE - 0.5, BOXSIZE / 2.0);
vec3 chairPosition(5.0f, 0.0f, 9.0f);
vec3 standPosition(2.5f, 0.0f, 4.0f);
vec3 starPosition(2.53f, 2.7f, 4.08f);

vec3 chairRotation(0.0f, -5 * PI / 4, 0.0f);

GLuint VBO_DOWN, VBO_UP, VBO_LEFT, VBO_RIGHT, VBO_FRONT, VBO_BACK;
GLuint gWorld;
GLuint gCamera;
GLuint gProjection;
GLuint gEyeWorldPos, gLightWorldPos;
GLuint shaderProgramFace;
GLuint shaderProgramFaceDown;
GLuint shaderProgramStand;
GLuint shaderProgramChair;
GLuint shaderProgramLamp;
GLuint shaderProgramStar;
GLuint gSamplerChair;

GLuint shaderProgramShadow;

Mesh *pStand;
Mesh *pChair;
Mesh *pLamp;
Mesh *pStar;

ShadowMapFBO *pShadowMap1, *pShadowMap2, *pShadowMap3, *pShadowMap4, *pShadowMap0;
GLuint gTextureLocation1, gTextureLocation2, gTextureLocation3, gTextureLocation4, gTextureLocation0;
Camera lightCamera(lightPosition, vec3(BOXSIZE / 2.0, 0.0f, BOXSIZE / 2.0) - lightPosition, camUp, INIT_WIDTH, INIT_HEIGHT);
GLuint gLight;


mat4 projMatr;
mat4 lightMatr;
mat4 worldMatrChair;


Texture *pTextureFaceNormal;
GLuint gSamplerFaceNormal;
Texture *pTextureFace;
GLuint gSamplerFace;

Texture *pTextureFaceNormalDown;
GLuint gSamplerFaceNormalDown;
Texture *pTextureFaceDown;
GLuint gSamplerFaceDown;

GLuint gFogB, gFogC;
float FogB = 0.011, FogC = 2.5;

int prevWidth = 0, prevHeight = 0;

void resize(int width, int height)
{
	delete pShadowMap0;
	delete pShadowMap1;
	delete pShadowMap2;
	delete pShadowMap3;
	delete pShadowMap4;

	pShadowMap0 = new ShadowMapFBO();
	pShadowMap0->init(glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT));

	pShadowMap1 = new ShadowMapFBO();
	pShadowMap1->init(glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT));

	pShadowMap2 = new ShadowMapFBO();
	pShadowMap2->init(glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT));

	pShadowMap3 = new ShadowMapFBO();
	pShadowMap3->init(glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT));

	pShadowMap4 = new ShadowMapFBO();
	pShadowMap4->init(glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT));
}

void renderSceneShadow(vec3 move, ShadowMapFBO *pShadowMap)
{
	lightCamera = Camera(lightPosition + move, vec3(BOXSIZE / 2.0, 0.0f, BOXSIZE / 2.0) - lightPosition - move, camUp, glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT));
	mat4 camMatr = cam.getTrans();
	Transform transChair;
	lightMatr = lightCamera.getTrans();
	pShadowMap->bindForWriting();
	glClear(GL_DEPTH_BUFFER_BIT);
	glUseProgram(shaderProgramShadow);
	gWorld = glGetUniformLocation(shaderProgramShadow, "gWorld");
	gCamera = glGetUniformLocation(shaderProgramShadow, "gCamera");
	gProjection = glGetUniformLocation(shaderProgramShadow, "gProjection");
	gSamplerChair = glGetUniformLocation(shaderProgramShadow, "gSampler");
	transChair.scale(0.03f, 0.03f, 0.03f);
	transChair.position(chairPosition.x, chairPosition.y, chairPosition.z);
	transChair.rotate(chairRotation.x, chairRotation.y, chairRotation.z);
	worldMatrChair = transChair.getTrans();
	glUniformMatrix4fv(gWorld, 1, GL_TRUE, &worldMatrChair[0][0]);
	glUniformMatrix4fv(gProjection, 1, GL_TRUE, &projMatr[0][0]);
	glUniformMatrix4fv(gCamera, 1, GL_TRUE, &lightMatr[0][0]);
	pChair->render(cam.pos, worldMatrChair);


	gWorld = glGetUniformLocation(shaderProgramShadow, "gWorld");
	gCamera = glGetUniformLocation(shaderProgramShadow, "gCamera");
	gProjection = glGetUniformLocation(shaderProgramShadow, "gProjection");
	Transform transStand;
	transStand.scale(0.003f, 0.003f, 0.003f);
	transStand.position(standPosition.x, standPosition.y, standPosition.z);
	transStand.rotate(-PI / 2, 0.0f, 0.0f);
	mat4 worldMatrStand = transStand.getTrans();
	glUniformMatrix4fv(gWorld, 1, GL_TRUE, &worldMatrStand[0][0]);
	glUniformMatrix4fv(gCamera, 1, GL_TRUE, &lightMatr[0][0]);
	glUniformMatrix4fv(gProjection, 1, GL_TRUE, &projMatr[0][0]);

	gEyeWorldPos = glGetUniformLocation(shaderProgramStand, "gEyeWorldPos");
	gLightWorldPos = glGetUniformLocation(shaderProgramStand, "gLightWorldPos");

	glUniform3f(gEyeWorldPos, cam.pos.x, cam.pos.y, cam.pos.z);
	glUniform3f(gLightWorldPos, lightPosition.x, lightPosition.y, lightPosition.z);

	pStand->render(cam.pos, worldMatrStand);

	gWorld = glGetUniformLocation(shaderProgramShadow, "gWorld");
	gCamera = glGetUniformLocation(shaderProgramShadow, "gCamera");
	gProjection = glGetUniformLocation(shaderProgramShadow, "gProjection");
	Transform transStar;
	transStar.scale(0.02f, 0.02f, 0.02f);
	transStar.position(starPosition.x, starPosition.y, starPosition.z);
	transStar.rotate(0.0f, PI, 0.0f);
	mat4 worldMatrStar = transStar.getTrans();
	glUniformMatrix4fv(gWorld, 1, GL_TRUE, &worldMatrStar[0][0]);
	glUniformMatrix4fv(gCamera, 1, GL_TRUE, &lightMatr[0][0]);
	glUniformMatrix4fv(gProjection, 1, GL_TRUE, &projMatr[0][0]);

	gEyeWorldPos = glGetUniformLocation(shaderProgramStar, "gEyeWorldPos");
	gLightWorldPos = glGetUniformLocation(shaderProgramStar, "gLightWorldPos");

	glUniform3f(gEyeWorldPos, cam.pos.x, cam.pos.y, cam.pos.z);
	glUniform3f(gLightWorldPos, lightPosition.x, lightPosition.y, lightPosition.z);

	pStar->render(cam.pos, worldMatrStar);
}

void display()
{
	if (prevWidth != glutGet(GLUT_WINDOW_WIDTH) || prevHeight != glutGet(GLUT_WINDOW_HEIGHT)) {
		prevWidth = glutGet(GLUT_WINDOW_WIDTH);
		prevHeight = glutGet(GLUT_WINDOW_HEIGHT);
		resize(prevWidth, prevHeight);
	}
	
	cam.setSize(glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT));
	proj = Projection(PI / 3, glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT), 0.1f, 100.0f);
	
	glDepthMask(GL_TRUE);

	mat4 camMatr = cam.getTrans();
	projMatr = proj.getTrans();
	Transform transChair;
	
	mat4 worldMatrChair;
	
	renderSceneShadow(vec3(0.0f, 0.0f, 0.0f), pShadowMap0);
	renderSceneShadow(vec3(DELTA, 0.0f, 0.0f), pShadowMap1);
	renderSceneShadow(vec3(-DELTA, 0.0f, 0.0f), pShadowMap2);
	renderSceneShadow(vec3(0.0f, 0.0f, DELTA), pShadowMap3);
	renderSceneShadow(vec3(0.0f, 0.0f, -DELTA), pShadowMap4);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);


	static double prevTime = 0;
	if ((double)clock() / CLOCKS_PER_SEC - prevTime > TIME_STEP) {
		cam.rotateCamera();
		prevTime = (double)clock() / CLOCKS_PER_SEC;
	}

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	pShadowMap1->bindForReading(GL_TEXTURE1);
	pShadowMap2->bindForReading(GL_TEXTURE2);
	pShadowMap3->bindForReading(GL_TEXTURE3);
	pShadowMap4->bindForReading(GL_TEXTURE4);
	pShadowMap0->bindForReading(GL_TEXTURE30);



	glUseProgram(shaderProgramFace);
	gWorld = glGetUniformLocation(shaderProgramFace, "gWorld");
	gCamera = glGetUniformLocation(shaderProgramFace, "gCamera");
	gProjection = glGetUniformLocation(shaderProgramFace, "gProjection");
	Transform transFace;
	transFace.scale(1.0f, 1.0f, 1.0f);
	transFace.position(0.0f, 0.0f, 2.0f);
	transFace.rotate(0.0f, 0.0f, 0.0f);
	mat4 worldMatrFace = transFace.getTrans();
	glUniformMatrix4fv(gWorld, 1, GL_TRUE, &worldMatrFace[0][0]);
	glUniformMatrix4fv(gCamera, 1, GL_TRUE, &camMatr[0][0]);
	glUniformMatrix4fv(gProjection, 1, GL_TRUE, &projMatr[0][0]);

	pTextureFace->bind(GL_TEXTURE0);
	gSamplerFace = glGetUniformLocation(shaderProgramFace, "gColorMap");
	glUniform1i(gSamplerFace, 0);

	pTextureFaceNormal->bind(GL_TEXTURE1);
	gSamplerFaceNormal = glGetUniformLocation(shaderProgramFace, "gNormalMap");
	glUniform1i(gSamplerFaceNormal, 1);

	gFogB = glGetUniformLocation(shaderProgramFace, "FogB");
	gFogC = glGetUniformLocation(shaderProgramFace, "FogC");
	glUniform1f(gFogB, FogB);
	glUniform1f(gFogC, FogC);

	gEyeWorldPos = glGetUniformLocation(shaderProgramFace, "gEyeWorldPos");
	gLightWorldPos = glGetUniformLocation(shaderProgramFace, "gLightWorldPos");

	glUniform3f(gEyeWorldPos, cam.pos.x, cam.pos.y, cam.pos.z);
	glUniform3f(gLightWorldPos, lightPosition.x, lightPosition.y, lightPosition.z);

	drawBox(VBO_UP, VBO_LEFT, VBO_RIGHT, VBO_FRONT, VBO_BACK);



	glUseProgram(shaderProgramFaceDown);
	gWorld = glGetUniformLocation(shaderProgramFaceDown, "gWorld");
	gCamera = glGetUniformLocation(shaderProgramFaceDown, "gCamera");
	gLight = glGetUniformLocation(shaderProgramFaceDown, "gLight");
	gProjection = glGetUniformLocation(shaderProgramFaceDown, "gProjection");
	gTextureLocation1 = glGetUniformLocation(shaderProgramFaceDown, "gShadowMap1");
	gTextureLocation2 = glGetUniformLocation(shaderProgramFaceDown, "gShadowMap2");
	gTextureLocation3 = glGetUniformLocation(shaderProgramFaceDown, "gShadowMap3");
	gTextureLocation4 = glGetUniformLocation(shaderProgramFaceDown, "gShadowMap4");
	gTextureLocation0 = glGetUniformLocation(shaderProgramFaceDown, "gShadowMap0");
	gLightWorldPos = glGetUniformLocation(shaderProgramFaceDown, "gLightWorldPos");
	glUniform1i(gTextureLocation1, 1);
	glUniform1i(gTextureLocation2, 2);
	glUniform1i(gTextureLocation3, 3);
	glUniform1i(gTextureLocation4, 4);
	glUniform1i(gTextureLocation0, 30);
	transFace.scale(1.0f, 1.0f, 1.0f);
	transFace.position(0.0f, 0.0f, 2.0f);
	transFace.rotate(0.0f, 0.0f, 0.0f);
	worldMatrFace = transFace.getTrans();
	glUniformMatrix4fv(gWorld, 1, GL_TRUE, &worldMatrFace[0][0]);
	glUniformMatrix4fv(gCamera, 1, GL_TRUE, &camMatr[0][0]);
	glUniformMatrix4fv(gProjection, 1, GL_TRUE, &projMatr[0][0]);
	glUniformMatrix4fv(gLight, 1, GL_TRUE, &lightMatr[0][0]);
	glUniform3f(gLightWorldPos, lightPosition.x, lightPosition.y, lightPosition.z);

	pTextureFaceDown->bind(GL_TEXTURE21);
	gSamplerFaceDown = glGetUniformLocation(shaderProgramFaceDown, "gColorMap");
	glUniform1i(gSamplerFaceDown, 21);

	pTextureFaceNormalDown->bind(GL_TEXTURE22);
	gSamplerFaceNormalDown = glGetUniformLocation(shaderProgramFaceDown, "gNormalMap");
	glUniform1i(gSamplerFaceNormalDown, 22);

	gFogB = glGetUniformLocation(shaderProgramFaceDown, "FogB");
	gFogC = glGetUniformLocation(shaderProgramFaceDown, "FogC");
	glUniform1f(gFogB, FogB);
	glUniform1f(gFogC, FogC);

	gEyeWorldPos = glGetUniformLocation(shaderProgramFaceDown, "gEyeWorldPos");
	gLightWorldPos = glGetUniformLocation(shaderProgramFaceDown, "gLightWorldPos");
	glUniform3f(gEyeWorldPos, cam.pos.x, cam.pos.y, cam.pos.z);
	glUniform3f(gLightWorldPos, lightPosition.x, lightPosition.y, lightPosition.z);

	drawBoxDown(VBO_DOWN);




	glUseProgram(shaderProgramStand);
	gWorld = glGetUniformLocation(shaderProgramStand, "gWorld");
	gCamera = glGetUniformLocation(shaderProgramStand, "gCamera");
	gLight = glGetUniformLocation(shaderProgramStand, "gLight");
	gProjection = glGetUniformLocation(shaderProgramStand, "gProjection");
	gTextureLocation1 = glGetUniformLocation(shaderProgramStand, "gShadowMap1");
	gTextureLocation2 = glGetUniformLocation(shaderProgramStand, "gShadowMap2");
	gTextureLocation3 = glGetUniformLocation(shaderProgramStand, "gShadowMap3");
	gTextureLocation4 = glGetUniformLocation(shaderProgramStand, "gShadowMap4");
	gTextureLocation0 = glGetUniformLocation(shaderProgramStand, "gShadowMap0");
	glUniform1i(gTextureLocation1, 1);
	glUniform1i(gTextureLocation2, 2);
	glUniform1i(gTextureLocation3, 3);
	glUniform1i(gTextureLocation4, 4);
	glUniform1i(gTextureLocation0, 30);
	Transform transStand;
	transStand.scale(0.003f, 0.003f, 0.003f);
	transStand.position(standPosition.x, standPosition.y, standPosition.z);
	transStand.rotate(-PI / 2, 0.0f, 0.0f);
	mat4 worldMatrStand = transStand.getTrans();
	glUniformMatrix4fv(gWorld, 1, GL_TRUE, &worldMatrStand[0][0]);
	glUniformMatrix4fv(gCamera, 1, GL_TRUE, &camMatr[0][0]);
	glUniformMatrix4fv(gProjection, 1, GL_TRUE, &projMatr[0][0]);
	glUniformMatrix4fv(gLight, 1, GL_TRUE, &lightMatr[0][0]);

	gEyeWorldPos = glGetUniformLocation(shaderProgramStand, "gEyeWorldPos");
	gLightWorldPos = glGetUniformLocation(shaderProgramStand, "gLightWorldPos");
	glUniform3f(gEyeWorldPos, cam.pos.x, cam.pos.y, cam.pos.z);
	glUniform3f(gLightWorldPos, lightPosition.x, lightPosition.y, lightPosition.z);

	gFogB = glGetUniformLocation(shaderProgramStand, "FogB");
	gFogC = glGetUniformLocation(shaderProgramStand, "FogC");
	glUniform1f(gFogB, FogB);
	glUniform1f(gFogC, FogC);

	pStand->render(cam.pos, worldMatrStand);


	glUseProgram(shaderProgramChair);
	gWorld = glGetUniformLocation(shaderProgramChair, "gWorld");
	gCamera = glGetUniformLocation(shaderProgramChair, "gCamera");
	gLight = glGetUniformLocation(shaderProgramChair, "gLight");
	gProjection = glGetUniformLocation(shaderProgramChair, "gProjection");
	gSamplerChair = glGetUniformLocation(shaderProgramChair, "gSampler");
	gTextureLocation0 = glGetUniformLocation(shaderProgramChair, "gShadowMap0");
	gTextureLocation1 = glGetUniformLocation(shaderProgramChair, "gShadowMap1");
	gTextureLocation2 = glGetUniformLocation(shaderProgramChair, "gShadowMap2");
	gTextureLocation3 = glGetUniformLocation(shaderProgramChair, "gShadowMap3");
	gTextureLocation4 = glGetUniformLocation(shaderProgramChair, "gShadowMap4");
	gEyeWorldPos = glGetUniformLocation(shaderProgramChair, "gEyeWorldPos");
	gLightWorldPos = glGetUniformLocation(shaderProgramChair, "gLightWorldPos");
	glUniform1i(gTextureLocation0, 30);
	glUniform1i(gTextureLocation1, 1);
	glUniform1i(gTextureLocation2, 2);
	glUniform1i(gTextureLocation3, 3);
	glUniform1i(gTextureLocation4, 4);
	transChair.scale(0.03f, 0.03f, 0.03f);
	transChair.position(chairPosition.x, chairPosition.y, chairPosition.z);
	transChair.rotate(chairRotation.x, chairRotation.y, chairRotation.z);
	worldMatrChair = transChair.getTrans();
	glUniformMatrix4fv(gWorld, 1, GL_TRUE, &worldMatrChair[0][0]);
	glUniformMatrix4fv(gCamera, 1, GL_TRUE, &camMatr[0][0]);
	glUniformMatrix4fv(gProjection, 1, GL_TRUE, &projMatr[0][0]);
	glUniformMatrix4fv(gLight, 1, GL_TRUE, &lightMatr[0][0]);
	glUniform1i(gSamplerChair, 0);
	glUniform3f(gEyeWorldPos, cam.pos.x, cam.pos.y, cam.pos.z);
	glUniform3f(gLightWorldPos, lightPosition.x, lightPosition.y, lightPosition.z);

	gFogB = glGetUniformLocation(shaderProgramChair, "FogB");
	gFogC = glGetUniformLocation(shaderProgramChair, "FogC");
	glUniform1f(gFogB, FogB);
	glUniform1f(gFogC, FogC);

	pChair->render(cam.pos, worldMatrChair);


	glUseProgram(shaderProgramLamp);
	gWorld = glGetUniformLocation(shaderProgramLamp, "gWorld");
	gCamera = glGetUniformLocation(shaderProgramLamp, "gCamera");
	gProjection = glGetUniformLocation(shaderProgramLamp, "gProjection");

	Transform transLamp;
	transLamp.scale(0.05f, 0.05f, 0.05f);
	transLamp.position(lightPosition.x, lightPosition.y + 0.45, lightPosition.z);
	transLamp.rotate(-PI / 2, 0.0f, 0.0f);
	mat4 worldMatrLamp = transLamp.getTrans();
	glUniformMatrix4fv(gWorld, 1, GL_TRUE, &worldMatrLamp[0][0]);
	glUniformMatrix4fv(gCamera, 1, GL_TRUE, &camMatr[0][0]);
	glUniformMatrix4fv(gProjection, 1, GL_TRUE, &projMatr[0][0]);

	gEyeWorldPos = glGetUniformLocation(shaderProgramLamp, "gEyeWorldPos");
	gLightWorldPos = glGetUniformLocation(shaderProgramLamp, "gLightWorldPos");

	glUniform3f(gEyeWorldPos, cam.pos.x, cam.pos.y, cam.pos.z);
	glUniform3f(gLightWorldPos, lightPosition.x, lightPosition.y, lightPosition.z);

	gFogB = glGetUniformLocation(shaderProgramLamp, "FogB");
	gFogC = glGetUniformLocation(shaderProgramLamp, "FogC");
	glUniform1f(gFogB, FogB);
	glUniform1f(gFogC, FogC);

	pLamp->render(cam.pos, worldMatrLamp);

	glDepthMask(GL_FALSE);

	glUseProgram(shaderProgramStar);
	gWorld = glGetUniformLocation(shaderProgramStar, "gWorld");
	gCamera = glGetUniformLocation(shaderProgramStar, "gCamera");
	gLight = glGetUniformLocation(shaderProgramStar, "gLight");
	gProjection = glGetUniformLocation(shaderProgramStar, "gProjection");
	gTextureLocation0 = glGetUniformLocation(shaderProgramStar, "gShadowMap0");
	gTextureLocation1 = glGetUniformLocation(shaderProgramStar, "gShadowMap1");
	gTextureLocation2 = glGetUniformLocation(shaderProgramStar, "gShadowMap2");
	gTextureLocation3 = glGetUniformLocation(shaderProgramStar, "gShadowMap3");
	gTextureLocation4 = glGetUniformLocation(shaderProgramStar, "gShadowMap4");
	glUniform1i(gTextureLocation0, 30);
	glUniform1i(gTextureLocation1, 1);
	glUniform1i(gTextureLocation2, 2);
	glUniform1i(gTextureLocation3, 3);
	glUniform1i(gTextureLocation4, 4);
	Transform transStar;
	transStar.scale(0.02f, 0.02f, 0.02f);
	transStar.position(starPosition.x, starPosition.y, starPosition.z);
	transStar.rotate(PI / 2, PI, 0.0f);
	mat4 worldMatrStar = transStar.getTrans();
	glUniformMatrix4fv(gWorld, 1, GL_TRUE, &worldMatrStar[0][0]);
	glUniformMatrix4fv(gCamera, 1, GL_TRUE, &camMatr[0][0]);
	glUniformMatrix4fv(gProjection, 1, GL_TRUE, &projMatr[0][0]);

	gEyeWorldPos = glGetUniformLocation(shaderProgramStar, "gEyeWorldPos");
	gLightWorldPos = glGetUniformLocation(shaderProgramStar, "gLightWorldPos");
	glUniformMatrix4fv(gLight, 1, GL_TRUE, &lightMatr[0][0]);

	glUniform3f(gEyeWorldPos, cam.pos.x, cam.pos.y, cam.pos.z);
	glUniform3f(gLightWorldPos, lightPosition.x, lightPosition.y, lightPosition.z);

	gFogB = glGetUniformLocation(shaderProgramStar, "FogB");
	gFogC = glGetUniformLocation(shaderProgramStar, "FogC");
	glUniform1f(gFogB, FogB);
	glUniform1f(gFogC, FogC);

	pStar->render(cam.pos, worldMatrStar);

	glutSwapBuffers();
}

void specialKeyboard(int key, int x, int y)
{
	cam.move(key);
}

void keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 'q':
		exit(0);
		break;
	}
}

void mouse(int x, int y)
{
	cam.rotate(x, y);
}


int main(int argc, char *argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(INIT_WIDTH, INIT_HEIGHT);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("Project");
	glutWarpPointer(INIT_WIDTH / 2, INIT_HEIGHT / 2);

	glutDisplayFunc(display);
	glutIdleFunc(display);
	glutSpecialFunc(specialKeyboard);
	glutKeyboardFunc(keyboard);
	glutPassiveMotionFunc(mouse);

	glewInit();

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	constructBox(VBO_DOWN, VBO_UP, VBO_LEFT, VBO_RIGHT, VBO_FRONT, VBO_BACK);

	pStand = new Mesh();
	pStand->loadMesh("models/stand.3ds", -1, 0);

	pChair = new Mesh();
	pChair->loadMesh("models/Wooden Chair.obj", 1, 0);

	pLamp = new Mesh();
	pLamp->loadMesh("models/lamp.3ds", -1, 0);
	
	pStar = new Mesh();
	pStar->loadMesh("models/crystal.obj", -1, 1);

	pShadowMap0 = new ShadowMapFBO();
	pShadowMap0->init(glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT));

	pShadowMap1 = new ShadowMapFBO();
	pShadowMap1->init(glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT));

	pShadowMap2 = new ShadowMapFBO();
	pShadowMap2->init(glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT));

	pShadowMap3 = new ShadowMapFBO();
	pShadowMap3->init(glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT));

	pShadowMap4 = new ShadowMapFBO();
	pShadowMap4->init(glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT));

	shaderProgramFace = compileShaders("face.vert", "face.frag");
	shaderProgramFaceDown = compileShaders("facedown.vert", "facedown.frag");
	shaderProgramStand = compileShaders("stand.vert", "stand.frag");
	shaderProgramChair = compileShaders("chair.vert", "chair.frag");
	shaderProgramLamp = compileShaders("lamp.vert", "lamp.frag");
	shaderProgramStar = compileShaders("star.vert", "star.frag");
	shaderProgramShadow = compileShaders("shadow.vert", "shadow.frag");

	pTextureFaceNormal = new Texture(GL_TEXTURE_2D, "textures/154_norm.jpg");
	if (!pTextureFaceNormal->load()) {
		std::cout << "Error loading texture" << std::endl;
	}

	pTextureFace = new Texture(GL_TEXTURE_2D, "textures/154.jpg");
	if (!pTextureFace->load()) {
		std::cout << "Error loading texture" << std::endl;
	}


	pTextureFaceNormalDown = new Texture(GL_TEXTURE_2D, "textures/183_norm.jpg");
	if (!pTextureFaceNormalDown->load()) {
		std::cout << "Error loading texture" << std::endl;
	}

	pTextureFaceDown = new Texture(GL_TEXTURE_2D, "textures/183.jpg");
	if (!pTextureFaceDown->load()) {
		std::cout << "Error loading texture" << std::endl;
	}

	glutMainLoop();

	delete pStand;
	delete pChair;
	delete pLamp;
	delete pStar;
	delete pShadowMap0;
	delete pShadowMap1;
	delete pShadowMap2;
	delete pShadowMap3;
	delete pShadowMap4;

	return 0;
}