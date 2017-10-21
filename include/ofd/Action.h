#ifndef __OFD_ACTION_H__
#define __OFD_ACTION_H__

#include "ofd/Common.h"
#include "utils/xml.h"

namespace ofd{

	class Action : public utils::XMLable {
	public:
		Action();
		virtual ~Action();

        virtual void GenerateXML(utils::XMLWriter &writer) const override;
        virtual bool FromXML(utils::XMLElementPtr element) override;

    public:

	}; // class Action

	class Actions : public utils::XMLable{
	public:
		Actions();
		virtual ~Actions();

        virtual void GenerateXML(utils::XMLWriter &writer) const override;
        virtual bool FromXML(utils::XMLElementPtr element) override;

    public:

	}; // class Actions

}; // namespace ofd
    
#endif // __OFD_ACTION_H__