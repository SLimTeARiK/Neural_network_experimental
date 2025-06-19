
struct alignas(16) f_rgba {
    float r, g, b, a;

    __attribute__((always_inline)) f_rgba operator*(float k) const noexcept {
            f_rgba new_color;
            _mm_store_ps(&new_color.r, _mm_mul_ps(reinterpret_cast<const __m128&>(r), _mm_set1_ps(k)));
            return new_color;
    }
};

__attribute__((always_inline)) void f_rgb_cell_ratio(
    f_rgba& __restrict new_struct,
    const f_rgba& __restrict old_struct,
    float ratio
) noexcept {
    reinterpret_cast<__m128&>(new_struct.r) = _mm_mul_ps(
        reinterpret_cast<const __m128&>(old_struct.r),
        _mm_set1_ps(ratio)
    );
}

__attribute__((always_inline)) void f_rgb_cell_blend(
    f_rgba& __restrict dst,
    const f_rgba& __restrict src,
    float alpha
) noexcept {
    reinterpret_cast<__m128&>(dst.r) = _mm_add_ps(
        _mm_mul_ps(reinterpret_cast<const __m128&>(src.r), _mm_set1_ps(alpha)),
        _mm_mul_ps(reinterpret_cast<const __m128&>(dst.r), _mm_set1_ps(1.0f - alpha))
    );
}

struct alignas(16) f_rgb {
    float r, g, b;

    __attribute__((always_inline)) f_rgb operator=(f_rgba color) const noexcept {
            return f_rgb{.r = color.r*color.a, .g = color.g*color.a, .b = color.b*color.b};
    }
    __attribute__((always_inline)) f_rgb operator=(float a) const noexcept {
            return f_rgb{.r = r*a, .g = g*a, .b = b*a};
    }
};

f_rgb rgba_to_rgb(const f_rgba& color) {
    return f_rgb{.r = color.r*color.a, .g = color.g*color.a, .b = color.b*color.b};  // Uniform initialization (C++11)
}
