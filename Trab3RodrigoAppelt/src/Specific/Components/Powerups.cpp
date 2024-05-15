#include "Powerups.h"


// #include "../../Misc/TaskManager.cpp"
#include "../../gl_canvas2d.h"
#include "../../3D/ObjLoader.h"
#include "../../Misc/TaskManager.h"
#include "../../Misc/SoundPlayer.h"

ExtraBall::ExtraBall(Vector2 pos){
    position = pos;
}

void ExtraBall::render(float blockSize, Vector2 gameAreaStart){
    CV::translate(Vector2::zero());
    CV::color(1,1,1);
    CV::obj(ObjLoader::get("powerupBall"),
        gameAreaStart + position.multiply(blockSize) + Vector2(blockSize * 0.5f),
        Vector2(9,9)
    );
    #if PHYSICS_DEBUG
    //render collider
    CV::color(1,0,0);
    CV::circle(
        gameAreaStart + position.multiply(blockSize) + Vector2(blockSize * 0.5f),
        15,
        15
    );
    #endif 
}

Laser::Laser(Vector2 pos, Direction dir){
    position = pos;
    direction = dir;
    dirty = false;
}

void Laser::render(float blockSize, Vector2 gameAreaStart){
    CV::color(1,1,1);
    CV::translate(Vector2::zero());
    CV::obj(ObjLoader::get("powerupLaser"),
        gameAreaStart + position.multiply(blockSize) + Vector2(blockSize*0.5f),
        Vector2(12,12)
    );
    #if PHYSICS_DEBUG
    //render collider
    CV::color(1,0,0);
    CV::rect(
        gameAreaStart + position.multiply(blockSize) + Vector2(blockSize * 0.5f)
            + Vector2(-15,-15),
        gameAreaStart + position.multiply(blockSize) + Vector2(blockSize * 0.5f)
            + Vector2(15,15)
    );
    #endif
}

void Laser::activateVFX(float blockSize, Vector2 gameAreaStart, Vector2 gameAreaSize){
    SoundPlayer::play("laserZap");
    dirty = true;
    TaskManager::schedule(
        [=](){
            Vector2 blockCenter = gameAreaStart + position.multiply(blockSize) + Vector2(blockSize*0.5f);
            float laserWidth = 0.2;
            
            CV::translate(0,0);
            CV::color(Vector3::fromHex(0xff006e));
            // fds o laser vai ir pras 2 direcoes

            // horizontal laser
            if(direction == Direction::Horizontal){
                Vector2 hp1 = Vector2(
                    gameAreaStart.x, 
                    blockCenter.y - blockSize * laserWidth
                );
                Vector2 hp2 = Vector2(
                    gameAreaStart.x + gameAreaSize.x,
                    blockCenter.y + blockSize * laserWidth
                );
                CV::rectFill(hp1, hp2);
            }

            // vertical laser
            if(direction == Direction::Vertical) {
                Vector2 vp1 = Vector2(
                    blockCenter.x - blockSize * laserWidth,
                    gameAreaStart.y
                );
                Vector2 vp2 = Vector2(
                    blockCenter.x + blockSize * laserWidth,
                    gameAreaStart.y + gameAreaSize.y
                );
                CV::rectFill(vp1, vp2);
            }
        },
        TaskManager::TaskType::DURATION,
        0.2f
    );
}
