#include <GL/glew.h>
#include <GL/gl.h> 
#include <GL/glu.h>
#include <GL/glut.h>

#include "bitmap.h"
#include "glutwidget.hpp"
#include "shader_utils.hpp"
#include <iostream>
#include <cmath>
#include <ctime>
#include <algorithm>

#define PI 3.1415926

int glutWidget::m_pos_attribute_location;
unsigned int glutWidget::m_vertexbuffer;
unsigned int glutWidget::m_frame;
unsigned int glutWidget::m_program;
unsigned int glutWidget::m_vertexsh;
unsigned int glutWidget::m_fragmentsh;
int glutWidget::x0 = 0;
int glutWidget::y0 = 0;
int glutWidget::camera = 0;
float glutWidget::rotx = 0;
float glutWidget::roty = 0;
float glutWidget::rotz = 0;
float glutWidget::cposx = 0;
float glutWidget::cposy = 0;
float glutWidget::cposz = 3;
float glutWidget::zoom = 10;
float glutWidget::walk_speed = 0.1;


static int degree = 20;
static float t;

static unsigned int m_texture;
static unsigned int street_texture;
static unsigned int car_texture;
static unsigned int structure_texture;
static unsigned int skybox_texture;
static unsigned int wall_texture;


/*
 Initializes GLUT context
 */
glutWidget::glutWidget(int argc, char** argv)
{
    m_frame = 0;
    glutInitWindowSize(glutWidget::m_width, glutWidget::m_height);
    glutInit(&argc,argv);
    glutInitDisplayString("samples rgb double depth");
    glutCreateWindow("City");
    glutMouseFunc(mouseHandler);     //what to call when user clicks or scrolls
    glutKeyboardFunc(keyDown);       //what to call when user presses a key
    glutKeyboardUpFunc(keyUp);       //what to call when user releases a key
    glutSpecialFunc(specialKeyDown); //what to call when user presses a special key
    glutSpecialFunc(specialKeyUp);   //what to call when user releases a special key
    glutMotionFunc(mouseMove);       //what to call when user moves the mouse
    glutDisplayFunc(render);         //what to call when window needs redrawing
    glutIdleFunc(update);            //what to call when no user input given
    initOpenGL();
}


/*
 Checks whether graphics driver supports all required OpenGL features
 */
void glutWidget::checkExtensions()
{
    //query some extensions to make sure they are present
    if(glewGetExtension("GL_ARB_shading_language_100") != GL_TRUE)
    {
        std::cout << "ERROR: Shading language extension not present." << std::endl;
    }
    if(glewGetExtension("GL_ARB_vertex_program") != GL_TRUE)
    {
        std::cout << "ERROR: Vertex program extension not present." << std::endl;
    }
    if(glewGetExtension("GL_ARB_vertex_shader") != GL_TRUE)
    {
        std::cout << "ERROR: Vertex shader extension not present." << std::endl;
    }
    if(glewGetExtension("GL_ARB_fragment_program") != GL_TRUE)
    {
        std::cout << "ERROR: Fragment program extension not present." << std::endl;
    }
    if(glewGetExtension("GL_ARB_fragment_shader") != GL_TRUE)
    {
        std::cout << "ERROR: Fragment shader extension not present." << std::endl;
    }
    if(glewGetExtension("GL_ARB_vertex_buffer_object") != GL_TRUE)
    {
        std::cout << "ERROR: VBO extension not present." << std::endl;
    }    
}




/*
 Initializes opengl states
 */
