#ifndef __OFD_EXTENSION_H__
#define __OFD_EXTENSION_H__

#include "ofd/Common.h"
#include "utils/xml.h"

namespace ofd{

	class Extension : public utils::XMLable {
	public:
		Extension();
		virtual ~Extension();

        virtual void GenerateXML(utils::XMLWriter &writer) const override;
        virtual bool FromXML(utils::XMLElementPtr element) override;

    public:

	}; // class Extension

	class Extensions : public utils::XMLable{
	public:
		Extensions();
		virtual ~Extensions();

        virtual void GenerateXML(utils::XMLWriter &writer) const override;
        virtual bool FromXML(utils::XMLElementPtr element) override;

    public:

	}; // class Extensions

}; // namespace ofd
    
#endif // __OFD_EXTENSION_H__