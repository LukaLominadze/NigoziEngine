#pragma once

#include "ngpch.h"

#include "Nigozi/Layers/GameLayer.h"
#include "Nigozi/Components/Transform.h"

namespace Nigozi
{
    class TransformLayer : public GameLayer<Transform>
    {
    public:
        TransformLayer() = default;
        ~TransformLayer() = default;

        virtual void SetIndex(uint32_t layerIndex) override;

        virtual void OnAttach() override;
        virtual void OnUpdate() override;
        virtual void OnRender() override;
        virtual void OnDetach() override;

        BUILD_GET_INDEX_FUNC
    };
}