void glutWidget::initOpenGL()
{
    glewExperimental = GL_TRUE; 
    GLenum err = glewInit();                             //initialize GLEW - this enables us to use extensions
    if(err != GLEW_OK)
    {
        std::cout << "ERROR: Loading GLEW failed." << std::endl;
        exit(-1);
    }
    checkExtensions();
    glClearColor(1, 1, 1, 0);   //default "empty"/background color is set to white
    
    glEnable(GL_DEPTH_TEST);
    
    CBitmap skybox("skybox.bmp");               //read bitmap image
    glGenTextures(1, &m_texture);               //allocate 1 texture
    glUniform1i(skybox_texture, 0);			        //pass texture location to vertex shader
    glBindTexture(GL_TEXTURE_2D, skybox_texture);    //bind this texture to be active
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, skybox.GetWidth(), skybox.GetHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE, skybox.GetBits());

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);   //specify minificaton filtering
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);   //specify magnificaton filtering
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);        //specify texture coordinate treatment
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);        //specify texture coordinate treatment
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);        //specify texture coordinate treatment
    
    glBindTexture(GL_TEXTURE_2D, 0);
    
    CBitmap streets("streets.bmp");               //read bitmap image
    glGenTextures(1, &street_texture);          //allocate 1 texture
    glUniform1i(street_texture, 0);			        //pass texture location to vertex shader
    glBindTexture(GL_TEXTURE_2D, street_texture);    //bind this texture to be active
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, streets.GetWidth(), streets.GetHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE, streets.GetBits());

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);   //specify minificaton filtering
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);   //specify magnificaton filtering
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);        //specify texture coordinate treatment
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);        //specify texture coordinate treatment


    glBindTexture(GL_TEXTURE_2D, 0);    //bind default texture to be active
    
    CBitmap car("car.bmp");               //read bitmap image
    glGenTextures(1, &car_texture);          //allocate 1 texture
    glUniform1i(car_texture, 0);			        //pass texture location to vertex shader
    glBindTexture(GL_TEXTURE_2D, car_texture);    //bind this texture to be active
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, car.GetWidth(), car.GetHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE, car.GetBits());

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);   //specify minificaton filtering
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);   //specify magnificaton filtering
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);        //specify texture coordinate treatment
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);        //specify texture coordinate treatment
    
    glBindTexture(GL_TEXTURE_2D, 0);    //bind default texture to be active
    
    CBitmap structure("structure.bmp");               //read bitmap image
    glGenTextures(1, &structure_texture);          //allocate 1 texture
    glUniform1i(structure_texture, 0);			        //pass texture location to vertex shader
    glBindTexture(GL_TEXTURE_2D, structure_texture);    //bind this texture to be active
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, structure.GetWidth(), structure.GetHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE, structure.GetBits());

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);   //specify minificaton filtering
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);   //specify magnificaton filtering
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);        //specify texture coordinate treatment
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);        //specify texture coordinate treatment


    glBindTexture(GL_TEXTURE_2D, 0);    //bind default texture to be active
    
    CBitmap wall("wall.bmp");               //read bitmap image
    glGenTextures(1, &wall_texture);          //allocate 1 texture
    glUniform1i(wall_texture, 0);			        //pass texture location to vertex shader
    glBindTexture(GL_TEXTURE_2D, wall_texture);    //bind this texture to be active
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, wall.GetWidth(), wall.GetHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE, wall.GetBits());

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);   //specify minificaton filtering
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);   //specify magnificaton filtering
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);        //specify texture coordinate treatment
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);        //specify texture coordinate treatment


    glBindTexture(GL_TEXTURE_2D, 0);    //bind default texture to be active
    
    makeShaders();          //load data of fragment and vertex programs/shaders - compile shaders
    
    glMatrixMode(GL_PROJECTION);       
    glLoadIdentity();                                             //initializes projection matrix with identity
    gluPerspective(60,(float)m_width/(float)m_height,0.1,1000);  //set up projection mode (field of view, aspect ratio, near and far clipping plane)
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();                       //initializes modelview matrix with identity
    
    glEnable(GL_CULL_FACE);

}


/*
    Redraws window contents
 */
