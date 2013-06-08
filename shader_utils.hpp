char *readShader(const char * filename);
void printInfoLog(unsigned int obj);

//Functions for communication between the main program and the shaders
void setUniform1i(unsigned int m_program, const char *name, int value);
void setUniform1f(unsigned int m_program, const char *name, float value);
void setUniform2f(unsigned int m_program, const char *name, float value1, float value2);