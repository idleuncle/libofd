#ifndef __OFD_PERMISSION_H__
#define __OFD_PERMISSION_H__

#include "utils/xml.h"
using namespace utils;

namespace ofd{

    class Permission : public XMLable{
    public:
        Permission();
        virtual ~Permission();

        virtual void GenerateXML(utils::XMLWriter &writer) const override;
        virtual bool FromXML(utils::XMLElementPtr objectElement) override;
        
    public:
        bool Edit = true;
        bool Annot = true;
        bool Export = true;
        bool Signature = true;
        bool Watermark = true;
        bool PrintScreen = true;
        // Print
        bool PrintRestriction = false;
        bool Printable = true;
        int Copies = -1;

        // ValidPeriod
        bool ValidPeriodRestriction = false;
        std::string StartDate;
        std::string EndDate;

    }; // class Permission

}; // namespace ofd

#endif // __OFD_PERMISSION_H__
