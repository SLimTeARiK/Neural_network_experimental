
template <typename... T>
static constexpr uint8_t allowed_type = ((
    std::is_arithmetic_v<T> ||
    std::is_same_v<T, std::float16_t> ||
    std::is_same_v<T, std::bfloat16_t>) &&
    (...));

uint32_t quantity_weight_data(uint8_t size_weight, const uint8_t num_layers, const uint8_t * const layer_sizes, uint8_t alignment){
    uint32_t quantity_weight = 0;
    for(uint32_t i = 1; i < num_layers; ++i){
        quantity_weight += ((layer_sizes[i-1]*size_weight)%alignment+layer_sizes[i-1]*size_weight)*layer_sizes[i];
    }
    return quantity_weight;
}



template <typename INPUT, typename INTERMEDIATE, typename OUTPUT, typename WEIGHT, const uint8_t num_layers, const uint8_t * const layer_sizes, uint8_t alignment>
struct const_neural_mini8 {
#if DEBUG
    static_assert(!allowed_type<INPUT, INTERMEDIATE, OUTPUT, WEIGHT>,
                    "Только целочисленные и floating-point типы допустимы");
    static_assert(!alignment,
                    "Только целочисленные и floating-point типы допустимы");
    static_assert(num_layers < 2,
                    "failed to create const_neural_object(id:" + id + "): layers < 2!");
    static_assert([](){for (uint32_t i = 0; i < num_layers; --i)
                    if (layer_sizes[i] == 0) return true;},
                    "failed to create const_neural_object(id:" + id + "): 0 neurons per layer!");
#endif

using neuron_data_size = std::integral_constant<uint8_t,
    *std::max_element(layer_sizes+1, layer_sizes+num_layers-2)*std::max(sizeof(INPUT), sizeof(INTERMEDIATE), sizeof(OUTPUT)) < alignment ?
    alignment : *std::max_element(layer_sizes+1, layer_sizes+num_layers-2)*std::max(sizeof(INPUT), sizeof(INTERMEDIATE), sizeof(OUTPUT))>;


alignas(alignment) uint8_t input_data[layer_sizes[0]*sizeof(INPUT)];
alignas(alignment) uint8_t output_data[layer_sizes[num_layers-1]*sizeof(OUTPUT)];

alignas(alignment) uint8_t weight_data[quantity_weight_data(sizeof(WEIGHT), num_layers, layer_sizes, alignment)];//ссылка на данные весов

const_neural_mini8(){}

void random_weight() {
    for (uint32_t i{0}; i < quantity_weight_data(num_layers, layer_sizes, ); ++i)
        weight_data[i] = get_random<float>(0.0f, 1.0f);
}

void calculate_dot_product() {
alignas(alignment) uint8_t neuron_data[neuron_data_size*2];//промежуточные данные нейронов


const WEIGHT* const weight = weight_data;

for(uint16_t layer_i = 1, weight_i = 0; layer_i < num_layers; ++layer_i){
    const uint16_t number_connection = layer_sizes[layer_i-1];
    const uint16_t number_connection_number_by_32 = (number_connection/32)*32;
    const uint16_t number_connection_number_by_16 = (number_connection/16)*16;
    const uint16_t number_connection_number_by_8 = (number_connection/8)*8;
    const uint16_t number_connection_number_by_4 = (number_connection/4)*4;
    const uint16_t number_of_residual_compounds = number_connection%4;


    uint16_t i = 0;

    const uint8_t neuron_data_flip = layer_i%2;
    const uint8_t offset_of_weight_data = layer_sizes[i-1]*sizeof(WEIGHT))%alignment;

    switch(layer_i){
    default:
        const INTERMEDIATE* const input = neuron_data[neuron_data_flip ? neuron_data_size : neuron_data_size*2];
        OUTPUT* const output = neuron_data[neuron_data_flip ? neuron_data_size*2 : neuron_data_size];
    break;
    case 1:
        const INPUT* const input = input_data;
        INTERMEDIATE* const output = neuron_data[neuron_data_flip ? neuron_data_size*2 : neuron_data_size];
    break;
    case num_layers-1:
        const INTERMEDIATE* const input = neuron_data[neuron_data_flip ? neuron_data_size : neuron_data_size*2];
        INTERMEDIATE* const output = output_data;
    break;
    }
    [[assume(input != output)]];


    for (; i < number_connection_number_by_32; ++i, ++weight_i) {
        output[i] += input[i] * weight[weight_i];
    }

    // AVX-512
    for (; i < number_connection_number_by_16; ++i, ++weight_i) {
        output[i] += input[i] * weight[weight_i];
    }

    // AVX/AVX2
    for (; i < number_connection_number_by_8; ++i, ++weight_i) {
        output[i] += input[i] * weight[weight_i];
    }

    // SSE
    for (; i < number_connection_number_by_4; ++i, ++weight_i) {
        output[i] += input[i] * weight[weight_i];
    }

    // Остаточные элементы
    for (; i < number_of_residual_compounds); ++i, ++weight_i) {
        output[i] += input[i] * weight[weight_i];
    }
    weight_i + offset_of_weight_data;

}

}

uint8_t mind(){




    float max_value = 0, max_value_index = 0;
    for(uint8_t i{0}; i < layer_sizes[num_layers-1]; ++i)
    if(max_value < neuron_data[layer_sizes[num_layers-2] + i]){
        max_value = neuron_data[layer_sizes[num_layers-2] + i];
        max_value_index = i;
    }
    return max_value_index;
}




//мой хуй словно сказания, ходит из уст в уста
//Техноблейд (Dream SMP)
//MineShield3
//«SYRP | NMX | NYANMIX | NYANMIXED» (нянмикс)
//тампоны для алены
//https://www.youtube.com/@kamyshnya
//https://music.yandex.ru/album/34672939/track/134554863
};
