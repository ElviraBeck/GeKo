#include "Emitter.h"

//TODO: COMMENTS & VAR RENAMING

Emitter::Emitter(const int OUTPUT, glm::vec3 position, double emitterLifetime, double emitFrequency,
	int particlesPerEmit, double particleLifeTime, bool particleMortal)
{
	m_output = static_cast<FLOW> (OUTPUT); //set if won't generate (-1), constant (0) or just once (1)

	//set Emitter properties
	setPosition(position);
	setEmitterLifetime(emitterLifetime);
	setEmitterMortality(emitterLifetime);

	//set properties for the emitting
	setEmitFrequency(emitFrequency);
	setParticlesPerEmit(particlesPerEmit);

	//set properties for the particles
	setParticleLifetime(particleLifeTime);
	setParticleMortality(particleMortal);

	//our default shader (Point Sprites)
	VertexShader vsParticle(loadShaderSource(SHADERS_PATH + std::string("/ParticleSystem/ParticleSystemPointSprites.vert")));
	FragmentShader fsParticle(loadShaderSource(SHADERS_PATH + std::string("/ParticleSystem/ParticleSystemPointSprites.frag")));
	emitterShader = new ShaderProgram(vsParticle, fsParticle);
	
	//our default compute shader
	ComputeShader csParticle(loadShaderSource(SHADERS_PATH + std::string("/ParticleSystem/ParticleSystem.comp")));
	compute = new ShaderProgram(csParticle);

	updateSize(); //?
	startTime();
}
//TODO: Memory?
Emitter::~Emitter()
{
	m_textureList.clear();
}

void Emitter::startTime(){
	updateTime = glfwGetTime();
	deltaTime = updateTime;
	generateTime = deltaTime;
}

//TODO: MADELEINE ANGLE TO UBO
void Emitter::loadBuffer(){
	//?
	glGenBuffers(1, &position_ssbo);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, position_ssbo);
	glBufferData(GL_SHADER_STORAGE_BUFFER, numMaxParticle * sizeof(glm::vec4), NULL, GL_DYNAMIC_DRAW);
	GLint bufMask = GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT;
	positions = (glm::vec4*) glMapBufferRange (GL_SHADER_STORAGE_BUFFER, 0, numMaxParticle * sizeof(glm::vec4), bufMask);
	for (int i = 0; i < numMaxParticle; i++)
	{
		positions[i] = glm::vec4(getPosition(), -1.0f);
	}
	glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

	//?
	glGenBuffers(1, &velocity_ssbo);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, velocity_ssbo);
	glBufferData(GL_SHADER_STORAGE_BUFFER, numMaxParticle * sizeof(glm::vec4), NULL, GL_DYNAMIC_DRAW);
	glm::vec4* velocity = (glm::vec4*) glMapBufferRange(GL_SHADER_STORAGE_BUFFER, 0, numMaxParticle * sizeof(glm::vec4), bufMask);
	for (int i = 0; i < numMaxParticle; i++)
	{
		velocity[i].x = 0.0f;
		velocity[i].y = 0.0f;
		velocity[i].z = 0.0f;
		velocity[i].w = 0.0f;
	}
	glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

	//TODO: Maybe something else than SSBO. Just 2 of 4
	glGenBuffers(1, &angle_ssbo);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, angle_ssbo);
	glBufferData(GL_SHADER_STORAGE_BUFFER, numMaxParticle * sizeof(glm::vec4), NULL, GL_DYNAMIC_DRAW);
	glm::vec4* angle = (glm::vec4*) glMapBufferRange(GL_SHADER_STORAGE_BUFFER, 0, numMaxParticle * sizeof(glm::vec4), bufMask);
	for (int i = 0; i < numMaxParticle; i++)
	{
		angle[i].x = 0.0f;
		angle[i].y = 0.0f;
		angle[i].z = 0.0f;
		angle[i].w = 0.0f;
	}
	glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
}

