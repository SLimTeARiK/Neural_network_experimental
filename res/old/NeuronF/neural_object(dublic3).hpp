#if __has_include(<memory>) && defined(__cpp_lib_aligned_new)
    #include <memory> // для std::aligned_alloc
    #define HAS_STD_ALIGNED_MALLOC
#endif

#if defined(HAS_STD_ALIGNED_MALLOC)
    #define ALIGNED_ALLOC(size, alignment) std::aligned_alloc(alignment, size)
    #define ALIGNED_FREE(ptr) aligned_free(ptr)
#else
    #define ALIGNED_ALLOC(size, alignment) my_aligned_malloc(alignment, size)
    #define ALIGNED_FREE(ptr) my_aligned_free(ptr)


#include <cstdlib> // Для malloc и free
#include <stdexcept> // Для исключений

// Собственная реализация aligned_malloc
void* my_aligned_malloc(size_t size, size_t alignment) {
    // Проверка, что alignment является степенью двойки
    if (alignment == 0 || (alignment & (alignment - 1)) != 0) {
        throw std::invalid_argument("Alignment must be a power of two.");
    }

    // Выделяем память с дополнительным запасом для выравнивания
    size_t extra = alignment - 1 + sizeof(void*);
    void* original = std::malloc(size + extra);
    if (!original) {
        throw std::bad_alloc();
    }

    // Выравниваем указатель
    void* aligned = reinterpret_cast<void*>(
        (reinterpret_cast<uintptr_t>(original) + extra) & ~(alignment - 1)
    );

    // Сохраняем оригинальный указатель перед выровненным блоком
    reinterpret_cast<void**>(aligned)[-1] = original;

    return aligned;
}

// Собственная реализация aligned_free
void my_aligned_free(void* ptr) {
    if (ptr) {
        // Восстанавливаем оригинальный указатель
        void* original = reinterpret_cast<void**>(ptr)[-1];
        std::free(original);
    }
}
#endif

struct cfloat_neuron{
#ifdef __AVX512F__
    const uint32_t number_connection_number_by_512_bit, number_connection_number_by_256_bit, number_connection_number_by_128_bit, number_of_residual_compounds;    // Код с AVX-512
#else
    const uint32_t number_connection_number_by_256_bit, number_connection_number_by_128_bit, number_of_residual_compounds;    // Код с AVX2 или SSE
#endif


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
        #ifdef __AVX512F__
            number_connection_number_by_512_bit(_number_of_connections / 16),
            number_connection_number_by_256_bit((_number_of_connections - number_connection_number_by_512_bit * 16) / 8),
            number_connection_number_by_128_bit((_number_of_connections - number_connection_number_by_512_bit * 16 - number_connection_number_by_256_bit * 8) / 4),
            number_of_residual_compounds(       (_number_of_connections - number_connection_number_by_512_bit * 16 - number_connection_number_by_256_bit * 8 - number_connection_number_by_128_bit * 4) % 4),

        #else
            number_connection_number_by_256_bit(_number_of_connections / 8),
            number_connection_number_by_128_bit(_number_of_connections - (number_connection_number_by_256_bit * 8) / 4),
            number_of_residual_compounds(      (_number_of_connections - (number_connection_number_by_256_bit * 8) - number_connection_number_by_128_bit * 4) % 4),

        #endif
            output_value(_output_value),
            connections(reinterpret_cast<const float*>(_connections)),
            weight_connections(reinterpret_cast<const float*>(_weight_connections))

