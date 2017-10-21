#ifndef __OFD_CUSTOMTAG_H__
#define __OFD_CUSTOMTAG_H__

#include "ofd/Common.h"
#include "utils/xml.h"

namespace ofd{

	class CustomTag : public utils::XMLable {
	public:
		CustomTag();
		virtual ~CustomTag();

        virtual void GenerateXML(utils::XMLWriter &writer) const override;
        virtual bool FromXML(utils::XMLElementPtr element) override;

    public:

	}; // class CustomTag

	class CustomTags : public utils::XMLable{
	public:
		CustomTags();
		virtual ~CustomTags();

        virtual void GenerateXML(utils::XMLWriter &writer) const override;
        virtual bool FromXML(utils::XMLElementPtr element) override;

    public:

	}; // class CustomTags

}; // namespace ofd
    
#endif // __OFD_CUSTOMTAG_H__