#include <cmath>
#include <type_traits>
#include <functional>
#include <stdexcept>

// Типы активации для основной функции
enum class ActivationType {
    Sigmoid,
    ReLU,
    Tanh,
    Linear
};

// Основная функция активации для float
float activate(float x, ActivationType type) {
    switch(type) {
        case ActivationType::Sigmoid:
            return 1.0f / (1.0f + std::exp(-x));
        case ActivationType::ReLU:
            return (x > 0.0f) ? x : 0.0f;
        case ActivationType::Tanh:
            return std::tanh(x);
        case ActivationType::Linear:
            return x;
        default:
            throw std::invalid_argument("Unknown activation type");
    }
}

// Шаблонная версия для других типов
template<typename T>
T activate(T x, ActivationType type) {
    static_assert(std::is_arithmetic_v<T>, "Unsupported type for activation");

    // Для целочисленных типов используем преобразование к float
    if constexpr (std::is_integral_v<T>) {
        return static_cast<T>(activate(static_cast<float>(x), type));
    }
    else {
        // Для double используем отдельные вычисления
        switch(type) {
            case ActivationType::Sigmoid:
                return 1.0 / (1.0 + std::exp(-x));
            case ActivationType::ReLU:
                return (x > 0.0) ? x : 0.0;
            case ActivationType::Tanh:
                return std::tanh(x);
            case ActivationType::Linear:
                return x;
            default:
                throw std::invalid_argument("Unknown activation type");
        }
    }
}

// Явная специализация для поддерживаемых типов
template uint8_t activate<uint8_t>(uint8_t, ActivationType);
template uint16_t activate<uint16_t>(uint16_t, ActivationType);
template uint32_t activate<uint32_t>(uint32_t, ActivationType);
template uint64_t activate<uint64_t>(uint64_t, ActivationType);
template int8_t activate<int8_t>(int8_t, ActivationType);
template int16_t activate<int16_t>(int16_t, ActivationType);
template int32_t activate<int32_t>(int32_t, ActivationType);
template int64_t activate<int64_t>(int64_t, ActivationType);
template double activate<double>(double, ActivationType);

// Версия с пользовательской функцией активации
template<typename T, typename F>
T custom_activate(T x, F&& func) {
    static_assert(std::is_invocable_r_v<T, F, T>,
                 "Custom activation must be callable with type T");
    return std::forward<F>(func)(x);
}
