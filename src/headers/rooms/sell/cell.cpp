//доделать if (x > 0 || y > 0 || x < width - 1 || y < height - 1){
/*
012
345
678
*/
#include <cstdint>

inline void nearby_cells_checker(uint16_t x, uint16_t y) noexcept {
    constexpr SDL_Color color0 = {0, 0, 0, 0};

    float * const vision = map_area[x][y].bio_cell.vision();

    uint16_t left_x = x - 1, right_x = x + 1, up_y = y - 1, down_y = y + 1;

    (SDL_Color&)vision[0*4] = map_area[left_x][up_y].object    ?    color0 : ((SDL_Color&)map_area[left_x][up_y].object)    * light[up_y];
    (SDL_Color&)vision[1*4] = map_area[x][up_y].object         ?    color0 : ((SDL_Color&)map_area[x][up_y].object)         * light[up_y];
    (SDL_Color&)vision[2*4] = map_area[right_x][up_y].object   ?    color0 : ((SDL_Color&)map_area[right_x][up_y].object)   * light[up_y];
    (SDL_Color&)vision[3*4] = map_area[left_x][y].object       ?    color0 : ((SDL_Color&)map_area[left_x][y].object)       * light[y];
    (SDL_Color&)vision[4*4] = map_area[right_x][y].object      ?    color0 : ((SDL_Color&)map_area[right_x][y].object)      * light[y];
    (SDL_Color&)vision[5*4] = map_area[left_x][down_y].object  ?    color0 : ((SDL_Color&)map_area[left_x][down_y].object)  * light[down_y];
    (SDL_Color&)vision[6*4] = map_area[x][down_y].object       ?    color0 : ((SDL_Color&)map_area[x][down_y].object)       * light[down_y];
    (SDL_Color&)vision[7*4] = map_area[right_x][down_y].object ?    color0 : ((SDL_Color&)map_area[right_x][down_y].object) * light[down_y];
}

inline void attack(neural_bio_cell& neural_cell, const uint16_t x, const uint16_t y, const uint16_t offset_x, const uint16_t offset_y) noexcept {

    map_content &attacked_map = map_area[x + offset_x][y + offset_y];
    void * const attacked_obj = attacked_map.object;
    if(!attacked_obj){
        neural_cell.energy -= 0.4;
        return ;
    }

    switch (attacked_map.type) {
        case obj_type::WALL:
            neural_cell.energy -= 0.4;
            return ;

        case obj_type::BIO_CELL:

            if(neural_cell.energy > 0.4)
            {
                const float attacked_obj_energy = ((neural_bio_cell&)attacked_obj).energy;
                if(attacked_obj_energy < 2){
                    neural_cell.energy +=  attacked_obj_energy;
                    ((neural_bio_cell&)attacked_obj).energy = 0;
                }else if(const uint16_t stolen_energy = attacked_obj_energy * 0.1 > 2){
                    neural_cell.energy += stolen_energy - 0.4;
                    ((neural_bio_cell&)attacked_obj).energy -= stolen_energy;
                }else{
                    neural_cell.energy += 1.4;
                    ((neural_bio_cell&)attacked_obj).energy -= 1.4;
                }
            }
            return ;

        case obj_type::BIOMATERIAL:

            if(neural_cell.energy > 0.4) {
                const float attacked_obj_energy = ((biomaterial&)attacked_obj).energy;
                const bool is_weak = (attacked_obj_energy < 2);
                neural_cell.energy += is_weak ? attacked_obj_energy : 2;
                ((biomaterial&)attacked_obj).energy = is_weak ? 0 : attacked_obj_energy - 2;
            }
            return ;
    }

}

inline void go(neural_bio_cell& neural_cell, const uint16_t x, const uint16_t y, const uint16_t offset_x, const uint16_t offset_y) noexcept {



    if(map_content* const go_to_map = &map_area[x + offset_x][y + offset_y] == nullptr)
    {
        if(neural_cell.energy > 0.1)
            return ;
        neural_cell.energy -= 0.1;
        go_to_map->bio_cell = &neural_cell;
        go_to_map->type = obj_type::BIO_CELL;
        map_area[x][y].bio_cell = nullptr;
    }
    else {
        neural_cell.energy -= 0.2;
    }
}

inline void transfer_energy(neural_bio_cell& neural_cell, const uint16_t x, const uint16_t y, const uint16_t offset_x, const uint16_t offset_y) noexcept {

    if(map_content* const transfer_energy_to_map = &map_area[x + offset_x][y + offset_y] == nullptr)
    {
        neural_cell.energy -= 0.1;
        transfer_energy_to_map->bio_cell = &neural_cell;
        transfer_energy_to_map->type = obj_type::BIO_CELL;
        map_area[x][y].bio_cell = nullptr;
    }
    else {
        neural_cell.energy -= 0.2;
    }
}

//король неверленда

inline void apply_response(neural_bio_cell& neural_cell, const uint16_t x, const uint16_t y) noexcept {
//{void,attack[4],go[8],transfer_energy[4],separate[4],chloroplast_add}

switch (neural_cell.run()) {

case 0://do nothing

    break;
case 1://attack left
    attack(neural_cell, -1, 0);
    break;
case 2://attack right
    attack(neural_cell, 1, 0);
    break;
case 3://attack up
    attack(neural_cell, 0, -1);
    break;
case 4://attack down
    attack(neural_cell, 0, 1);
    break;

case 5://go left
    go(neural_cell, -1, 0);
    break;
case 6://go up left
    go(neural_cell, -1, -1);
    break;
case 7://go down left
    go(neural_cell, -1, 1);
    break;
case 8://go right
    go(neural_cell, 1, 0);
    break;
case 9://go up right
    go(neural_cell, 1, -1);
    break;
case 10://go down right
    go(neural_cell, 1, 1);
    break;
case 11://go up
    go(neural_cell, 0, -1);
    break;
case 12://go down
    go(neural_cell, 0, 1);
    break;

case 13://transfer_energy left
    break;
case 14://transfer_energy right
    break;
case 15://transfer_energy up
    break;
case 16://transfer_energy down
    break;

case 17://separate left
    break;
case 18://separate right
    break;
case 19://separate up
    break;
case 20://separate down
    break;

case 21://chloroplast_add
    break;
}
}
