#include "pch.h"
#include "Structs.h"
#include "RenderContext.h"
#include "ImageLoader.h"

namespace gce
{
    ColorBrush::ColorBrush(Color color, float32 opacity)
    {
        type = COLOR;

        HRESULT res = RenderContext::Get2DDeviceContext()->CreateSolidColorBrush(D2D1::ColorF(static_cast<float32>(color.r) / 255.0f, static_cast<float32>(color.g) / 255.0f, static_cast<float32>(color.b) / 255.0f, static_cast<float32>(color.a) / 255.0f), &m_pColorBrush);
        m_pColorBrush->SetOpacity(opacity);
    }

    void ColorBrush::SetColor(Color color) { m_pColorBrush->SetColor(D2D1_COLOR_F(static_cast<float32>(color.r) / 255.0f, static_cast<float32>(color.g) / 255.0f, static_cast<float32>(color.b) / 255.0f, static_cast<float32>(color.a) / 255.0f)); }
    void ColorBrush::SetOpacity(float32 opacity) { m_pColorBrush->SetOpacity(opacity); }

    LinearGradientBrush::LinearGradientBrush(RectanglePosF rect, uint8 colorCount, Color* pColors, float32 opacity)
    {
        type = LINEAR_GRADIENT;

        ID2D1GradientStopCollection* pGradientStops = nullptr;

        D2D1_GRADIENT_STOP* gradientStops = new D2D1_GRADIENT_STOP[colorCount];
        for (uint8 i = 0; i < colorCount; i++)
        {
            gradientStops[i].color = D2D1::ColorF(static_cast<float32>(pColors[i].r) / 255.0f, static_cast<float32>(pColors[i].g) / 255.0f, static_cast<float32>(pColors[i].b) / 255.0f, static_cast<float32>(pColors[i].a) / 255.0f);
            gradientStops[i].position = 0.0f + (i / max(1, (colorCount - 1.0f)));
        }

        HRESULT hr = RenderContext::Get2DDeviceContext()->CreateGradientStopCollection(
            gradientStops,
            colorCount,
            D2D1_GAMMA_2_2,
            D2D1_EXTEND_MODE_CLAMP,
            &pGradientStops
        );

        D2D1_BRUSH_PROPERTIES brushProp;
        brushProp.opacity = opacity;
        brushProp.transform = D2D1::Matrix3x2F::Identity();

        hr = RenderContext::Get2DDeviceContext()->CreateLinearGradientBrush(
            D2D1::LinearGradientBrushProperties(D2D1::Point2F(rect.left, rect.top), D2D1::Point2F(rect.right, rect.bottom)),
            brushProp,
            pGradientStops,
            &m_pLinearGradientBrush
        );

        pGradientStops->Release();
    };

    RadialGradientBrush::RadialGradientBrush(Vector2f32 center, Vector2f32 centerOffset, float32 radiusX, float32 radiusY, uint8 colorCount, Color* pColors, float32 opacity)
    {
        type = RADIANT_GRADIENT;

        ID2D1GradientStopCollection* pGradientStops = NULL;

        D2D1_GRADIENT_STOP* gradientStops = new D2D1_GRADIENT_STOP[colorCount];
        for (uint8 i = 0; i < colorCount; i++)
        {
            gradientStops[i].color = D2D1::ColorF(pColors[i].r / 255.0f, pColors[i].g / 255.0f, pColors[i].b / 255.0f, pColors[i].a / 255.0f);
            gradientStops[i].position = 0.0f + i / max(1, (colorCount - 1.0f));
        }

        HRESULT hr = RenderContext::Get2DDeviceContext()->CreateGradientStopCollection(
            gradientStops,
            colorCount,
            D2D1_GAMMA_2_2,
            D2D1_EXTEND_MODE_CLAMP,
            &pGradientStops
        );

        D2D1_BRUSH_PROPERTIES brushProp;
        brushProp.opacity = opacity;
        brushProp.transform = D2D1::Matrix3x2F::Identity();

        hr = RenderContext::Get2DDeviceContext()->CreateRadialGradientBrush(
            D2D1::RadialGradientBrushProperties(D2D1::Point2F(center.x, center.y), D2D1::Point2F(centerOffset.x, centerOffset.y), radiusX, radiusY),
            brushProp,
            pGradientStops,
            &m_pRadiantGradientBrush
        );

        pGradientStops->Release();

    };

    BitMapBrush::BitMapBrush(std::string const& path)
    {
        type = BIT_MAP;

        ImageData data = ImageLoader::LoadTexture(String(RES_PATH) + path);
        ID2D1Bitmap* pBitMap = nullptr;

        ImageLoader::LoadBitMap((RES_PATH)+path, &pBitMap);
        m_size = { pBitMap->GetSize().width,pBitMap->GetSize().height };

        D2D1_BRUSH_PROPERTIES brushProp;
        brushProp.opacity = 1.0f;
        brushProp.transform = D2D1::Matrix3x2F::Identity();

        D2D1_BITMAP_BRUSH_PROPERTIES bitMapBrushProp;
        bitMapBrushProp.extendModeX = D2D1_EXTEND_MODE_CLAMP;
        bitMapBrushProp.extendModeY = D2D1_EXTEND_MODE_CLAMP;
        bitMapBrushProp.interpolationMode = D2D1_BITMAP_INTERPOLATION_MODE_LINEAR;

        HRESULT hr = RenderContext::Get2DDeviceContext()->CreateBitmapBrush(
            pBitMap,
            bitMapBrushProp,
            brushProp,
            &m_pBitMapBrush);

        pBitMap->Release();
    }

    void BitMapBrush::SetTransformMatrix(Vector3f32 const& position, Vector3f32 const& scale, float32 rotation)
    {
        m_pBitMapBrush->SetTransform(D2D1::Matrix3x2F::Scale(scale.x, scale.y) * D2D1::Matrix3x2F::Rotation(rotation) * D2D1::Matrix3x2F::Translation(position.x, position.y));
    }

    void BitMapBrush::SetOpacity(float32 opacity)
    {
        m_pBitMapBrush->SetOpacity(opacity);
    }

}
