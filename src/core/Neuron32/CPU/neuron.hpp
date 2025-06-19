#pragma once



struct alignas(32) float_neuron {
    uint32_t number_connection_number_by_16;
    uint32_t number_connection_number_by_8;
    uint32_t number_connection_number_by_4;
    uint32_t number_of_residual_compounds;

    float* __restrict__ connections;
    float* __restrict__ weight_connections;

    float_neuron() {}

    float_neuron(
        uint32_t _number_of_connections,
        float* __restrict__ _connections,
        float* __restrict__ _weight_connections
    ) :
        number_connection_number_by_16(_number_of_connections / 16),
        number_connection_number_by_8((_number_of_connections % 16) / 8),
        number_connection_number_by_4((_number_of_connections % 8) / 4),
        number_of_residual_compounds(_number_of_connections % 4),
        connections(_connections),
        weight_connections(_weight_connections)
    {}

    float calculate_dot_product() {
        float output_value = 0;
        uint32_t i = 0;

        // Блоки по 16 элементов (может векторизоваться в AVX-512)
        //#pragma GCC unroll(4)
        for (; i < number_connection_number_by_16 * 16; i += 16) {
            // Явно развернутый цикл для лучшей векторизации
            output_value += connections[i] * weight_connections[i];
            output_value += connections[i+1] * weight_connections[i+1];
            output_value += connections[i+2] * weight_connections[i+2];
            output_value += connections[i+3] * weight_connections[i+3];
            output_value += connections[i+4] * weight_connections[i+4];
            output_value += connections[i+5] * weight_connections[i+5];
            output_value += connections[i+6] * weight_connections[i+6];
            output_value += connections[i+7] * weight_connections[i+7];
            output_value += connections[i+8] * weight_connections[i+8];
            output_value += connections[i+9] * weight_connections[i+9];
            output_value += connections[i+10] * weight_connections[i+10];
            output_value += connections[i+11] * weight_connections[i+11];
            output_value += connections[i+12] * weight_connections[i+12];
            output_value += connections[i+13] * weight_connections[i+13];
            output_value += connections[i+14] * weight_connections[i+14];
            output_value += connections[i+15] * weight_connections[i+15];
        }

        // Блоки по 8 элементов (может векторизоваться в AVX/AVX2)
        //#pragma GCC unroll(4)
        for (; i < (number_connection_number_by_16 * 16 + number_connection_number_by_8 * 8); i += 8) {
            output_value += connections[i] * weight_connections[i];
            output_value += connections[i+1] * weight_connections[i+1];
            output_value += connections[i+2] * weight_connections[i+2];
            output_value += connections[i+3] * weight_connections[i+3];
            output_value += connections[i+4] * weight_connections[i+4];
            output_value += connections[i+5] * weight_connections[i+5];
            output_value += connections[i+6] * weight_connections[i+6];
            output_value += connections[i+7] * weight_connections[i+7];
        }

        // Блоки по 4 элемента (может векторизоваться в SSE)
        //#pragma GCC unroll(4)
        for (; i < (number_connection_number_by_16 * 16 + number_connection_number_by_8 * 8 + number_connection_number_by_4 * 4); i += 4) {
            output_value += connections[i] * weight_connections[i];
            output_value += connections[i+1] * weight_connections[i+1];
            output_value += connections[i+2] * weight_connections[i+2];
            output_value += connections[i+3] * weight_connections[i+3];
        }

        // Остаточные элементы
        for (; i < (number_connection_number_by_16 * 16 + number_connection_number_by_8 * 8 +
                   number_connection_number_by_4 * 4 + number_of_residual_compounds); ++i) {
            if (connections[i] > 0) {
                output_value += connections[i] * weight_connections[i];
            }
        }

        return output_value;
    }

    inline void run() {
        calculate_dot_product();
    }
};


