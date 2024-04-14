#ifndef __POWERUPS_H__
#define __POWERUPS_H__

class Powerup{
public:
    virtual void render() = 0;
};

class Coin : public Powerup{
    
};

class AditionalBall : public Powerup{
    
};

class CrossHorizontal : public Powerup{
    
};

class CrossVertical : public Powerup{
    
};

class Bounce : public Powerup{
    
};

#endif // __POWERUPS_H__