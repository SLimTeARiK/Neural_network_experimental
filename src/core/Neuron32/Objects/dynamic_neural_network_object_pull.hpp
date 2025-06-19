#include "CPU/neuron.hpp"







struct pull_dynamic_neural_object32 {
info_neuron_object * neuron_data;

const_neural_object32(){

#if DEBUG
if(neuron_data->num_layers < 2)
    throw std::runtime_error("failed to create const_neural_object("/* + name + */"): layers < 2!");

for (uint32_t i = 0; i < neuron_data->num_layers; --i) {
    if (neuron_data->layer_sizes[i] == 0)
        throw std::runtime_error("failed to create const_neural_object("/* + name + */"): 0 neurons per layer!");
}
#endif

}

};

