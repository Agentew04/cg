#include "Skybox.h"

#include <vector>
#include <string>


#include <GL/glut.h>
#include <GL/freeglut_ext.h>

#include "../../2D/Bmp.h"

void Engine::Components::Skybox::Start()
{
    std::vector<std::string> faces = {
        skyboxPath + "px.bmp",
        skyboxPath + "nx.bmp",
        skyboxPath + "py.bmp",
        skyboxPath + "ny.bmp",
        skyboxPath + "pz.bmp",
        skyboxPath + "nz.bmp"
    };

    unsigned int textureID[6];
    for(int i=0;i<6;i++){
        textureID[i] = loadTexture(faces[i]);
    }
    leftTextureId = textureID[0];
    rightTextureId = textureID[1];
    topTextureId = textureID[2];
    bottomTextureId = textureID[3];
    frontTextureId = textureID[4];
    backTextureId = textureID[5];
}

GLuint Engine::Components::Skybox::loadTexture(const std::string& path){
    Bmp bmp = Bmp(path.c_str());
    bmp.convertBGRtoRGB();

    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(
        GL_TEXTURE_2D,
        0,
        GL_RGB,
        bmp.getWidth(),
        bmp.getHeight(),
        0,
        GL_RGB,
        GL_UNSIGNED_BYTE,
        bmp.getImage()
    );

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    return textureID;
}

void Engine::Components::Skybox::Render(){

    float offset = 0.5f;
    // disable lighting
    glDisable(GL_LIGHTING);

    glMatrixMode(GL_MODELVIEW);

    glDisable(GL_DEPTH_TEST);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, frontTextureId);
    glBegin(GL_QUADS);
    glTexCoord2f(0, 0); glVertex3f(-distance-offset, -distance-offset, distance+offset);
    glTexCoord2f(1, 0); glVertex3f(distance+offset, -distance, distance+offset);
    glTexCoord2f(1, 1); glVertex3f(distance+offset, distance+offset, distance+offset);
    glTexCoord2f(0, 1); glVertex3f(-distance-offset, distance+offset, distance+offset);
    glEnd();

    glBindTexture(GL_TEXTURE_2D, backTextureId);
    glBegin(GL_QUADS);
    glTexCoord2f(0, 0); glVertex3f(distance+offset, -distance-offset, -distance-offset);
    glTexCoord2f(1, 0); glVertex3f(-distance-offset, -distance-offset, -distance-offset);
    glTexCoord2f(1, 1); glVertex3f(-distance-offset, distance, -distance-offset);
    glTexCoord2f(0, 1); glVertex3f(distance+offset, distance+offset, -distance-offset);
    glEnd();

    glBindTexture(GL_TEXTURE_2D, leftTextureId);
    glBegin(GL_QUADS);
    glTexCoord2f(0, 0); glVertex3f(distance+offset, -distance-offset, distance+offset);
    glTexCoord2f(1, 0); glVertex3f(distance+offset, -distance-offset, -distance-offset);
    glTexCoord2f(1, 1); glVertex3f(distance+offset, distance+offset, -distance-offset);
    glTexCoord2f(0, 1); glVertex3f(distance+offset, distance+offset, distance+offset);
    glEnd();

    glBindTexture(GL_TEXTURE_2D, rightTextureId);
    glBegin(GL_QUADS);
    glTexCoord2f(0, 0); glVertex3f(-distance-offset, -distance-offset, -distance-offset);
    glTexCoord2f(1, 0); glVertex3f(-distance-offset, -distance-offset, distance+offset);
    glTexCoord2f(1, 1); glVertex3f(-distance-offset, distance+offset, distance+offset);
    glTexCoord2f(0, 1); glVertex3f(-distance-offset, distance+offset, -distance-offset);
    glEnd();

    glBindTexture(GL_TEXTURE_2D, topTextureId);
    glBegin(GL_QUADS);
    glTexCoord2f(0, 1); glVertex3f(-distance-offset, distance+offset, -distance-offset);
    glTexCoord2f(1, 1); glVertex3f(distance+offset, distance+offset, -distance-offset);
    glTexCoord2f(1, 0); glVertex3f(distance+offset, distance+offset, distance+offset);
    glTexCoord2f(0, 0); glVertex3f(-distance-offset, distance+offset, distance+offset);
    glEnd();

    glBindTexture(GL_TEXTURE_2D, bottomTextureId);
    glBegin(GL_QUADS);
    glTexCoord2f(0, 1); glVertex3f(-distance-offset, -distance-offset, distance+offset);
    glTexCoord2f(1, 1); glVertex3f(distance+offset, -distance-offset, distance+offset);
    glTexCoord2f(1, 0); glVertex3f(distance+offset, -distance-offset, -distance-offset);
    glTexCoord2f(0, 0); glVertex3f(-distance-offset, -distance-offset, -distance-offset);
    glEnd();

    glPopMatrix();

    glEnable(GL_DEPTH_TEST);
    glDisable(GL_TEXTURE_2D);
    glEnable(GL_LIGHTING);
}

void Engine::Components::Skybox::Update(){

}

void Engine::Components::Skybox::Destroy(){
    glDeleteTextures(1, &leftTextureId);
    glDeleteTextures(1, &rightTextureId);
    glDeleteTextures(1, &topTextureId);
    glDeleteTextures(1, &bottomTextureId);
    glDeleteTextures(1, &frontTextureId);
    glDeleteTextures(1, &backTextureId);
}