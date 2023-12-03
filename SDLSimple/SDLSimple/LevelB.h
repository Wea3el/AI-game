#include "Scene.h"

class LevelB : public Scene {
public:
    // ————— STATIC ATTRIBUTES ————— //
    int ENEMY_COUNT = 2;
    
    // ————— CONSTRUCTOR ————— //
    ~LevelB();
    
    // ————— METHODS ————— //
    void initialise() override;
    void update(float delta_time,int lives) override;
    void render(ShaderProgram *program) override;
};