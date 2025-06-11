// Copyright (c) 2025 Rafael Valoto/Publisher. All rights reserved.
// Created for: WindowsDualsense_ds5w - Plugin to support DualSense controller on Windows.
// Planned Release Year: 2025


#include "FDualSenseLibraryManager.h"

UFDualSenseLibraryManager* UFDualSenseLibraryManager::Instance;
TMap<int32, UDualSenseLibrary*> UFDualSenseLibraryManager::LibraryInstances;
TMap<int32, bool*> UFDualSenseLibraryManager::IsBroadcasting;


