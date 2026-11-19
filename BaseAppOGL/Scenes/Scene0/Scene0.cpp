#include "Scene0.h"


Scene0::Scene0()
{
	// Cria a câmera
	pCamera = NULL;
	pCamera = new CCamera(glm::vec3(0.0f, 3.0f, 10.0f));
	pCamera->MovementSpeed = 100.0f;

	// Cria o Timer
	pTimer = NULL;
	pTimer = new CTimer();
	pTimer->Init();

	// Cria gerenciador de impressão de texto na tela
	pText = NULL;
	pText = new CText();
	pText->InitTextManager();

	// Mostra a GUI
	bShowGUI = false;

	// Background color (noite)
	backgroundColor = glm::vec4(0.03f, 0.04f, 0.055f, 1.0f);
	
	// iluminacao
	ambientColor = glm::vec3(0.25f, 0.30f, 0.45f);
	lightPos = glm::vec3(0.0f, 3.0f, 0.0f);
	lightColor = glm::vec3(1.0f, 0.6f, 0.3f) * 4.0f;

	//neblina
	fogDensity = 0.013;


	// Cria os shaders
	pShader = NULL;
	pShader = new CShader();
	pShader->LoadShader("Grid", "Scenes/Common/grid.vert", "Scenes/Common/grid.frag");
	pShader->LoadShader("Axis", "Scenes/Common/axis.vert", "Scenes/Common/axis.frag");
	pShader->LoadShader("Text2D", "Scenes/Common/Text2D.vert", "Scenes/Common/Text2D.frag");

	//shader chao
	pShader->LoadShader("Assets", "Scenes/Common/Assets.vert", "Scenes/Common/Assets.frag");
	//shader skybox
	pShader->LoadShader("Skybox", "Scenes/Common/Skybox.vert", "Scenes/Common/Skybox.frag");
	// shader agua
	pShader->LoadShader("Water", "Scenes/Common/Water.vert", "Scenes/Common/Water.frag");


	// Cria o gerenciador de grid e axis
	pGridAxis = NULL;
	pGridAxis = new CGridAxis(50.0f);

	// Cria o gerenciador de texturas
	pTextures = NULL;
	
	//Textura do chao
	pTextures = new CTextures();
	//folhas
	pTextures->LoadTextureAnisotropic(0,"Scenes/Textures/chaoFolhas.png");
	pTextures->LoadTextureAnisotropic(1, "Scenes/Textures/chaoFolhasNormal.png");
	//cascalho
	pTextures->LoadTextureAnisotropic(2, "Scenes/Textures/chaoPedras.png");
	pTextures->LoadTextureAnisotropic(3, "Scenes/Textures/trilhaMask.png");
	pTextures->LoadTextureAnisotropic(4, "Scenes/Textures/chaoPedrasNormal.png");
	//agua 
	pTextures->LoadTextureLinear(5, "Scenes/Textures/heightmap.png");

	//Cria o chao plano
	//pFloor = NULL;
	//pFloor = new CPrimitives;
	//pFloor->generatePlane(100.0f, 100.0f, 1, 1);
	//pFloor->setupBuffers();
	
	//Cria o terreno irregular
	pTerrain = NULL;
	pTerrain = new CTerrain("Scenes/Textures/heightmap.png");

	// Cria o skybox
	std::vector<std::string> faces
	{
		"Scenes/SkyboxNoite/right.jpg",
		"Scenes/SkyboxNoite/left.jpg",
		"Scenes/SkyboxNoite/top.jpg",
		"Scenes/SkyboxNoite/bottom.jpg",
		"Scenes/SkyboxNoite/front.jpg",
		"Scenes/SkyboxNoite/back.jpg"
	};
	pSkybox = NULL;
	pSkybox = new CSkybox(faces);

	waterPos = glm::vec3(-13.0f, -0.1f, 9.0f);
	pWater = NULL;
	pWater = new CPrimitives();
	pWater->generatePlane(18.0f, 18.0f, 1, 1);
	pWater->setupBuffers();
}

Scene0::~Scene0()
{

	if (pCamera)
	{
		delete pCamera;
		pCamera = NULL;
	}

	if (pShader)
	{
		delete pShader;
		pShader = NULL;
	}

	if (pGridAxis)
	{
		delete pGridAxis;
		pGridAxis = NULL;
	}

	if (pTextures)
	{
		delete pTextures;
		pTextures = NULL;
	}

	if (pTimer)
	{
		delete pTimer;
		pTimer = NULL;
	}

	if (pText)
	{
		delete pText;
		pText = NULL;
	}
	// deleta do buffer
	//if (pFloor)
	//{
	//	delete pFloor;
	//	pFloor = NULL;
	//}

	if (pTerrain)   // deleta do buffer chao irregular
	{
		delete pTerrain;
		pTerrain = NULL;
	}

	if (pSkybox) // deleta do buffer skybox
	{
		delete pSkybox;
		pSkybox = NULL;
	}

		if (pWater)
	{
		delete pWater;
		pWater = NULL;
	}

}

