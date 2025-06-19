template <typename... T>
static constexpr uint8_t allowed_type = ((
    std::is_arithmetic_v<T> ||
    std::is_same_v<T, std::float16_t> ||
    std::is_same_v<T, std::float16_t>) &&
    (...));

template<typename T1, typename T2>
struct is_same_base_type_impl {
    static const bool value =
        std::is_same<
            typename std::remove_cv<typename std::remove_reference<T1>::type>::type,
            typename std::remove_cv<typename std::remove_reference<T2>::type>::type
        >::value;
};

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
alignas(alignment) uint8_t weight_data[quantity_weight_data(sizeof(WEIGHT), num_layers, layer_sizes, alignment)];

const_neural_mini8(){}

void random_weight() {
    for (uint32_t i{0}; i < quantity_weight_data(sizeof(WEIGHT), num_layers, layer_sizes, alignment); ++i)
        weight_data[i] = get_random<float>(0.0f, 1.0f);
}

// Функция активации ReLU с автоматическим преобразованием типов
template <typename T>
void apply_activation(T* data, uint8_t size) {
    for (uint8_t i = 0; i < size; ++i) {
        data[i] = data[i] > (T)0 ? data[i] : (T)0;
    }
}

void calculate_dot_product() {
    alignas(alignment) uint8_t neuron_data[neuron_data_size*2];

    // Указатели на веса и текущий слой
    const WEIGHT* weight_ptr = (const WEIGHT*)weight_data;
    INTERMEDIATE* current_layer = (INTERMEDIATE*)neuron_data;
    INTERMEDIATE* next_layer = (INTERMEDIATE*)(neuron_data + neuron_data_size);

    // Обработка первого слоя (входной слой -> скрытый слой 1)
    {
        const INPUT* input_ptr = (const INPUT*)input_data;
        const uint8_t input_size = layer_sizes[0];
        const uint8_t output_size = layer_sizes[1];

        // Очистка выходного слоя
        for (uint8_t i = 0; i < output_size; ++i) {
            next_layer[i] = (INTERMEDIATE)0;
        }

        // Векторизованное умножение
        using simd_input = std::experimental::fixed_size_simd<INPUT, 16>;
        using simd_weight = std::experimental::fixed_size_simd<WEIGHT, 16>;
        using simd_intermediate = std::experimental::fixed_size_simd<INTERMEDIATE, 16>;

        for (uint8_t neuron = 0; neuron < output_size; ++neuron) {
            INTERMEDIATE sum = (INTERMEDIATE)0;
            uint8_t i = 0;

            // Обработка по 16 элементов
            for (; i + 16 <= input_size; i += 16) {
                simd_input input_vec;
                simd_weight weight_vec;

                // Загрузка данных
                for (uint8_t j = 0; j < 16; ++j) {
                    input_vec[j] = input_ptr[i + j];
                    weight_vec[j] = weight_ptr[neuron * input_size + i + j];
                }

                // Преобразование и умножение
                simd_intermediate converted_input = (simd_intermediate)input_vec;
                simd_intermediate converted_weight = (simd_intermediate)weight_vec;
                sum += (INTERMEDIATE)std::experimental::reduce(converted_input * converted_weight);
            }

            // Остаточные элементы
            for (; i < input_size; ++i) {
                sum += (INTERMEDIATE)(input_ptr[i] * weight_ptr[neuron * input_size + i]);
            }

            next_layer[neuron] = sum;
        }

        // Применение функции активации
        apply_activation(next_layer, output_size);

        // Обновление указателей для следующего слоя
        std::swap(current_layer, next_layer);
        weight_ptr += input_size * output_size;
    }

    // Обработка скрытых слоев
    for (uint8_t layer = 1; layer < num_layers - 1; ++layer) {
        const uint8_t input_size = layer_sizes[layer];
        const uint8_t output_size = layer_sizes[layer + 1];

        // Очистка выходного слоя
        for (uint8_t i = 0; i < output_size; ++i) {
            next_layer[i] = (INTERMEDIATE)0;
        }

        // Векторизованное умножение
        using simd_intermediate = std::experimental::fixed_size_simd<INTERMEDIATE, 16>;
        using simd_weight = std::experimental::fixed_size_simd<WEIGHT, 16>;

        for (uint8_t neuron = 0; neuron < output_size; ++neuron) {
            INTERMEDIATE sum = (INTERMEDIATE)0;
            uint8_t i = 0;

            // Обработка по 16 элементов
            for (; i + 16 <= input_size; i += 16) {
                simd_intermediate input_vec;
                simd_weight weight_vec;

                // Загрузка данных
                for (uint8_t j = 0; j < 16; ++j) {
                    input_vec[j] = current_layer[i + j];
                    weight_vec[j] = weight_ptr[neuron * input_size + i + j];
                }

                // Умножение и суммирование
                sum += (INTERMEDIATE)std::experimental::reduce(input_vec * (simd_intermediate)weight_vec);
            }

            // Остаточные элементы
            for (; i < input_size; ++i) {
                sum += (INTERMEDIATE)(current_layer[i] * weight_ptr[neuron * input_size + i]);
            }

            next_layer[neuron] = sum;
        }

        // Применение функции активации
        apply_activation(next_layer, output_size);

        // Обновление указателей для следующего слоя
        std::swap(current_layer, next_layer);
        weight_ptr += input_size * output_size;
    }

    // Копирование результата в выходной буфер
    OUTPUT* output_ptr = (OUTPUT*)output_data;
    for (uint8_t i = 0; i < layer_sizes[num_layers - 1]; ++i) {
        output_ptr[i] = (OUTPUT)current_layer[i];
    }
}

uint8_t mind(){
    float max_value = 0;
    uint8_t max_value_index = 0;
    const OUTPUT* output_ptr = (const OUTPUT*)output_data;

    for(uint8_t i{0}; i < layer_sizes[num_layers-1]; ++i) {
        if(max_value < output_ptr[i]){
            max_value = output_ptr[i];
            max_value_index = i;
        }
    }
    return max_value_index;
}
};
