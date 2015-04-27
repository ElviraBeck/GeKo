#pragma once

#include"AL/al.h"
#include"AL/alc.h"
#include"AL/efx.h"
#include <string>
#include <iostream>
//#include <windows.h>
#include <vector>
#include <glm/ext.hpp>
#include <map>

//#include <GeKo_Graphics/Scenegraph/Node.h>


class SoundFileHandler{
	public:
		SoundFileHandler(int maxNumberOfSources);
		~SoundFileHandler();

		void generateSource(ALfloat position[3], const char *filepath);
		void generateSource(glm::vec3 position, const char *filepath);
		void generateSource(std::string name, ALfloat position[3], const char *filepath);
		void generateSource(std::string name, glm::vec3 position, const char *filepath);
		void setPitch(int i, float f);
		void setPitch(std::string name, float f);
		void setGain(int i, float f);
		void setGain(std::string name, float f);
		void updateSourcePosition(int i, ALfloat position[3]);
		void updateSourcePosition(std::string name, ALfloat position[3]);
		void updateSourcePosition(int i, glm::vec3 position);
		void updateSourcePosition(std::string name, glm::vec3 position);
		glm::vec3 getSourcePosition(int i);
		glm::vec3 getSourcePosition(std::string name);
		void updateSourceVelocity(int i, ALfloat velocity[3]);
		void updateSourceVelocity(std::string name, ALfloat velocity[3]);
		void updateSourceVelocity(int i, glm::vec3 velocity);
		void updateSourceVelocity(std::string name, glm::vec3 velocity);
		void enableLooping(int i);
		void enableLooping(std::string name);
		void disableLooping(int i);
		void disableLooping(std::string name);
		void updateListenerPosition(ALfloat position[3]);
		void updateListenerPosition(glm::vec3 position);
		glm::vec3 getListenerPosition();
		void updateListenerVelocity(ALfloat velocity[3]);
		void updateListenerVelocity(glm::vec3 velocity);
		glm::vec3 getListenerVelocity();
		void updateListenerOrientation(ALfloat orientation[6]);
		void updateListenerOrientation(glm::vec3 at, glm::vec3 up);
		void playSource(int i);
		void playSource(std::string name);
		void pauseSource(int i);
		void pauseSource(std::string name);
		void stopSource(int i);
		void stopSource(std::string name);
		void playAllSources();
		void pauseAllSources();
		void stopAllSources();
		bool sourceIsPlaying(std::string name);


	protected:
		ALenum m_tempformat;
		int m_maxNumberOfSources;
		int m_numberOfSources;
		ALint m_iState;
		ALuint* m_buffers;
		ALuint* m_sources;
		int generateBufferData(const char *filepath, int numberOfSources);
		int endWithError(char* msg);

		std::map<std::string, int> m_sourceMap;
		char m_type[4];
		int m_size, m_chunkSize;
		short m_formatType, m_channels;
		int m_sampleRate, m_avgBytesPerSec;
		short m_bytesPerSample, m_bitsPerSample;
		int m_dataSize;
		ALuint m_frequency;
		ALenum m_format;
		FILE *m_fp = NULL;
		unsigned char* m_buf;


};