void Scene0::DrawScene(float deltaTime)
{
	// Update timer (must be called at start of frame)
	pTimer->Update();

	// Clear the color and depth buffers
	glClearColor(backgroundColor.r, backgroundColor.g, backgroundColor.b, backgroundColor.a);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Create transformations 
	// Note: In a real application, you would typically calculate the projection and view matrices 
	// once per frame and pass them to your shaders
	glm::mat4 projection = glm::perspective(glm::radians(pCamera->Zoom), ASPECT_RATIO, Z_NEAR, Z_FAR);
	glm::mat4 view = pCamera->GetViewMatrix();
	glm::mat4 model = glm::mat4(1.0f);


	// chao plano
	//pShader->Use("Assets"); 
	//pShader->SetMat4("projection", projection);
	//pShader->SetMat4("view", view);
	//pShader->SetMat4("model", glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -0.01f, 0.0f)));
	//pShader->SetInt("Texture0", 0);
	//pShader->SetFloat("uTiling", 15.0f); // pro chao nao ficar esticado e feio
	//glActiveTexture(GL_TEXTURE0);
	//pTextures->ApplyTexture(0);
	//pFloor->render();


	//chao irregular
	pShader->Use("Assets"); 
	pShader->SetMat4("projection", projection);
	pShader->SetMat4("view", view);
	pShader->SetMat4("model", glm::mat4(1.0f));
	pShader->SetInt("Texture0", 0);
	pShader->SetFloat("uTiling", 30.0f); // pro chao nao ficar esticado e feio
	//normal mapping
	pShader->SetInt("NormalMap", 1);
	pShader->SetInt("NormalMap1", 4);
	pShader->SetBool("uUseNormalMap", true);
	pShader->SetFloat("uNormalStrength", 1.0f);
	
	//multi texture
	pShader->SetInt("Texture1", 2);
	pShader->SetInt("MaskMap", 3);
	pShader->SetBool("uUseMultiTex", true);

	// iluminacao e neblina 
	pShader->SetVec3("uViewPos", pCamera->Position);
	pShader->SetVec3("uAmbient", ambientColor);
	pShader->SetVec3("uLightPos", lightPos);
	pShader->SetVec3("uLightColor", lightColor);
	pShader->SetVec3("uFogColor", glm::vec3(backgroundColor));
	pShader->SetFloat("uFogDensity", fogDensity);
	pShader->SetFloat("uWaterLevel", waterPos.y);

	glActiveTexture(GL_TEXTURE4);      // unidade 4 -> normal map cascalho
	pTextures->ApplyTexture(4);

	glActiveTexture(GL_TEXTURE3);      // unidade 3 -> mask map trilha
	pTextures->ApplyTexture(3);

	glActiveTexture(GL_TEXTURE2);      // unidade 2 ->cascalho
	pTextures->ApplyTexture(2);

	glActiveTexture(GL_TEXTURE1);      // unidade 1 -> normal map
	pTextures->ApplyTexture(1);
	
	glActiveTexture(GL_TEXTURE0);     // FOLHAS E TALZ
	pTextures->ApplyTexture(0);
	
	pTerrain->DrawTerrain();

	// ================= SKYBOX =================
	pShader->Use("Skybox");
	pShader->SetMat4("projection", projection);
	pShader->SetMat4("view", view);
	pShader->SetInt("skybox", 0);
	pShader->SetVec3("uFogColor", glm::vec3(backgroundColor));
	pShader->SetFloat("uHorizonFog", 0.15f);
	pSkybox->DrawSkybox();


	// ================= LAGO (transparente) =================
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDepthMask(GL_FALSE);
	glDisable(GL_CULL_FACE);

	pShader->Use("Water");
	pShader->SetMat4("projection", projection);
	pShader->SetMat4("view", view);
	pShader->SetMat4("model", glm::translate(glm::mat4(1.0f), waterPos));
	pShader->SetVec3("uViewPos", pCamera->Position);
	pShader->SetVec3("uLightPos", lightPos);
	pShader->SetVec3("uLightColor", lightColor);
	pShader->SetVec3("uFogColor", glm::vec3(backgroundColor));
	pShader->SetFloat("uFogDensity", fogDensity);
	pShader->SetFloat("uTime", (float)(pTimer->GetTime() / 1000.0));
	pShader->SetInt("skybox", 0);        // cubemap na unidade 0
	pShader->SetInt("uHeightMap", 1);    // heightmap na unidade 1

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, pSkybox->GetSkyboxTexture());   // céu = espelho
	glActiveTexture(GL_TEXTURE1);
	pTextures->ApplyTexture(5);                                         // heightmap
	glActiveTexture(GL_TEXTURE0);

	pWater->render();

	glEnable(GL_CULL_FACE);
	glDepthMask(GL_TRUE);
	glDisable(GL_BLEND);
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Draw the 3D grid
	//pShader->Use("Grid");
	//pShader->SetMat4("uProj", projection);
	//pShader->SetMat4("uView", view);
	//pShader->SetFloat("uGridSpacing", 1.0f);
	//pShader->SetFloat("uFadeStart", 18.f);
	//->SetFloat("uFadeEnd", 100.f);
	//pShader->SetVec3("uCamPos", pCamera->Position);
	//pGridAxis->DrawGrid();
	// Draw the 3D axis
	//pShader->Use("Axis");
	//pShader->SetMat4("uProj", projection);
	//pShader->SetMat4("uView", view);
	//pGridAxis->DrawAxis();

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//                               DESENHA OS OBJETOS DA CENA (INÍCIO)
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



















	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//                               DESENHA OS OBJETOS DA CENA (FIM)
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Draw text on the screen (disable depth test and use orthographic projection)
	glDisable(GL_DEPTH_TEST);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	projection = glm::ortho(0.0f, static_cast<float>(SCR_WIDTH), 0.0f, static_cast<float>(SCR_HEIGHT));

	pShader->Use("Text2D");
	pShader->SetMat4("projection", projection);
	pShader->SetVec3("textColor", 1.0f, 1.0f, 1.0f);

	pText->RenderText(std::format("CamPosition\tx: {0:.2f} \t y: {1:.2f} \t z: {2:.2f}", pCamera->Position.x, pCamera->Position.y, pCamera->Position.z), 5.0f, 100.0f, 0.3f);
	pText->RenderText(std::format("CamFoward\tx: {0:.2f} \t y: {1:.2f} \t z: {2:.2f}", pCamera->Front.x, pCamera->Front.y, pCamera->Front.z), 5.0f, 80.0f, 0.3f);
	pText->RenderText(std::format("[ENTER] Show GUI: {0}", bShowGUI ? "Yes" : "No"), 5.0f, 60.0f, 0.3f);
	pText->RenderText(std::string("[CAPSLOCK] Wireframe Mode"), 5.0f, 40.0f, 0.3f);

	pText->RenderText(std::format("FPS: {0:.0f}\tDeltatime: {1:.2f}ms\tTimer: {2:.2f}s", pTimer->GetFPS(), pTimer->GetDeltaTime(), pTimer->GetTime() / 1000), 5.0f, 10.0f, 0.3f);
	glEnable(GL_DEPTH_TEST);
}


