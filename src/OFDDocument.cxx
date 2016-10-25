#include <iostream>
#include <sstream>
#include <iterator>
#include "OFDPackage.h"
#include "OFDDocument.h"
#include "OFDPage.h"
#include "utils.h"
#include "logger.h"

#include <tinyxml2.h>
using namespace tinyxml2;

using namespace ofd;

OFDDocument::OFDDocument(OFDPackage *package, const std::string &filename)
    : m_package(package), m_filename(filename), m_opened(false), 
    m_rootDir(filename.substr(0, filename.rfind('/'))) {
        m_attributes.clear();
}


OFDDocument:: ~OFDDocument() {
    this->Close();
}

bool OFDDocument::Open() {
    if ( IsOpened() ) {
        LOG(DEBUG) << "Document is already opened.";
        return true;
    }

    m_opened = false;

    if ( m_package == nullptr ) {
        LOG(WARNING) << "m_package == nullptr.";
        return false;
    }

    if ( parseXML() ){
        if ( parsePublicResXML() ){
            if ( parseDocumentResXML() ){
                if ( !loadFonts() ){
                    LOG(WARNING) << "loadFonts() failed.";
                }
                m_opened = true;
            } else {
                LOG(WARNING) << "parseDocumentResXML() failed.";
            }
        } else {
            LOG(WARNING) << "parsePublicResXML() failed.";
        }
    } else {
        LOG(WARNING) << "parseXML() failed.";
    }

    return IsOpened();
}

void OFDDocument::Attributes::clear() {
    CommonData.clear();
    //for ( size_t i = 0 ; i < Pages.size() ; i++ ) {
        //OFDPage *page = Pages[i];
        //if ( page != nullptr ){
            //delete page;
        //}
    //}
    Pages.clear();
}

void OFDDocument::Close() {
    if ( !IsOpened() ) return;

    m_attributes.clear();
}

std::string OFDDocument::String() const {
    std::stringstream ss;
    ss << std::endl 
        << "------------------------------" << std::endl
        << "OFDDocument" << std::endl 
        << "PhysicalBox: " << m_attributes.CommonData.PageArea.PhysicalBox.x0 << ", " 
        << m_attributes.CommonData.PageArea.PhysicalBox.y0 << ", "
        << m_attributes.CommonData.PageArea.PhysicalBox.x1 << ", "
        << m_attributes.CommonData.PageArea.PhysicalBox.y1 << std::endl;
    ss << "PublicRes: " << m_attributes.CommonData.PublicRes << std::endl;
    ss << "DocumentRes: " << m_attributes.CommonData.DocumentRes << std::endl;
    ss << "MaxUnitID: " << m_attributes.CommonData.MaxUnitID; 
    ss << std::endl
       << "------------------------------" << std::endl;

    return ss.str();
}

        //OFDPackagePtr p = m_package.lock(); 
        //if ( p == nullptr ) return false; 

#define OFDPACKAGE_GET_FILE_CONTENT(filename, content) \
    std::string content; \
    { \
        bool ok = false; \
        std::tie(content, ok) = m_package->GetFileContent(filename); \
        if ( !ok ) { \
            LOG(WARNING) << "package->GetFileContent() failed. filename: " << filename; \
            return false; \
        } \
    }

#define TEXT_TO_XML(content, xmldoc) \
    XMLDocument *xmldoc = new XMLDocument(); \
    { \
        XMLError rc = xmldoc->Parse(content.c_str()); \
        if ( rc != XML_SUCCESS ){ \
            LOG(WARNING) << content; \
            LOG(WARNING) << "xmldoc->Parse() failed."; \
            delete xmldoc; \
            return false; \
        } \
    } \


