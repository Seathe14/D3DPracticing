# List of all C++ sources in the D3DPracticing application
set(D3DPracticing_SOURCES
    App.cpp
    BaseException.cpp
    Bindable.cpp
    Box.cpp
    Drawable.cpp
    DrawableBase.cpp
    DxgiInfoManager.cpp
    Graphics.cpp
    IndexBuffer.cpp
    InputLayout.cpp
    Keyboard.cpp
    Mouse.cpp
    PixelShader.cpp
    Topology.cpp
    TransformConstantBuffer.cpp
    VertexBuffer.cpp
    VertexShader.cpp
    Window.cpp
    WinMain.cpp
)

set(D3DPracticing_HEADERS
    App.h
    BaseException.h
    Bindable.h
    Box.h
    Drawable.h
    DrawableBase.h
    DxgiInfoManager.h
    Graphics.h
    IndexBuffer.h
    InputLayout.h
    Keyboard.h
    Mouse.h
    PixelShader.h
    Topology.h
    TransformConstantBuffer.h
    VertexBuffer.h
    VertexShader.h
    Window.h
)

set(HLSL
    VertexShader.hlsl
    PixelShader.hlsl)


add_executable(D3DPracticing WIN32 ${D3DPracticing_SOURCES} ${D3DPracticing_HEADERS})

target_sources(D3DPracticing PRIVATE ${HLSL})

set_source_files_properties(VertexShader.hlsl PROPERTIES
                            VS_SHADER_TYPE "Vertex"
                            VS_SHADER_MODEL "5.0"
                            VS_SHADER_ENTRYPOINT "main"
                            VS_SHADER_OBJECT_FILE_NAME "$(ProjectDir)/ShaderOutput/%(Filename).cso")

set_source_files_properties(PixelShader.hlsl PROPERTIES
                            VS_SHADER_TYPE "Pixel"
                            VS_SHADER_MODEL "5.0"
                            VS_SHADER_ENTRYPOINT "main"
                            VS_SHADER_OBJECT_FILE_NAME "$(ProjectDir)/ShaderOutput/%(Filename).cso")


target_compile_definitions(D3DPracticing PRIVATE
    IS_DEBUG=$<$<CONFIG:Debug>:1>$<$<NOT:$<CONFIG:Debug>>:0>
)

target_include_directories(D3DPracticing PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

if(WIN32)
    target_link_libraries(D3DPracticing PRIVATE d3d11 dxguid d3dcompiler Shlwapi)
endif()