struct float_neuron_manual_vectorization{
#ifdef __AVX512F__
    uint32_t number_connection_number_by_512_bit, number_connection_number_by_256_bit, number_connection_number_by_128_bit, number_of_residual_compounds;    // Код с AVX-512
#elif defined(__AVX2__) || defined(__AVX__)
    uint32_t number_connection_number_by_256_bit, number_connection_number_by_128_bit, number_of_residual_compounds;    // Код с AVX2 или SSE
#elif defined(__SSE4_1__) || defined(__SSE3__)
    uint32_t number_connection_number_by_128_bit, number_of_residual_compounds;
#endif


float* __restrict__  connections;

float* __restrict__  weight_connections;

    float_neuron_manual_vectorization(){}

    float_neuron_manual_vectorization(
        uint32_t _number_of_connections,
        float* __restrict__  _connections,
        float* __restrict__  _weight_connections
    ) :
        #ifdef __AVX512F__
            number_connection_number_by_512_bit(_number_of_connections / 16),
            number_connection_number_by_256_bit((_number_of_connections % 16) / 8),
            number_connection_number_by_128_bit((_number_of_connections % 8) / 4),
            number_of_residual_compounds(       (_number_of_connections % 4)),
        #elif defined(__AVX2__) || defined(__AVX__)
            number_connection_number_by_256_bit(_number_of_connections / 8),
            number_connection_number_by_128_bit((_number_of_connections % 8) / 4),
            number_of_residual_compounds(      (_number_of_connections % 4)),
        #elif defined(__SSE4_1__) || defined(__SSE3__)
            number_connection_number_by_128_bit(_number_of_connections / 4),
            number_of_residual_compounds(      (_number_of_connections % 4)),
        #else
            number_of_residual_compounds(       _number_of_connections ),
        #endif
            connections(_connections),
            weight_connections(_weight_connections)

    {


    }


float calculate_dot_product() {
float output_value = 0;
uint32_t i{0};

#if defined(__SSE4_1__) || defined(__SSE3__)
__m128 bufer128;

#if defined(__AVX2__) || defined(__AVX__)
__m256 bufer256;

#ifdef __AVX512F__
// Код с AVX-512
const __m512 zero512 = _mm512_setzero_ps();
__m512 bufer512 = zero512;

for(; i < number_connection_number_by_512_bit; i+=16){
    _mm_prefetch(&connections[i + 16], _MM_HINT_T1);
    _mm_prefetch(&weight_connections[i + 16], _MM_HINT_T1);

    if (_mm512_testz_ps(_mm512_load_ps(&connections[i]), zero512) == 1)
        continue;
#ifdef __FMA__
    // Попарное умножение элементов
    bufer512 = _mm512_fmadd_ps(_mm512_load_ps(&connections[i]), _mm512_load_ps(&weight_connections[i]), bufer512);
#else
    bufer512 += _mm512_mul_ps(_mm512_load_ps(&connections[i]), _mm512_load_ps(&weight_connections[i]));
#endif

}
// Попарное сложение элементов
bufer256 = _mm256_add_ps(_mm512_castps512_ps256(bufer512), _mm512_extractf32x8_ps(bufer512, 1));
#else

bufer256 = _mm256_setzero_ps();

#endif




const __m256 zero256 = _mm256_setzero_ps();
for(; i < number_connection_number_by_256_bit; i+=8){

    _mm_prefetch(&connections[i + 8], _MM_HINT_T1);
    _mm_prefetch(&weight_connections[i + 8], _MM_HINT_T1);

    if (_mm256_testz_ps(_mm256_load_ps(&connections[i]), zero256) == 1)
        continue;

#ifdef __FMA__
    // Попарное умножение элементов
    bufer256 = _mm256_fmadd_ps(_mm256_load_ps(&connections[i]), _mm256_load_ps(&weight_connections[i]), bufer256);
#else
    bufer256 += _mm256_mul_ps(_mm256_load_ps(&connections[i]), _mm256_load_ps(&weight_connections[i]));
#endif

}
bufer128 = _mm_add_ps(_mm256_castps256_ps128(bufer256), _mm256_extractf128_ps(bufer256, 1));
#else
bufer128 = _mm_setzero_ps()
#endif

const __m128 zero128 = _mm_setzero_ps();
for(; i < number_connection_number_by_128_bit; i+=4){

    _mm_prefetch(&connections[i], _MM_HINT_T1);
    _mm_prefetch(&weight_connections[i], _MM_HINT_T1);

    if (_mm_testz_ps(_mm_load_ps(&connections[i]), zero128) == 1)
        continue;

#ifdef __FMA__
    // Попарное умножение элементов
    bufer128 = _mm_fmadd_ps(_mm_load_ps(&connections[i]), _mm1_load_ps(&weight_connections[i]), bufer128);
#else
    bufer128 += _mm_mul_ps(_mm_load_ps(&connections[i]), _mm_load_ps(&weight_connections[i]));
#endif
}

#else
//std::cout << "Vectorization is not supported!\n";
#endif

for(; i < number_of_residual_compounds; ++i){
    if(connections[i] > 0){
        output_value += connections[i] * weight_connections[i];
    }
}

float result_f[4];
_mm_store_ps(result_f, bufer128);  // Сохраняем данные из bufer128 в массив
output_value += result_f[0] + result_f[1] + result_f[2] + result_f[3];


return output_value;
}

void run(){
calculate_dot_product();
}
};

