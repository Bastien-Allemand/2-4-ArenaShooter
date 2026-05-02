
#define NUM_CONTROL_POINTS 3
#include "Common.hlsli"

PatchTess CalcHSPatchConstants(
	InputPatch<VertexOut, NUM_CONTROL_POINTS> patch,
	uint PatchID : SV_PrimitiveID)
{
	PatchTess pt;

    pt.EdgeTessFactor[0] = 0.5f * (patch[1].tessFactor + patch[2].tessFactor);
    pt.EdgeTessFactor[1] = 0.5f * (patch[2].tessFactor + patch[0].tessFactor);
    pt.EdgeTessFactor[2] = 0.5f * (patch[0].tessFactor + patch[1].tessFactor);
    pt.InsideTessFactor = (pt.EdgeTessFactor[0] + pt.EdgeTessFactor[1] + pt.EdgeTessFactor[2]) / 3.0f;
	
    pt.EdgeTessFactor[0] = max(pt.EdgeTessFactor[0], 1.0f);
    pt.EdgeTessFactor[1] = max(pt.EdgeTessFactor[1], 1.0f);
    pt.EdgeTessFactor[2] = max(pt.EdgeTessFactor[2], 1.0f);
    pt.InsideTessFactor = max(pt.InsideTessFactor, 1.0f);

	return pt;
}

[domain("tri")]
[partitioning("fractional_odd")]
[outputtopology("triangle_cw")]
[outputcontrolpoints(3)]
[patchconstantfunc("CalcHSPatchConstants")]

HullOut main( 
	InputPatch<VertexOut, NUM_CONTROL_POINTS> patch, 
	uint i : SV_OutputControlPointID,
	uint PatchID : SV_PrimitiveID )
{
	HullOut hout;

	hout.posW = patch[i].posW;
    hout.normal = patch[i].normal;
    hout.tangent = patch[i].tangent;
    hout.uv = patch[i].uv;

	return hout;
}