void Emitter::update(){

	deltaTime = glfwGetTime() - updateTime; //time remain since last update
	updateTime = glfwGetTime();

	emitLifetime -= deltaTime;
	if (emitLifetime < 0 && emitterMortal){
		m_output = UNUSED;
	}

	//Bind CS and all SSBO's
	compute->bind();
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, position_ssbo);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, velocity_ssbo);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, angle_ssbo);

	//Uniform Vars
	compute->sendFloat("deltaTime", (float)deltaTime);
	compute->sendVec3("emitterPos", getPosition()); //can be saved position, or parameter
	compute->sendFloat("fullLifetime", particleLifetime);
	compute->sendInt("particleMortal", m_particleMortal);

	compute->sendVec4("gravity", m_gravity);
	compute->sendFloat("gravityRange", m_gravityRange);
	compute->sendInt("gravityFunc", m_gravityFunction);
	
	compute->sendInt("useTrajectory", m_useTrajectory);
	compute->sendInt("useDirectionGravity", m_useDirectionGravity);
	compute->sendInt("usePointGravity", m_usePointGravity);
	compute->sendInt("useChaoticSwarmMotion", m_useChaoticSwarmMotion);
	
	compute->sendInt("useMovementVertical", m_movementVertical);
	compute->sendInt("useMovementHorizontalX", m_movementHorizontalX);
	compute->sendInt("useMovementHorizontalZ", m_movementHorizontalZ);

	//Unbind CD and all SSBO's
	glDispatchCompute(computeGroupCount, 1, 1); //?
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, 0);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, 0);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, 0);
	glMemoryBarrier(GL_VERTEX_ATTRIB_ARRAY_BARRIER_BIT);
	emitterShader->unbind();
}
//TODO: MADELEINE ANGLE TO UBO
void Emitter::pushParticle(int numberNewParticle){
	auto emitPosition = getPosition();

	glBindBuffer(GL_SHADER_STORAGE_BUFFER, position_ssbo);
	GLint bufMask = GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT;

	glm::vec4* positions = (glm::vec4*) glMapBufferRange (GL_SHADER_STORAGE_BUFFER, 0, numMaxParticle * sizeof(glm::vec4), bufMask);	
	if (getAreaEmittingXZ()){ //emits in a xz area, like rain
		auto accuracy = getAreaAccuracy(); //how near it will be generated
		auto areaSize = getAreaSize(); //how big the area is
		float randomNumber;
		glm::vec3 pos;

		for (int i = 0; i < numberNewParticle; i++)
		{
			int index = (indexBuffer + i) % numMaxParticle;
			
			randomNumber = (rand() % (2 * accuracy + 1) - accuracy) / (float) accuracy; //-1 .. 1 with a certain comma accuracy
			pos.x = emitPosition.x + areaSize * randomNumber;
			randomNumber = (rand() % (2 * accuracy + 1) - accuracy) / (float) accuracy; //-1 .. 1
			pos.z = emitPosition.z + areaSize * randomNumber;
			pos.y = emitPosition.y;

			positions[index] = glm::vec4(pos, particleLifetime);
		}
	}
	else if (getAreaEmittingXY()){ //will be emitted in xy area
		auto accuracy = getAreaAccuracy();
		auto areaSize = getAreaSize();
		float randomNumber;
		glm::vec3 pos;

		for (int i = 0; i < numberNewParticle; i++)
		{
			int index = (indexBuffer + i) % numMaxParticle;

			randomNumber = (rand() % (2 * accuracy + 1) - accuracy) / (float)accuracy; //-1 .. 1 with a certain comma accuracy
			pos.x = emitPosition.x + areaSize * randomNumber;
			randomNumber = (rand() % (2 * accuracy + 1) - accuracy) / (float)accuracy; //-1 .. 1
			pos.y = emitPosition.y + areaSize * randomNumber;
			pos.z = emitPosition.z;

			positions[index] = glm::vec4(pos, particleLifetime);
		}
	}
	else{ //will be emitted like a jet
		for (int i = 0; i < numberNewParticle; i++)
		{
			int index = (indexBuffer + i) % numMaxParticle;
			positions[index] = glm::vec4(emitPosition, particleLifetime);
		}
	}

	glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
	
	auto speed = getSpeed();
	
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, velocity_ssbo);
	
	glm::vec4* velocity = (glm::vec4*) glMapBufferRange (GL_SHADER_STORAGE_BUFFER, 0, numMaxParticle * sizeof(glm::vec4), bufMask); //direction of movement
	for (int i = 0; i < numberNewParticle; i++)
	{
		glm::vec3 vel = this->m_pfunc(); //gets a random vector from the desired vector space
		if (glm::length(vel) != 0.0)
			vel = glm::normalize(vel);
		int index = (indexBuffer + i) % numMaxParticle;
		velocity[index] = glm::vec4(vel, speed);
	}

	glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

	//TODO just 2 of 4
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, angle_ssbo);
	
	glm::vec4* angle = (glm::vec4*) glMapBufferRange(GL_SHADER_STORAGE_BUFFER, 0, numMaxParticle * sizeof(glm::vec4), bufMask); //angle of movement as option to velocity
	for (int i = 0; i < numberNewParticle; i++)
	{
		int index = (indexBuffer + i) % numMaxParticle;
		int phi = (rand() % 360);
		int theta = (rand() % 90);
		angle[index] = glm::vec4(phi, theta, 0.0, 0.0);
	}
	glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

	indexBuffer = (indexBuffer + numberNewParticle) % numMaxParticle;
}
void Emitter::generateParticle()
{
	switch (m_output)
	{
	case CONSTANT: //we generate constant new particle
		if (glfwGetTime() - generateTime >= emitFrequency){
			deltaTime = glfwGetTime() - generateTime;
			while (deltaTime >= emitFrequency) {
				pushParticle(particlesPerEmit);
				deltaTime -= emitFrequency;
			}
			generateTime = glfwGetTime();
		}
		break;
	case ONCE: //we generate only one time particle
		if (glfwGetTime() - generateTime >= emitFrequency){
			pushParticle(particlesPerEmit);
			m_output = UNUSED;
		}
		break;
	case UNUSED: // we dont generate new particle
		break;
	}
}

