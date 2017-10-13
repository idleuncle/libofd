#ifndef __OFD_OUTLINE_H__
#define __OFD_OUTLINE_H__

#include "utils/xml.h"

namespace ofd{

    class OutlineElem : public utils::XMLable{
    public:
        OutlineElem();
        virtual ~OutlineElem();

        virtual void GenerateXML(utils::XMLWriter &writer) const override;
        virtual bool FromXML(utils::XMLElementPtr element) override;

    public:
        std::string Title;
        int Count = 0;
        bool Expanded = true;
        // Actions
        // std::vector<CT_Action>
        std::vector<OutlineElemPtr> OutlineElems;

    }; // class OutlineElem

}; // namespace ofd

#endif // __OFD_OUTLINE_H__
