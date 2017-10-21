#ifndef __OFD_ATTACHMENT_H__
#define __OFD_ATTACHMENT_H__

#include "ofd/Common.h"
#include "utils/xml.h"

namespace ofd{

    class Attachment : public utils::XMLable {
    public:
        Attachment();
        virtual ~Attachment();

        virtual void GenerateXML(utils::XMLWriter &writer) const override;
        virtual bool FromXML(utils::XMLElementPtr element) override;

    public:
        ST_RefID ID=0;
        std::string Name;
        std::string Format;
        std::string CreationDate;
        std::string ModDate;
        double Size = 0;
        bool Visible = true;
        std::string Usage = "none";
        ST_Loc FileLoc;


    }; // class Attachment

    class Attachments : public utils::XMLable{
    public:
        Attachments();
        virtual ~Attachments();

        virtual void GenerateXML(utils::XMLWriter &writer) const override;
        virtual bool FromXML(utils::XMLElementPtr element) override;

    public:

    }; // class Attachments
    
}; // namespace ofd

#endif // __OFD_ATTACHMENT_H__
