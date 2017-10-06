#include <assert.h>
#include "ofd/TextPage.h"

namespace ofd{
    namespace text{

// ==================== class TextLine ====================

TextLine::TextLine(TextParagraphPtr textParagraph)
    //m_textParagraph(textParagraph)
{
}

TextLine::~TextLine(){
}

//const TextParagraphPtr TextLine::GetParagraph() const{
    //return m_textParagraph.lock();
//}

//TextParagraphPtr TextLine::GetParagraph(){
    //return m_textParagraph.lock();
//}

void TextLine::AddTextObject(ObjectPtr object){
    double x0 = object->Boundary.XMin;
    double y0 = object->Boundary.YMin;
    double width = object->Boundary.Width();
    double height = object->Boundary.Height();
    double x1 = x0 + width;
    double y1 = y0 + height;
    if (m_objects.size() == 0){
        m_boundary.x = x0;
        m_boundary.y = y0;
        m_boundary.width = width;
        m_boundary.height = height;
    } else {
        double xx0 = m_boundary.x;
        double yy0 = m_boundary.y;
        double xx1 = xx0 + m_boundary.width;
        double yy1 = yy0 + m_boundary.height;
        m_boundary.x = xx0 <= x0 ? xx0 : x0;
        m_boundary.y = yy0 <= y0 ? yy0 : y0;
        xx1 = xx1 >= x1 ? xx1 : x1;
        yy1 = yy1 >= y1 ? yy1 : y1;
        m_boundary.width = xx1 - xx0;
        m_boundary.height = yy1 - yy0;
    }

    m_objects.push_back(object);
}

// ==================== class TextParagraph ====================

TextParagraph::TextParagraph(TextPagePtr textPage)
    //m_textPage(textPage)
{
}

TextParagraph::~TextParagraph(){
}

//const TextPagePtr TextParagraph::GetPage() const{
    //return m_textPage.lock();
//}

//TextPagePtr TextParagraph::GetPage(){
    //return m_textPage.lock();
//}

TextLinePtr TextParagraph::AddTextObject(ObjectPtr object){
    TextLinePtr theLine = nullptr;

    if (m_textLines.size() == 0){
        theLine = std::make_shared<TextLine>(GetSelf());
        theLine->AddTextObject(object);
        m_textLines.push_back(theLine);
    } else {
        for (auto textLine : m_textLines){
            Rect lineBoundary = textLine->GetBoundary();
            double y = object->Boundary.YMin;
            if (fabs(y - lineBoundary.y) < 4){
                textLine->AddTextObject(object);
                theLine = textLine;
                break;
            }
        }
        if (theLine == nullptr){
            theLine = std::make_shared<TextLine>(GetSelf());
            theLine->AddTextObject(object);
            m_textLines.push_back(theLine);
        }
    }

    assert(theLine != nullptr);
    return theLine;
}

// ==================== class TextPage ====================
 
TextPage::TextPage(){
    TextParagraphPtr textParagraph = std::make_shared<TextParagraph>(nullptr);
    //TextParagraphPtr textParagraph = std::make_shared<TextParagraph>(GetSelf());
    m_textParagraphs.push_back(textParagraph);
}

TextPage::~TextPage(){
}

TextLinePtr TextPage::AddTextObject(ObjectPtr object){
    //TextObject *textObject = (TextObject*)object.get();
    return m_textParagraphs[0]->AddTextObject(object);
}

}; // namespace text
}; // namespace ofd
