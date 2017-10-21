#ifndef __OFD_ANNOTATION_H__
#define __OFD_ANNOTATION_H__

#include "ofd/Common.h"
#include "utils/xml.h"

namespace ofd{

	class Annotation : public utils::XMLable {
	public:
		Annotation();
		virtual ~Annotation();

        virtual void GenerateXML(utils::XMLWriter &writer) const override;
        virtual bool FromXML(utils::XMLElementPtr element) override;

    public:
    	ST_RefID PageID;
    	ST_Loc FileLoc;
	}; // class Annotation

	class Annotations : public utils::XMLable {
	public:
		Annotations();
		virtual ~Annotations();

        virtual void GenerateXML(utils::XMLWriter &writer) const override;
        virtual bool FromXML(utils::XMLElementPtr element) override;

    public:
	}; // class Annotations

    class Annot : public utils::XMLable{
	public:
		Annot();
		virtual ~Annot();

        virtual void GenerateXML(utils::XMLWriter &writer) const override;
        virtual bool FromXML(utils::XMLElementPtr element) override;

    public:
        ST_ID ID;
        std::string Type;
        std::string Creator;
        std::string LastModDate;
        bool Visibule = true;
        std::string Subtype;
        bool Print = true;
        bool NoZoom = false;
        bool NoRotate = false;
        bool ReadOnly = true;

        std::string Remark;
        std::vector<std::string> Parameters;
        // CT_PageBlock
        ObjectArray m_objects;
    }; // class Annot

    class PageAnnot : public utils::XMLable{
	public:
		PageAnnot();
		virtual ~PageAnnot();

        virtual void GenerateXML(utils::XMLWriter &writer) const override;
        virtual bool FromXML(utils::XMLElementPtr element) override;

    public:
    }; // class PageAnnot

}; // namespace ofd
    
#endif // __OFD_ANNOTATION_H__
