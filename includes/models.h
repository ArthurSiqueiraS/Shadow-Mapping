Shader *shader, *simpleDepthShader, *debugDepthQuad, *lightShader;
Model *cyborg, *sphere, *rock, *cube, *openCube, *floorPanel;
glm::mat4 lightMat, cyborgMat, sphereMat, rockMat, cubeMat, openCubeMat, floorMat, model;

unsigned int planeVAO;

void loadCyborg() {
    cyborg = new Model(FileSystem::getPath("resources/objects/cyborg/cyborg.obj"));
    cyborgMat = glm::translate(cyborgMat, glm::vec3(-5.0, -0.5, 0.0));
}

void loadSphere() {
	sphere = new Model(FileSystem::getPath("resources/objects/planet/planet.obj"));
    sphereMat = glm::translate(sphereMat, glm::vec3(0.0, 5.0, -3.0));
    sphereMat = glm::scale(sphereMat, glm::vec3(0.4));
}

void loadRock() {
	rock = new Model(FileSystem::getPath("resources/objects/rock/rock.obj"));
    rockMat = glm::translate(rockMat, glm::vec3(0.0, -0.5, 5.0));
    rockMat = glm::scale(rockMat, glm::vec3(0.2));
}

void loadCubes() {
	// From https://www.turbosquid.com/
	cube = new Model(FileSystem::getPath("resources/objects/cube.obj"));
	openCube = new Model(FileSystem::getPath("resources/objects/openCube.obj"));

    openCubeMat = glm::translate(openCubeMat, glm::vec3(5.0, 0.0, 0.0));
    openCubeMat = glm::rotate(openCubeMat, glm::radians(90.0f), glm::vec3(0.0, 1.0, 0.0));
    openCubeMat = glm::rotate(openCubeMat, glm::radians(45.0f), glm::vec3(0.0, 0.0, 1.0));
}
 
void loadFloor() {
	// From https://www.free3d.com
	floorPanel = new Model(FileSystem::getPath("resources/objects/floor/floor.obj"));
	floorMat = glm::scale(floorMat, glm::vec3(0.2, 0.2, 0.5));
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

	loadCyborg();
    loadSphere();
    loadRock();
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
	sphere->Draw(*lightShader);
}

void render(const Shader &shader, Model obj, glm::mat4 model, float modelBias) {
    shader.setMat4("model", model);
    shader.setFloat("modelBias", modelBias);
    obj.Draw(shader);
}

void renderPlane(const Shader &shader) {
	glBindVertexArray(planeVAO);
    shader.setMat4("model", model);
    glDrawArrays(GL_TRIANGLES, 0, 6);
}

// renders the 3D scene
// --------------------
void renderScene(const Shader &shader, glm::vec3 lightColor, glm::vec3 lightPos)
{   
	shader.setFloat("modelBias", 0.0);
	// render(shader, *floorPanel, floorMat, 0.0);
    renderPlane(shader);
    render(shader, *cube, cubeMat, 0.5);
    render(shader, *openCube, openCubeMat, 0.5);
    render(shader, *sphere, sphereMat, 0.0);
    render(shader, *rock, rockMat, 0.5);
    render(shader, *cyborg, cyborgMat, 0.5);
    // Must be rendered last
    renderBulb(lightColor, lightPos);
}