void glutWidget::render()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);     //clears color and depth bits of framebuffer
    
    //Convenience variables
    float t = (float) ((int) (((float) clock()) * 50.0 / CLOCKS_PER_SEC) % 30) / 30.0;
    int t2 = ((int) (((float) clock()) * 50.0 / CLOCKS_PER_SEC) % 240) / 30;
    float t3 = (float) clock() * 5.0 / CLOCKS_PER_SEC;
    
    //These are the control points for the bezier curves
    GLfloat curves[8][4][3] =
    {
        {
            {-8, 0, -8.5},
            {-7, 0, -8.5},
            {-5, 0, -8.5},
            {-4, 0, -8.5}
        },
        {
            {-4, 0, -8.5},
            {-3.5, 0, -8.5},
            {-3.5, 0, -8.5},
            {-3.5, 0, -8}
        },
        {
            {-3.5, 0, -8},
            {-3.5, 0, -7},
            {-3.5, 0, -5},
            {-3.5, 0, -4}
        },
        {
            {-3.5, 0, -4},
            {-3.5, 0, -3.5},
            {-3.5, 0, -3.5},
            {-4, 0, -3.5}
        },
        {
            {-4, 0, -3.5},
            {-5, 0, -3.5},
            {-7, 0, -3.5},
            {-8, 0, -3.5}
        },
        {
            {-8, 0, -3.5},
            {-8.5, 0, -3.5},
            {-8.5, 0, -3.5},
            {-8.5, 0, -4}
        },
        {
            {-8.5, 0, -4},
            {-8.5, 0, -5},
            {-8.5, 0, -7},
            {-8.5, 0, -8}
        },
        {
            {-8.5, 0, -8},
            {-8.5, 0, -8.5},
            {-8.5, 0, -8.5},
            {-8, 0, -8.5}
        }
    };
    
    
    //Manually evaluate the bezier curves
    float s = 1 - t;
    float AB[2] = {curves[t2][0][0]*s + curves[t2][1][0]*t, curves[t2][0][2]*s + curves[t2][1][2]*t};
    float BC[2] = {curves[t2][1][0]*s + curves[t2][2][0]*t, curves[t2][1][2]*s + curves[t2][2][2]*t};
    float CD[2] = {curves[t2][2][0]*s + curves[t2][3][0]*t, curves[t2][2][2]*s + curves[t2][3][2]*t};
    float ABC[2] = {AB[0]*s + BC[0]*t, AB[1]*s + BC[1]*t};
    float BCD[2] = {BC[0]*s + CD[0]*t, BC[1]*s + CD[1]*t};
    float pos[2] = {ABC[0]*s + BCD[0]*t, ABC[1]*s + BCD[1]*t};
    float slope = (ABC[1] - BCD[1]) / (ABC[0] - BCD[0]);
    float theta = atan(slope);
    
    //Rotation/translation matrix
    float matrix[4][4] = 
    {
        {cos(theta), 0, -sin(theta), 0},
        {0, 1, 0, 0},
        {sin(theta), 0, cos(theta), 0},
        {pos[0], 0.01, pos[1], 1}
    };
    
    //Keep the texture pointing in the right direction throughout the loop
    if((ABC[0] - BCD[0]) < 0)
    {
        matrix[0][2] = -(matrix[0][2]);
        matrix[2][0] = -(matrix[2][0]);
    }
    else if((ABC[0] - BCD[0]) > 0)
    {
        matrix[0][0] = -(matrix[0][0]);
        matrix[2][2] = -(matrix[2][2]);
    }
    
    
    
    //Draw streets
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, street_texture);
    glUseProgram(m_program);
    glBegin(GL_TRIANGLE_STRIP);
    
    glColor3f(1,1,1);
    glTexCoord2f(0,1);
    glNormal3f(0,1,0);
    glVertex3f(-10,0,-10);
    
    glColor3f(1,1,1);
    glTexCoord2f(0,0);
    glNormal3f(0,1,0);
    glVertex3f(-10,0,10);
    
    glColor3f(1,1,1);
    glTexCoord2f(1,1);
    glNormal3f(0,1,0);
    glVertex3f(10,0,-10);
    
    glColor3f(1,1,1);
    glTexCoord2f(1,0);
    glNormal3f(0,1,0);
    glVertex3f(10,0,10);
    
    glEnd();
    glUseProgram(0);
    glDisable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);
    //
    
    //Draw cars
    for (int i = 0; i < 9; i++) {
        matrix[3][0] = pos[0] + ((i % 3) * 6);
        matrix[3][2] = pos[1] + ((i / 3) * 6);
        glMatrixMode(GL_MODELVIEW);
        glPushMatrix();
        glMultMatrixf(matrix[0]);
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, car_texture);
        glUseProgram(m_program);

        drawCar();

        glUseProgram(0);
        glDisable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, 0);
        glPopMatrix();
        
        if (i+1 == camera) {
            glMatrixMode(GL_MODELVIEW);
            glLoadIdentity();
            gluLookAt(matrix[3][0],0.5,matrix[3][2],sin(roty)+matrix[3][0],sin(rotx)+0.2,-cos(roty)+matrix[3][2],0,1,0); //Camera rotation/translation is done here
        }
    }
    //
    
    //Draw structures
    for (int i = 0; i < 9; i++) {
        glMatrixMode(GL_MODELVIEW);
        glPushMatrix();
        glTranslatef((i % 3) * 6, 0, (i / 3) * 6);
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, structure_texture);
        glUseProgram(m_program);
        
        drawStructure();
        
        glUseProgram(0);
        glDisable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, 0);
        glPopMatrix();
    }
    //
    
    //Draw skybox
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, skybox_texture);
    glUseProgram(m_program);
    
    drawSkybox();
    
    glUseProgram(0);
    glDisable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);
    //
    
    //Draw walls
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, wall_texture);
    glUseProgram(m_program);
    
    drawWalls();
    
    glUseProgram(0);
    glDisable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);
    //
    
    if (!camera) {
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        gluLookAt(cposx,0.25,cposz,sin(roty)+cposx,sin(rotx)+cposy,-cos(roty)+cposz,0,1,0); //Camera rotation/translation is done here
    }
    
    glutSwapBuffers();  //swaps front and back buffer for double buffering
}


