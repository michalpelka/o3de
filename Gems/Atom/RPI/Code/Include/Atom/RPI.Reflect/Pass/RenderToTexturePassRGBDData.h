/*
 * Copyright (c) Contributors to the Open 3D Engine Project.
 * For complete copyright and license terms please see the LICENSE at the root of this distribution.
 *
 * SPDX-License-Identifier: Apache-2.0 OR MIT
 *
 */
#pragma once

#include <Atom/RPI.Reflect/Pass/PassData.h>

namespace AZ
{
    namespace RPI
    {
        struct RenderToTexturePassRGBDData
            : public PassData
        {
            AZ_RTTI(RenderToTexturePassRGBDData, "{57EF2E64-4255-46CD-8D72-E4631887D84E}", PassData);
            AZ_CLASS_ALLOCATOR(RenderToTexturePassRGBDData, SystemAllocator, 0);

            RenderToTexturePassRGBDData() = default;
            virtual ~RenderToTexturePassRGBDData() = default;

            static void Reflect(ReflectContext* context)
            {
                if (auto* serializeContext = azrtti_cast<SerializeContext*>(context))
                {
                    serializeContext->Class<RenderToTexturePassRGBDData, PassData>()
                        ->Version(0)
                        ->Field("OutputWidth", &RenderToTexturePassRGBDData::m_width)
                        ->Field("OutputHeight", &RenderToTexturePassRGBDData::m_height)
                        ->Field("OutputFormatColor", &RenderToTexturePassRGBDData::m_format_color)
                        ->Field("OutputFormatDepth", &RenderToTexturePassRGBDData::m_format_depth);
                }
            }

            uint32_t m_width = 256;
            uint32_t m_height = 256;
            RHI::Format m_format_color = RHI::Format::R8G8B8A8_UNORM;
            RHI::Format m_format_depth = RHI::Format::R8G8B8A8_UNORM;

        };
    } // namespace RPI
} // namespace AZ

