#include "FireboyWatergirl.h"
#include "Controller.h"
#include "Engine.h"
#include "GameOver.h"
#include "Home.h"
#include "Level.h"
#include "Player.h"

std::vector<Game*> FireboyWatergirl::levels = {};
Game*   FireboyWatergirl::current_level = nullptr;
Game*   FireboyWatergirl::last_level = nullptr;
int     FireboyWatergirl::level_index = 0;
Player* FireboyWatergirl::fireboy   = nullptr;
Player* FireboyWatergirl::watergirl = nullptr;
Audio*  FireboyWatergirl::audio = nullptr;
bool    FireboyWatergirl::viewBBox = false;
int     FireboyWatergirl::game_speed = 250;

void FireboyWatergirl::Init() 
{
    // cria sistema de áudio
    audio = new Audio();
    audio->Add(MENU, "Resources/sfx/Menu.wav");
    audio->Add(MUSIC, "Resources/sfx/Level.wav");
    audio->Add(DIED, "Resources/sfx/Death.wav");
    audio->Add(FB_JUMP, "Resources/sfx/FireboyJump.wav");
    audio->Add(WG_JUMP, "Resources/sfx/WatergirlJump.wav");
    audio->Add(GAMEOVER, "Resources/sfx/GameOver.wav");
    audio->Add(LEVEL_START, "Resources/sfx/LevelStart.wav");
    audio->Add(BUTTON_SELECT, "Resources/sfx/ButtonSelect.wav");

    // cria gerenciador de controles
    Controller* gamepad_fire = new Controller();
    Controller* gamepad_water = new Controller();

    // tenta inicializar um controle do xbox
    bool controller_on_fire = gamepad_fire->XboxInitialize(PLAYER1);
    bool controller_on_water = false;
    if (controller_on_fire) {
        gamepad_water->Initialize();
        controller_on_water = gamepad_water->DeviceNext();
    }

    // bounding box não visível
    viewBBox = false;

    // cria jogadores
    fireboy = new Player(true, gamepad_fire, controller_on_fire, true);
    watergirl = new Player(false, gamepad_water, controller_on_water, false);

    // cria níveis
    levels = {
        new Home(),
        new Level(1),
        new GameOver(),
        new Level(2),
        new GameOver(),
        new Level(3),
        new GameOver(),
    };

    // inicializa nível de abertura do jogo
    current_level = levels[0];
    current_level->Init();
}

void FireboyWatergirl::Update()
{
    // habilita/desabilita visualização da bounding box
    if (window->KeyPress('B'))
        viewBBox = !viewBBox;

    // atualiza nível
    current_level->Update();
} 

void FireboyWatergirl::Draw()
{
    // desenha nível
    current_level->Draw();
} 

void FireboyWatergirl::Finalize()
{
    current_level->Finalize();

    delete fireboy;
    delete watergirl;
    delete audio;

    for (auto level : levels) delete level;
}

void FireboyWatergirl::NextLevel() 
{
    last_level = current_level;
    current_level->Finalize();
    level_index = (level_index + 1) % levels.size();
    current_level = levels[level_index];
    current_level->Init();
};

void FireboyWatergirl::GameOverL()
{
    last_level = current_level;
    current_level->Finalize();
    level_index = levels.size() - 1;
    current_level = levels[level_index];
    current_level->Init();
};

void FireboyWatergirl::LastLevel() {
    last_level = current_level;
    current_level->Finalize();
    level_index = (level_index - 1) % levels.size();
    current_level = levels[level_index];
    current_level->Init();
}

void FireboyWatergirl::HomeLevel() {
    last_level = current_level;
    current_level->Finalize();
    level_index = 0;
    current_level = levels[level_index];
    current_level->Init();
}


// ------------------------------------------------------------------------------
//                                  WinMain                                      
// ------------------------------------------------------------------------------

int APIENTRY WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{
    Engine * engine = new Engine();

    engine->window->Mode(WINDOWED);
    engine->window->Size(960, 768);
   
    // configura o motor
    engine->window->Color(30, 50, 80);
    engine->window->Title("Element Twins");
    engine->window->Icon(IDI_ICON);
    engine->window->Cursor(IDC_CURSOR);
    engine->graphics->VSync(true);

    // inicia o jogo
    int status = engine->Start(new FireboyWatergirl());

    delete engine;
    return status;
}
