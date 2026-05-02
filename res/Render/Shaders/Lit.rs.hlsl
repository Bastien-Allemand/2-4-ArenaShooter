// ROOTSIG
// 0 - ObjectData - b0
// 1 - PassData - b1
// 2 - LightPassData - b2
// 3 - Textures descriptor table - t0
// 4 - Texture id - b2
// 5 - Texture Sampler - s0


#define ROOTSIG \
"RootFlags(ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT), " \
"CBV(b0), " \
"CBV(b1), " \
"CBV(b2), " \
"DescriptorTable(SRV(t0, numDescriptors = unbounded, flags = DESCRIPTORS_VOLATILE)), " \
"RootConstants(num32BitConstants=1, b3), " \
"StaticSampler(s0, filter=FILTER_MIN_MAG_MIP_POINT)"