void Emitter::update(glm::vec3 playerPosition){

	deltaTime = glfwGetTime() - updateTime; //time remain since last update
	updateTime = glfwGetTime();

	emitLifetime -= deltaTime;
	if (emitLifetime < 0 && emitterMortal){
		m_output = UNUSED;
	}

	//Bind CS and all SSBO's
	compute->bind();
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, position_ssbo);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, velocity_ssbo);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, angle_ssbo);

	//Uniform Vars
	compute->sendFloat("deltaTime", (float)deltaTime);
	compute->sendVec3("emitterPos", getPosition()); //can be saved position, or parameter
	compute->sendFloat("fullLifetime", particleLifetime);
	compute->sendInt("particleMortal", m_particleMortal);

	compute->sendVec4("gravity", m_gravity);
	compute->sendFloat("gravityRange", m_gravityRange);
	compute->sendInt("gravityFunc", m_gravityFunction);

	compute->sendInt("useTrajectory", m_useTrajectory);
	compute->sendInt("useDirectionGravity", m_useDirectionGravity);
	compute->sendInt("usePointGravity", m_usePointGravity);
	compute->sendInt("useChaoticSwarmMotion", m_useChaoticSwarmMotion);

	compute->sendInt("useMovementVertical", m_movementVertical);
	compute->sendInt("useMovementHorizontalX", m_movementHorizontalX);
	compute->sendInt("useMovementHorizontalZ", m_movementHorizontalZ);

	//Unbind CD and all SSBO's
	glDispatchCompute(computeGroupCount, 1, 1); //?
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, 0);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, 0);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, 0);
	glMemoryBarrier(GL_VERTEX_ATTRIB_ARRAY_BARRIER_BIT);
	emitterShader->unbind();
}
//TODO: MADELEINE ANGLE TO UBO
void Emitter::pushParticle(int numberNewParticle, glm::vec3 playerPosition){
	auto emitPosition = getPosition() + playerPosition;

	glBindBuffer(GL_SHADER_STORAGE_BUFFER, position_ssbo);
	GLint bufMask = GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT;

	glm::vec4* positions = (glm::vec4*) glMapBufferRange(GL_SHADER_STORAGE_BUFFER, 0, numMaxParticle * sizeof(glm::vec4), bufMask);
	if (getAreaEmittingXZ()){ //emits in a xz area, like rain
		auto accuracy = getAreaAccuracy(); //how near it will be generated
		auto areaSize = getAreaSize(); //how big the area is
		float randomNumber;
		glm::vec3 pos;

		for (int i = 0; i < numberNewParticle; i++)
		{
			int index = (indexBuffer + i) % numMaxParticle;

			randomNumber = (rand() % (2 * accuracy + 1) - accuracy) / (float)accuracy; //-1 .. 1 with a certain comma accuracy
			pos.x = emitPosition.x + areaSize * randomNumber;
			randomNumber = (rand() % (2 * accuracy + 1) - accuracy) / (float)accuracy; //-1 .. 1
			pos.z = emitPosition.z + areaSize * randomNumber;
			pos.y = emitPosition.y;

			positions[index] = glm::vec4(pos, particleLifetime);
		}
	}
	else if (getAreaEmittingXY()){ //will be emitted in xy area
		auto accuracy = getAreaAccuracy();
		auto areaSize = getAreaSize();
		float randomNumber;
		glm::vec3 pos;

		for (int i = 0; i < numberNewParticle; i++)
		{
			int index = (indexBuffer + i) % numMaxParticle;

			randomNumber = (rand() % (2 * accuracy + 1) - accuracy) / (float)accuracy; //-1 .. 1 with a certain comma accuracy
			pos.x = emitPosition.x + areaSize * randomNumber;
			randomNumber = (rand() % (2 * accuracy + 1) - accuracy) / (float)accuracy; //-1 .. 1
			pos.y = emitPosition.y + areaSize * randomNumber;
			pos.z = emitPosition.z;

			positions[index] = glm::vec4(pos, particleLifetime);
		}
	}
	else{ //will be emitted like a jet
		for (int i = 0; i < numberNewParticle; i++)
		{
			int index = (indexBuffer + i) % numMaxParticle;
			positions[index] = glm::vec4(emitPosition, particleLifetime);
		}
	}

	glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

	auto speed = getSpeed();

	glBindBuffer(GL_SHADER_STORAGE_BUFFER, velocity_ssbo);

	glm::vec4* velocity = (glm::vec4*) glMapBufferRange(GL_SHADER_STORAGE_BUFFER, 0, numMaxParticle * sizeof(glm::vec4), bufMask); //direction of movement
	for (int i = 0; i < numberNewParticle; i++)
	{
		glm::vec3 vel = this->m_pfunc(); //gets a random vector from the desired vector space
		if (glm::length(vel) != 0.0)
			vel = glm::normalize(vel);
		int index = (indexBuffer + i) % numMaxParticle;
		velocity[index] = glm::vec4(vel, speed);
	}

	glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

	//TODO just 2 of 4
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, angle_ssbo);

	glm::vec4* angle = (glm::vec4*) glMapBufferRange(GL_SHADER_STORAGE_BUFFER, 0, numMaxParticle * sizeof(glm::vec4), bufMask); //angle of movement as option to velocity
	for (int i = 0; i < numberNewParticle; i++)
	{
		int index = (indexBuffer + i) % numMaxParticle;
		int phi = (rand() % 360);
		int theta = (rand() % 90);
		angle[index] = glm::vec4(phi, theta, 0.0, 0.0);
	}
	glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

	indexBuffer = (indexBuffer + numberNewParticle) % numMaxParticle;
}
void Emitter::generateParticle(glm::vec3 playerPosition)
{
	switch (m_output)
	{
	case CONSTANT: //we generate constant new particle
		if (glfwGetTime() - generateTime >= emitFrequency){
			deltaTime = glfwGetTime() - generateTime;
			while (deltaTime >= emitFrequency) {
				pushParticle(particlesPerEmit, playerPosition);
				deltaTime -= emitFrequency;
			}
			generateTime = glfwGetTime();
		}
		break;
	case ONCE: //we generate only one time particle
		if (glfwGetTime() - generateTime >= emitFrequency){
			pushParticle(particlesPerEmit, playerPosition);
			m_output = UNUSED;
		}
		break;
	case UNUSED: // we dont generate new particle
		break;
	}
}

