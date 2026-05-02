// 0 - Object Data - b0
// 1 - Pass Data - b1
// 2 - Textures arrays - t0
// 3 - Texture ID - b2

// CIEL

#define ROOTSIG \
"RootFlags(ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT), " \
"CBV(b0), " \
"CBV(b1), " \
"DescriptorTable(SRV(t0, numDescriptors = unbounded, flags = DESCRIPTORS_VOLATILE)), " \
"RootConstants(num32BitConstants=1, b2), " \
"StaticSampler(s0, filter = FILTER_MIN_MAG_MIP_POINT)" \
