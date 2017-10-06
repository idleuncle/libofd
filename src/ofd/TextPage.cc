#include <assert.h>
#include "ofd/TextPage.h"

using namespace ofd;

// ==================== class TextLine ====================

TextLine::TextLine(TextParagraphPtr textParagraph):
    m_textParagraph(textParagraph)
{
}

TextLine::~TextLine(){
}

const TextParagraphPtr TextLine::GetParagraph() const{
    return m_textParagraph.lock();
}

TextParagraphPtr TextLine::GetParagraph(){
    return m_textParagraph.lock();
}

void TextLine::AddTextObject(ObjectPtr object){
    if (m_objects.size() == 0){
        m_boundary.x = object->Boundary.XMin;
        m_boundary.y = object->Boundary.YMin;
        m_boundary.width = object->Boundary.Width();
        m_boundary.height = object->Boundary.Height();
    } else {
    }

    m_objects.push_back(object);
}

// ==================== class TextParagraph ====================

TextParagraph::TextParagraph(TextPagePtr textPage):
    m_textPage(textPage)
{
}

TextParagraph::~TextParagraph(){
}

const TextPagePtr TextParagraph::GetPage() const{
    return m_textPage.lock();
}

TextPagePtr TextParagraph::GetPage(){
    return m_textPage.lock();
}

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
            if (fabs(y - lineBoundary.y) < 1e-3){
                textLine->AddTextObject(object);
                theLine = textLine;
                break;
            }
        }
        if (theLine == nullptr){
            theLine = std::make_shared<TextLine>(GetSelf());
            theLine->AddTextObject(object);
        }
    }

    assert(theLine != nullptr);
    return theLine;
}

// ==================== class TextPage ====================

TextPage::TextPage(){
    m_textParagraphs.push_back(std::make_shared<TextParagraph>(GetSelf()));
}

TextPage::~TextPage(){
}

TextLinePtr TextPage::AddTextObject(ObjectPtr object){
    //TextObject *textObject = (TextObject*)object.get();
    return m_textParagraphs[0]->AddTextObject(object);
}
