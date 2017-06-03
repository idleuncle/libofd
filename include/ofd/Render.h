#ifndef __OFD_RENDER_H__
#define __OFD_RENDER_H__

#include <memory>
#include <tuple>
#include "ofd/Common.h"
#include "ofd/DrawState.h"

namespace ofd{

    // offsetX, offsetY, scaling
    typedef std::tuple<double, double, double> ViewArea;

    // ======== class Render ========
    class Render {
    public:
        Render();
        virtual ~Render();

        virtual void ClearPage(){};
        virtual void DrawPage(PagePtr page, ViewArea viewArea);
        ViewArea GetViewArea() const;
        void SetViewArea(ViewArea viewArea);

        const DrawState& GetDrawState() const {return m_drawState;};
        DrawState& GetDrawState() {return m_drawState;};
        void SetDrawState(const DrawState &drawState){m_drawState = drawState;};


    private:
        ViewArea m_viewArea;
        DrawState m_drawState;

    }; // class Render

    typedef std::shared_ptr<Render> RenderPtr;

    enum class RenderType{
        Cairo,
    };

    class RenderFactory{
    public:
        //static RenderPtr CreateRenderInstance(RenderType renderType); 
    }; // class RenderFactory

}; // namespace ofd

#endif // __OFD_RENDER_H__
