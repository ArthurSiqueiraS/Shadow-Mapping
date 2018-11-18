#define nCubes 3
#define nOCubes 3

Shader *shader, *simpleDepthShader, *debugDepthQuad, *lightShader;
Model *light, *cyborg, *cube, *openCube, *floorPanel;
glm::mat4 lightMat, cyborgMat, cubeMats[nCubes], openCubeMats[nOCubes], floorMat, model;

unsigned int planeVAO;

void loadLight() {
	light = new Model(FileSystem::getPath("resources/objects/planet/planet.obj"));
}

void loadCyborg() {
	cyborg = new Model(FileSystem::getPath("resources/objects/cyborg/cyborg.obj"));
}

void loadCubes() {
	// From https://www.turbosquid.com/
	cube = new Model(FileSystem::getPath("resources/objects/cube.obj"));
	openCube = new Model(FileSystem::getPath("resources/objects/openCube.obj"));
}
 
void loadFloor() {
	// From https://www.free3d.com
	floorPanel = new Model(FileSystem::getPath("resources/objects/floor/floor.obj"));
	floorMat = glm::rotate(floorMat, glm::radians(90.0f), glm::vec3(-1.0, 0.0, 0.0));
}

void loadPlane() {
	// set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    float planeVertices[] = {
        // positions            // normals         // texcoords
         25.0f, -0.5f,  25.0f,  0.0f, 1.0f, 0.0f,  25.0f,  0.0f,
        -25.0f, -0.5f,  25.0f,  0.0f, 1.0f, 0.0f,   0.0f,  0.0f,
        -25.0f, -0.5f, -25.0f,  0.0f, 1.0f, 0.0f,   0.0f, 25.0f,

         25.0f, -0.5f,  25.0f,  0.0f, 1.0f, 0.0f,  25.0f,  0.0f,
        -25.0f, -0.5f, -25.0f,  0.0f, 1.0f, 0.0f,   0.0f, 25.0f,
         25.0f, -0.5f, -25.0f,  0.0f, 1.0f, 0.0f,  25.0f, 25.0f
    };
    // plane VAO
    unsigned int planeVBO;
    glGenVertexArrays(1, &planeVAO);
    glGenBuffers(1, &planeVBO);
    glBindVertexArray(planeVAO);
    glBindBuffer(GL_ARRAY_BUFFER, planeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(planeVertices), planeVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glBindVertexArray(0);

    glDeleteBuffers(1, &planeVBO);
}

void loadModels() {
	// build and compile shaders
    // -------------------------
    shader = new Shader(FileSystem::getPath("resources/shadow_mapping.vs").c_str(), FileSystem::getPath("resources/shadow_mapping.fs").c_str());
    simpleDepthShader = new Shader(FileSystem::getPath("resources/depth.vs").c_str(), FileSystem::getPath("resources/depth.fs").c_str());
    debugDepthQuad = new Shader(FileSystem::getPath("resources/debug.vs").c_str(), FileSystem::getPath("resources/debug.fs").c_str());
    lightShader = new Shader(FileSystem::getPath("resources/light.vs").c_str(), FileSystem::getPath("resources/light.fs").c_str());

	loadLight();
	loadCyborg();
	loadCubes();
	loadFloor();
	loadPlane();
}

void renderBulb(glm::vec3 lightColor, glm::vec3 lightPos) {
	glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
    glm::mat4 view = camera.GetViewMatrix();		
    lightMat = glm::translate(model, lightPos);
    lightMat = glm::scale(lightMat, glm::vec3(0.1));

	lightShader->use();
	lightShader->setMat4("model", lightMat);
	lightShader->setMat4("projection", projection);
    lightShader->setMat4("view", view);
    lightShader->setVec3("lightColor", lightColor);
	light->Draw(*lightShader);
}

void renderCyborg(const Shader &shader) {
	shader.setMat4("model", cyborgMat);
	cyborg->Draw(shader);
}

void renderCube(const Shader &shader, glm::mat4 model) {
	shader.setMat4("model", model);
	cube->Draw(shader);
}

void renderOpenCube(const Shader &shader, glm::mat4 model) {
	shader.setMat4("model", model);
	openCube->Draw(shader);
}

void renderCubes(const Shader &shader) {
	for(int i = 0; i < nCubes; ++i)
		renderCube(shader, cubeMats[i]);
	for(int i = 0; i < nOCubes; ++i)
		renderCube(shader, openCubeMats[i]);	
}

void renderFloor() {
	// floor
    // glBindVertexArray(planeVAO);
    shader->setMat4("model", floorMat);
    // glDrawArrays(GL_TRIANGLES, 0, 6);
    floorPanel->Draw(*shader);

}