void Emitter::render(Camera &cam)
{
	auto useTexture = getUseTexture();

	if (getUsePointSprites()){ //if we dont use a geometry shader..

		glDepthFunc(GL_FALSE);

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); //how we calculate transperancy

		if (useTexture){
			glEnable(GL_POINT_SPRITE);
			glTexEnvi(GL_POINT_SPRITE, GL_COORD_REPLACE, GL_TRUE);	//the fragment color gets interpolated
		}

		emitterShader->bind();
		glBindBuffer(GL_ARRAY_BUFFER, position_ssbo);

		//Uniform Vars
		emitterShader->sendFloat("fullLifetime", (float)particleLifetime);
		emitterShader->sendInt("particleMortal", m_particleMortal);
		emitterShader->sendFloat("birthTime", m_birthTime);
		emitterShader->sendFloat("deathTime", m_deathTime);
		emitterShader->sendMat4("viewMatrix", cam.getViewMatrix());
		emitterShader->sendMat4("projectionMatrix", cam.getProjectionMatrix());

		if (m_useScaling){
			emitterShader->sendInt("useScaling", 1);
			emitterShader->sendInt("scalingCount", m_scalingCount);
			emitterShader->sendFloatArray("scalingData", m_scalingCount, m_scalingData);
			emitterShader->sendFloat("fullLifetime", (float)particleLifetime);
			emitterShader->sendInt("particleMortal", m_particleMortal);
		}
		else{
			emitterShader->sendInt("useScaling", 0);
			emitterShader->sendFloat("size", particleDefaultSize);
		}

		if (useTexture)
			emitterShader->sendSampler2D("tex", m_textureList.at(0).getTexture());
		emitterShader->sendInt("useTexture", useTexture);


		glVertexPointer(4, GL_FLOAT, 0, (void*)0);
		glEnableClientState(GL_VERTEX_ARRAY);

		if (useTexture){
			glEnable(GL_PROGRAM_POINT_SIZE);
		}
		glDrawArrays(GL_POINTS, 0, numMaxParticle);

		glDisableClientState(GL_VERTEX_ARRAY);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glDepthFunc(GL_TRUE);
		glDisable(GL_BLEND);
		emitterShader->unbind();
	}
	else if (getUseGeometryShader() && useTexture){
		glDepthFunc(GL_FALSE);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		emitterShader->bind();
		glBindBuffer(GL_ARRAY_BUFFER, position_ssbo);

		//Uniform Vars
		emitterShader->sendMat4("viewMatrix", cam.getViewMatrix());
		emitterShader->sendMat4("projectionMatrix", cam.getProjectionMatrix());
		emitterShader->sendFloat("birthTime", m_birthTime);
		emitterShader->sendFloat("deathTime", m_deathTime);
		emitterShader->sendFloat("fullLifetime", (float)particleLifetime);
		emitterShader->sendSampler2D("texture", m_textureList.at(0).getTexture());
		emitterShader->sendVec4("camPos", cam.getPosition());

		emitterShader->sendInt("rotateLeft", m_rotateLeft);
		emitterShader->sendFloat("rotationSpeed", m_rotationSpeed);

		if (m_useScaling){
			emitterShader->sendInt("useScaling", 1);
			emitterShader->sendInt("scalingCount", m_scalingCount);
			emitterShader->sendFloatArray("scalingData", m_scalingCount, m_scalingData);
			emitterShader->sendInt("particleMortal", m_particleMortal);
		}
		else{
			emitterShader->sendInt("useScaling", 0);
			emitterShader->sendFloat("size", particleDefaultSize);
		}

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);
		glDrawArrays(GL_POINTS, 0, numMaxParticle);
		glDisableVertexAttribArray(0);

		glDisableClientState(GL_VERTEX_ARRAY);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glDepthFunc(GL_TRUE);
		glDisable(GL_BLEND);
		emitterShader->unbind();
	}
	else{
		perror("Problem in Emitter.cpp: Geometry Shader maybe miss a texture");
	}
}

