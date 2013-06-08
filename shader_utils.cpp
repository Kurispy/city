#include <stdio.h>
#include <GL/glew.h>
#include <GL/gl.h> 
#include <GL/glu.h>

#include <iostream>


/*
    Read shader code from file
 */
char *readShader(const char * filename)
{
	FILE* file = fopen(filename,"rt");
	if(file)
	{
		fseek(file, 0, SEEK_END);	
		int signs = ftell(file);            //total number of bytes to read
		char *content = new char[signs+1];  //+1 for 0 terminated string
		rewind(file);
		signs = fread(content,1,sizeof(char)*signs,file);
		content[signs] = '\0';	//create null-terminated string
		fclose(file);
		return content;
	}
	return NULL;	
}


/*
    Print info about shader compilation/state
 */
void printInfoLog(unsigned int obj)
{
	int infoll = 0;
	glGetShaderiv(obj, GL_OBJECT_INFO_LOG_LENGTH_ARB, &infoll);
    
	if(infoll > 0)
	{
		char *infoLog = new char[infoll];
        int charsWritten = 0;
		glGetShaderInfoLog(obj, infoll, &charsWritten, infoLog);
        std::cout <<  infoLog << std::endl;
		delete[] infoLog;
	}
}

//Functions for communication between the main program and the shaders

void setUniform1i(unsigned int m_program, const char *name, int value)
{
    glUniform1i(glGetUniformLocation(m_program, name), value);
}


void setUniform1f(unsigned int m_program, const char *name, float value)
{
    glUniform1f(glGetUniformLocation(m_program, name), value);
}


void setUniform2f(unsigned int m_program, const char *name, float value1, float value2)
{
    glUniform2f(glGetUniformLocation(m_program, name), value1, value2);
}
