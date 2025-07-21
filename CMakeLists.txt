if (WIN32)
else ()
    #target_link_libraries(${PROJECT_NAME} libcef.so libcef_dll_wrapper.a gtk-4)
endif ()

cmake_minimum_required(VERSION 3.29)
if (WIN32)
    if (POLICY CMP0091)
        cmake_policy(SET CMP0091 NEW)
    endif (POLICY CMP0091)
endif ()
project("cef")
set(CEF_ROOT "C:/Users/fsx/source/repos/cef_binary_137.0.17+gf354b0e+chromium-137.0.7151.104_windows64")
file(MAKE_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
file(COPY "${CEF_ROOT}/cmake/FindCEF.cmake" DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
find_package(CEF REQUIRED)
add_subdirectory(${CEF_LIBCEF_DLL_WRAPPER_PATH} libcef_dll_wrapper)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
#set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MTd")
if (WIN32)
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup")
endif ()
file(GLOB HEADS ${PROJECT_SOURCE_DIR}/src/*.h)
file(GLOB SOURS ${PROJECT_SOURCE_DIR}/src/*.cpp)
if (WIN32)
    add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")
else ()
    set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
endif ()
ADD_LOGICAL_TARGET("libcef_lib" "${CEF_LIB_DEBUG}" "${CEF_LIB_RELEASE}")
SET_CEF_TARGET_OUT_DIR()
#set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup")
if (WIN32)
    add_executable(${PROJECT_NAME} WIN32 ${HEADS} ${SOURS})
else ()
    add_executable(${PROJECT_NAME} ${HEADS} ${SOURS})
endif ()
# 添加对 libcef_dll_wrapper 库的依赖
# 基于该配置，可以保证每次编译当前 cef_app target时候，确保 libcef_dll_wrapper 静态库编译完成
add_dependencies(${PROJECT_NAME} libcef_dll_wrapper)
target_include_directories(${PROJECT_NAME} PRIVATE ${CEF_INCLUDE_PATH})
# 链接库配置
target_link_libraries(
        ${PROJECT_NAME}
        PRIVATE
        # libcef_dll_wrapper库链接
        libcef_dll_wrapper
        libcef_lib
        # 该变量来自cef_variables.cmake中定义的配置
        # 主要是针对不同的平台，链接对应平台的一些标准库（Windows、Linux）或者framework（macOS）
        ${CEF_STANDARD_LIBS}
)
target_include_directories(${PROJECT_NAME} PRIVATE ${PROJECT_SOURCE_DIR})
#if (WIN32)
    #target_link_directories(${PROJECT_NAME} PRIVATE ${PROJECT_SOURCE_DIR}/lib)
    #target_link_libraries(${PROJECT_NAME} libcef cef_sandbox libcef_dll_wrapper comctl32 gdi32 rpcrt4 shlwapi ws2_32 Advapi32 dbghelp Delayimp OleAut32 PowrProf Propsys psapi)
#else ()
#endif ()
#target_link_libraries(${PROJECT_NAME} PRIVATE nfd)
#set_target_properties(${PROJECT_NAME} PROPERTIES WIN32_EXECUTABLE ON)
#使用MSVC时链接MT多线程静态库
#必须要在project或者enable_lanuage之前，设置policy CMP0091为NEW才能生效
if (WIN32)
    set_property(TARGET ${PROJECT_NAME} PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
endif ()
if(CMAKE_CONFIGURATION_TYPES)
    COPY_FILES("${PROJECT_NAME}" "${CEF_BINARY_FILES}" "${CEF_BINARY_DIR}" "${CEF_TARGET_OUT_DIR}")
    COPY_FILES("${PROJECT_NAME}" "${CEF_RESOURCE_FILES}" "${CEF_RESOURCE_DIR}" "${CEF_TARGET_OUT_DIR}")
else()
    COPY_FILES("${PROJECT_NAME}" "${CEF_BINARY_FILES}" "${CEF_BINARY_DIR}" "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_BUILD_TYPE}")
    COPY_FILES("${PROJECT_NAME}" "${CEF_RESOURCE_FILES}" "${CEF_RESOURCE_DIR}" "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_BUILD_TYPE}")
endif()
if (WIN32)
    add_definitions(-DNOMINMAX)
    add_definitions(-DWIN32)
    add_definitions(-D_WINDOWS)
    add_definitions(-D__STDC_CONSTANT_MACROS)
    add_definitions(-D__STDC_FORMAT_MACROS)
    add_definitions(-D_WIN32)
    add_definitions(-DUNICODE)
    add_definitions(-D_UNICODE)
    add_definitions(-DWINVER=0x0601)
    add_definitions(-D_WIN32_WINNT=0x601)
    add_definitions(-DWIN32_LEAN_AND_MEAN)
    add_definitions(-D_HAS_EXCEPTIONS=0)
    add_definitions(-DPSAPI_VERSION=1)
    add_definitions(-DCEF_USE_SANDBOX)
    add_definitions(-DCEF_USE_ATL)
    add_definitions(-D_HAS_ITERATOR_DEBUGGING=0)
    #屏蔽 CEF 框架中低版本 C++ 警告
    add_definitions(-D_SILENCE_ALL_CXX17_DEPRECATION_WARNINGS)
else ()
    set_target_properties(${PROJECT_NAME} PROPERTIES
        INSTALL_RPATH "$ORIGIN"
        BUILD_WITH_INSTALL_RPATH TRUE
    )
endif ()