void Emitter::updateSize()
{
	if (!m_particleMortal){
		numMaxParticle = particlesPerEmit;
		m_deathTime = 0.0;
	}
	else
		numMaxParticle = (int)((particlesPerEmit * particleLifetime) / emitFrequency);
	computeGroupCount = numMaxParticle / 16 + 1; //+1 to kill the Point. 16 is the local_size_x in the CS

	loadBuffer();
}

//GS & PS swichting

void Emitter::switchToGeometryShader(){
	//Geometry Shader will be used, instead  of Point Sprites
	VertexShader vsParticle(loadShaderSource(SHADERS_PATH + std::string("/ParticleSystem/ParticleSystemGeometryShader.vert")));
	GeometryShader gsParticle(loadShaderSource(SHADERS_PATH + std::string("/ParticleSystem/ParticleSystemGeometryShader.geom")));
	FragmentShader fsParticle(loadShaderSource(SHADERS_PATH + std::string("/ParticleSystem/ParticleSystemGeometryShader.frag")));
	emitterShader = new ShaderProgram(vsParticle, gsParticle, fsParticle);

	m_usePointSprites = false;
	m_useGeometryShader = true;
}
void Emitter::switchToPointSprites(){
	//Point Sprites Shader will be used instead of Geometry Shader
	VertexShader vsParticle(loadShaderSource(SHADERS_PATH + std::string("/ParticleSystem/ParticleSystemPointSprites.vert")));
	FragmentShader fsParticle(loadShaderSource(SHADERS_PATH + std::string("/ParticleSystem/ParticleSystemPointSprites.frag")));
	emitterShader = new ShaderProgram(vsParticle, fsParticle);

	m_usePointSprites = true;
	m_useGeometryShader = false;
}