/*
<?xml version="1.0" encoding="UTF-8"?>

<ofd:Res xmlns:ofd="http://www.ofdspec.org" BaseLoc="Res">
  <ofd:Fonts>
    <ofd:Font ID="16" FontName="PBCDEE" FamilyName="PBCDEE" Charset="unicode"><ofd:FontFile>font_2.otf</ofd:FontFile></ofd:Font>
    <ofd:Font ID="371" FontName="SBCDEE" FamilyName="SBCDEE" Charset="unicode"><ofd:FontFile>font_8.otf</ofd:FontFile></ofd:Font>
  </ofd:Fonts>

  <ofd:ColorSpaces>
    <ofd:ColorSpace ID="4" Type="CMYK"/>
    <ofd:ColorSpace ID="15" Type="Gray"/>
  </ofd:ColorSpaces>

</ofd:Res>
*/
bool OFDDocument::parsePublicResXML(){
    std::string publicResFileName = m_rootDir + "/" + m_attributes.CommonData.PublicRes;

    OFDPACKAGE_GET_FILE_CONTENT(publicResFileName, content);

    TEXT_TO_XML(content, xmldoc);

    
    // <ofd:Res>
    XMLElement *rootElement = xmldoc->RootElement();
    if ( rootElement != nullptr ){
        std::string BaseLoc = rootElement->Attribute("BaseLoc");
        m_attributes.PublicRes.BaseLoc = BaseLoc;
        VLOG(3) << "PublicRes Root Element Name: " << rootElement->Name() << " BaseLoc: " << BaseLoc;
        VLOG(3) << GetChildElements(rootElement);


        // <ofd:Fonts>
        const XMLElement *fontsElement = rootElement->FirstChildElement("ofd:Fonts");
        if ( fontsElement != nullptr ){
            //VLOG(3) << GetChildElements(fontsElement);
            //// <ofd:Font>
            const XMLElement *fontElement = fontsElement->FirstChildElement("ofd:Font");
            while ( fontElement != nullptr ){
                OFDFont font;
                font.ID = fontElement->IntAttribute("ID");
                font.FontName = fontElement->Attribute("FontName");
                font.FamilyName = fontElement->Attribute("FamilyName");
                font.Charset = fontElement->Attribute("Charset");

                // <ofd:FontFile>
                const XMLElement *fontFileElement = fontElement->FirstChildElement("ofd:FontFile");
                if ( fontFileElement != nullptr ){
                    font.FontFile = fontFileElement->GetText();
                }
                
                m_attributes.PublicRes.AppendFont(font);

                VLOG(3) << font.ToString();

                fontElement = fontElement->NextSiblingElement("ofd:Font");
            };
        }

        // <ofd:ColorSpaces>
        const XMLElement *colorSpacesElement = rootElement->FirstChildElement("ofd:ColorSpaces");
        if ( colorSpacesElement != nullptr ){
            // <ofd:ColorSpace>
            const XMLElement *colorSpaceElement = colorSpacesElement->FirstChildElement("ofd:ColorSpace");
            while ( colorSpaceElement != nullptr ){
                OFDColorSpace colorSpace;
                colorSpace.ID = colorSpaceElement->IntAttribute("ID");
                std::string colorSpaceType = colorSpaceElement->Attribute("Type");
                if ( colorSpaceType == "CMYK" ){
                    colorSpace.Type = OFDColorSpaceType::CMYK;
                } else if ( colorSpaceType == "Gray" ) {
                    colorSpace.Type = OFDColorSpaceType::GRAY;
                } else {
                    colorSpace.Type = OFDColorSpaceType::UNKNOWN;
                }

                m_attributes.PublicRes.AppendColorSpace(colorSpace);

                VLOG(3) << colorSpace.ToString();

                colorSpaceElement = colorSpaceElement->NextSiblingElement("ofd:ColorSpace");
            }
        }
    }

    return true;
}

/*
<?xml version="1.0" encoding="UTF-8"?>

<ofd:Res xmlns:ofd="http://www.ofdspec.org" BaseLoc="Res">
  <ofd:MultiMedias><ofd:MultiMedia ID="1216" Type="Image"><ofd:MediaFile>Image_12.jpg</ofd:MediaFile></ofd:MultiMedia>
  <ofd:MultiMedia ID="34" Type="Image"><ofd:MediaFile>Image_5.jpg</ofd:MediaFile></ofd:MultiMedia>
</ofd:MultiMedias>

</ofd:Res>
*/
bool OFDDocument::parseDocumentResXML(){
    std::string documentResFileName = m_rootDir + "/" + m_attributes.CommonData.DocumentRes;

    OFDPACKAGE_GET_FILE_CONTENT(documentResFileName, content);

    TEXT_TO_XML(content, xmldoc);

    // <ofd:Res>
    XMLElement *rootElement = xmldoc->RootElement();
    if ( rootElement != nullptr ){
        std::string BaseLoc = rootElement->Attribute("BaseLoc");
        m_attributes.DocumentRes.BaseLoc = BaseLoc;
        VLOG(3) << "DocumentRes Root Element Name: " << rootElement->Name() << " BaseLoc: " << BaseLoc;
        VLOG(3) << GetChildElements(rootElement);


        // <ofd:MultiMedias>
        const XMLElement *multiMediasElement = rootElement->FirstChildElement("ofd:MultiMedias");
        if ( multiMediasElement != nullptr ){
            //VLOG(3) << GetChildElements(multiMediasElement);
            //// <ofd:MultiMedia>
            const XMLElement *multiMediaElement = multiMediasElement->FirstChildElement("ofd:MultiMedia");
            while ( multiMediaElement != nullptr ){
                OFDMultiMedia multiMedia;
                multiMedia.ID = multiMediaElement->IntAttribute("ID");
                std::string strType = multiMediaElement->Attribute("Type");
                multiMedia.Type = OFDMultiMediaType::UNKNOWN;
                if ( strType == "Image" ){
                    multiMedia.Type = OFDMultiMediaType::IMAGE;
                }

                // <ofd:MediaFile>
                const XMLElement *mediaFileElement = multiMediaElement->FirstChildElement("ofd:MediaFile");
                if ( mediaFileElement != nullptr ){
                    multiMedia.MediaFile = mediaFileElement->GetText();
                }

                m_attributes.DocumentRes.AppendMultiMedia(multiMedia);

                VLOG(3) << multiMedia.ToString();

                multiMediaElement = multiMediaElement->NextSiblingElement("ofd:MultiMedia");
            };
        }
    }

    return true;
}

