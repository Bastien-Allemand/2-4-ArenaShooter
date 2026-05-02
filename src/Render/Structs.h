#ifndef RENDER_STRUCT_H_INCLUDED
#define RENDER_STRUCT_H_INCLUDED

#include <DirectXMath.h>
#include <d2d1.h>

#include "Maths/Color.h"
#include "Maths/Matrix.h"
#include "Maths/Vector3.h"
#include "Maths/Vector4.h"
#include "Maths/Vector2.hpp"
#include "define.h"

#define MAX_LIGHTS 16

namespace gce
{
    class RenderCamera;
    class Texture;
    class Geometry;
    class D12PipelineObject;
    class FrameResources;

	struct Vertex
	{
        Vector3f32 pos;
        Vector3f32 normal;
        Vector3f32 tangent;
        Vector2f32 uv;
	};

    struct Material
    {

        int32 ambientTextureID;
        Vector3f32 fresnelR0;

        int32 roughnessTextureID;
        float32 subsurface;
        int32 normalTextureID;
        int32 metalnessTextureID;

        int32 displacementTextureID;
        int32 albedoTextureID;
        int32 useTextureAmbient;
        int32 useTextureNormal;
       
        int32 useTextureAlbedo;
        int32 useTextureRoughness;
        int32 useDisplacementTexture;
        int32 useTextureMetalness;
    };

    enum class LightType: int32
    {
        NONE = -1,
        DIRECTIONAL,
        POINT,
        SPOT
    };

    struct LightData
    {
        Vector3f32 strength;
        int32 id;
        
        Vector4 color;

        Vector3f32 direction;
        int32 numFramesDirty;

        Vector3f32 position;
        float32 padding1;

        Vector3f32 rimLightColor;
        float32 range;

        float32 spotAngle;
        float32 penumbraAngle;
        float32 intensity;
        float32 rimLightIntensity;

        LightType type;
        float32 padding2;
        float32 padding3;
        float32 padding4;

    };

    struct LightPassData
    {
        LightData light[MAX_LIGHTS];

        float32 maxTessDistance = 100.0f;
        float32 minTessDistance = 8.0f;
        float32 minTessFactor = 2.0f;
        float32 maxTessFactor = 32.0f;

        float32 ambientIntensity = 0.5f;
        float32 padding2;
        float32 padding3;
        float32 padding4;
    };

    struct ObjectData
    {
        Matrix world;
        Material material;

        uint32 indexCount = 0;
        uint32 startIndexLocation = 0;
        int32 baseVertexLocation = 0;
        uint32 padding;

    };

    struct SkyBoxData
    {
        ObjectData objData;
        int32 isLockOncamera;

        float64 padding;
    };

    struct PassData
    {
        Matrix viewProj;
        Matrix invView;
        Matrix view;
        Matrix proj;
        Matrix invProj;
        Matrix invViewProj;

        Vector3f32 eyePos;
        float32 padding;
    };;

    struct RenderItem
    {
        Geometry* pGeometry;
        D12PipelineObject const* pPso;

        ObjectData objData;
        uint32 id;
        int32 numFramesDirty;
        bool toDestroy = false;
    };

    struct ShapeUi
    {

    protected:

        enum ShapeType : uint8
        {
            RECT,
            ROUND_RECT,
            ELLIPSE,
            LINE,
        };
        virtual ShapeType GetType() const = 0;

        friend class Window;
    };

    struct RoundRectanglePosF : public ShapeUi
    {
        float32 left, top, right, bottom;
        float32 radiusX, radiusY;
        operator D2D1_ROUNDED_RECT() const
        {
            return { {left, top, right, bottom}, radiusX, radiusY };
        }
        RoundRectanglePosF(float32 _left, float32 _top, float32 _right, float32 _bottom, float32 _radiusX, float32 _radiusY) :
            left(_left), top(_top), right(_right), bottom(_bottom), radiusX(_radiusX), radiusY(_radiusY) {
        }

    protected:
        ShapeType GetType() const override { return ROUND_RECT; }
        friend class Window;
    };