/*
 Handles user event: a mouse button was pressed
 */
void glutWidget::mouseHandler(int button, int state, int x, int y)
{
    if (state == GLUT_UP)
        return;
    
    switch(button)
    {
        case 0: //Left click
            x0 = x;
            y0 = y;
            break;
        case 2: //Right click
            
            break;
        case 3: //Scroll up
            
            break;
        case 4: //Scroll down
            
            break;
    }
}


/*
 Handles user event: a key was pressed
 */
void glutWidget::keyDown(unsigned char key, int, int) 
{
    switch(key)
    {
        case '0':
            camera = 0;
            break;
        case '1':
            camera = 1;
            break;
        case '2':
            camera = 2;
            break;
        case '3':
            camera = 3;
            break;
        case '4':
            camera = 4;
            break;
        case '5':
            camera = 5;
            break;
        case '6':
            camera = 6;
            break;
        case '7':
            camera = 7;
            break;
        case '8':
            camera = 8;
            break;
        case '9':
            camera = 9;
            break;
        case 'q':
        case 27:
            stop(); //quit the main loop, exit program
            break;
        case 'c':
            
            break;
        case 'h':
            
            break;
        case 'w':
            cposx += walk_speed * sin(roty);
            cposz -= walk_speed * cos(roty);
            break;
        case 'a':
            cposx -= walk_speed * cos(roty);
            cposz -= walk_speed * sin(roty);
            break;
        case 's':
            cposx -= walk_speed * sin(roty);
            cposz += walk_speed * cos(roty);
            break;
        case 'd':
            cposx += walk_speed * cos(roty);
            cposz += walk_speed * sin(roty);
            break;
            
    }
}


/*
 Handles user event: a key was released
 */
void glutWidget::keyUp(unsigned char key, int, int) 
{  
    
}


/*
 Handles user event: a special key was pressed
 */
void glutWidget::specialKeyDown(int key, int, int) 
{
    
}


/*
 Handles user event: a special key was released
 */
void glutWidget::specialKeyUp(int key, int, int) 
{  
    switch(key)
    {
        case GLUT_KEY_UP:
            
            break;
        case GLUT_KEY_DOWN:
            
            break;
        case GLUT_KEY_LEFT:
            
            break;
        case GLUT_KEY_RIGHT:
            
            break;
    }
}

void glutWidget::mouseMove(int x, int y)
{
    float dx = ((x - x0)) / 5.0;
    float dy = ((y - y0)) / 5.0;
    
    roty += dx * (3.14159265 / 180.0);
    rotx -= dy * (3.14159265 / 180.0);
    
    if(rotx > 3.14159265 / 2)
        rotx = 3.14159265 / 2;
    else if(rotx < -3.14159265 / 2)
        rotx = -3.14159265 / 2;
    
    x0 = x;
    y0 = y;
}



/*
    Forces program to stop, cleans up, and exits
 */
void glutWidget::stop()
{
    //clean up shaders
    glDetachShader(m_program,m_fragmentsh);
    
    glDeleteProgram(m_program);
	glDeleteShader(m_fragmentsh);
    
    exit(0);
}

/*
    Reads in and compiles shader objects
 */
