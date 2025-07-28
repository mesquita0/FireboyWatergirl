#include "LevelLoader.h"
#include "FireboyWatergirl.h"
#include "Level.h"
#include "WorldEntity.h"
#include <fstream>
#include <istream>
#include <limits>
#include <string>

constexpr int finish_offset = 140;
constexpr int player_start_offset = 100;

void  skipComments(std::ifstream& fin);
float getAttribute(std::ifstream& fin, float default_value);

void loadLevel(Level& level, Window* window, const std::string& path) {
    std::ifstream fin;
    fin.open(path);

    skipComments(fin);

    std::string background;
    float cR, cG, cB; 

    // Carregar background
    std::getline(fin >> std::ws, background, ' ');
    background = "Resources/" + background + ".png";
    cR = getAttribute(fin, 1);
    cG = getAttribute(fin, 1);
    cB = getAttribute(fin, 1);
    level.backg = new Background(background, Color{cR, cG, cB, 1});

    skipComments(fin);

    // Adiciona jogadores na cena
    FireboyWatergirl::fireboy->Reset();
    FireboyWatergirl::watergirl->Reset();
    level.scene->Add(FireboyWatergirl::fireboy, MOVING);
    level.scene->Add(FireboyWatergirl::watergirl, MOVING);

    int posXfire, posYfire, posXwater, posYwater;
    float player_scale;

    // Posição inicial dos jogadores
    fin >> posXfire;
    fin >> posYfire;
    fin >> posXwater;
    fin >> posYwater;
    fin >> player_scale;
    FireboyWatergirl::fireboy->setInitialPosition(posXfire, posYfire);
    FireboyWatergirl::watergirl->setInitialPosition(posXwater, posYwater);
    FireboyWatergirl::fireboy->MoveTo(posXfire, posYfire);
    FireboyWatergirl::fireboy->ScaleTo(player_scale);
    FireboyWatergirl::watergirl->MoveTo(posXwater, posYwater);
    FireboyWatergirl::watergirl->ScaleTo(player_scale);

    char layer;
    float posX, posY, posZ, rotation, scale;
    uint  entityType;
    WorldEntity* entity;

    // Carrega entidades do jogo
    fin >> posX;
    while (!fin.eof()) {
        if (fin.good()) {
            
            // Lê posições y e z
            fin >> posY;
            skipComments(fin);
            fin >> layer;
            switch (layer) {
            case 'F': posZ = Layer::FRONT;  break;
            case 'U': posZ = Layer::UPPER;  break;
            case 'M': posZ = Layer::MIDDLE; break;
            case 'L': posZ = Layer::LOWER;  break;
            case 'B': posZ = Layer::BACK;   break;
            default:  posZ = Layer::UPPER;  break;
            }

            // Lê linha com informações da entidade
            fin >> entityType;
            scale = getAttribute(fin, 1);
            rotation = getAttribute(fin, 0);
            cR = getAttribute(fin, 1);
            cG = getAttribute(fin, 1);
            cB = getAttribute(fin, 1);

            entity = new WorldEntity(posX, posY, posZ, (EntityTypeSprite)entityType, scale, rotation * (PI/180), Color{cR, cG, cB, 1});

            ObjectGroup object_group = entity->isMovable() ? MOVING : STATIC;
            level.scene->Add(entity, object_group);
        }
        else {
            fin.clear();
            skipComments(fin);
        }

        fin >> posX;
    }
    fin.close();
}

void skipComments(std::ifstream& fin) {
    fin >> std::ws;
    while (fin.peek() == '/' || fin.peek() == '/n') {
        fin.ignore((std::numeric_limits<std::streamsize>::max)(), '\n');
        fin >> std::ws;
    }
}

float getAttribute(std::ifstream& fin, float default_value) {
    float result;
    fin >> std::ws;

    if (std::isdigit(fin.peek())) {
        fin >> result;
    }
    else {
        result = default_value;
        fin.get();
    }

    return result;
}
