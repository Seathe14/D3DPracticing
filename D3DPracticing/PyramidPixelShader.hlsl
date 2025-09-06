cbuffer CBuf
{
    float4 face_colors[5];
}

float4 main(uint pid : SV_PrimitiveID) : SV_TARGET
{
    uint idx = pid;
    if (pid >= 4)
    {
        idx = 4;
    }
    return face_colors[idx];
}
