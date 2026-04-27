#include "engine/graphics/Renderer.h"
#include <vector>

static std::vector<std::pair<const std::string, std::shared_ptr<Renderer>(*)()>> RendererTypes;

std::shared_ptr<Renderer> Renderer::Make(std::string classname) {
    for (const auto& makefunc : RendererTypes) {
        if (makefunc.first == classname) return makefunc.second();
    }
    Engine::Error("Tried to instanciate an unknown Renderer type!");
}

void Renderer::AddRenderer(const std::string classname, std::shared_ptr<Renderer> (*makefunc)() ) {
    RendererTypes.emplace_back(classname, makefunc);
}
