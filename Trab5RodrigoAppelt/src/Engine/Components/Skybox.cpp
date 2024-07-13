#include "Skybox.h"

#include <vector>
#include <string>
#include <GL/glut.h>
#include <GL/freeglut_ext.h>
#include <chrono>


#include "../../2D/Bmp.h"
#include "../../2D/lodepng.h"
#include "../Engine.h"
#include "Camera.h"

void Engine::Components::Skybox::Start()
{
    auto start = std::chrono::high_resolution_clock::now();

    std::vector<std::string> faces = {
        skyboxPath + "px.png",
        skyboxPath + "nx.png",
        skyboxPath + "py.png",
        skyboxPath + "ny.png",
        skyboxPath + "pz.png",
        skyboxPath + "nz.png"
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

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end - start;

    log(LogLevel::INFO, "Skybox loading done! (Took " + std::to_string(duration.count()) + " seconds)");
}

GLuint Engine::Components::Skybox::loadTexture(const std::string& path){
    // Bmp bmp = Bmp(path.c_str());
    // bmp.convertBGRtoRGB();
    std::vector<unsigned char> image; // The raw pixels
    unsigned width, height;
    auto start = std::chrono::high_resolution_clock::now();
    unsigned error = lodepng::decode(image, width, height, path);
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end - start;

    if(error){
        std::cout << "decoder error " << error << ": " << lodepng_error_text(error) << std::endl;
        return 0;
    }

    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(
        GL_TEXTURE_2D,
        0,
        GL_RGB,
        width, // bmp.getWidth(),
        height,// bmp.getHeight(),
        0,
        GL_RGBA,
        GL_UNSIGNED_BYTE,
        image.data()// bmp.getImage()
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
    glPushMatrix();
    glTranslatef(camPos.x, camPos.y, camPos.z);

    glDisable(GL_DEPTH_TEST);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, frontTextureId);
    glBegin(GL_QUADS);
    glTexCoord2f(0, 1); glVertex3f(-distance-offset, -distance-offset, distance+offset);
    glTexCoord2f(0, 0); glVertex3f(-distance-offset, distance+offset, distance+offset);
    glTexCoord2f(1, 0); glVertex3f(distance+offset, distance+offset, distance+offset);
    glTexCoord2f(1, 1); glVertex3f(distance+offset, -distance, distance+offset);
    glEnd();

    glBindTexture(GL_TEXTURE_2D, backTextureId);
    glBegin(GL_QUADS);
    glTexCoord2f(0, 1); glVertex3f(distance+offset, -distance-offset, -distance-offset);
    glTexCoord2f(1, 1); glVertex3f(-distance-offset, -distance-offset, -distance-offset);
    glTexCoord2f(1, 0); glVertex3f(-distance-offset, distance, -distance-offset);
    glTexCoord2f(0, 0); glVertex3f(distance+offset, distance+offset, -distance-offset);
    glEnd();

    glBindTexture(GL_TEXTURE_2D, leftTextureId);
    glBegin(GL_QUADS);
    glTexCoord2f(0, 1); glVertex3f(distance+offset, -distance-offset, distance+offset);
    glTexCoord2f(1, 1); glVertex3f(distance+offset, -distance-offset, -distance-offset);
    glTexCoord2f(1, 0); glVertex3f(distance+offset, distance+offset, -distance-offset);
    glTexCoord2f(0, 0); glVertex3f(distance+offset, distance+offset, distance+offset);
    glEnd();

    glBindTexture(GL_TEXTURE_2D, rightTextureId);
    glBegin(GL_QUADS);
    glTexCoord2f(0, 1); glVertex3f(-distance-offset, -distance-offset, -distance-offset);
    glTexCoord2f(1, 1); glVertex3f(-distance-offset, -distance-offset, distance+offset);
    glTexCoord2f(1, 0); glVertex3f(-distance-offset, distance+offset, distance+offset);
    glTexCoord2f(0, 0); glVertex3f(-distance-offset, distance+offset, -distance-offset);
    glEnd();

    glBindTexture(GL_TEXTURE_2D, topTextureId);
    glBegin(GL_QUADS);
    glTexCoord2f(0, 0); glVertex3f(-distance-offset, distance+offset, -distance-offset);
    glTexCoord2f(1, 0); glVertex3f(distance+offset, distance+offset, -distance-offset);
    glTexCoord2f(1, 1); glVertex3f(distance+offset, distance+offset, distance+offset);
    glTexCoord2f(0, 1); glVertex3f(-distance-offset, distance+offset, distance+offset);
    glEnd();

    glBindTexture(GL_TEXTURE_2D, bottomTextureId);
    glBegin(GL_QUADS);
    glTexCoord2f(0, 0); glVertex3f(-distance-offset, -distance-offset, distance+offset);
    glTexCoord2f(1, 0); glVertex3f(distance+offset, -distance-offset, distance+offset);
    glTexCoord2f(1, 1); glVertex3f(distance+offset, -distance-offset, -distance-offset);
    glTexCoord2f(0, 1); glVertex3f(-distance-offset, -distance-offset, -distance-offset);
    glEnd();


    glPopMatrix();
    glEnable(GL_DEPTH_TEST);
    glDisable(GL_TEXTURE_2D);
    glEnable(GL_LIGHTING);
}

void Engine::Components::Skybox::Update(float){
    auto cams = Engine::instance->GetAllComponentsOfType<Camera>();
    for(auto &cam: cams){
        if(cam->isActive){
            if(auto camActor = cam->actor.lock()){
                camPos = camActor->position;
            }
        }
    }
}

void Engine::Components::Skybox::Destroy(){
    glDeleteTextures(1, &leftTextureId);
    glDeleteTextures(1, &rightTextureId);
    glDeleteTextures(1, &topTextureId);
    glDeleteTextures(1, &bottomTextureId);
    glDeleteTextures(1, &frontTextureId);
    glDeleteTextures(1, &backTextureId);
}