    struct RectanglePosF : public ShapeUi
    {
        float32 left, top, right, bottom;
        operator D2D1_RECT_F() const
        {
            return { left, top, right, bottom };
        }

        RectanglePosF(float32 _left, float32 _top, float32 _right, float32 _bottom) :
            left(_left), top(_top), right(_right), bottom(_bottom) {
        }

    protected:
        ShapeType GetType() const override { return RECT; }
        friend class Window;
    };

    struct Ellipse : public ShapeUi
    {
        Vector2f32 center;
        float32 radiusX, radiusY;
        operator D2D1_ELLIPSE() const
        {
            return { D2D1_POINT_2F(center.x,center.y),radiusX,radiusY };
        }

        Ellipse(Vector2f32 _center, float32 _radiusX, float32 _radiusY) :
            center(_center), radiusX(_radiusX), radiusY(_radiusY) {
        }

    protected:
        ShapeType GetType() const override { return ELLIPSE; }
        friend class Window;
    };

    struct Line : public ShapeUi
    {
        Vector2f32 start;
        Vector2f32 end;
        float32 width;
        D2D1_POINT_2F Start() const
        {
            return { start.x,start.y };
        }

        D2D1_POINT_2F End() const
        {
            return { end.x,end.y };
        }
        Line(Vector2f32 _start, Vector2f32 _end, float32 _width) :
            start(_start), end(_end), width(_width) {
        }

    protected:

        ShapeType GetType() const override { return LINE; }
        friend class Window;
    };

    struct UiBrush
    {
    protected:
        enum BrushType
        {
            COLOR,
            LINEAR_GRADIENT,
            RADIANT_GRADIENT,
            BIT_MAP,
        };

        virtual ID2D1Brush* GetBrush() const = 0;
        virtual ~UiBrush() {};

        BrushType type;

        friend class Window;
    };

    struct ColorBrush : public UiBrush
    {

        ColorBrush(Color color, float32 opacity = 1.0f);
        void SetColor(Color color);
        void SetOpacity(float32 opacity);
        ~ColorBrush() override { m_pColorBrush->Release(); }

    protected:

        ID2D1Brush* GetBrush() const override { return m_pColorBrush; }
        ID2D1SolidColorBrush* m_pColorBrush = nullptr;

        friend class Window;
    };

    struct LinearGradientBrush : public UiBrush
    {
        LinearGradientBrush(RectanglePosF rect, uint8 colorCount, Color* pColors, float32 opacity = 1.0f);
        ~LinearGradientBrush() override { m_pLinearGradientBrush->Release(); }

    protected:

        ID2D1Brush* GetBrush() const override { return m_pLinearGradientBrush; }
        ID2D1LinearGradientBrush* m_pLinearGradientBrush = nullptr;

        friend class Window;
    };

    struct RadialGradientBrush : public UiBrush
    {

        RadialGradientBrush(Vector2f32 center, Vector2f32 centerOffset, float32 radiusX, float32 radiusY, uint8 colorCount, Color* pColors, float32 opacity = 1.0f);
        ~RadialGradientBrush() override { m_pRadiantGradientBrush->Release(); }
    protected:

        ID2D1Brush* GetBrush() const override { return m_pRadiantGradientBrush; }
        ID2D1RadialGradientBrush* m_pRadiantGradientBrush = nullptr;

        friend class Window;
    };

    struct BitMapBrush : public UiBrush
    {

        BitMapBrush(std::string const& path);
        void SetTransformMatrix(Vector3f32 const& position, Vector3f32 const& scale, float32 rotation);
        void SetOpacity(float32 opacity);
        ~BitMapBrush() override { m_pBitMapBrush->Release(); }

        Vector2f32 const& GetSize() const { return m_size; }
    protected:

        ID2D1Brush* GetBrush() const override { return m_pBitMapBrush; }
        ID2D1BitmapBrush* m_pBitMapBrush = nullptr;
        Vector2f32 m_size;

        friend class Window;
    };
}

#endif