#ifndef __OFD_TEXTPAGE_H__
#define __OFD_TEXTPAGE_H__

#include <vector>
#include "ofd/TextObject.h"
#include "utils/Geometry.h"

using namespace utils::geometry;

namespace ofd{
    namespace text{

    class TextParagraph;
    typedef std::shared_ptr<TextParagraph> TextParagraphPtr;
    //typedef std::weak_ptr<TextParagraph> TextParagraphRef;
    class TextPage;
    typedef std::shared_ptr<TextPage> TextPagePtr;
    //typedef std::weak_ptr<TextPage> TextPageRef;
    class TextLine;
    typedef std::shared_ptr<TextLine> TextLinePtr;
    //typedef std::weak_ptr<TextLine> TextLineRef;

    class TextLine : public std::enable_shared_from_this<TextLine> {
    public:
        TextLine(TextParagraphPtr textParagraph);
        virtual ~TextLine();

        TextLinePtr GetSelf(){return shared_from_this();};
        //const TextParagraphPtr GetParagraph() const;
        //TextParagraphPtr GetParagraph();

        void AddTextObject(ObjectPtr object);

        Rect GetBoundary() const {return m_boundary;};

    private:
        Rect m_boundary;
        //TextParagraphRef m_textParagraph;
        std::vector<ObjectPtr> m_objects;

    }; // class TextLine

    class TextParagraph : public std::enable_shared_from_this<TextParagraph> {
    public:
        TextParagraph(TextPagePtr textPage);
        virtual ~TextParagraph();

        TextParagraphPtr GetSelf(){return shared_from_this();};
        //const TextPagePtr GetPage() const;
        //TextPagePtr GetPage();

        TextLinePtr AddTextObject(ObjectPtr object);
        size_t GetTextLinesCount() const {return m_textLines.size();};
        TextLinePtr GetTextLine(size_t idx) const {return m_textLines[idx];};

        Rect GetBoundary() const {return m_boundary;};

    private:
        Rect m_boundary;
        //TextPageRef m_textPage;
        std::vector<TextLinePtr> m_textLines;

    }; // TextParagraph

    class TextPage : public std::enable_shared_from_this<TextPage>{
    public:
        TextPage();
        virtual ~TextPage();

        TextPagePtr GetSelf(){return shared_from_this();};
        Rect GetBoundary() const {return m_boundary;};

        TextLinePtr AddTextObject(ObjectPtr object);
        size_t GetTextLinesCount() const{return m_textParagraphs[0]->GetTextLinesCount();};
        TextLinePtr GetTextLine(size_t idx) const {return m_textParagraphs[0]->GetTextLine(idx);};

    private:
        Rect m_boundary;
        std::vector<TextParagraphPtr> m_textParagraphs;
    }; // class TextPage

    }; // namespace text
}; // namespace ofd

#endif // __OFD_TEXTPAGE_H__
