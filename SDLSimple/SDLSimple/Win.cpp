#include "Win.h"
#include "Utility.h"


#define WIN_WIDTH 20
#define WIN_HEIGHT 8


const char  SPRITESHEET_FILEPATH[]  = "assets/doc.png",
            MAP_TILESET_FILEPATH[]  = "assets/oak_woods_tileset.png",
            BGM_FILEPATH[]          = "assets/Ancient Mystery Waltz Presto.mp3",

            DEATH_FILEPATH[]        = "assets/Goblin Death.wav",
            ENEMY1_FILEPATH[]        = "assets/MutilatedStumbler.png",
            ENEMY2_FILEPATH[]        = "assets/GhastlyEye.png",
            ENEMY3_FILEPATH[]        = "assets/BrittleArcher.png",
            TEXT_FILEPATH[]         = "assets/font1.png",
            BULLET_FILEPATH[]         = "assets/new_bullet.png";

unsigned int WIN_DATA[] =
{
    0,  0,  0, 0,   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0,  0,  0, 0,   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0,  0,  0, 0,   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0,  0,  0, 0,   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0,  0,  0, 0,   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0,  0,  0, 0,   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0,  0,  0, 0,   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0,  0,  0, 0,   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
};

Win::~Win()
{
    delete[] m_state.enemies;
    delete    m_state.player;
    delete    m_state.map;

    Mix_FreeMusic(m_state.bgm);
}

void Win::initialise()
{
    GLuint map_texture_id =  Utility::load_texture(MAP_TILESET_FILEPATH);
    m_state.map = new Map(WIN_WIDTH, WIN_HEIGHT, WIN_DATA, map_texture_id, 1.0f, 25, 24);

    

    
    // ————— GEORGE SET-UP ————— //
    // Existing
    m_state.player = new Entity();
    m_state.player->set_entity_type(PLAYER);
    m_state.player->set_position(glm::vec3(0.0f, 0.0f, 0.0f));
    m_state.player->set_movement(glm::vec3(0.0f));
    m_state.player->set_speed(2.5f);
    m_state.player->set_acceleration(glm::vec3(0.0f, -9.81f, 0.0f));
    m_state.player->m_texture_id = Utility::load_texture(SPRITESHEET_FILEPATH);

    m_state.bullet = new Entity();
    m_state.bullet->set_entity_type(BULLET);
    m_state.bullet->set_position(glm::vec3(0.0f, 0.0f, 0.0f));
    m_state.bullet->set_movement(glm::vec3(0.0f));
    m_state.bullet->set_speed(2.5f);
    
    m_state.bullet->m_texture_id = Utility::load_texture(BULLET_FILEPATH);
    m_state.bullet->deactivate();
    
    
    // Walking
    m_state.player->m_walking[m_state.player->LEFT] = new int[] { 1, 5, 9, 13 };
    m_state.player->m_walking[m_state.player->RIGHT] = new int[4] { 3, 7, 11, 15 };
    m_state.player->m_walking[m_state.player->UP] = new int[4] { 2, 6, 10, 14 };
    m_state.player->m_walking[m_state.player->DOWN] = new int[4] { 0, 4, 8, 12 };

    m_state.player->m_animation_indices = new int[8]{0,1,2,3,4,5,6,7} ;// start George looking left
    m_state.player->m_animation_frames = 4;
    m_state.player->m_animation_index = 0;
    m_state.player->m_animation_time = 0.0f;
    m_state.player->m_animation_cols = 8;
    m_state.player->m_animation_rows = 1;
    m_state.player->set_height(1.0f);
    m_state.player->set_width(1.0f);

    // Jumping
    m_state.player->m_jumping_power = 5.0f;


    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096);

    m_state.bgm = Mix_LoadMUS(BGM_FILEPATH);
    Mix_PlayMusic(m_state.bgm, -1);
    Mix_VolumeMusic(MIX_MAX_VOLUME / 16.0f);

    m_state.death_sfx = Mix_LoadWAV(DEATH_FILEPATH);

    
    
    GLuint enemy1_texture_id = Utility::load_texture(ENEMY1_FILEPATH);
    GLuint enemy3_texture_id = Utility::load_texture(ENEMY3_FILEPATH);
    m_state.enemies = new Entity[m_number_of_enemies];
    for (int i = 0; i < m_number_of_enemies; i++){

        m_state.enemies[i].set_movement(glm::vec3(0.0f));
        m_state.enemies[i].set_speed(0.5f);
        m_state.enemies[i].set_acceleration(glm::vec3(0.0f, -9.81f, 0.0f));
        m_state.enemies[i].m_animation_indices =new int[4] { 0, 1, 2, 3 };
        m_state.enemies[i].m_animation_frames = 4;
        m_state.enemies[i].m_animation_index = 0;
        m_state.enemies[i].m_animation_time = 0.0f;
        m_state.enemies[i].m_animation_cols = 4;
        m_state.enemies[i].m_animation_rows = 1;
        m_state.enemies[i].set_height(1.0f);
        m_state.enemies[i].set_width(1.0f);
    }
    m_state.enemies[0].set_position(glm::vec3(4.0f, 10.0f, 0.0f));
    m_state.enemies[0].set_entity_type(ENEMY);
    m_state.enemies[0].set_ai_type(GUARD);
    m_state.enemies[0].set_ai_state(IDLE);
    m_state.enemies[0].m_texture_id = enemy1_texture_id;
    
   
    
    
    m_state.enemies[1].set_position(glm::vec3(10.0f, 0.0f, 0.0f));
    m_state.enemies[1].set_entity_type(ENEMY);
    m_state.enemies[1].set_ai_type(WALKER);
    m_state.enemies[1].set_ai_state(WALKING);
    m_state.enemies[1].m_texture_id = enemy3_texture_id;
   
    

    }


void Win::update(float delta_time, int lives)
{
    
    
        
    
}


void Win::render(ShaderProgram *program)
{
    m_state.map->render(program);
    GLuint text_texture_id = Utility::load_texture(TEXT_FILEPATH);
    Utility::draw_text(program, text_texture_id, "YOU WIN", 0.5f, -0.1f, glm::vec3(-2.0f,0.0f, 0.0f));
   
}

