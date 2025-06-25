#include "LevelLoader.h"
#include "FireboyWatergirl.h"
#include "Level.h"
#include "WorldEntity.h"
#include <fstream>
#include <limits>
#include <string>

constexpr int finish_offset = 140;
constexpr int player_start_offset = 100;

void loadLevel(Level& level, Window* window, const std::string& path) {
    std::ifstream fin;
    fin.open(path);
    Color white{ 1,1,1,1 };

    int background;
    float cR, cG, cB;

    // Ignora coment�rios
    fin >> background;
    while (!fin.good())
    {
        fin.clear();
        fin.ignore((std::numeric_limits<std::streamsize>::max)(), '\n');
        fin >> background;
    }

    // Carregar background
    fin >> cR;
    fin >> cG;
    fin >> cB;
    std::string a = "fsd";
    level.backg = new Background(a, Color{cR, cG, cB, 1});
    level.scene->Add(level.backg, STATIC);

    int ground_level, length;
    WorldEntity* entity;

    // Ignora coment�rios
    fin >> ground_level;
    while (!fin.good())
    {
        fin.clear();
        fin.ignore((std::numeric_limits<std::streamsize>::max)(), '\n');
        fin >> ground_level;
    }

    // Carrega ch�o do jogo
    fin >> length;
    fin >> cR;
    fin >> cG;
    fin >> cB;
    int i = 0;
    level.num_ground_blocks = 0;
    while (i < (length + window->Width())) {
        entity = new WorldEntity(i, ground_level, GROUND1, Color{ cR, cG, cB, 1 });
        entity->Translate(0, entity->Height() / 2.0f);
        level.scene->Add(entity, STATIC);
        i += entity->Width();
        level.num_ground_blocks++;
    }

    level.scene->Add(new WorldEntity(player_start_offset, ground_level, MOVE_CAMERA, white), STATIC);
    level.scene->Add(new WorldEntity(length - window->Width() - 40, ground_level, STOP_CAMERA, white), STATIC);
    level.scene->Add(new WorldEntity(length - finish_offset, ground_level, FINISH, white), STATIC);

    // Adiciona jogador na cena
    //level.scene->Add(FireboyWatergirl::player, MOVING);
    //FireboyWatergirl::player->maxPoints(length - finish_offset - player_start_offset);

    // Jogador come�a no ch�o e fora da tela
    //FireboyWatergirl::player->MoveTo(
    //    -FireboyWatergirl::player->Width() / 2.0f,
    //    ground_level - FireboyWatergirl::player->Height() / 2.0f
    //);

    float posX, posY;
    uint  entityType;

    // Carrega entidades do jogo
    fin >> posX;
    while (!fin.eof())
    {
        if (fin.good())
        {
            // Pega a posi��o y da entidade
            while (fin.peek() == ' ') fin.get();
            if (std::isdigit(fin.peek())) {
                fin >> posY;
            }
            else {
                posY = ground_level;
                fin.get();
            }

            // L� linha com informa��es da entidade
            fin >> entityType;
            entity = new WorldEntity(posX, posY, (EntityTypeSprite)entityType, white);
            entity->Translate(0, -1.0f * entity->Height() / 2.0f);

            level.scene->Add(entity, STATIC);
            if (entity->Type() == BLOCK)
                level.scene->Add(new WorldEntity(posX, entity->Y(), BLOCK_SIDE, white), STATIC);
        }
        else
        {
            // Ignora coment�rios
            fin.clear();
            fin.ignore((std::numeric_limits<std::streamsize>::max)(), '\n');
        }

        fin >> posX;
    }
    fin.close();
}