void glutWidget::makeShaders()
{
    m_program = glCreateProgram();
	
    char *shadercode = readShader("shaders/vertexshader.vert");	//reads shader code (you can edit shader code with a text editor)
    m_vertexsh = glCreateShader(GL_VERTEX_SHADER_ARB);
    glShaderSource(m_vertexsh, 1, (const GLcharARB **)&shadercode,NULL);
    delete[] shadercode;
    glCompileShader(m_vertexsh);    //compiles shader
    printInfoLog(m_vertexsh);       //prints errors during shader compilation
    
    
    shadercode = readShader("shaders/fragmentshader.frag");     //reads shader code (you can edit shader code with a text editor)
    m_fragmentsh = glCreateShader(GL_FRAGMENT_SHADER_ARB);
    glShaderSource(m_fragmentsh, 1, (const GLcharARB **)&shadercode,NULL);
    delete[] shadercode;
    glCompileShader(m_fragmentsh);  //compiles shader
    printInfoLog(m_fragmentsh);     //prints errors during shader compilation

    glAttachShader(m_program,m_vertexsh);
    glAttachShader(m_program,m_fragmentsh);

    glLinkProgram(m_program);   //compiles fragment and vertex shader into a shader program
    printInfoLog(m_program);    //prints errors during program compilation
}



/*
 Starts the main loop
 */
void glutWidget::run()
{
    glutMainLoop();
}



/*
    Called whenever no user input given
 */
void glutWidget::update()
{
    m_frame++;
    glutPostRedisplay(); //marks window for redrawing
}

void glutWidget::drawCar() {
    glBegin(GL_QUADS);    
    
    //Right lower
    glTexCoord2f(0,0.02);
    glNormal3f(0,0,1);
    glVertex3f(-.5,0,.19);
    glTexCoord2f(1,0.02);
    glNormal3f(0,0,1);
    glVertex3f(.5,0,.19);
    glTexCoord2f(1,0.21);
    glNormal3f(0,0,1);
    glVertex3f(.5,.21,.19);
    glTexCoord2f(0,0.21);
    glNormal3f(0,0,1);
    glVertex3f(-.5,.21,.19);
    //Right upper
    glTexCoord2f(0,0.21);
    glNormal3f(0,0,1);
    glVertex3f(-.5,.21,.19);
    glTexCoord2f(0.77,0.21);
    glNormal3f(0,0,1);
    glVertex3f(.27,.21,.19);
    glTexCoord2f(0.59,0.32);
    glNormal3f(0,0,1);
    glVertex3f(.09,.32,.16);
    glTexCoord2f(0.07,0.32);
    glNormal3f(0,0,1);
    glVertex3f(-.43,.32,.16);
    //Left lower
    glTexCoord2f(0,0.21);
    glNormal3f(0,0,-1);
    glVertex3f(-.5,.21,-.19);
    glTexCoord2f(1,0.21);
    glNormal3f(0,0,-1);
    glVertex3f(.5,.21,-.19);
    glTexCoord2f(1,0.02);
    glNormal3f(0,0,-1);
    glVertex3f(.5,0,-.19);
    glTexCoord2f(0,0.02);
    glNormal3f(0,0,-1);
    glVertex3f(-.5,0,-.19);
    //Left upper
    glTexCoord2f(0.07,0.32);
    glNormal3f(0,0,-1);
    glVertex3f(-.43,.32,-.16);
    glTexCoord2f(0.59,0.32);
    glNormal3f(0,0,-1);
    glVertex3f(.09,.32,-.16);
    glTexCoord2f(0.77,0.21);
    glNormal3f(0,0,-1);
    glVertex3f(.27,.21,-.19);
    glTexCoord2f(0,0.21);
    glNormal3f(0,0,-1);
    glVertex3f(-.5,.21,-.19);
    //Front grill
    glTexCoord2f(0.57,0.79);
    glNormal3f(1,0,0);
    glVertex3f(.5,0,.19);
    glTexCoord2f(0.94,0.79);
    glNormal3f(1,0,0);
    glVertex3f(.5,0,-.19);
    glTexCoord2f(0.94,0.97);
    glNormal3f(1,0,0);
    glVertex3f(.5,.21,-.19);
    glTexCoord2f(0.57,0.97);
    glNormal3f(1,0,0);
    glVertex3f(.5,.21,.19);
    //Hood
    glTexCoord2f(0.92,0.42);
    glNormal3f(0,1,0);
    glVertex3f(.5,.21,.19);
    glTexCoord2f(0.92,0.69);
    glNormal3f(0,1,0);
    glVertex3f(.5,.21,-.19);
    glTexCoord2f(0.71,0.69);
    glNormal3f(0,1,0);
    glVertex3f(.27,.21,-.19);
    glTexCoord2f(0.71,0.42);
    glNormal3f(0,1,0);
    glVertex3f(.27,.21,.19);
    //Windshield
    glTexCoord2f(0.70,0.42);
    glNormal3f(0,1,0);
    glVertex3f(.27,.21,.19);
    glTexCoord2f(0.70,0.69);
    glNormal3f(0,1,0);
    glVertex3f(.27,.21,-.19);
    glTexCoord2f(0.54,0.69);
    glNormal3f(0,1,0);
    glVertex3f(.09,.32,-.16);
    glTexCoord2f(0.54,0.42);
    glNormal3f(0,1,0);
    glVertex3f(.09,.32,.16);
    //Roof
    glTexCoord2f(0.50,0.46);
    glNormal3f(0,1,0);
    glVertex3f(.09,.32,.16);
    glTexCoord2f(0.50,0.67);
    glNormal3f(0,1,0);
    glVertex3f(.09,.32,-.16);
    glTexCoord2f(0.07,0.67);
    glNormal3f(0,1,0);
    glVertex3f(-.43,.32,-.16);
    glTexCoord2f(0.07,0.46);
    glNormal3f(0,1,0);
    glVertex3f(-.43,.32,.16);
    //Back lower
    glTexCoord2f(0.08,0.73);
    glNormal3f(-1,0,0);
    glVertex3f(-.5,0,-.19);
    glTexCoord2f(0.44,0.73);
    glNormal3f(-1,0,0);
    glVertex3f(-.5,0,.19);
    glTexCoord2f(0.44,0.88);
    glNormal3f(-1,0,0);
    glVertex3f(-.5,.21,.19);
    glTexCoord2f(0.08,0.88);
    glNormal3f(-1,0,0);
    glVertex3f(-.5,.21,-.19);
    //Back upper
    glTexCoord2f(0.08,0.88);
    glNormal3f(-1,0,0);
    glVertex3f(-.5,.21,-.19);
    glTexCoord2f(0.44,0.88);
    glNormal3f(-1,0,0);
    glVertex3f(-.5,.21,.19);
    glTexCoord2f(0.42,1);
    glNormal3f(-1,0,0);
    glVertex3f(-.43,.32,.16);
    glTexCoord2f(0.12,1);
    glNormal3f(-1,0,0);
    glVertex3f(-.43,.32,-.16);
    
    glEnd();
}

