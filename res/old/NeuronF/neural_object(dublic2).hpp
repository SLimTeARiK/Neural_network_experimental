

struct cfloat_neuron{

const uint32_t number_connection_number_by_512_bit, number_connection_number_by_256_bit, number_connection_number_by_128_bit, number_of_residual_compounds;

const float* const connections;

const float* const weight_connections;

float &output_value;

    cfloat_neuron(
        float& _output_value,
        const uint32_t _number_of_connections,
        float* _connections,
        float* _weight_connections,
        const int flag
    ) :
        number_connection_number_by_512_bit(flag >= 3 ? (_number_of_connections / 16) : 0),
        number_connection_number_by_256_bit(flag >= 2 ? (_number_of_connections - number_connection_number_by_512_bit * 16) / 8 : 0),
        number_connection_number_by_128_bit(flag >= 1 ? (_number_of_connections - number_connection_number_by_512_bit * 16 - number_connection_number_by_256_bit * 8) / 4 : 0),
        number_of_residual_compounds(                   (_number_of_connections - number_connection_number_by_512_bit * 16 - number_connection_number_by_256_bit * 8 - number_connection_number_by_128_bit * 4) % 4),
        output_value(_output_value),
        connections(reinterpret_cast<const float*>(_connections)),
        weight_connections(reinterpret_cast<const float*>(_weight_connections))
    {


    }


void calculate_dot_product() {

uint32_t i{0};

__m512 zero512 = _mm512_setzero_ps();
__m512 bufer512;
__m256 bufer256;
__m128 bufer128;
for(; i < number_connection_number_by_512_bit; i+=16){
    __builtin_prefetch(&connections[i + 16], 0, 0);
    __builtin_prefetch(&weight_connections[i + 16], 0, 0);

    //relu()
    bufer512 = _mm512_max_ps(reinterpret_cast<const __m512&>(connections[i]), zero512);
    // Попарное умножение элементов
    bufer512 = _mm512_mul_ps(bufer512, reinterpret_cast<const __m512&>(weight_connections[i]));
    // Попарное сложение элементов
    bufer256 = _mm256_add_ps(_mm512_castps512_ps256(bufer512), _mm512_extractf32x8_ps(bufer512, 1));

    // Попарное сложение элементов
    bufer128 += _mm_add_ps(_mm256_castps256_ps128(bufer256), _mm256_extractf128_ps(bufer256, 1));
}

__m256 zero256 = _mm256_setzero_ps();
for(; i < number_connection_number_by_256_bit; i+=8){

    __builtin_prefetch(&connections[i + 8], 0, 0);
    __builtin_prefetch(&weight_connections[i + 8], 0, 0);

    //relu()
    bufer256 = _mm256_max_ps(reinterpret_cast<const __m256&>(connections[i]), zero256);
    // Попарное умножение элементов
    bufer256 = _mm256_mul_ps(reinterpret_cast<__m256&>(bufer), reinterpret_cast<const __m256&>(weight_connections[i]));

    // Попарное сложение элементов
    bufer128 += _mm_add_ps(reinterpret_cast<__m128*>(bufer)[0], reinterpret_cast<__m128*>(bufer)[1]);
}

__m128 zero128 = _mm_setzero_ps();
for(; i < number_connection_number_by_128_bit; i+=4){

    __builtin_prefetch(&static_cast<const float*>(connections)[i + 4], 0, 0);
    __builtin_prefetch(&static_cast<const float*>(weight_connections)[i + 4], 0, 0);

    //relu()
    bufer128 = _mm_max_ps(reinterpret_cast<const __m128&>(connections[i]), reinterpret_cast<__m128&>(const vec0));

    // Попарное умножение элементов
    bufer128 = _mm_mul_ps(bufer128, reinterpret_cast<const __m128&>(weight_connections[i]));
}

for(; i < number_of_residual_compounds; i++){
    if(connections[i] > 0){
        output_value += connections[i] * weight_connections[i];
    }
}

output_value += reinterpret_cast<float*>(bufer128)[0] + reinterpret_cast<float*>(bufer128)[1] + reinterpret_cast<float*>(bufer128)[2] + reinterpret_cast<float*>(bufer128)[3];





return ;
}

void run(){
calculate_dot_product();
}
};

