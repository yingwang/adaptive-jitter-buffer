//
//  IRenderer.hpp
//  AJB
//
//  Created by Ying Wang on 17/06/16.
//  Copyright © 2016 Ying. All rights reserved.
//

#ifndef IRenderer_h
#define IRenderer_h

namespace AJB {
    
class IRenderer
{
public:
    /*
     Renders the given buffer. This call will not block for any significant period, and the
     buffer will be copied internally so can be deleted/reused as soon as this call is
     completed.
     */
    virtual void RenderFrame(const char* buffer, int length) = 0;
    virtual ~IRenderer() {}
};

}
#endif /* IRenderer_h */
