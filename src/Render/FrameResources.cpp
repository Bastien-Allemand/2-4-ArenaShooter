#include "pch.h"
#include "FrameResources.h"

#include "RenderCamera.h"
#include "RenderContext.h"

namespace gce
{
    FrameResource::FrameResource():
        m_pPassCB(1)
    {
        ThrowIfFailed(RenderContext::GetDevice()->CreateCommandAllocator(
            D3D12_COMMAND_LIST_TYPE_DIRECT,
            IID_PPV_ARGS(&m_pCommandAllocator)), "Failed to create frame ressource cmd allocator");

        m_pPassCB.Put(new DynamicBuffer(sizeof(Matrix)));
        m_pPassCB[0]->SetName(L"PassBuffer");
        
        m_pLightsCB = new DynamicBuffer(sizeof(LightPassData));
        m_pLightsCB->SetName(L"LightPassBuffer");
    }

    FrameResource::~FrameResource()
    {
        m_pCommandAllocator->Release();

        for (int32 i = 0; i < m_pPassCB.Capacity(); i++)
            if(m_pPassCB.IsEmpty(i) == false) delete m_pPassCB[i];

        for (uint32 i = 0; i < m_pObjDatasCB.Capacity(); ++i) {
            if (!m_pObjDatasCB.IsEmpty(i)) {
                delete m_pObjDatasCB[i];
            }
        }

       delete m_pLightsCB; 
    }

    //==== MANAGER ====

    FrameResourcesManager::FrameResourcesManager()
    {
        ThrowIfFailed(RenderContext::GetDevice()->CreateFence(m_currentFenceValue, D3D12_FENCE_FLAG_SHARED, IID_PPV_ARGS(&m_pFence)),
            "Failed to create frame ressource fence");

        m_frameResources.Reserve(s_numFrameResources);
        for (int32 i = 0; i < s_numFrameResources; ++i)
        {
            m_frameResources.std.emplace_back();
        }

        D3D12_COMMAND_QUEUE_DESC queueDesc = {};
        queueDesc.NodeMask = 0;
        queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
        queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
        ThrowIfFailed(RenderContext::GetDevice()->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&m_pCmdQueue)), "Failed to create frame ressource cmd queue");

        RenderContext::GetDevice()->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, m_frameResources[0].m_pCommandAllocator, nullptr, IID_PPV_ARGS(&m_pCmdList));
        m_pCmdList->Close();

    }

    FrameResourcesManager::~FrameResourcesManager()
    {
        m_pFence->Release();
        m_pCmdList->Release();
        m_pCmdQueue->Release();
    }

    void FrameResourcesManager::BeginFrame()
    {
        FrameResourcesManager& instance = Instance();
        instance.m_currentFrameIndex = (instance.m_currentFrameIndex + 1) % s_numFrameResources;

        FrameResource* currentFrameResource = GetCurrentFrameResource();

        // Debug logging
        //OutputDebugStringA(("BeginFrame - FrameIndex: " + std::to_string(instance.m_currentFrameIndex) +
        //          ", BeginFrame - Current Fence: " + std::to_string(m_currentFenceValue) +
        //          ", GPU Fence: " + std::to_string(m_fence->GetCompletedValue()) +
        //          ", CPU / Frame Fence: " + std::to_string(currentFrameResource->m_fenceValue) + "\n").c_str());

        while (instance.m_pFence->GetCompletedValue() < instance.m_currentFenceValue)
        {
            Sleep(1);

            HANDLE eventHandle = CreateEventEx(nullptr, nullptr, false, EVENT_ALL_ACCESS);
            if (eventHandle)
            {
                instance.m_pFence->SetEventOnCompletion(currentFrameResource->m_fenceValue, eventHandle);
                WaitForSingleObject(eventHandle, INFINITE);
                CloseHandle(eventHandle);
            }
        }

        currentFrameResource->m_pCommandAllocator->Reset();
        instance.m_pCmdList->Reset(currentFrameResource->m_pCommandAllocator, nullptr);
    }

    void FrameResourcesManager::EndFrame()
    {
        FrameResourcesManager& instance = Instance();
        instance.m_pCmdList->Close();

        ID3D12CommandList* lists[] = { instance.m_pCmdList };
        instance.m_pCmdQueue->ExecuteCommandLists(1, lists);

        instance.m_currentFenceValue++;
        instance.m_pCmdQueue->Signal(instance.m_pFence, instance.m_currentFenceValue);
    }

    void FrameResourcesManager::AddRenderItem(RenderItem& item)
    {
        FrameResourcesManager& instance = Instance();

        item.numFramesDirty = s_numFrameResources;
        item.id = instance.m_allRitems.Put(&item);

        for (int32 i = 0; i < s_numFrameResources; ++i)
        {
            DynamicBuffer* buffer = new DynamicBuffer(sizeof(ObjectData));
            instance.m_frameResources[i].m_pObjDatasCB.Put(buffer);
            buffer->SetName(L"ObjCBuffer");
        }
    }

    void FrameResourcesManager::AddCamera( RenderCamera& camera )
    {
        FrameResourcesManager& instance = Instance();
        
        for (int32 i = 0; i < s_numFrameResources; ++i)
        {
            DynamicBuffer* buffer = new DynamicBuffer(sizeof(PassData));
            camera.m_cameraId = instance.m_frameResources[i].m_pPassCB.Put(buffer);
            buffer->SetName(L"PassDataBuffer");
        }
    }

    void FrameResourcesManager::RemoveRenderItem(RenderItem& item)
    {
        item.toDestroy = true;
    }

    void FrameResourcesManager::Flush()
    {
        FrameResource* currentFrameResource = GetCurrentFrameResource();
        m_currentFenceValue++;
        Instance().m_pCmdQueue->Signal(m_pFence, m_currentFenceValue);

        if (m_pFence->GetCompletedValue() < m_currentFenceValue)
        {
            HANDLE eventHandle = CreateEventEx(nullptr, L"", false, EVENT_ALL_ACCESS);
            m_pFence->SetEventOnCompletion(m_currentFenceValue, eventHandle);
            WaitForSingleObject(eventHandle, INFINITE);
            CloseHandle(eventHandle);
        }
    }

    FrameResource* FrameResourcesManager::GetCurrentFrameResource()
    {
        return &Instance().m_frameResources[Instance().m_currentFrameIndex];
    }

    void FrameResourcesManager::UpdateObjCBuffer() 
    {
        FrameResourcesManager& instance = Instance();
        if (instance.m_allRitems.Size() == 0)
            return;

        for (auto& item : instance.m_allRitems)
        {
        	GetCurrentFrameResource()->m_pObjDatasCB[item->id]->CopyData(item->objData); //TODO MOVE THAT !
 /*           if (item->numFramesDirty > 0)
            {
                item->numFramesDirty--;
            }*/
        }
    }
}
