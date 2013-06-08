#include <iostream>
#include "glutwidget.hpp"


int main (int argc, char** argv)
{
    glutWidget gluw = glutWidget::init(argc,argv);
    gluw.run();
    
    return 0;
}