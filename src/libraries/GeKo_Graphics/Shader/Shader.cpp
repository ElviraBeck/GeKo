#include "Shader.h"

template<typename ... Args>
void attachShaders(GLuint handle, GLuint shaderHandle, Args ...args)
{
  glAttachShader(handle, shaderHandle);
  attachShaders(handle, args...);
}
void attachShaders(GLuint handle, GLuint shaderHandle)
{
  glAttachShader(handle, shaderHandle);
}

std::string loadShaderSource(std::string path)
{
	std::ifstream input(path);
    std::string source((std::istreambuf_iterator<char>(input)),
    std::istreambuf_iterator<char>());
    //std::cout << path << " "<< source << std::endl;
  return source;
}

void checkShader(GLuint shader) {
  GLint status;
  glGetShaderiv(shader, GL_COMPILE_STATUS, &status);

  if (status == GL_FALSE) {
    GLint infoLogLength;
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLogLength);

    GLchar* infoLog = new GLchar[infoLogLength + 1];
    glGetShaderInfoLog(shader, infoLogLength, NULL, infoLog);

    std::cout << "ERROR: Unable to compile shader" << std::endl << infoLog << std::endl;
    delete[] infoLog;
  }
  else {
    //std::cout << "SUCCESS: Shader compiled" << std::endl;
  }
}
BaseShader::BaseShader(const std::string &shaderSource, ShaderType shaderType)
{
  handle = glCreateShader(shaderType);
  auto source = shaderSource.c_str();
  auto size = (GLint)shaderSource.size();
  glShaderSource(handle, 1, &source, &size);
  glCompileShader(handle);
  checkShader(handle);
}

ShaderProgram::ShaderProgram(const VertexShader &vs, const FragmentShader &fs)
{
  handle = glCreateProgram();
  attachShaders(handle, vs.handle, fs.handle);
  glLinkProgram(handle);
}

ShaderProgram::ShaderProgram(const ComputeShader &cs)
{
  handle = glCreateProgram();
  attachShaders(handle, cs.handle);
  glLinkProgram(handle);
}

ShaderProgram::ShaderProgram(const VertexShader &vs, const GeometryShader &gs, const FragmentShader &fs)
{
	handle = glCreateProgram();
	attachShaders(handle, vs.handle, gs.handle, fs.handle);
	glLinkProgram(handle);
}

void ShaderProgram::bind() const
{
  glUseProgram(handle);
}

void ShaderProgram::unbind() const
{
  glUseProgram(0);
}

GLuint ShaderProgram::getLocation(std::string uniform) {
    return  glGetUniformLocation(handle,uniform.c_str());
}

void ShaderProgram::sendInt(std::string uniform, int i) {
	glUniform1i(getLocation(uniform), i);
}

void ShaderProgram::sendDouble(std::string uniform, double d) {
	glUniform1d(getLocation(uniform), d);
}

void ShaderProgram::sendFloat(std::string uniform, float f) {
	glUniform1f(getLocation(uniform), f);
}

void ShaderProgram::sendVec2(std::string uniform, glm::vec2 v) {
	glUniform2f(getLocation(uniform), v.x, v.y);
}

void ShaderProgram::sendVec3(std::string uniform, glm::vec3 v) {
    glUniform3f(getLocation(uniform),v.x,v.y,v.z);
}

void ShaderProgram::sendVec4(std::string uniform, glm::vec4 v) {
	glUniform4f(getLocation(uniform), v.x, v.y, v.z, v.w);
}

void ShaderProgram::sendFloatArray(std::string uniform, int size, GLfloat array []) {
	glUniform1fv(getLocation(uniform), size, array);
}

void ShaderProgram::sendMat4(std::string uniform, glm::mat4 m) {
	glUniformMatrix4fv(getLocation(uniform), 1, false, glm::value_ptr(m));
}

void ShaderProgram::sendSampler2D(std::string uniform, GLuint sampler2Dhandler) {
	glUniform1i(getLocation(uniform), 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, sampler2Dhandler);
}

void ShaderProgram::sendSampler2D(std::string uniform, GLuint sampler2Dhandler, int textureIndex) {

	if (textureIndex > 32)
	{
		std::cerr << "Error in ShaderProgram::sendSampler2D - textureIndex too high" << std::endl;
		return;
	}

	glUniform1i(getLocation(uniform), textureIndex);
	//0x84C0 = GL_TEXTURE0
	glActiveTexture(0x84C0 + textureIndex);
	glBindTexture(GL_TEXTURE_2D, sampler2Dhandler);
}

void ShaderProgram::sendSkyboxTexture(std::string uniform, GLuint sampler2Dhandler, int textureIndex)
{
	if (textureIndex > 32)
	{
		std::cerr << "Error in ShaderProgram::sendSampler2D - textureIndex too high" << std::endl;
		return;
	}

	glUniform1i(getLocation(uniform), textureIndex);
	glActiveTexture(0x84C0 + textureIndex);
	glBindTexture(GL_TEXTURE_CUBE_MAP, sampler2Dhandler);
}

void ShaderProgram::sendLightData(std::string uniformPosition, std::string uniformColor, std::string uniformDirection, std::string uniformExponent, std::string uniformAngle, std::string uniformRadius, PointLight light) {
	sendVec4(uniformPosition, light.m_position);
	sendVec4(uniformColor, light.m_color);
	sendVec3(uniformDirection, light.m_direction);
	sendFloat(uniformExponent, light.m_exponent);
	sendFloat(uniformAngle, light.m_angle);
}

void ShaderProgram::sendLightData(std::string uniformPosition, std::string uniformColor, std::string uniformDirection, std::string uniformExponent, std::string uniformAngle, std::string uniformRadius, DirectionLight light){
	sendVec4(uniformPosition, light.m_position);
	sendVec4(uniformColor, light.m_color);
	sendVec3(uniformDirection, light.m_direction);
	sendFloat(uniformExponent, light.m_exponent);
	sendFloat(uniformAngle, light.m_angle);
}

void ShaderProgram::sendLightData(std::string uniformPosition, std::string uniformColor, std::string uniformDirection, std::string uniformExponent, std::string uniformAngle, std::string uniformRadius, ConeLight light){
	sendVec4(uniformPosition, light.m_position);
	sendVec4(uniformColor, light.m_color);
	sendVec3(uniformDirection, light.m_direction);
	sendFloat(uniformExponent, light.m_exponent);
	sendFloat(uniformAngle, light.m_angle);
}