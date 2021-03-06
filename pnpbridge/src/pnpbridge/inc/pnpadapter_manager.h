// Copyright (c) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#pragma once

#ifdef __cplusplus
extern "C"
{
#endif

// PnpAdapterManger's representation of a PNPADPTER and its configuration
typedef struct _PNP_ADAPTER_TAG {
    PPNP_ADAPTER adapter;

    // List of pnp interfaces created under this adapter
    SINGLYLINKEDLIST_HANDLE pnpInterfaceList;

    // Lock to protect pnpInterfaceList modification
    LOCK_HANDLE InterfaceListLock;
} PNP_ADAPTER_TAG, *PPNP_ADAPTER_TAG;

// Structure uses to share context between adapter manager and adapter interface
typedef struct _PNP_ADAPTER_CONTEXT_TAG {
    PPNP_ADAPTER_TAG adapter;
    JSON_Object* deviceConfig;
} PNP_ADAPTER_CONTEXT_TAG, *PPNP_ADAPTER_CONTEXT_TAG;

// Structure used for an instance of Pnp Adapter Manager
typedef struct _PNP_ADAPTER_MANAGER {
    MAP_HANDLE pnpAdapterMap;
    PPNP_ADAPTER_TAG* pnpAdapters;
} PNP_ADAPTER_MANAGER, *PPNP_ADAPTER_MANAGER;

typedef enum {
    PublishInvalid,
    PublishAlways,
    PublishOnDeviceFound
} PublishMode;

// Pnp adapter interface structure
typedef struct _PNPADAPTER_INTERFACE_TAG {
    void* context;
    int key;
    PublishMode publishMode;
    char* interfaceId;
    DIGITALTWIN_INTERFACE_CLIENT_HANDLE pnpInterfaceClient;
    PNPADPATER_INTERFACE_PARAMS params;
    PNPADAPTER_CONTEXT adapterContext;
    LIST_ITEM_HANDLE adapterEntry;
} PNPADAPTER_INTERFACE_TAG, *PPNPADAPTER_INTERFACE_TAG;

/**
* @brief    PnpAdapterManager_Create creates the Azure Pnp Interface adapter manager.
*
* @remarks  PnpAdapterManager_Create initializes all the available Pnp Interface adapters by calling their
            initialize method, if implemented. The Pnp Interface adapter is added to a PnpAdapterMap,
            whose key will be the PnpAdapters identity. This identity will be used to find the PnpAdapter
            based on the discovery extension change message.

* @param    adapter           Pointer to get back an initialized PPNP_ADAPTER_MANAGER.
*
* @returns  PNPBRIDGE_OK on success and other PNPBRIDGE_RESULT values on failure.
*/
PNPBRIDGE_RESULT PnpAdapterManager_Create(PPNP_ADAPTER_MANAGER* adapter, JSON_Value* config);

/**
* @brief    PnpAdapterManager_Release uninitializes the PnpAdapter resources.
*
* @remarks  PnpAdapterManager_Release calls shutdown on all the Pnp adapters and cleans up the PnpAdapterManager

* @param    adapter           Pointer to an initialized PPNP_ADAPTER_MANAGER.
*
* @returns  VOID.
*/
void PnpAdapterManager_Release(PPNP_ADAPTER_MANAGER adapter);

PNPBRIDGE_RESULT PnpAdapterManager_SupportsIdentity(PPNP_ADAPTER_MANAGER adapterMgr, JSON_Object* Message, bool* supported, int* key);
PNPBRIDGE_RESULT PnpAdapterManager_CreatePnpInterface(PPNP_ADAPTER_MANAGER adapterMgr, MX_IOT_HANDLE_TAG* IotHandle, int key, JSON_Object* deviceConfig, PNPMESSAGE DeviceChangeMessage);

PNPBRIDGE_RESULT PnpAdapterManager_GetAllInterfaces(PPNP_ADAPTER_MANAGER adapterMgr, DIGITALTWIN_INTERFACE_CLIENT_HANDLE** interfaces, int* count);
bool PnpAdapterManager_IsInterfaceIdPublished(PPNP_ADAPTER_MANAGER adapterMgr, const char* interfaceId);
void PnpAdapterManager_InvokeStartInterface(PPNP_ADAPTER_MANAGER adapterMgr);
void PnpAdapterManager_ReleaseAdapter(PPNP_ADAPTER_TAG adapterTag);

void PnpAdapterManager_AddInterface(PPNP_ADAPTER_TAG adapter, PNPADAPTER_INTERFACE_HANDLE pnpAdapterInterface);
void PnpAdapterManager_RemoveInterface(PPNP_ADAPTER_TAG adapter, PNPADAPTER_INTERFACE_HANDLE pnpAdapterInterface);


#ifdef __cplusplus
}
#endif