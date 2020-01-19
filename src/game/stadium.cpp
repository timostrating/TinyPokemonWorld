//
// Created by sneeuwpop on 13-01-20.
//

#include "stadium.h"
#include "../graphics/camera.h"

void Stadium::render()
{
    shader.use();
    glUniformMatrix4fv(MVP, 1, GL_FALSE, &(Camera::main->combined * transform)[0][0]);
    if(hover) glUniform4f(u_color, 0.7, 0.6, 0.5, 1.0);
    else      glUniform4f(u_color, 0.6, 0.5, 0.4, 1.0);
    mesh->render();

    hover = false;
}

void Stadium::onHover()
{
    hover = true;
}

void Stadium::onClick()
{
    // TODO: fireworks
}