// 0 - PassData - b0
// 1 - Descriptor table - t0
// 2 - Sampler - s0


#define ROOTSIG \
"RootFlags(ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT), " \
"CBV(b0), " \
"DescriptorTable(SRV(t0, numDescriptors = unbounded, flags = DESCRIPTORS_VOLATILE)), " \
"StaticSampler(s0, " \
	"addressU = TEXTURE_ADDRESS_BORDER, " \
	"addressV = TEXTURE_ADDRESS_BORDER, " \
	"addressW = TEXTURE_ADDRESS_BORDER, " \
	"borderColor = STATIC_BORDER_COLOR_OPAQUE_BLACK, " \
	"filter = FILTER_COMPARISON_MIN_MAG_LINEAR_MIP_POINT, " \
	"comparisonFunc = COMPARISON_LESS_EQUAL) "