//physic:

void Emitter::usePhysicTrajectory(glm::vec4 gravity, float speed){
	 setGravity(gravity);
	 m_useTrajectory = true;
	 m_useDirectionGravity = false;
	 m_usePointGravity = false;
	 m_useChaoticSwarmMotion = false;

	 setSpeed(speed);
}
void Emitter::usePhysicDirectionGravity(glm::vec4 gravity, float speed){
	setGravity(gravity);
	m_useTrajectory = false;
	m_useDirectionGravity = true;
	m_usePointGravity = false;
	m_useChaoticSwarmMotion = false;

	setSpeed(speed);
}
void Emitter::usePhysicPointGravity(glm::vec4 gravity, float gravityRange, int gravityFunction, float speed){
	setGravity(gravity);
	m_useTrajectory = false;
	m_useDirectionGravity = false;
	m_usePointGravity = true;
	m_useChaoticSwarmMotion = false;

	setSpeed(speed);
	m_gravityFunction = gravityFunction;
	m_gravityRange = gravityRange;
}
void Emitter::usePhysicSwarmCircleMotion(bool movementVertical, bool movementHorizontalX, bool movementHorizontalZ, float movementLength){
	m_useTrajectory = false;
	m_useDirectionGravity = false;
	m_usePointGravity = false;
	m_useChaoticSwarmMotion = true;

	m_movementVertical = movementVertical;
	m_movementHorizontalX = movementHorizontalX;
	m_movementHorizontalZ = movementHorizontalZ;
	m_movementLength = movementLength;
}

