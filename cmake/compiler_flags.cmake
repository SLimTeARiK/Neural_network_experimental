# ����������� �����������
if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    set(COMPILER_IS_CLANG TRUE)
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    set(COMPILER_IS_GCC TRUE)
endif()

# ================================================================
# ����� ����� ��� ���� ������������ � ������������
# ================================================================

# ����������� ����� ��� Windows
set(WINDOWS_COMPILE_FLAGS
    -mavx                   # ��������� AVX ����������
    -mavx2                  # ��������� AVX2 ����������
    -fexceptions            # ��������� ��������� ����������
    -fdiagnostics-color=always # ������� ��������������� ���������
)

# ����� �������� ��� Windows
set(WINDOWS_LINK_FLAGS
    -lmingw32               # ���������� MinGW
    -mwindows               # �������� GUI-���������� (�� �����������)
    -static                 # ����������� �������� ���� ���������
    -static-libgcc          # ����������� �������� libgcc
    -static-libstdc++       # ����������� �������� libstdc++
    -lstdc++                # ����� �������� ����������� ���������� C++
)

# ����������� �� ����� �������� (LTO)
set(LTO_FLAGS -flto)

# ================================================================
# ����������� ����� �� ������������
# ================================================================

# ����������� � ��������������
set(DIAGNOSTICS_GCC
    -Winvalid-pch           # �������� ������������������ ���������� (GCC)
)

set(DIAGNOSTICS_CLANG
    -Winvalid-pch           # �������� ������������������ ���������� (Clang)
)

# ������ �� �����������
set(OPT_REPORT_GCC
    -fopt-info-vec          # ���������� � ������������
    -fopt-info-vec-missed   # ������� ������� ������������
    -fopt-info-vec-optimized# ����� �� �������� ������������
    -ftime-report           # ����� ������ ���������� (GCC)
)

set(OPT_REPORT_CLANG
    -Rpass=.*               # ����� �� �������� ������������ (Clang)
    -Rpass-missed=.*        # ����� � ����������� ������������ (Clang)
    -ftime-report           # ����� ������ ���������� (Clang)
)

set(SUPPRESS_WARNINGS_CLANG
    -Wno-invalid-utf8
    -Wno-comment
)

# ================================================================
# ������������ DEBUG (��� ����������� + ���� ������)
# ================================================================
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    # ������� ���������
    set(OPTIMIZATION_DEBUG
        -O0                  # ��� �����������
    )

    set(DEBUG_INFO
        -g                   # ������� ���������� ����������
        -ggdb3               # ����������� ���������� ���������� ��� GDB
        -fno-omit-frame-pointer # ���������� ��������� ������
    )

    # �����������
    set(DIAGNOSTICS
        -Wall                # ��� ����������� ��������������
        -Wextra              # �������������� ��������������
        -pedantic            # ������� ������������ ���������
    )

    # ����������
    set(SANITIZERS -fsanitize=address)

    # ������ ������ � ����������� �� �����������
    if(COMPILER_IS_GCC)
        set(COMPILER_SPECIFIC
            ${DIAGNOSTICS_GCC}
            ${OPT_REPORT_GCC}
        )
    elseif(COMPILER_IS_CLANG)
        set(COMPILER_SPECIFIC
            ${DIAGNOSTICS_CLANG}
            ${OPT_REPORT_CLANG}
            ${SUPPRESS_WARNINGS_CLANG}
        )
    endif()

    # �������� ����� ��� Debug
    target_compile_options(${PROJECT_NAME} PUBLIC
        ${OPTIMIZATION_DEBUG}
        ${DEBUG_INFO}
        ${DIAGNOSTICS}
        ${SUPPRESS_WARNINGS}
        ${COMPILER_SPECIFIC}
        ${SANITIZERS}
    )

# ================================================================
# ������������ RELEASE (���� ����������� + ��� ������)
# ================================================================
elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
    # ����������� �����������
    set(OPTIMIZATION_RELEASE
        -O3                  # ������������ ����������� ��������
        -march=native        # ������������� ���������� ����������
        -DNDEBUG             # ���������� assert � ����������� ����
    )

    # ������������
    set(SECURITY
        -fstack-protector-strong # ������ �� ������������ �����
    )

    target_compile_options(${PROJECT_NAME} PUBLIC
        ${OPTIMIZATION_RELEASE}
        ${SECURITY}
    )

# ================================================================
# ������������ TEST (���� ����������� + ���� ������)
# ================================================================
elseif(CMAKE_BUILD_TYPE STREQUAL "Test")
    # ��������� ���������
    set(OPTIMIZATION_TEST
        -O3                  # ����������� �����������
        -march=native        # ������������� ���������� ����������
        -DDEBUG              # ��������� ����������� ����
    )

    set(DEBUG_INFO
        -g
        -ggdb3
        -fno-omit-frame-pointer
    )

    set(DIAGNOSTICS
        -Wall
        -Wextra
        -pedantic
    )

    set(SECURITY
        -fstack-protector-strong
    )

    # ������ ������ � ����������� �� �����������
    if(COMPILER_IS_GCC)
        set(COMPILER_SPECIFIC
            ${DIAGNOSTICS_GCC}
            ${OPT_REPORT_GCC}
        )
    elseif(COMPILER_IS_CLANG)
        set(COMPILER_SPECIFIC
            ${DIAGNOSTICS_CLANG}
            ${OPT_REPORT_CLANG}
        )
    endif()

    target_compile_options(${PROJECT_NAME} PUBLIC
        ${OPTIMIZATION_TEST}
        ${DEBUG_INFO}
        ${DIAGNOSTICS}
        ${SECURITY}
        ${COMPILER_SPECIFIC}
    )
endif()

# ================================================================
# ����� ����� ��� ���� ������������
# ================================================================
target_compile_options(${PROJECT_NAME} PUBLIC
    ${LTO_FLAGS}            # ����������� �� ����� ��������
    ${WINDOWS_COMPILE_FLAGS}
)

target_link_options(${PROJECT_NAME} PUBLIC
    ${WINDOWS_LINK_FLAGS}
)

# ================================================================
# �������������� ���������
# ================================================================
#find_package(NUMA)
#if(NUMA_FOUND)
#    target_compile_definitions(${PROJECT_NAME} PUBLIC -DHAVE_NUMA=1)
#else()
#    target_compile_definitions(${PROJECT_NAME} PUBLIC -DHAVE_NUMA=0)
#endif()
