/*
 * Copyright (c) Contributors to the Open 3D Engine Project.
 * For complete copyright and license terms please see the LICENSE at the root of this distribution.
 *
 * SPDX-License-Identifier: Apache-2.0 OR MIT
 *
 */
#pragma once

#include <AzCore/Memory/SystemAllocator.h>

#include <Atom/RPI.Reflect/Pass/RenderToTexturePassRGBDData.h>

#include <Atom/RPI.Public/Pass/AttachmentReadback.h>
#include <Atom/RPI.Public/Pass/ParentPass.h>

namespace AZ
{
    namespace RPI
    {
        //! RenderToTexturePassRGBD creates a render target and a child pass tree then have the child pass tree render to this render target.
        //! The RenderToTexturePassRGBD's pass descriptor defines the render target's size and format and the child pass's template name.
        //! RenderToTexturePassRGBD can also read back the render target.
        //! This is useful to render a render pipeline to a render target and (optional) read back its data to cpu memory for later use. 
        class RenderToTexturePassRGBD final
            : public ParentPass
        {
        public:
            AZ_RTTI(RenderToTexturePassRGBD, "{115BCE79-6FD5-47B5-AEAC-4B1818D84CAF}", ParentPass);
            AZ_CLASS_ALLOCATOR(RenderToTexturePassRGBD, SystemAllocator, 0);

            RenderToTexturePassRGBD(const PassDescriptor& descriptor);
            ~RenderToTexturePassRGBD();

            // Pass class need to have its own Create function for its PassFactory Creator
            static Ptr<RenderToTexturePassRGBD> Create(const PassDescriptor& descriptor);

            Ptr<ParentPass> Recreate() const override;
            
            void ReadbackOutput(AZStd::shared_ptr<AttachmentReadback> readback);

            void ResizeOutput(uint32_t width, uint32_t height);

        protected:
            // Pass behavior overrides
            void BuildInternal() override;
            void FrameBeginInternal(FramePrepareParams params) override;

            // Function to be called when output size changed
            void OnUpdateOutputSize();

        private:
            using Base = ParentPass;

            RHI::Scissor m_scissor;
            RHI::Viewport m_viewport;

            // The child pass used to drive rendering
            Ptr<Pass> m_childPass = nullptr;

            // Name of the template used to create the child pass. Needed for Recreate()
            Name m_childTemplateName;

            Ptr<PassAttachment> m_outputAttachment;
            Ptr<PassAttachment> m_outputAttachmentDepth;

            // saved settings for this pass
            RenderToTexturePassRGBDData m_passData;
        };

    }   // namespace RPI
}   // namespace AZ