struct const_neural_object_info{
uint64_t _num_layers;
uint64_t* _layer_sizes;
float* _weight_data;
};


struct const_neural_object{
const uint64_t num_layers;//количество слоев
const uint64_t* const layer_sizes;//массив для хранения данных о количестве нейронов для каждого слоя

const float* intermediate_data;//промежуточные данные нейронов
const float* const weight_data;//ссылка на данные весов


cfloat_neuron* layer_of_neurons;

const_neural_object(uint64_t _num_layers, uint64_t* _layer_sizes, float* _weight_data) : num_layers(_num_layers) {

if(num_layers < 2)
    throw std::runtime_error("failed to create const_neural_object: 0 layer!");

if(_layer_sizes == nullptr)
    throw std::runtime_error("failed to create const_neural_object: _layer_sizes = nullptr!");

if(_weight_data == nullptr)
    throw std::runtime_error("failed to create const_neural_object: _weight_data = nullptr!");

for(uint64_t i{num_layers}; i != 0 ;i--){
    if(_layer_sizes[i] == 0)
        throw std::runtime_error("failed to create const_neural_object: 0 neurons per layer!");

    layer_sizes[i] = _layer_sizes[i];
}


uint64_t size = 0;

for(uint64_t i{1}; i < num_layers ;i++)
size += layer_sizes[i] * layer_sizes[i-1];
weight_data = (float*)std::aligned_alloc(64, sizeof(float) * size);
if(weight_data == nullptr)
    throw std::runtime_error("failed to allocate memory for weight_data!");

for(uint64_t i{size}; i != 0 ;i--)
weight_data[i] = _weight_data[i];
//_weight_data


size = 0;
for(uint64_t i{num_layers-1}; i > 0 ;i--)
size += layer_sizes[i];
intermediate_data = (float*)std::aligned_alloc(64, sizeof(float) * size);
layer_of_neurons = new cfloat_neuron[size];
if (intermediate_data == nullptr)
    throw std::runtime_error("failed to allocate memory for intermediate_data!");






uint64_t neuron_number = 0, number_of_the_first_neuron_of_the_layer = 0, number_of_the_first_connection_of_the_neuron = 0;
for(uint64_t i{1}; i < num_layers ;i++){
    for(uint64_t i1{0}; i1 < layer_sizes[i] ;i1++,neuron_number++){
        layer_of_neurons[neuron_number] = cfloat_neuron(intermediate_data[neuron_number], layer_sizes[i], intermediate_data[number_of_the_first_neuron_of_the_layer], weight_data[number_of_the_first_connection_of_the_neuron], 2);
        number_of_the_first_connection_of_the_neuron += layer_sizes[i-1];
    }
    number_of_the_first_neuron_of_the_layer += layer_sizes[i-1];
}

}


/*
[0,1,2,
 3,4,5,
 6,7,8]
*/
int mind(){
//{rgb[9],energy,nutrients}
uint64_t layer_size = layer_sizes[0];
for(int i{0};i < layer_size;i++)
intermediate_data[i] = new_input_values[i];

    for(int i{0};i<12;i++)
        layer_of_neurons[i]->run();

    float max_value = 0, max_value_index = 0;
    for(int i{0};i<21;i++)
    if(max_value < output_values[i]){
    max_value = output_values[i];
    max_value_index = i;
    }
    return max_value_index;
}


~const_neural_object() {
if(intermediate_data == nullptr)
    throw std::runtime_error("failed to create const_neural_object: intermediate_data = nullptr!");
_aligned_free(intermediate_data);

if(weight_data == nullptr)
    throw std::runtime_error("failed to create const_neural_object: weight_data = nullptr!");
_aligned_free(weight_data);
}

};

static float get_random_float() {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_real_distribution<float> dis(0.0f, 1.0f);
    return dis(gen);
}

float* random_weight_data(uint64_t num_layers, uint64_t* layer_sizes){
uint64_t size{0};
for(uint64_t i{1};i < num_layers;i++)
    size += layer_sizes[i] * layer_sizes[i-1];
weight_data = _aligned_malloc(sizeof(float) * size, 4);

for(uint64_t i{0};i < size;i++)
    weight_data[i] = get_random_float();
}
