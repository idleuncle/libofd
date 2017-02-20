#ifndef __OFD_PATHOBJECT_H__
#define __OFD_PATHOBJECT_H__

#include <memory>
#include "ofd/Color.h"
#include "ofd/Object.h"
#include "ofd/Path.h"
#include "ofd/Common.h"

namespace ofd{

    class PathObject;
    typedef std::shared_ptr<PathObject> PathObjectPtr;

    enum class PathRule{
        NonZero = 0,
        EvenOdd,
    };

    // ======== class PathObject ========
    // OFD P52，Page.xsd.
    class PathObject : public Object{
        public:
            static ColorPtr DefaultStrokeColor;
            static ColorPtr DefaultFillColor;

        public:
            PathObject(LayerPtr layer);
            virtual ~PathObject();

            // =============== Public Attributes ================
        public:
            bool                Stroke;      // 是否勾边，默认值为false。
            bool                Fill;        // 是否填充，默认值为true。
            PathRule            Rule;        // 图形的填充规则，当Fill属性存在时出现，
                                             // 默认值为NonZero。
            // 填充色，默认值为透明色。
            ColorPtr GetFillColor() const{return FillColor;};
            void SetFillColor(ColorPtr fillColor){
                if ( !fillColor->Equal(DefaultFillColor) ){
                    FillColor = fillColor;
                }
            };

            // 勾边色，默认值为黑色。
            ColorPtr GetStrokeColor() const{return StrokeColor;};
            void SteStrokeColor(ColorPtr strokeColor){
                if ( !strokeColor->Equal(DefaultStrokeColor) ){
                    StrokeColor = strokeColor;
                }
            };

        private:
            ColorPtr            FillColor;   // 填充色，默认值为透明色。
            ColorPtr            StrokeColor; // 勾边色，默认值为黑色。

            // =============== Public Methods ================
        public:


            // ---------------- Private Attributes ----------------
        public:
            const PathPtr GetPath() const {return m_path;};
            PathPtr GetPath() {return m_path;};
            void SetPath(PathPtr path){m_path = path;};

        protected:
            virtual void GenerateAttributesXML(utils::XMLWriter &writer) const override;
            virtual void GenerateElementsXML(utils::XMLWriter &writer) const override;

            virtual bool FromAttributesXML(utils::XMLElementPtr objectElement) override;
            virtual bool IterateElementsXML(utils::XMLElementPtr childElement) override;

    private:
        PathPtr m_path;

    }; // class OFDPathObject

}; // namespace ofd

#endif // __OFD_PATHOBJECT_H__
