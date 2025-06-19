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
    layer_sizes((uint64_t* const)ALIGNED_ALLOC(8, sizeof(uint64_t) * _num_layers)) {

#if DEBUG

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

#endif

std::copy(_layer_sizes, _layer_sizes + num_layers + 1, layer_sizes);

for (uint64_t i = num_layers; i > 0; --i) {
    if (layer_sizes[i] == 0)
        throw std::runtime_error("failed to create const_neural_object: 0 neurons per layer!");
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
