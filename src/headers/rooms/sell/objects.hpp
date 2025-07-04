
// Типы объектов
enum obj_type : uint8_t {EMPTY, BIO_CELL, BIOMATERIAL, WALL};


struct alignas(4) barrier_object {
SDL_Color color = {0, 0, 0, 0};
};

struct alignas(8) biomaterial{
SDL_Color color = {125, 125, 125, 125};
uint8_t alive;//0 неактивна, 1 активна необработана, 2 активна необработана
uint8_t energy;
};

struct alignas(32) neural_bio_cell{//8 цвет, 9 idx, 3 параметры жизни, 2 состояние, 4 brain, 6 мутации : 32 общ
//{nearby_cells_light[8][4],energy,nutrients,armor}{attack[4],go[8],transfer_energy[4],separate[4],chloroplast_add,armor+0.1}
alignas(4)const SDL_Color color;//aтака/фотосинтез/бег/броня постоянные
alignas(4)SDL_Color color2;//aтака/фотосинтез/бег/броня в зависимости от действий

uint8_t nearest_cells_idx[8];
uint8_t index;

uint8_t energy = 5;
uint8_t chloroplast = 0;
uint8_t nutrients = 0;

uint8_t alive = 0;//0 неактивна, 1 активна
uint8_t fatigue = 0;//(fatigue % 2) ? несходил : сходил


static constexpr uint8_t num_layers = 4;
alignas(32)static constexpr uint8_t layers[num_layers] = {38, 16, 16, 21}; // Локальный массив для инициализации brain
const_neural_mini8<char, float, float, float, num_layers, layers, 32>& brain;//заменить на индес в массиве uint32_t


uint16_t mutation_chance = 5;//Вероятность мутации
float mutation_strength = 0.001f;//Размер изменения

//Конструктор
neural_bio_cell()
    :
    brain(new const_neural_mini8<char, float, float, float, num_layers, layers, 32>),
    color({128, 128, 128, 255}){
        brain.random_weight();
}


//Конструктор
neural_bio_cell(uint16_t y = 0, uint16_t x = 0)
    :
    x(x),
    y(y),
    brain(new const_neural_object32<num_layers, layers>),
    color({128, 128, 128, 255}){
        brain.random_weight();
}

inline float* vision() const {
    return (float*)brain.neuron_data;
}
inline void vision(f_rgba* nearby_cells) const noexcept {
    std::memcpy((void*)brain.neuron_data, nearby_cells, 36 * sizeof(float));
}

inline void absorb(float*const nutrient) noexcept {
    if(nutrients < 5)
    nutrients += *nutrient*0.3f;
    if(energy < 1 && nutrients > 1){
    energy += 2;
    --nutrients;
    }
}

//Приннятие решений
uint16_t run() noexcept {
    brain.neuron_data[36] = energy;
    brain.neuron_data[37] = nutrients;
    brain.mind();
    return 0;
}

bool operator==(std::nullptr_t) const noexcept {
    return this == nullptr; // Проверяем, указывает ли this на nullptr
}

//копировать
void copy_with_mutations(neural_bio_cell& in) noexcept {//с изменениями
if (in.num_layers != num_layers || !std::equal(layers, layers + num_layers, in.layers)) {
        //обработка ошибки
        std::cerr << "copy_with_mutations Error: Inappropriate neural network configuration.";
        return ;
}
uint16_t i{0}
for(; i < quantity_weight_data(num_layers, layers)/16;){
    alignas(32)uint16_t rand[16];
    vector_rand(0, 65535-1, rand, 16);
    for(uint16_t n{0}; n < 16;++i, ++n)
    if(rand[n] < mutation_chance)
        in.brain.weight_data[i] = get_random<uint8_t>(0, 1) ? brain.weight_data[i]+mutation_strength : brain.weight_data[i]-mutation_strength ;
    else
        in.brain.weight_data[i] = brain.weight_data[i];
}
for(; i < (quantity_weight_data(num_layers, layers)%16)/8;){
    alignas(16)uint16_t rand[8];
    vector_rand(0, 65535-1, rand, 8);
    for(uint16_t n{0}; n < 8;++i, ++n)
    if(rand[n] < mutation_chance)
        in.brain.weight_data[i] = get_random<uint8_t>(0, 1) ? brain.weight_data[i]+mutation_strength : brain.weight_data[i]-mutation_strength ;
    else
        in.brain.weight_data[i] = brain.weight_data[i];
}
for(; i < (quantity_weight_data(num_layers, layers)%8)/4;){
    alignas(8)uint16_t rand[4];
    vector_rand(0, 65535-1, rand, 4);
    for(uint16_t n{0}; n < 4;++i, ++n)
    if(rand[n] < mutation_chance)
        in.brain.weight_data[i] = get_random<uint8_t>(0, 1) ? brain.weight_data[i]+mutation_strength : brain.weight_data[i]-mutation_strength ;
    else
        in.brain.weight_data[i] = brain.weight_data[i];
}
for(; i < (quantity_weight_data(num_layers, layers)%4;){
    if(get_random<uint16_t>(0, 65535-1) < mutation_chance)
        in.brain.weight_data[i] = get_random<uint8_t>(0, 1) ? brain.weight_data[i]+mutation_strength : brain.weight_data[i]-mutation_strength ;
    else
        in.brain.weight_data[i] = brain.weight_data[i];
}


if(get_random<uint16_t>(0, 65535-1) < mutation_chance){
    if(mutation_chance > 65 535 - 7)
        in.mutation_chance = mutation_chance-1 ;
    else if(mutation_chance < 5)
        in.mutation_chance = mutation_chance+1 ;
    else
        in.mutation_chance = get_random<uint8_t>(0, 1) ? mutation_chance+1 : mutation_chance-1 ;
}

}

void copy_without_modification(neural_bio_cell& in) noexcept {//без изменений
if (in.num_layers != num_layers || !std::equal(layers, layers + num_layers, in.layers)) {
        //обработка ошибки
        std::cerr << "copy_without_modification Error: Inappropriate neural network configuration.";
        return ;
}

for(uint16_t i{0}; i < quantity_weight_data(num_layers, layers);++i)
in.brain.weight_data[i] = brain.weight_data[i];

in.mutation_chance = mutation_chance;

}
};

