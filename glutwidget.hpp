#ifndef _GLUTWIDGET_HPP_
#define _GLUTWIDGET_HPP_

/*
 GLUT widget singleton
 */
class glutWidget
{
private:
    static unsigned int m_program;                 //shader program
    static unsigned int m_vertexsh;                //vertex shader
    static unsigned int m_fragmentsh;              //fragment shader
    static int m_pos_attribute_location;           //points to attribute location in vertex shader
    static unsigned int m_vertexbuffer;
    static unsigned int m_frame;
    static const unsigned int m_width = 800;
    static const unsigned int m_height = 800;
    static int camera;                             //Controls which camera is active
    static int x0, y0;
    static float rotx, roty, rotz;
    static float cposx, cposy, cposz;
    static float zoom;
    static float walk_speed;
    static void render();
    static void update();
    static void mouseHandler(int button, int state, int x, int y);
    static void keyDown(unsigned char key, int, int);
    static void keyUp(unsigned char key, int, int);
    static void specialKeyDown(int key, int, int);
    static void specialKeyUp(int key, int, int);
    static void mouseMove(int x, int y);
    static void stop();
    static void initOpenGL();
    static void drawCubes();
    static void makeShaders();
    static void drawCar();
    static void drawStructure();
    static void drawSkybox();
    static void drawWalls();
    glutWidget(int argc, char **argv);
    static void checkExtensions();
public:
    static glutWidget& init(int argc, char **argv)
    {
        static glutWidget gw(argc, argv);
        return gw;
    }
    static void run();
};





#endif

