#pragma once

#include <Containers/Vector.hpp>
#include "Structs.h"
#include "DynamicBuffer.h"
#include "RenderTarget.h"
#include "Containers/FreeList.hpp"
#include <queue>

namespace gce
{
    struct FrameResource
    {
    public:
        FrameResource();
        ~FrameResource();

        FreeList<DynamicBuffer*> m_pPassCB;
        DynamicBuffer* m_pLightsCB = nullptr;
        FreeList<DynamicBuffer*> m_pObjDatasCB{20};
        ID3D12CommandAllocator* m_pCommandAllocator = nullptr;

    private:
        uint64 m_fenceValue = 0;

        friend class FrameResourcesManager;
    };

    class FrameResourcesManager
    {
    public:
        FrameResourcesManager();
        FrameResourcesManager(FrameResourcesManager const&) = delete;

        FrameResourcesManager& operator=(FrameResourcesManager const& other) = delete;
        FrameResourcesManager& operator=(FrameResourcesManager&& other) noexcept = delete;

        ~FrameResourcesManager();

        static void BeginFrame();
        static void EndFrame();

        static void AddRenderItem(RenderItem& item);
        static void AddCamera(RenderCamera& camera);
        static void RemoveRenderItem(RenderItem& item);
        void Flush();

        static FrameResource* GetCurrentFrameResource();
        static constexpr int32 GetNumFrameResources() { return s_numFrameResources; }
        static ID3D12GraphicsCommandList* GetCmdList() { return Instance().m_pCmdList; }
        static ID3D12CommandQueue* GetCmdQueue() { return Instance().m_pCmdQueue; }

        static void UpdateObjCBuffer();


        static FrameResourcesManager& Instance()
        {
            static FrameResourcesManager fr;
            return fr;
        }

    protected:
        static constexpr int32 s_numFrameResources = 3;
        int32 m_currentFrameIndex = 0;
        Vector<FrameResource> m_frameResources;
        FreeList<RenderItem*> m_allRitems { 20 };

        ID3D12CommandQueue* m_pCmdQueue = nullptr;
        ID3D12GraphicsCommandList* m_pCmdList = nullptr;

        ID3D12Fence* m_pFence = nullptr;
        uint64 m_currentFenceValue = 0;

        friend struct FrameResource;
        friend class RenderTarget;
        friend class LightManager;
    };
}