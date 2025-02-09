/*
 * Copyright (c) Contributors to the Open 3D Engine Project.
 * For complete copyright and license terms please see the LICENSE at the root of this distribution.
 *
 * SPDX-License-Identifier: Apache-2.0 OR MIT
 *
 */

#include <AzCore/Interface/Interface.h>
#include <AzToolsFramework/Prefab/Instance/InstanceToTemplateInterface.h>
#include <AzToolsFramework/Prefab/Undo/PrefabUndoUtils.h>

namespace AzToolsFramework
{
    namespace Prefab
    {
        namespace PrefabUndoUtils
        {
            void AppendAddEntityPatch(
                PrefabDom& patches,
                const PrefabDomValue& newEntityDom,
                const AZStd::string& newEntityAliasPath)
            {
                AZ_Assert(patches.IsArray(), "AppendAddEntityPatch - Provided patches should be an array object DOM value.");

                AppendUpdateValuePatch(patches, newEntityDom, newEntityAliasPath, PatchType::Add);
            }

            void AppendUpdateValuePatch(
                PrefabDom& patches,
                const PrefabDomValue& domValue,
                const AZStd::string& pathToUpdate,
                const PatchType patchType)
            {
                AZ_Assert(patches.IsArray(), "AppendUpdateValuePatch - Provided patches should be an array object DOM value.");

                PrefabDomValue patch(rapidjson::kObjectType);
                rapidjson::Value path = rapidjson::Value(
                    pathToUpdate.data(), aznumeric_caster(pathToUpdate.length()), patches.GetAllocator());
                rapidjson::Value value;
                value.CopyFrom(domValue, patches.GetAllocator(), true);
                if (patchType == PatchType::Add)
                {
                    patch.AddMember(rapidjson::StringRef("op"), rapidjson::StringRef("add"), patches.GetAllocator());
                }
                else if (patchType == PatchType::Edit)
                {
                    patch.AddMember(rapidjson::StringRef("op"), rapidjson::StringRef("replace"), patches.GetAllocator());
                }
                else
                {
                    AZ_Assert(false, "AppendUpdateValuePatch - Unsupported operation type.");
                }
                patch.AddMember(rapidjson::StringRef("path"), AZStd::move(path), patches.GetAllocator())
                    .AddMember(rapidjson::StringRef("value"), AZStd::move(value), patches.GetAllocator());
                patches.PushBack(patch.Move(), patches.GetAllocator());
            }

            void AppendRemovePatch(
                PrefabDom& patches,
                const AZStd::string& pathToRemove)
            {
                AZ_Assert(patches.IsArray(), "AppendRemovePatch - Provided patches should be an array object DOM value.");

                PrefabDomValue removeTargetEntityPatch(rapidjson::kObjectType);
                rapidjson::Value path = rapidjson::Value(pathToRemove.data(),
                    aznumeric_caster(pathToRemove.length()), patches.GetAllocator());
                removeTargetEntityPatch.AddMember(rapidjson::StringRef("op"), rapidjson::StringRef("remove"), patches.GetAllocator())
                    .AddMember(rapidjson::StringRef("path"), AZStd::move(path), patches.GetAllocator());
                patches.PushBack(removeTargetEntityPatch.Move(), patches.GetAllocator());
            }

            void AppendUpdateEntityPatch(
                PrefabDom& patches,
                const PrefabDomValue& entityDomBeforeUpdate,
                const PrefabDomValue& entityDomAfterUpdate,
                const AZStd::string& entityAliasPath)
            {
                AZ_Assert(patches.IsArray(), "AppendUpdateEntityPatch - Provided patches should be an array object DOM value.");

                auto instanceToTemplateInterface = AZ::Interface<InstanceToTemplateInterface>::Get();
                AZ_Assert(instanceToTemplateInterface, "AppendUpdateEntityPatch - Could not get InstanceToTemplateInterface.");

                PrefabDom newPatches(&(patches.GetAllocator()));
                instanceToTemplateInterface->GeneratePatch(newPatches, entityDomBeforeUpdate, entityDomAfterUpdate);
                instanceToTemplateInterface->AppendEntityAliasPathToPatchPaths(newPatches, entityAliasPath);

                for (auto& newPatch : newPatches.GetArray())
                {
                    patches.PushBack(newPatch.Move(), patches.GetAllocator());
                }
            }

            void GenerateUpdateEntityPatch(
                PrefabDom& patches,
                const PrefabDomValue& entityDomBeforeUpdate,
                const PrefabDomValue& entityDomAfterUpdate,
                const AZStd::string& entityAliasPathForPatches)
            {
                auto instanceToTemplateInterface = AZ::Interface<InstanceToTemplateInterface>::Get();
                AZ_Assert(instanceToTemplateInterface, "GenerateUpdateEntityPatch - Could not get InstanceToTemplateInterface.");

                instanceToTemplateInterface->GeneratePatch(patches, entityDomBeforeUpdate, entityDomAfterUpdate);

                instanceToTemplateInterface->AppendEntityAliasPathToPatchPaths(patches, entityAliasPathForPatches);
            }

            void UpdateEntityInInstanceDom(
                PrefabDomReference instanceDom, const PrefabDomValue& entityDom, const AZStd::string& entityAliasPath)
            {
                if (!entityAliasPath.empty())
                {
                    PrefabDomValue endStateCopy(entityDom, instanceDom->get().GetAllocator());

                    PrefabDomPath entityPathInDom(entityAliasPath.c_str());
                    entityPathInDom.Set(instanceDom->get(), endStateCopy.Move());
                }
            }

            void RemoveValueInInstanceDom(PrefabDomReference instanceDom, const AZStd::string& pathToRemove)
            {
                if (!pathToRemove.empty())
                {
                    PrefabDomPath domPathToRemove(pathToRemove.c_str());
                    domPathToRemove.Erase(instanceDom->get());
                }
            }
        } // namespace PrefabUndoUtils
    } // namespace Prefab
} // namespace AzToolsFramework
