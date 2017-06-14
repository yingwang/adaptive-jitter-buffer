//
//  Renderer.hpp
//  AJB
//
//  Created by Ying Wang on 18/06/16.
//  Copyright © 2016 Ying. All rights reserved.
//

#ifndef Renderer_hpp
#define Renderer_hpp

#include "IRenderer.hpp"

namespace AJB {
    
class Renderer : public IRenderer
{
public:
    void RenderFrame(const char* buffer, int length);
    Renderer() {}
};

}

#endif /* Renderer_hpp */