    {


    }


void calculate_dot_product() {

uint32_t i{0};


__m256 bufer256;
__m128 bufer128;
__m128 result = _mm_setzero_ps();
#ifdef __AVX512F__
// Код с AVX-512
const __m512 zero512 = _mm512_setzero_ps();
__m512 bufer512;
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
    result += _mm_add_ps(_mm256_castps256_ps128(bufer256), _mm256_extractf128_ps(bufer256, 1));
}
#endif


const __m256 zero256 = _mm256_setzero_ps();
for(; i < number_connection_number_by_256_bit; i+=8){

    __builtin_prefetch(&connections[i + 8], 0, 0);
    __builtin_prefetch(&weight_connections[i + 8], 0, 0);

    //relu()
    bufer256 = _mm256_max_ps(reinterpret_cast<const __m256&>(connections[i]), zero256);
    // Попарное умножение элементов
    bufer256 = _mm256_mul_ps(bufer256, reinterpret_cast<const __m256&>(weight_connections[i]));

    // Попарное сложение элементов
    result += _mm_add_ps(_mm256_castps256_ps128(bufer256), _mm256_extractf128_ps(bufer256, 1));
}

const __m128 zero128 = _mm_setzero_ps();
for(; i < number_connection_number_by_128_bit; i+=4){

    __builtin_prefetch(&static_cast<const float*>(connections)[i + 4], 0, 0);
    __builtin_prefetch(&static_cast<const float*>(weight_connections)[i + 4], 0, 0);

    //relu()
    bufer128 = _mm_max_ps(reinterpret_cast<const __m128&>(connections[i]), zero128);

    // Попарное умножение элементов
    result += _mm_mul_ps(bufer128, reinterpret_cast<const __m128&>(weight_connections[i]));
}

for(; i < number_of_residual_compounds; ++i){
    if(connections[i] > 0){
        output_value += connections[i] * weight_connections[i];
    }
}

float result_f[4];
_mm_store_ps(result_f, result);  // Сохраняем данные из result в массив
output_value += result[0] + result_f[1] + result_f[2] + result_f[3];





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
constexpr uint64_t quantity_weight_data(const uint64_t num_layers, const uint64_t* const layer_sizes){
uint64_t size = 0;
for(uint64_t i{1}; i < num_layers; ++i)
    size += layer_sizes[i] * layer_sizes[i-1];
return size;
}
constexpr uint64_t quantity_neurons(const uint64_t num_layers, const uint64_t* const layer_sizes){
uint64_t size = 0;
for (uint64_t i{0}; i < num_layers; ++i)
    size += layer_sizes[i];
return size;
}

struct const_neural_object{
const uint64_t num_layers;//количество слоев
uint64_t* const layer_sizes;//массив для хранения данных о количестве нейронов для каждого слоя

float* const intermediate_data;//промежуточные данные нейронов
float* const weight_data;//ссылка на данные весов


cfloat_neuron* layer_of_neurons;

const_neural_object(const uint64_t _num_layers, const uint64_t* const _layer_sizes, float* _weight_data) :
    num_layers(_num_layers),
    weight_data((float*)ALIGNED_ALLOC(32, sizeof(float) * quantity_weight_data(_num_layers, _layer_sizes))),
    intermediate_data((float*)ALIGNED_ALLOC(32, sizeof(float) * quantity_neurons(_num_layers, _layer_sizes))),
    layer_sizes((const uint64_t* const)ALIGNED_ALLOC(8, sizeof(uint64_t) * _num_layers)) {

if(weight_data == nullptr)
    throw std::runtime_error("failed to create const_neural_object: weight_data = nullptr!");

if(intermediate_data == nullptr)
    throw std::runtime_error("failed to create const_neural_object: intermediate_data = nullptr!");

if(layer_sizes == nullptr)
    throw std::runtime_error("failed to create const_neural_object: layer_sizes = nullptr!");

if(_layer_sizes == nullptr)
    throw std::runtime_error("failed to create const_neural_object: _layer_sizes = nullptr!");

if(_weight_data == nullptr)
    throw std::runtime_error("failed to create const_neural_object: _weight_data = nullptr!");

if(num_layers < 2)
    throw std::runtime_error("failed to create const_neural_object: layers < 2!");

std::copy(_layer_sizes, _layer_sizes + num_layers + 1, layer_sizes);

for (uint64_t i = num_layers; i > 0; --i) {
    if (layer_sizes[i] == 0)
        throw std::runtime_error("failed to create const_neural_object: 0 neurons per layer!");
}


uint64_t size = quantity_neurons(_num_layers, _layer_sizes);

if(weight_data == nullptr)
    throw std::runtime_error("failed to allocate memory for weight_data!");

layer_of_neurons = new cfloat_neuron[size];
if (intermediate_data == nullptr)
    throw std::runtime_error("failed to allocate memory for intermediate_data!");


uint64_t neuron_number = 0, number_of_the_first_neuron_of_the_layer = 0, number_of_the_first_connection_of_the_neuron = 0;
for(uint64_t i{1}; i < num_layers ;++i){
    for(uint64_t i1{0}; i1 < layer_sizes[i] ;++i1,++neuron_number){
        layer_of_neurons[neuron_number] = cfloat_neuron(intermediate_data[neuron_number], layer_sizes[i], intermediate_data[number_of_the_first_neuron_of_the_layer], weight_data[number_of_the_first_connection_of_the_neuron]);
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
for(int i{0};i < layer_size;++i)
intermediate_data[i] = new_input_values[i];

    for(int i{0};i<12;++i)
        layer_of_neurons[i]->run();

    float max_value = 0, max_value_index = 0;
    for(int i{0};i<21;++i)
    if(max_value < output_values[i]){
        max_value = output_values[i];
        max_value_index = i;
    }
    return max_value_index;
}
/*
115
65
69
*/
~const_neural_object() {
if(intermediate_data == nullptr)
    throw std::runtime_error("failed to create const_neural_object: intermediate_data = nullptr!");
ALIGNED_FREE(intermediate_data);

if(weight_data == nullptr)
    throw std::runtime_error("failed to create const_neural_object: weight_data = nullptr!");
ALIGNED_FREE(weight_data);

if(layer_sizes == nullptr)
    throw std::runtime_error("failed to create const_neural_object: weight_data = nullptr!");
ALIGNED_FREE(layer_sizes);
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
for(uint64_t i{1};i < num_layers;++i)
    size += layer_sizes[i] * layer_sizes[i-1];
weight_data = _aligned_malloc(sizeof(float) * size, 4);

for(uint64_t i{0};i < size;++i)
    weight_data[i] = get_random_float();
}