//setters:

void Emitter::setOutputMode(const int OUTPUT)
{
	m_output = static_cast<FLOW> (OUTPUT);
}
void Emitter::setPosition(glm::vec3 newPosition)
{
	emitterPosition = newPosition;
}
void Emitter::movePosition(glm::vec3 playerPosition){
	setPosition(getPosition()+playerPosition);
}
void Emitter::setEmitterMortality(double emitterLifetime)
{
	if (emitterLifetime < 0.001)
		emitterMortal = false;
	else
		emitterMortal = true;
}
void Emitter::setEmitterLifetime(double emitterLifetime)
{
	emitLifetime = emitterLifetime;
}
void Emitter::setEmitFrequency(float newEmitFrequency)
{
	emitFrequency = newEmitFrequency;
	updateSize();
}
void Emitter::setParticlesPerEmit(int numberParticles)
{
	particlesPerEmit = numberParticles;
	updateSize();
}
void Emitter::setParticleLifetime(float newLifetime)
{
	particleLifetime = newLifetime;
	updateSize();
}
void Emitter::setParticleMortality(bool particleMortality)
{
	m_particleMortal = particleMortality;
	if (!m_particleMortal || emitFrequency == 0.0 || particleLifetime == 0.0){
		m_particleMortal = false;
		emitFrequency = 0.0;
		m_deathTime = 0.0;
		m_output = static_cast<FLOW> (1); //set output to once
	}
}
void Emitter::setGravity(glm::vec4 newGravity)
{
	m_gravity = newGravity;
}
void Emitter::setComputeShader(std::string address){
	ComputeShader csParticle(loadShaderSource(SHADERS_PATH + address));
	compute = new ShaderProgram(csParticle);
}
void Emitter::setSpeed(float speed){
	m_speed = speed;
}
void Emitter::setAreaEmitting(bool areaEmittingXY, bool areaEmittingXZ, float size, int accuracy){

	if (areaEmittingXY && areaEmittingXZ){
		perror("Choose beteween areaEmittingXY and -XZ. If both are enabled XZ will be used");
	}
	else{
		m_areaEmittingXY = areaEmittingXY; //emits in a XY area
		m_areaEmittingXZ = areaEmittingXZ; //emits in XZ, like Rain
		m_areaSize = size;
		if (accuracy > 10000)
			m_areaAccuracy = 10000;
		else m_areaAccuracy = accuracy;
	}
}
//TODO: MADELEINE
void Emitter::addTexture(Texture &texture, float percentageLife){
	m_textureList.push_back(texture);
}
//TODO: MADELEINE
void Emitter::deleteTexture(int position){
	m_textureList.erase(m_textureList.begin() + position);
}

