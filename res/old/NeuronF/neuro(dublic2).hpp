
float neuron_calculate (
        const uint32_t _number_of_connections,
        const float* const connections,
        const float* const weight_connections
    ){
#ifdef __AVX512F__
    const uint64_t number_connection_number_by_512_bit =   (_number_of_connections / 16),
    number_connection_number_by_256_bit =   (_number_of_connections - number_connection_number_by_512_bit * 16) / 8),
    number_connection_number_by_128_bit =   (_number_of_connections - number_connection_number_by_512_bit * 16 - number_connection_number_by_256_bit * 8) / 4),
    number_of_residual_compounds =          (_number_of_connections - number_connection_number_by_512_bit * 16 - number_connection_number_by_256_bit * 8 - number_connection_number_by_128_bit * 4) % 4);

#else
    const uint32_t number_connection_number_by_256_bit = _number_of_connections / 8,
    number_connection_number_by_128_bit = (_number_of_connections - (number_connection_number_by_256_bit * 8)) / 4,
    number_of_residual_compounds =       (_number_of_connections - (number_connection_number_by_256_bit * 8) - (number_connection_number_by_128_bit * 4)) % 4;

#endif

uint32_t i{0};


#ifdef __AVX512F__
// Код с AVX-512
alignas(64) const __m512 zero512 = _mm512_setzero_ps();
alignas(64) __m512 bufer512;
#endif

alignas(32) const __m256 zero256 = _mm256_setzero_ps();
alignas(32) __m256 bufer256;
alignas(16) const __m128 zero128 = _mm_setzero_ps();
alignas(16) __m128 bufer128;
alignas(16) __m128 result = _mm_setzero_ps();


#ifdef __AVX512F__
// Код с AVX-512
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

for(; i < number_connection_number_by_128_bit; i+=4){

    __builtin_prefetch(&static_cast<const float*>(connections)[i + 4], 0, 0);
    __builtin_prefetch(&static_cast<const float*>(weight_connections)[i + 4], 0, 0);

    //relu()
    bufer128 = _mm_max_ps(reinterpret_cast<const __m128&>(connections[i]), zero128);

    // Попарное умножение элементов
    result += _mm_mul_ps(bufer128, reinterpret_cast<const __m128&>(weight_connections[i]));
}

float output_value = 0;
for(; i < number_of_residual_compounds; ++i){
    if(connections[i] > 0){
        output_value += connections[i] * weight_connections[i];
    }
}

float result_fvec[4];
_mm_store_ps(result_fvec, result);  // Сохраняем данные из result в массив
output_value += result_fvec[0] + result_fvec[1] + result_fvec[2] + result_fvec[3];


return output_value;
}


struct float_neuron{
#ifdef __AVX512F__
    uint32_t number_connection_number_by_512_bit, number_connection_number_by_256_bit, number_connection_number_by_128_bit, number_of_residual_compounds;    // Код с AVX-512
#else
    uint32_t number_connection_number_by_256_bit, number_connection_number_by_128_bit, number_of_residual_compounds;    // Код с AVX2 или SSE
#endif


float* connections;

float* weight_connections;

    cfloat_neuron(){}

    cfloat_neuron(
        uint32_t _number_of_connections,
        float* _connections,
        float* _weight_connections
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
            connections(reinterpret_cast<float*>(_connections)),
            weight_connections(reinterpret_cast<float*>(_weight_connections))

    {


    }


void calculate_dot_product() {
float output_value = 0;
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
output_value += result_f[0] + result_f[1] + result_f[2] + result_f[3];


return output_value;
}

void run(){
calculate_dot_product();
}
};

