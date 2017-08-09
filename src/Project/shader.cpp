#include "shader.h"

GLuint compileShaders(char *vertex, char *fragment)
{
	std::ifstream fin;
	std::string s;
	std::string cont;
	GLuint shaderProgram = glCreateProgram();
	GLuint shaderObject;
	const GLchar* p[1];
	GLint lengths[1];
	GLint success;

	fin.open(vertex);
	while (std::getline(fin, s)) {
		cont += s;
		cont += '\n';
	}
	fin.close();
	shaderObject = glCreateShader(GL_VERTEX_SHADER);
	p[0] = cont.data();
	lengths[0] = strlen(cont.data());
	glShaderSource(shaderObject, 1, p, lengths);
	glCompileShader(shaderObject);
	glGetShaderiv(shaderObject, GL_COMPILE_STATUS, &success);
	if (!success) {
		GLchar InfoLog[1024];
		glGetShaderInfoLog(shaderObject, 1024, NULL, InfoLog);
		std::cerr << "Error compiling shader type vertex: '" << InfoLog << "'" << std::endl;
		exit(1);
	}
	glAttachShader(shaderProgram, shaderObject);
	cont.clear();

	fin.open(fragment);
	while (std::getline(fin, s)) {
		cont += s;
		cont += '\n';
	}
	fin.close();
	shaderObject = glCreateShader(GL_FRAGMENT_SHADER);
	p[0] = cont.data();
	lengths[0] = strlen(cont.data());
	glShaderSource(shaderObject, 1, p, lengths);
	glCompileShader(shaderObject);
	glGetShaderiv(shaderObject, GL_COMPILE_STATUS, &success);
	if (!success) {
		GLchar InfoLog[1024];
		glGetShaderInfoLog(shaderObject, 1024, NULL, InfoLog);
		std::cerr << "Error compiling shader type fragment: '" << InfoLog << "'" << std::endl;
		while (1);
		exit(1);
	}
	glAttachShader(shaderProgram, shaderObject);



	GLchar ErrorLog[1024] = { 0 };

	glLinkProgram(shaderProgram);
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, sizeof(ErrorLog), NULL, ErrorLog);
		std::cerr << "Error linking shader program: '" << ErrorLog << "'" << std::endl;
		while (1);
		exit(1);
	}

	glValidateProgram(shaderProgram);
	glGetProgramiv(shaderProgram, GL_VALIDATE_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, sizeof(ErrorLog), NULL, ErrorLog);
		std::cerr << "Invalid shader program: '" << ErrorLog << "'" << std::endl;
		while (1);
		exit(1);
	}


	return shaderProgram;
}