void Emitter::useTexture(bool useTexture, float particleSize, float birthTime, float deathTime, bool rotateLeft, float rotationSpeed){
	m_useTexture = useTexture;

	m_birthTime = birthTime;
	m_deathTime = deathTime;

	m_rotateLeft = rotateLeft;
	m_rotationSpeed = rotationSpeed;

	particleDefaultSize = particleSize;
}
void Emitter::useTexture(bool useTexture, std::vector<float> scalingSize, std::vector<float> scalingMoment, 
	float birthTime, float deathTime, bool rotateLeft, float rotationSpeed){
	m_useTexture = useTexture;
	
	m_birthTime = birthTime;
	m_deathTime = deathTime;

	m_rotateLeft = rotateLeft;
	m_rotationSpeed = rotationSpeed;

	if (scalingSize.empty() == false && scalingMoment.empty() == false){
		if (scalingSize.size() > 16 || scalingMoment.size() > 16)
			perror("Error in Emitter: We just support a maximum of 16 scaling-steps");
		else{
			if (scalingSize.size() != scalingMoment.size())
				perror("Error in Emitter: Size of scalingSize & scalingMoment is not equal");
			else {
				for (int i = 0; i < scalingSize.size(); i++){
					m_scalingData[2 * i] = scalingMoment[i];
					m_scalingData[2 * i + 1] = scalingSize[i];
					m_scalingCount = m_scalingCount + 2;
				}
				m_useScaling = true;
			}
		}
	}
}

//Velocity Getter
glm::vec3 Emitter::useVelocityZero(){
	return glm::vec3(0.0, 0.0, 0.0);
}
glm::vec3 Emitter::useVelocityLeftQuarterCircle(){
	return glm::vec3(((rand() % 100) / 100.0f) - 1.0f,
		((rand() % 100) / 100.0f),
		0.0f);
}
glm::vec3 Emitter::useVelocityRightQuarterCircle(){
	return glm::vec3(((rand() % 100) / 100.0f),
		((rand() % 100) / 100.0f),
		0.0f);
}
glm::vec3 Emitter::useVelocitySemiCircle(){
	return glm::vec3(((rand() % 200) / 100.0f) - 1.0f,
					((rand() % 100) / 100.0f),
					0.0f);
}
glm::vec3 Emitter::useVelocityCircle(){
	return	glm::vec3(((rand() % 200) / 100.0f) - 1.0f,
		((rand() % 200) / 100.0f) - 1.0f,
		0.0f);
}
glm::vec3 Emitter::useVelocitySemiSphere(){
	return glm::vec3(((rand() % 200) / 100.0f) - 1.0f,
		((rand() % 100) / 100.0f),
		((rand() % 200) / 100.0f) - 1.0f);
}
glm::vec3 Emitter::useVelocitySphere(){
	return glm::vec3(((rand() % 200) / 100.0f) - 1.0f,
					((rand() % 200) / 100.0f) - 1.0f,
					((rand() % 200) / 100.0f) - -1.0f);
}
void Emitter::setVelocity(glm::vec3 (*pfunc)()){
	m_pfunc = pfunc;
}

//getters:
int Emitter::getOutputMode()
{
	return m_output;
}
glm::vec3 Emitter::getPosition()
{
	return emitterPosition;
}
bool Emitter::getEmitterMortality()
{
	return emitterMortal;
}
double Emitter::getEmitterLifetime()
{
	return emitLifetime;
}
double Emitter::getEmitFrequency()
{
	return emitFrequency;
}
int Emitter::getParticlesPerEmit()
{
	return particlesPerEmit;
}
float Emitter::getParticleLifetime()
{
	return particleLifetime;
}
bool Emitter::getParticleMortality(){
	return m_particleMortal;
}
glm::vec4 Emitter::getGravity()
{
	return m_gravity;
}
float Emitter::getSpeed(){
	return m_speed;
}
bool Emitter::getAreaEmittingXY(){
	return m_areaEmittingXY;
}
bool Emitter::getAreaEmittingXZ(){
	return m_areaEmittingXZ;
}
float Emitter::getAreaSize(){
	return m_areaSize;
}
int Emitter::getAreaAccuracy(){
	return m_areaAccuracy;
}
bool Emitter::getUseTexture(){
	return m_useTexture;
}
bool Emitter::getUseGeometryShader(){
	return m_useGeometryShader;
}
bool Emitter::getUsePointSprites(){
	return m_usePointSprites;
}
float Emitter::getRotationSpeed(){
	return m_rotationSpeed;
}