/*
class cfloat_neuron{
#ifdef __AVX512F__
    const uint32_t number_connection_number_by_512_bit, number_connection_number_by_256_bit, number_connection_number_by_128_bit, number_of_residual_compounds;    // Код с AVX-512
#elif defined(__AVX2__) || defined(__AVX__)
    const uint32_t number_connection_number_by_256_bit, number_connection_number_by_128_bit, number_of_residual_compounds;    // Код с AVX2 или SSE
#elif defined(__SSE4_1__) || defined(__SSE3__)
    const uint32_t number_connection_number_by_128_bit, number_of_residual_compounds;
#endif


float* const connections;

float* const weight_connections;

    float_neuron(
        uint64_t _number_of_connections,
        float* _connections,
        float* _weight_connections
    ) :
        #ifdef __AVX512F__
            number_connection_number_by_512_bit(_number_of_connections / 16),
            number_connection_number_by_256_bit((_number_of_connections % 16) / 8),
            number_connection_number_by_128_bit((_number_of_connections % 8) / 4),
            number_of_residual_compounds(       (_number_of_connections % 4)),
        #elif defined(__AVX2__) || defined(__AVX__)
            number_connection_number_by_256_bit(_number_of_connections / 8),
            number_connection_number_by_128_bit((_number_of_connections % 8) / 4),
            number_of_residual_compounds(      (_number_of_connections % 4)),
        #elif defined(__SSE4_1__) || defined(__SSE3__)
            number_connection_number_by_128_bit(_number_of_connections / 4),
            number_of_residual_compounds(      (_number_of_connections % 4)),
        #else
            number_of_residual_compounds(       _number_of_connections ),
        #endif
            connections(_connections),
            weight_connections(_weight_connections)

    {


    }


float calculate_dot_product() {
float output_value = 0;
uint32_t i{0};

#if HAVE_NUMA//сделать позже
// Код для NUMA-систем
#else
// Код для SMP
#endif
#if defined(__SSE4_1__) || defined(__SSE3__)
__m128 bufer128;

#if defined(__AVX2__) || defined(__AVX__)
__m256 bufer256;

#ifdef __AVX512F__
// Код с AVX-512
const __m512 zero512 = _mm512_setzero_ps();
__m512 bufer512 = zero512;

for(; i < number_connection_number_by_512_bit; i+=16){
    _mm_prefetch(&connections[i + 16], _MM_HINT_T1);
    _mm_prefetch(&weight_connections[i + 16], _MM_HINT_T1);

    if (_mm512_testz_ps(_mm512_load_ps(connections[i]), _mm512_load_ps(connections[i])))
        continue;
#ifdef __FMA__
    // Попарное умножение элементов
    bufer512 = _mm512_fmadd_ps(_mm512_load_ps(connections[i]), _mm512_load_ps(weight_connections[i]), bufer512);
#else
    bufer512 += _mm512_mul_ps(_mm512_load_ps(connections[i]), _mm512_load_ps(weight_connections[i]));
#endif

}
// Попарное сложение элементов
bufer256 = _mm256_add_ps(_mm512_castps512_ps256(bufer512), _mm512_extractf32x8_ps(bufer512, 1));
#else

bufer256 = _mm256_setzero_ps();

#endif




const __m256 zero256 = _mm256_setzero_ps();
for(; i < number_connection_number_by_256_bit; i+=8){

    _mm_prefetch(&connections[i + 8], _MM_HINT_T1);
    _mm_prefetch(&weight_connections[i + 8], _MM_HINT_T1);

    if (_mm256_testz_ps(_mm256_load_ps(weight_connections[i]), zero256, _CMP_EQ_OQ) == 0xFFFF)
        continue;

#ifdef __FMA__
    // Попарное умножение элементов
    bufer256 = _mm256_fmadd_ps(_mm256_load_ps(weight_connections[i]), _mm256_load_ps(weight_connections[i]), bufer256);
#else
    bufer256 += _mm256_mul_ps(_mm256_load_ps(weight_connections[i]), _mm256_load_ps(weight_connections[i]));
#endif

}
bufer128 = _mm128_add_ps(_mm256_castps256_ps128(bufer256), _mm256_extractf32x8_ps(bufer256, 1));
#else
bufer128 = _mm_setzero_ps()
#endif

const __m128 zero128 = _mm_setzero_ps();
for(; i < number_connection_number_by_128_bit; i+=4){

    _mm_prefetch(&connections[i], _MM_HINT_T1);
    _mm_prefetch(&weight_connections[i], _MM_HINT_T1);

    if (_mm_testz_ps(_mm128_load_ps(weight_connections[i]), zero128, _CMP_EQ_OQ) == 0xFFFF)
        continue;

#ifdef __FMA__
    // Попарное умножение элементов
    bufer128 = _mm_fmadd_ps(_mm_load_ps(weight_connections[i]), _mm1_load_ps(weight_connections[i]), bufer128);
#else
    bufer128 += _mm_mul_ps(_mm_load_ps(weight_connections[i]), _mm_load_ps(weight_connections[i]));
#endif
}

#else
//std::cout << "Vectorization is not supported!\n";
#endif

for(; i < number_of_residual_compounds; ++i){
    if(connections[i] > 0){
        output_value += connections[i] * weight_connections[i];
    }
}

float result_f[4];
_mm_store_ps(result_f, bufer128);  // Сохраняем данные из bufer128 в массив
output_value += result_f[0] + result_f[1] + result_f[2] + result_f[3];


return output_value;
}

void run(){
float output = calculate_dot_product();
output = output < 0 0 : output;
}
};


struct float_neuron{
#ifdef __AVX512F__
    uint32_t number_connection_number_by_512_bit, number_connection_number_by_256_bit, number_connection_number_by_128_bit, number_of_residual_compounds;    // Код с AVX-512
#elif defined(__AVX2__) || defined(__AVX__)
    uint32_t number_connection_number_by_256_bit, number_connection_number_by_128_bit, number_of_residual_compounds;    // Код с AVX2 или SSE
#elif defined(__SSE4_1__) || defined(__SSE3__)
    uint32_t number_connection_number_by_128_bit, number_of_residual_compounds;
#endif


aligned::const_align64_memory<float> connections;

aligned::const_align64_memory<float> weight_connections;

    float_neuron(){}

    float_neuron(
        uint64_t _number_of_connections,
        float* _connections,
        float* _weight_connections
    ) :
        #ifdef __AVX512F__
            number_connection_number_by_512_bit(_number_of_connections / 16),
            number_connection_number_by_256_bit((_number_of_connections % 16) / 8),
            number_connection_number_by_128_bit((_number_of_connections % 8) / 4),
            number_of_residual_compounds(       (_number_of_connections % 4)),
        #elif defined(__AVX2__) || defined(__AVX__)
            number_connection_number_by_256_bit(_number_of_connections / 8),
            number_connection_number_by_128_bit((_number_of_connections % 8) / 4),
            number_of_residual_compounds(      (_number_of_connections % 4)),
        #elif defined(__SSE4_1__) || defined(__SSE3__)
            number_connection_number_by_128_bit(_number_of_connections / 4),
            number_of_residual_compounds(      (_number_of_connections % 4)),
        #else
            number_of_residual_compounds(       _number_of_connections ),
        #endif
            connections(const_align64_memory<float>(_connections, _number_of_connections, aligned::FLAG::VIEW)),
            weight_connections(const_align64_memory<float>(_weight_connections, _number_of_connections, aligned::FLAG::VIEW))

    {


    }


float calculate_dot_product() {
float output_value = 0;
uint32_t i{0};

#if HAVE_NUMA//сделать позже
// Код для NUMA-систем
#else
// Код для SMP
#endif
#if defined(__SSE4_1__) || defined(__SSE3__)
__m128 bufer128;

#if defined(__AVX2__) || defined(__AVX__)
__m256 bufer256;

#ifdef __AVX512F__
// Код с AVX-512
const __m512 zero512 = _mm512_setzero_ps();
__m512 bufer512 = zero512;

for(; i < number_connection_number_by_512_bit; i+=16){
    _mm_prefetch(&connections[i + 16], _MM_HINT_T1);
    _mm_prefetch(&weight_connections[i + 16], _MM_HINT_T1);

    if (_mm512_testz_ps(_mm512_load_ps(connections[i]), _mm512_load_ps(connections[i])))
        continue;
#ifdef __FMA__
    // Попарное умножение элементов
    bufer512 = _mm512_fmadd_ps(_mm512_load_ps(connections[i]), _mm512_load_ps(weight_connections[i]), bufer512);
#else
    bufer512 += _mm512_mul_ps(_mm512_load_ps(connections[i]), _mm512_load_ps(weight_connections[i]));
#endif

}
// Попарное сложение элементов
bufer256 = _mm256_add_ps(_mm512_castps512_ps256(bufer512), _mm512_extractf32x8_ps(bufer512, 1));
#else

bufer256 = _mm256_setzero_ps();

#endif




const __m256 zero256 = _mm256_setzero_ps();
for(; i < number_connection_number_by_256_bit; i+=8){

    _mm_prefetch(&connections[i + 8], _MM_HINT_T1);
    _mm_prefetch(&weight_connections[i + 8], _MM_HINT_T1);

    if (_mm256_testz_ps(_mm256_load_ps(weight_connections[i]), zero256, _CMP_EQ_OQ) == 0xFFFF)
        continue;

#ifdef __FMA__
    // Попарное умножение элементов
    bufer256 = _mm256_fmadd_ps(_mm256_load_ps(weight_connections[i]), _mm256_load_ps(weight_connections[i]), bufer256);
#else
    bufer256 += _mm256_mul_ps(_mm256_load_ps(weight_connections[i]), _mm256_load_ps(weight_connections[i]));
#endif

}
bufer128 = _mm128_add_ps(_mm256_castps256_ps128(bufer256), _mm256_extractf32x8_ps(bufer256, 1));
#else
bufer128 = _mm_setzero_ps()
#endif

const __m128 zero128 = _mm_setzero_ps();
for(; i < number_connection_number_by_128_bit; i+=4){

    _mm_prefetch(&connections[i], _MM_HINT_T1);
    _mm_prefetch(&weight_connections[i], _MM_HINT_T1);

    if (_mm_testz_ps(_mm128_load_ps(weight_connections[i]), zero128, _CMP_EQ_OQ) == 0xFFFF)
        continue;

#ifdef __FMA__
    // Попарное умножение элементов
    bufer128 = _mm_fmadd_ps(_mm_load_ps(weight_connections[i]), _mm1_load_ps(weight_connections[i]), bufer128);
#else
    bufer128 += _mm_mul_ps(_mm_load_ps(weight_connections[i]), _mm_load_ps(weight_connections[i]));
#endif
}

#else
//std::cout << "Vectorization is not supported!\n";
#endif

for(; i < number_of_residual_compounds; ++i){
    if(connections[i] > 0){
        output_value += connections[i] * weight_connections[i];
    }
}

float result_f[4];
_mm_store_ps(result_f, bufer128);  // Сохраняем данные из bufer128 в массив
output_value += result_f[0] + result_f[1] + result_f[2] + result_f[3];


return output_value;
}

void run(){
calculate_dot_product();
}
};
*/