bool OFDDocument::parseXML(){

    OFDPACKAGE_GET_FILE_CONTENT(m_filename, content);

    TEXT_TO_XML(content, xmldoc);

    XMLElement *rootElement = xmldoc->RootElement();
    if ( rootElement != nullptr ){
        VLOG(3) << "Root Element Name: " << rootElement->Name();
        VLOG(3) << GetChildElements(rootElement);

        const XMLElement *commonDataElement = rootElement->FirstChildElement("ofd:CommonData");
        const XMLElement *pagesElement = rootElement->FirstChildElement("ofd:Pages");
        if ( commonDataElement != nullptr ){
            VLOG(3) << GetChildElements(commonDataElement);

            const XMLElement *pageAreaElement = commonDataElement->FirstChildElement("ofd:PageArea");

            const XMLElement *physicalBoxElement = pageAreaElement->FirstChildElement("ofd:PhysicalBox");
            if ( physicalBoxElement != nullptr ){
                double x0, y0, x1, y1;
                bool ok;
                std::tie(x0, y0, x1, y1, ok) = parsePhysicalBoxElement(physicalBoxElement);
                if ( ok ) {
                    this->m_attributes.CommonData.PageArea.PhysicalBox.x0 = x0;
                    this->m_attributes.CommonData.PageArea.PhysicalBox.y0 = y0;
                    this->m_attributes.CommonData.PageArea.PhysicalBox.x1 = x1;
                    this->m_attributes.CommonData.PageArea.PhysicalBox.y1 = y1;  
                }
            }

            const XMLElement *publicResElement = commonDataElement->FirstChildElement("ofd:PublicRes");
            const XMLElement *documentResElement = commonDataElement->FirstChildElement("ofd:DocumentRes");
            const XMLElement *maxUnitIDElement = commonDataElement->FirstChildElement("ofd:MaxUnitID");

            this->m_attributes.CommonData.PublicRes = publicResElement->GetText();
            this->m_attributes.CommonData.DocumentRes = documentResElement->GetText();
            this->m_attributes.CommonData.MaxUnitID = atol(maxUnitIDElement->GetText());

        }

        if ( pagesElement != nullptr ) {
            VLOG(3) << GetChildElements(pagesElement);
            
            const XMLElement *pageElement = pagesElement->FirstChildElement("ofd:Page");
            while ( pageElement != nullptr ) {
                uint64_t pageid = pageElement->UnsignedAttribute("ID");
                std::string baseLoc = pageElement->Attribute("BaseLoc");
                VLOG(3) << "Page ID: "  << pageid << " BaseLoc: " << baseLoc; 

                //OFDPage *page = new OFDPage(this, pageid, m_rootDir + "/" + baseLoc);
                OFDPagePtr page = OFDPagePtr(new OFDPage(this, pageid, m_rootDir + "/" + baseLoc));
                m_attributes.Pages.push_back(page);

                pageElement = pageElement->NextSiblingElement("ofd:Page");
            }
            LOG(INFO) << GetPagesCount() << " pages in document.";
        }

    } else {
        LOG(ERROR) << "rootElement == nullptr";
    }

    delete xmldoc;
    xmldoc = nullptr;

    return true;
}

bool OFDDocument::loadFonts() {
    for ( size_t i = 0 ; i < GetFontsCount() ; i++ ){
        const OFDFont &font = GetFont(i);
        LOG(DEBUG) << font.ID;
    }
    return true;
}

