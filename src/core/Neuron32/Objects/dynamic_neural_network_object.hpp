#include "CPU/neuron.hpp"

alignas(64)struct info_neuron_object{
uint32_t index;//индекс
uint32_t num_layers;//количество слоев
uint32_t* layer_starts;//начальный индекс слоя
uint32_t* layer_sizes;//количенство нейронов в слое
alignas(64) float* neuron_data;//промежуточные данные нейронов
alignas(64) float* weight_data;//ссылка на данные весов
};

uint32_t size_allocation_of_aligned_memory(info_neuron_object& info){
uint32_t size = 0;
for(int i{0}; i < info.num_layers; ++i){
    info.layer_starts[i] = info.layer_sizes[i]/16 + info.layer_sizes[i]%16 != 0;
    size += info.layer_sizes[i]/16 + info.layer_sizes[i]%16 != 0;
}
return size;
}

struct neural_object32 {
info_neuron_object* info_neuron;

neural_object32(){





}

/*
[0,1,2,
 3,4,5,
 6,7,8]
*/

/*
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
*/
};
