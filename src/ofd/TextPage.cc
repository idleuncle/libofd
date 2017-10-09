#include <assert.h>
#include "ofd/TextPage.h"
#include "utils/logger.h"

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
    double x0 = object->Boundary.XMin();
    double y0 = object->Boundary.YMin();
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

void TextLine::SelectText(double x0, double y0, double x1, double y1){
    double xx0 = m_boundary.x;
    double yy0 = m_boundary.y;
    double xx1 = xx0 + m_boundary.width;
    double yy1 = yy0 + m_boundary.height;
    bool bSelected = false;
    if (x0 > xx1 || x1 < xx0 || y0 > yy1 || y1 < yy0){
        bSelected = false;
    } else {
        LOG_DEBUG("SelectText() point:(%.2f, %.2f), (%.2f, %.2f)", x0, x1, y0, y1);
        LOG_NOTICE("SelectText() text:(%.2f, %.2f), (%.2f, %.2f)", xx0, xx1, yy0, yy1);
        LOG_INFO("x0 > xx1:%d x1 < xx0:%d y0 > yy1:%d y1 < yy0:%d", x0 > xx1, x0 < xx0, y0 > yy1, y1 < yy0);
        bSelected = true;
    }
    for (auto obj : m_objects){
        if (obj->Type == ObjectType::TEXT){
            if (bSelected){
                TextObject *textObject = (TextObject*)obj.get();
                const Text::TextCode &textCode = textObject->GetTextCode(0);
                LOG_NOTICE("SelectText() %s", textCode.Text.c_str());
                //uint32_t startIndex = (uint32_t)-1;
                //uint32_t endIndex = (uint32_t)-1;
                //uint64_t flags = (uint64_t)startIndex << 32 & (uint64_t)endIndex;
                obj->Flags = 998;
            } else {
                obj->Flags = 0;
            }
        }
    }

    //for (auto obj : m_objects){
        //if (obj->Type == ObjectType::TEXT){
            //TextObject *textObject = (TextObject*)obj.get();
            //double xx0 = obj->Boundary.XMin();
            //double yy0 = obj->Boundary.YMin();
            //double xx1 = obj->Boundary.XMax();
            //double yy1 = obj->Boundary.YMax();
            //if (!(x0 > xx1 || x1 < xx0 || y0 > yy1 || y1 < yy0)){
                //const Text::TextCode &textCode = textObject->GetTextCode(0);
                //LOG_NOTICE("SelectText() %s", textCode.Text.c_str());
                //uint32_t startIndex = (uint32_t)-1;
                //uint32_t endIndex = (uint32_t)-1;
                //uint64_t flags = (uint64_t)startIndex << 32 & (uint64_t)endIndex;
                //obj->Flags = flags;
            //}
        //}
    //}
}

void TextLine::UnselectText(){
    for (auto obj : m_objects){
        if (obj->Type == ObjectType::TEXT){
            obj->Flags = 0;
        }
    }
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
            double y = object->Boundary.YMin();
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

void TextParagraph::SelectText(double x0, double y0, double x1, double y1){
    for (auto textLine : m_textLines){
        textLine->SelectText(x0, y0, x1, y1);
    }
}

void TextParagraph::UnselectText(){
    for (auto textLine : m_textLines){
        textLine->UnselectText();
    }
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


void TextPage::SelectText(double x0, double y0, double x1, double y1){
    for (auto textParagraph : m_textParagraphs){
        textParagraph->SelectText(x0, y0, x1, y1);
    }
}

void TextPage::UnselectText(){
    for (auto textParagraph : m_textParagraphs){
        textParagraph->UnselectText();
    }
}

}; // namespace text
}; // namespace ofd
