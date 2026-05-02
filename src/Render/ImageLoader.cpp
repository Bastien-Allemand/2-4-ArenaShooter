#include "pch.h"
#include "ImageLoader.h"
#include "StaticBuffer.h"

#include "RenderContext.h"

namespace gce
{

	ImageData ImageLoader::LoadTexture(const std::filesystem::path& imagepath)
	{
		ImageData imgData = {};

		HRESULT hr = CoInitialize(nullptr);
		if (FAILED(hr)) PRINT_COM_ERROR("Error while Initialize : ", hr);
		
		IWICImagingFactory* wicFactory = nullptr;
		hr = CoCreateInstance(CLSID_WICImagingFactory, nullptr, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&wicFactory));
		if (FAILED(hr)) PRINT_COM_ERROR("Error while loading image : ", hr);

		IWICBitmapDecoder* wicDecoder = nullptr;
		hr = wicFactory->CreateDecoderFromFilename(imagepath.wstring().c_str(), nullptr, GENERIC_READ, WICDecodeMetadataCacheOnDemand, &wicDecoder);
		if (FAILED(hr)) PRINT_COM_ERROR("Error while loading image : ", hr);

		PRINT_DEBUG(imagepath);
		IWICBitmapFrameDecode* wicFrame = nullptr;
		hr = wicDecoder->GetFrame(0, &wicFrame);
		if (FAILED(hr)) PRINT_COM_ERROR("Error while loading image : ", hr);

		//==== Convert to dxgi supported type
		IWICFormatConverter* wicFormatConverter = nullptr;
		hr = wicFactory->CreateFormatConverter(&wicFormatConverter);
		if (FAILED(hr)) PRINT_COM_ERROR("Error while loading image : ", hr);

		hr = wicFormatConverter->Initialize(wicFrame, GUID_WICPixelFormat32bppBGRA, WICBitmapDitherTypeNone, nullptr, 0.0, WICBitmapPaletteTypeCustom);
		if (FAILED(hr)) PRINT_COM_ERROR("Error while loading image : ", hr);

		hr = wicFormatConverter->GetPixelFormat(&imgData.pixelFormat);
		if (FAILED(hr)) PRINT_COM_ERROR("Error while loading image : ", hr);

		hr = wicFormatConverter->GetSize(&imgData.width, &imgData.height);
		if (FAILED(hr)) PRINT_COM_ERROR("Error while loading image : ", hr);

		IWICComponentInfo* wicComponentInfo = nullptr;
		hr = wicFactory->CreateComponentInfo(imgData.pixelFormat, &wicComponentInfo);
		if (FAILED(hr)) PRINT_COM_ERROR("Error while loading image : ", hr);

		IWICPixelFormatInfo* wicPixelFormatInfo = nullptr;
		hr = wicComponentInfo->QueryInterface(&wicPixelFormatInfo);
		if (FAILED(hr)) PRINT_COM_ERROR("Error while loading image : ", hr);

		UINT bitsPerPixel = 0;
		hr = wicPixelFormatInfo->GetBitsPerPixel(&bitsPerPixel);
		if (FAILED(hr)) PRINT_COM_ERROR("Error while loading image : ", hr);

		UINT channelCount = 0;
		hr = wicPixelFormatInfo->GetChannelCount(&channelCount);
		if (FAILED(hr)) PRINT_COM_ERROR("Error while loading image : ", hr);;

		imgData.bitsPerPixel = ((bitsPerPixel + 7) / 8);
		imgData.textureStride = imgData.bitsPerPixel * imgData.width;
		imgData.textureSize = imgData.textureStride * imgData.height;
		imgData.data.Resize(imgData.textureSize);
		imgData.chanelCount = channelCount;
		imgData.giPixelFormat = DXGI_FORMAT_B8G8R8A8_UNORM;

		hr = wicFormatConverter->CopyPixels(nullptr, imgData.textureStride, imgData.textureSize, reinterpret_cast<BYTE*>(imgData.data.Data()));
		if (FAILED(hr)) PRINT_COM_ERROR("Error while loading image : ", hr);

		WICRect copyRect;
		copyRect.X = 0;
		copyRect.Y = 0;
		copyRect.Height = imgData.height;
		copyRect.Width = imgData.width;

		wicFormatConverter->Release();
		wicFrame->Release();
		wicDecoder->Release();
		wicFactory->Release();

		return imgData;
	}

	void ImageLoader::LoadBitMap(std::filesystem::path const& imagePath, ID2D1Bitmap** ppBitMap)
	{
		HRESULT hr = CoInitialize(nullptr);
		if (FAILED(hr)) PRINT_COM_ERROR("Error while Initialize : ", hr);

		IWICImagingFactory* wicFactory = nullptr;
		hr = CoCreateInstance(CLSID_WICImagingFactory, nullptr, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&wicFactory));
		if (FAILED(hr)) PRINT_COM_ERROR("Error while loading image : ", hr);

		IWICBitmapDecoder* wicDecoder = nullptr;
		hr = wicFactory->CreateDecoderFromFilename(imagePath.wstring().c_str(), nullptr, GENERIC_READ, WICDecodeMetadataCacheOnLoad, &wicDecoder);
		if (FAILED(hr)) PRINT_COM_ERROR("Error while loading image : ", hr);

		PRINT_DEBUG(imagePath);
		IWICBitmapFrameDecode* wicFrame = nullptr;
		hr = wicDecoder->GetFrame(0, &wicFrame);
		if (FAILED(hr)) PRINT_COM_ERROR("Error while loading image : ", hr);

		//==== Convert to dxgi supported type
		IWICFormatConverter* wicFormatConverter = nullptr;
		hr = wicFactory->CreateFormatConverter(&wicFormatConverter);
		if (FAILED(hr)) PRINT_COM_ERROR("Error while loading image : ", hr);

		hr = wicFormatConverter->Initialize(wicFrame, GUID_WICPixelFormat32bppPBGRA, WICBitmapDitherTypeNone, nullptr, 0.0f, WICBitmapPaletteTypeMedianCut);
		if (FAILED(hr)) PRINT_COM_ERROR("Error while loading image : ", hr);
		
		hr = RenderContext::Get2DDeviceContext()->CreateBitmapFromWicBitmap(
			wicFormatConverter,
			NULL,
			ppBitMap);

		wicFormatConverter->Release();
		wicFrame->Release();
		wicDecoder->Release();
		wicFactory->Release();

	}

	void ImageLoader::CopyTextureRegion(ID3D12GraphicsCommandList* cmdList, StaticBuffer const* pBuffer, D3D12_PLACED_SUBRESOURCE_FOOTPRINT const& footprint, uint64 offset)
	{
		D3D12_TEXTURE_COPY_LOCATION srcLocation = {};
		srcLocation.pResource = pBuffer->m_pUploadBuffer;
		srcLocation.Type = D3D12_TEXTURE_COPY_TYPE_PLACED_FOOTPRINT;
		srcLocation.PlacedFootprint = footprint;

		D3D12_TEXTURE_COPY_LOCATION dstLocation = {};
		dstLocation.pResource = pBuffer->m_pDefaultBuffer;
		dstLocation.Type = D3D12_TEXTURE_COPY_TYPE_SUBRESOURCE_INDEX;
		dstLocation.SubresourceIndex = 0;

		cmdList->CopyTextureRegion(&dstLocation, 0, 0, 0, &srcLocation, nullptr);
	}
}
