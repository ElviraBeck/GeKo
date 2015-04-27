#include "GeKo_Graphics/ParticleSystem/ParticleSystem.h"

ParticleSystem::ParticleSystem(glm::vec3 position)
{
	effect = new Effect();
	setEffect(effect);
	setPosition(position);
}

ParticleSystem::ParticleSystem(glm::vec3 position, Effect* effect)
{
	this->effect = new Effect();
	setEffect(effect);
	setPosition(position);
}

ParticleSystem::ParticleSystem(glm::vec3 position, const char* filepath)
{
	effect = new Effect();
	loadEffect(filepath);
	setPosition(position);
}

ParticleSystem::~ParticleSystem()
{
	//TODO
}


void ParticleSystem::start()
{
	effect->start();
}

void ParticleSystem::stop()
{
	effect->stop();
}

void ParticleSystem::update(Camera &cam)
{
	effect->updateEmitters(cam);
}

void ParticleSystem::render(Camera &cam)
{
	effect->renderEmitters(cam);
}

void ParticleSystem::setPosition(glm::vec3 newPosition)
{
	position = newPosition;
	effect->setPosition(newPosition);
}

void ParticleSystem::setEffect(Effect* newEffect)
{
	effect = newEffect;
}

void ParticleSystem::loadEffect(const char* filepath)
{
	effect->loadEffect(filepath);
}

glm::vec3 ParticleSystem::getPosition()
{
	return position;
}

Effect* ParticleSystem::getEffect()
{
	return effect;
}