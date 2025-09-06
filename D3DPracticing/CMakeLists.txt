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

add_executable(D3DPracticing WIN32 ${D3DPracticing_SOURCES})

target_include_directories(D3DPracticing PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

if(WIN32)
    target_link_libraries(D3DPracticing PRIVATE d3d11 dxguid d3dcompiler Shlwapi)
endif()
