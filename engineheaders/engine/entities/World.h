#pragma once

#include <memory>
#include <vector>
#include <deque>
#include <algorithm>
#include "engine/filesystem/ADF.h"
#include "engine/master.h"
#include "engine/graphics/RWorld.h"
#include "engine/graphics/Renderer.h"

struct iEntHandler;

class EntityStorage : public std::vector<std::shared_ptr<iEntHandler>> {
    static inline constexpr int ResizeAdditionalSlotAmount = 16;

public:
    void AddEntityBack(std::shared_ptr<iEntHandler> Entity);

    int GetFreeIndex();

    void Update();
    void Clear();
};


class ENGINEEXPORT World : public EntityStorage {
    std::shared_ptr<RWorld> RenderWorld;
    std::string MapName = "";

public:
    void Restore(const ADFEntry& Saved);
    ADFEntry Save();

    //! Returns an uninitalized entity, or nullptr if classname is not valid.
    std::shared_ptr<iEntHandler> MakeEntity(std::string classname, std::optional<iEntHandler*> parent = std::nullopt);

    void Clear();

    World(std::shared_ptr<RWorld> Renderworld);
    World(std::shared_ptr<Renderer> Renderer);

    std::shared_ptr<RWorld> GetRWorld() { return RenderWorld; }

    void EntityStorageFromADF(const ADFEntry& Saved, EntityStorage* Storage, std::optional<iEntHandler*> parent = std::nullopt);
    ADFEntry EntityStorageToADF(EntityStorage* Storage);
};