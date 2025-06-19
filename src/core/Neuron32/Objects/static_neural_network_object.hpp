#include "../CPU/neuron.hpp"




// Типы функций активации
enum class activation_func : uint8_t {
    RELU,
    SIGMOID,
    TANH,
    SOFTMAX,
    LINEAR,
    SOFTPLUS,
    GAUSSIAN,
    STEP,
    //1 дополнительный параметр
    LEAKY_RELU,
    ELU,
    Swish,
    PReLU,
    GELU,
    SELU
};
// Конфигурация сети (память, точность)
struct network_config {
    const uint8_t activat_func[3]; //Функция активации
    uint8_t memory_optimization;  // Оптимизация памяти
    uint8_t use_fp16;             // Использовать половинную точность
};


template <typename INPUT, typename INTERMEDIATE, typename OUTPUT, typename WEIGHT, const uint32_t num_layers, const uint32_t layer_sizes[num_layers], network_config conf>
struct const_neural_object32 {
    static_assert(  std::is_same_v<INPUT, int8_t> ||
                    std::is_same_v<INPUT, int16_t> ||
                    std::is_same_v<INPUT, int32_t> ||
                    std::is_same_v<INPUT, int64_t> ||
                    std::is_same_v<INPUT, uint8_t> ||
                    std::is_same_v<INPUT, uint16_t> ||
                    std::is_same_v<INPUT, uint32_t> ||
                    std::is_same_v<INPUT, uint64_t> ||
                    std::is_same_v<INPUT, float> ||
                    std::is_same_v<INPUT, double,
                    "Только целочисленные и floating-point типы допустимы");

alignas(64) INPUT input_data[layer_sizes[0]];
alignas(64) INTERMEDIATE neuron_data[quantity_neurons(num_layers, layer_sizes)];//промежуточные данные нейронов
alignas(64) OUTPUT output_data[layer_sizes[num_layers-1]];
alignas(64) WEIGHT weight_data[quantity_weight_data(num_layers, layer_sizes)];//ссылка на данные весов

float_neuron neuron[quantity_neurons(num_layers, layer_sizes)];

const_neural_object32(){

#if DEBUG
if(num_layers < 2)
    throw std::runtime_error("failed to create const_neural_object("/* + name + */"): layers < 2!");

for (uint32_t i = 0; i < num_layers; --i) {
    if (layer_sizes[i] == 0)
        throw std::runtime_error("failed to create const_neural_object("/* + name + */"): 0 neurons per layer!");
}
#endif


uint32_t neuron_number = 0, number_of_the_first_neuron_of_the_layer = 0, number_of_the_first_connection_of_the_neuron = 0, i = 1;
for(; i < num_layers ;++i){
    for(uint32_t i1{0}; i1 < layer_sizes[i] ;++i1,++neuron_number){
        neuron[neuron_number] = float_neuron(
                                            layer_sizes[i],
                                            &neuron_data[number_of_the_first_neuron_of_the_layer],
                                            &weight_data[number_of_the_first_connection_of_the_neuron]
                                            );
        number_of_the_first_connection_of_the_neuron += layer_sizes[i-1];
    }
number_of_the_first_neuron_of_the_layer += layer_sizes[i-1];
}
}
void random_weight() {
    for (uint32_t i{0}; i < quantity_weight_data(num_layers, layer_sizes); ++i)
        weight_data[i] = get_random<float>(0.0f, 1.0f);
}

uint32_t mind(){

    for(int i{0}; i < quantity_neurons(num_layers, layer_sizes); ++i)
        neuron_data[i + layer_sizes[0]] = neuron[i].calculate_dot_product();


    float max_value = 0, max_value_index = 0;
    for(uint32_t i{0}; i < layer_sizes[num_layers-1]; ++i)
    if(max_value < neuron_data[layer_sizes[num_layers-2] + i]){
        max_value = neuron_data[layer_sizes[num_layers-2] + i];
        max_value_index = i;
    }
    return max_value_index;
}
};