void glutWidget::drawStructure() {
    glBegin(GL_QUADS);
    
    //Front
    glTexCoord2f(0,0);
    glNormal3f(0,0,1);
    glVertex3f(-8,0,-4);
    glTexCoord2f(1,0);
    glNormal3f(0,0,1);
    glVertex3f(-4,0,-4);
    glTexCoord2f(1,1);
    glNormal3f(0,0,1);
    glVertex3f(-4,5,-4);
    glTexCoord2f(0,1);
    glNormal3f(0,0,1);
    glVertex3f(-8,5,-4);
    //Right
    glTexCoord2f(0,0);
    glNormal3f(1,0,0);
    glVertex3f(-4,0,-4);
    glTexCoord2f(1,0);
    glNormal3f(1,0,0);
    glVertex3f(-4,0,-8);
    glTexCoord2f(1,1);
    glNormal3f(1,0,0);
    glVertex3f(-4,5,-8);
    glTexCoord2f(0,1);
    glNormal3f(1,0,0);
    glVertex3f(-4,5,-4);
    //Back
    glTexCoord2f(0,0);
    glNormal3f(0,0,-1);
    glVertex3f(-4,0,-8);
    glTexCoord2f(1,0);
    glNormal3f(0,0,-1);
    glVertex3f(-8,0,-8);
    glTexCoord2f(1,1);
    glNormal3f(0,0,-1);
    glVertex3f(-8,5,-8);
    glTexCoord2f(0,1);
    glNormal3f(0,0,-1);
    glVertex3f(-4,5,-8);
    //Left
    glTexCoord2f(0,0);
    glNormal3f(-1,0,0);
    glVertex3f(-8,0,-8);
    glTexCoord2f(1,0);
    glNormal3f(-1,0,0);
    glVertex3f(-8,0,-4);
    glTexCoord2f(1,1);
    glNormal3f(-1,0,0);
    glVertex3f(-8,5,-4);
    glTexCoord2f(0,1);
    glNormal3f(-1,0,0);
    glVertex3f(-8,5,-8);
    
    glEnd();
}

