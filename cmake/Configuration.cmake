



function(setup_cpp_target TARGET_NAME CPP_STANDARD)
    # ------------------------------
    # 设置 C++ 标准
    # ------------------------------
    set_target_properties(${TARGET_NAME} PROPERTIES
        CXX_STANDARD ${CPP_STANDARD}
        CXX_STANDARD_REQUIRED ON
        CXX_EXTENSIONS OFF        # 禁止 GNU 扩展（-std=c++20 而不是 -std=gnu++20）
        POSITION_INDEPENDENT_CODE ON
    )

    # ------------------------------
    # 编译器识别
    # ------------------------------
    set(_compiler_id ${CMAKE_CXX_COMPILER_ID})
    
    message(STATUS "CMAKE_CXX_COMPILER_ID : ${CMAKE_CXX_COMPILER_ID}, CMAKE_CXX_SIMULATE_ID : ${CMAKE_CXX_SIMULATE_ID}")
    # 检测 clang-cl（Clang with MSVC frontend）
    if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang" AND CMAKE_CXX_SIMULATE_ID STREQUAL "MSVC")
        set(_is_clang_cl TRUE)
    else()
        set(_is_clang_cl FALSE)
    endif()

    message(STATUS "setup_cpp_target: compiler = ${_compiler_id}, is_clang_cl = ${_is_clang_cl}")

    # ------------------------------
    # GCC
    # ------------------------------
    if (_compiler_id STREQUAL "GNU")
        target_compile_options(${TARGET_NAME} PRIVATE
            -Wall -Wextra -Wpedantic
            -Wconversion -Wsign-conversion
        )

    # ------------------------------
    # Clang-CL (使用 MSVC 语法的 LLVM)
    # ------------------------------
    elseif (_is_clang_cl)
        target_compile_options(${TARGET_NAME} PRIVATE
            /W4        # Clang-cl 支持 MSVC 风格的警告设置
            /permissive-  
        )

    # ------------------------------
    # Clang (包含 macOS 的 AppleClang)
    # ------------------------------
    elseif (_compiler_id STREQUAL "Clang" OR _compiler_id STREQUAL "AppleClang")
        target_compile_options(${TARGET_NAME} PRIVATE
            -Wall -Wextra -Wpedantic
            -Wconversion -Wsign-conversion
        )

    # ------------------------------
    # 纯 MSVC
    # ------------------------------
    elseif (_compiler_id STREQUAL "MSVC")
        target_compile_options(${TARGET_NAME} PRIVATE
            /W4
            /permissive-  # 使用更严格的标准模式
        )

    else()
        message(WARNING "setup_cpp_target: 未知的编译器 ${_compiler_id}，使用默认设置")
    endif()
endfunction()

list(APPEND CMAKE_PREFIX_PATH "${CMAKE_SOURCE_DIR}/package/spdlog")