void Scene0::ProcessSceneInput(GLFWwindow* window, float deltaTime)
{
	InputManager& input = InputManager::GetInstance();

	// Toggle GUI display and cursor mode when Enter key is pressed
	if (input.IsKeyJustPressed(GLFW_KEY_ENTER))
	{
		bShowGUI = !bShowGUI;
		input.SetCursorMode(bShowGUI ? GLFW_CURSOR_NORMAL : GLFW_CURSOR_DISABLED);

		std::cout << "Cursor Disabled: '" << bShowGUI << "'" << std::endl;
	}

	// Process camera movement and mouse input only when GUI is not displayed
	if (bShowGUI == false)
	{
		// Camera movement
		if (input.IsKeyPressed(GLFW_KEY_W))
			pCamera->ProcessKeyboard(FORWARD, deltaTime);
		if (input.IsKeyPressed(GLFW_KEY_S))
			pCamera->ProcessKeyboard(BACKWARD, deltaTime);
		if (input.IsKeyPressed(GLFW_KEY_A))
			pCamera->ProcessKeyboard(LEFT, deltaTime);
		if (input.IsKeyPressed(GLFW_KEY_D))
			pCamera->ProcessKeyboard(RIGHT, deltaTime);
		if (input.IsKeyPressed(GLFW_KEY_E))
			pCamera->ProcessKeyboard(UP, deltaTime);
		if (input.IsKeyPressed(GLFW_KEY_Q))
			pCamera->ProcessKeyboard(DOWN, deltaTime);

		// Increase/decrease movement speed
		if (input.IsKeyPressed(GLFW_KEY_LEFT_SHIFT))
			pCamera->MovementSpeed = SPEED_MULTIPLIER;
		else
			pCamera->MovementSpeed = SPEED;

		// Process mouse movement
		glm::vec2 mouseDelta = input.GetMouseDelta();
		if (mouseDelta.x != 0.0f || mouseDelta.y != 0.0f)
		{
			pCamera->ProcessMouseMovement(mouseDelta.x, mouseDelta.y);
		}

		// Process scroll
		float scrollOffset = input.GetScrollOffset();
		if (scrollOffset != 0.0f)
		{
			pCamera->ProcessMouseScroll(scrollOffset);
		}
	}
	else
	{
		ShowGUI(); // Call the ShowGUI function to display the GUI when bShowGUI is true
	}
}

void Scene0::ShowGUI()
{
	ImGuiColorEditFlags flags = ImGuiColorEditFlags_Float;

	ImGui::Begin("Scene Options:");
	ImGui::ColorPicker4("Background Color", (float*)&backgroundColor, flags);
	ImGui::End();
}