void glutWidget::drawSkybox() {
     glBegin(GL_QUADS);
     
     //Back
     glTexCoord2f(.5,1.0/3.0);
     glVertex3f(500,-500,-500);
     glTexCoord2f(.5,2.0/3.0);
     glVertex3f(500,500,-500);
     glTexCoord2f(.25,2.0/3.0);
     glVertex3f(-500,500,-500);
     glTexCoord2f(.25,1.0/3.0);
     glVertex3f(-500,-500,-500);
     //Front
     glTexCoord2f(1,0.334);
     glVertex3f(-500,-500,500);
     glTexCoord2f(1,0.665);
     glVertex3f(-500,500,500);
     glTexCoord2f(.75,0.665);
     glVertex3f(500,500,500);
     glTexCoord2f(.75,0.334);
     glVertex3f(500,-500,500);
     //Left
     glTexCoord2f(0,0.334);
     glVertex3f(-500,-500,500);
     glTexCoord2f(.25,0.334);
     glVertex3f(-500,-500,-500);
     glTexCoord2f(.25,0.665);
     glVertex3f(-500,500,-500);
     glTexCoord2f(0,0.665);
     glVertex3f(-500,500,500);
     //Right
     glTexCoord2f(.5,0.334);
     glVertex3f(500,-500,-500);
     glTexCoord2f(.75,0.334);
     glVertex3f(500,-500,500);
     glTexCoord2f(.75,0.665);
     glVertex3f(500,500,500);
     glTexCoord2f(.5,0.665);
     glVertex3f(500,500,-500);
     //Top
     glTexCoord2f(.251,2.0/3.0);
     glVertex3f(-500,500,-500);
     glTexCoord2f(.499,2.0/3.0);
     glVertex3f(500,500,-500);
     glTexCoord2f(.499,1.0);
     glVertex3f(500,500,500);
     glTexCoord2f(.251,1.0);
     glVertex3f(-500,500,500);
     //Bottom
     glTexCoord2f(.251,1.0/3.0);
     glVertex3f(-500,-500,-500);
     glTexCoord2f(.251,0.0/3.0);
     glVertex3f(-500,-500,500);
     glTexCoord2f(.499,0.0/3.0);
     glVertex3f(500,-500,500);
     glTexCoord2f(.499,1.0/3.0);
     glVertex3f(500,-500,-500);
     
     
     glEnd();
}

void glutWidget::drawWalls() {
    glBegin(GL_QUADS);
    
    glTexCoord2f(0,0);
    glVertex3f(-10,0,-10);
    glTexCoord2f(0,20);
    glVertex3f(10,0,-10);
    glTexCoord2f(1,20);
    glVertex3f(10,0.5,-10);
    glTexCoord2f(1,0);
    glVertex3f(-10,0.5,-10);
    
    glTexCoord2f(0,0);
    glVertex3f(10,0,-10);
    glTexCoord2f(0,20);
    glVertex3f(10,0,10);
    glTexCoord2f(1,20);
    glVertex3f(10,0.5,10);
    glTexCoord2f(1,0);
    glVertex3f(10,0.5,-10);
    
    glTexCoord2f(0,0);
    glVertex3f(10,0,10);
    glTexCoord2f(0,20);
    glVertex3f(-10,0,10);
    glTexCoord2f(1,20);
    glVertex3f(-10,0.5,10);
    glTexCoord2f(1,0);
    glVertex3f(10,0.5,10);
    
    glTexCoord2f(0,0);
    glVertex3f(-10,0,10);
    glTexCoord2f(0,20);
    glVertex3f(-10,0,-10);
    glTexCoord2f(1,20);
    glVertex3f(-10,0.5,-10);
    glTexCoord2f(1,0);
    glVertex3f(-10,0.5,10);
    
    glEnd();
}

