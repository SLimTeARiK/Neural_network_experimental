
namespace sell{
#include "objects.hpp"
#include "const_neural_mini8.hpp"


// Элемент карты
struct map_content {
    obj_type type;
    union {
        neural_bio_cell* bio_cell = nullptr;
        biomaterial* remains;
        barrier_object* wall;
        void* object;
    };
    ~map_content() {
        switch(type) {
            case obj_type::BIO_CELL: if(bio_cell)delete bio_cell; break;
            case obj_type::BIOMATERIAL: if(remains)delete remains; break;
            case obj_type::WALL: if(wall)delete wall; break;
        }
    }
};


template <uint16_t width = 100, uint16_t height = 100>
class area{
map_content map_area[width][height];//карта ссылок на объекты
float map_nutrients[width][height] = {5.0f};//карта минералов
SDL_Color map_color_actions[width][height] = {5.0f};//карта цвета действия

barrier_object walls[width*2 + (height-2)*2];//количество стен(возможно в будующем станет больше или будут динамическими)

neural_bio_cell neural_cells[width*height-(width*2)-(height-2)*2];//Кличесво возможно живых равно количеству свободного места

const float overhead_light = 12.0f;

template<size_t... I>
static constexpr auto generate_light(std::index_sequence<I...>) -> std::array<decltype(overhead_light / height), sizeof...(I)>  {
    return { { (overhead_light / height * I)... } };
}

constexpr static const float light[height] = {generate_light(std::make_index_sequence<height>{})};//свет на разных уровнях, в будующем буду дополнительно уменьшать от находящихся сверху.

uint16_t number_alive = 0;

area() : neural_cells(){
uint16_t iw = 0, in = 0;
// Инициализация
for (uint16_t xi = 0; xi < width; ++xi)
for (uint16_t yi = 0; yi < height; ++yi)
if (xi == 0 || xi == width-1 || yi == 0 || yi == height-1)
map_area[xi][yi] = map_content{.type = obj_type::WALL, .wall = walls + iw++};
else{
map_area[xi][yi] = get_random<int>(0, 1) ? map_content{.type = obj_type::BIO_CELL, .bio_cell = neural_cells + in++} : map_content{.type = obj_type::EMPTY, .bio_cell = nullptr};
//map_area[xi][yi] = tmp;

}
}

public: void next_step(){
number_alive = 0;


for(uint16_t x{1}; x < width-1; ++x){
    for(uint16_t y{1}; y < height-1; ++y){

        float* const nutrient_xy = map_nutrients[x][y];

        if(map_area[x][y].object){
            switch (map_area[x][y].type) {
            case obj_type::WALL:
                continue;
                break;
            case obj_type::EMPTY:
                continue;
                break;

            case obj_type::BIO_CELL:{
                neural_bio_cell& neural_cell = map_area[x][y].bio_cell;
                if(neural_cell.alive == 1 && neural_cell.fatigue % 2)
                if(neural_cell.energy < 0.01){
                    neural_cell.alive = 0;
                    ++neural_cell.fatigue;
                    continue;
                }
                ++neural_cell.fatigue;

                neural_cell.absorb(nutrient_xy);
                neural_cell.energy = neural_cell.chloroplast * light[y];

                nearby_cells_checker(x, y);//Доделать


                apply_response(neural_cell, x, y);//Доделать
                break;
                }
            case obj_type::BIOMATERIAL:
                continue;//позже допридумать и доделать
                if(map_area[x][y+1].object==nullptr){
                    map_area[x][y+1]->remains = map_area[x][y];
                    map_area[x][y+1]->type = obj_type::BIOMATERIAL;
                    map_area[x][y].remains = nullptr;
                }


                break;
            }
        }
        else{
            float nutr = *nutrient_xy;
            if(nutr < 5)
                *nutrient_xy = nutr < 4.5 ? nutr+0.5 : 5;
            continue;
        }






    }//for x
    x = 0;
}//for y
//y = 0;
}



~area(){}



private:
#include "cell.cpp"
};


}
