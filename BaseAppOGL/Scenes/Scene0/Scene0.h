#pragma once
#include "Engine/CSceneBase.h"
#include "Engine/CCamera.h"
#include "Engine/CGridAxis.h"
#include "Engine/CShader.h"
#include "Engine/CTextures.h"
#include "Engine/CTimer.h"
#include "Engine/CText.h"
#include "Engine/CPrimitives.h"
#include "Engine/cTerrain.h" // heightmap



class Scene0 : public CSceneBase
{
public:
		Scene0();
		~Scene0();

		virtual void DrawScene(float deltaTime);
        virtual void ProcessSceneInput(GLFWwindow* window, float deltaTime);

		void ShowGUI();
private:

	// Camera
	CCamera* pCamera;

	// Axis
	CGridAxis* pGridAxis;

	// Shader
	CShader* pShader;

	// Textures
	CTextures* pTextures;

	// Timer
	CTimer* pTimer;			

	// Texto
	CText* pText;			

	// Mostra a GUI
	bool bShowGUI;

	// Background color 
	glm::vec4 backgroundColor;

	// ilumionacao
	glm::vec3 ambientColor;
	glm::vec3 lightPos;
	glm::vec3 lightColor;

	//neblina
	float fogDensity;

	//Chao plano
	//CPrimitives* pFloor;

	//molda terreno
	CTerrain* pTerrain;

};

