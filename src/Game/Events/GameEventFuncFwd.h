
#pragma once

#include <functional>

struct DamageEvent;
struct RemoveEntityEvent;
struct ShockwaveEvent;
struct SpawnConstraintEvent;
struct DespawnConstraintEvent;
struct SpawnEntityEvent;
struct SpawnPhysicsEntityEvent;

using DamageFunc = std::function<bool (const DamageEvent&)>; 
using RemoveEntityFunc = std::function<bool (const RemoveEntityEvent&)>;
using ShockwaveFunc = std::function<bool (const ShockwaveEvent&)>;
using SpawnConstraintFunc = std::function<bool (const SpawnConstraintEvent&)>;
using DespawnConstraintFunc = std::function<bool (const DespawnConstraintEvent&)>;
using SpawnEntityFunc = std::function<bool (const SpawnEntityEvent&)>;
using SpawnPhysicsEntityFunc = std::function<bool (const SpawnPhysicsEntityEvent&)>;
