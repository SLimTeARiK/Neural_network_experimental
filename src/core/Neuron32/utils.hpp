
constexpr uint8_t is_aligned(const uintptr_t ptr, const uint8_t alignment) noexcept {
    return (ptr % alignment == 0);
}
uint8_t is_aligned(void* ptr, uint8_t alignment) noexcept {
    return (reinterpret_cast<uintptr_t>(ptr) % alignment == 0);
}


constexpr uint32_t quantity_weight_data(const uint32_t num_layers, const uint32_t* const layer_sizes) noexcept {
#if DEBUG

if(layer_sizes == nullptr)
    std::runtime_error("failed to argument quantity_weight_data: _layer_sizes = nullptr!");

if(num_layers < 2)
    std::runtime_error("failed to argument quantity_weight_data: layers < 2!");

for (uint32_t i = 0; i < num_layers; ++i) {
    if (layer_sizes[i] == 0)
        std::invalid_argument("failed to argument quantity_weight_data: layer[" + std::to_string(i) + "] = 0!");
}
#endif
uint32_t size = 0;
for(uint32_t i{1}; i < num_layers; ++i)
    size += layer_sizes[i] * layer_sizes[i-1];
return size;
}
constexpr uint32_t quantity_neurons(const uint32_t num_layers, const uint32_t* const layer_sizes) noexcept {
#if DEBUG

if(layer_sizes == nullptr)
    std::runtime_error("failed to argument quantity_neurons: _layer_sizes = nullptr!");

if(num_layers < 2)
    std::runtime_error("failed to argument quantity_neurons: layers < 2!");

for (uint32_t i = 0; i < num_layers; ++i) {
    if (layer_sizes[i] == 0)
        std::invalid_argument("failed to argument quantity_neurons: layer[" + std::to_string(i) + "] = 0!");
}
#endif
uint32_t size = 0;
for (uint32_t i{0}; i < num_layers; ++i)
    size += layer_sizes[i];
return size;
}


/*
#define random_weight_data(num_layers, layer_sizes) _random_weight_data(num_layers, layer_sizes, __FILE__, __LINE__)

float* _random_weight_data(const uint32_t num_layers, const uint32_t* const layer_sizes, const char* file, const int line) {
#if DEBUG
    for (uint32_t i = 0; i < num_layers; ++i) {
        if (layer_sizes[i] == 0)
            throw std::invalid_argument("[file: " + std::string(file) + " line: " + std::to_string(line) + "] random_weight_data: layer_sizes[" + std::to_string(i) + "] = 0");
    }
    if (quantity_weight_data(num_layers, layer_sizes) <= 0)
        throw std::invalid_argument("quantity_weight_data = 0");
#endif

    float* weight_data;
    uint32_t size{0};
    weight_data = (float*)aligned::allocate(sizeof(float) * quantity_weight_data(num_layers, layer_sizes));

    for (uint32_t i{0}; i < quantity_weight_data(num_layers, layer_sizes); ++i)
        weight_data[i] = get_random<float>(0.0f, 1.0f);

    return weight_data;
}
*/
