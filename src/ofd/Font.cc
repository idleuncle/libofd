#include <sstream>
#include <list>
#include <assert.h>
#include <tuple>
#include <inttypes.h>
// ---- freetype ----
#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_GLYPH_H
// ---- cairo ----
#include <cairo/cairo.h>
#include <cairo/cairo-ft.h>

#include "ofd/Font.h"
#include "ofd/Package.h"
#include "utils/logger.h"
#include "utils/xml.h"

using namespace ofd;
using namespace utils;

namespace ofd{

    std::string generateFontFileName(uint64_t fontID){
        char buf[1024];
        sprintf(buf, "Font_%" PRIu64 ".ttf", fontID);
        LOG(DEBUG) << "------- getFontFileName() fontID:" << fontID << " filename=" << std::string(buf);
        return std::string(buf);
    }

    std::map<std::string, std::string> Font::FontFiles;
}

// **************** class FreetypeInitiator ****************

class FreetypeInitiator{
public:
    FreetypeInitiator(){
        InitializeFreetype();
    }

    ~FreetypeInitiator(){
        FinalizeFreetype();
    }

    bool InitializeFreetype();
    void FinalizeFreetype();

public:
    static FT_Library ft_lib;
private:
    static bool ft_lib_initialized;
};

static struct {
  const char *name;
  const char *t1FileName;
  const char *ttFileName;
} linuxDefaultFonts[] __attribute__((unused)) = {
  {"Courier",               "n022003l.pfb", "cour.ttf"},
  {"Courier-Bold",          "n022004l.pfb", "courbd.ttf"},
  {"Courier-BoldOblique",   "n022024l.pfb", "courbi.ttf"},
  {"Courier-Oblique",       "n022023l.pfb", "couri.ttf"},
  {"Helvetica",             "n019003l.pfb", "arial.ttf"},
  {"Helvetica-Bold",        "n019004l.pfb", "arialbd.ttf"},
  {"Helvetica-BoldOblique", "n019024l.pfb", "arialbi.ttf"},
  {"Helvetica-Oblique",     "n019023l.pfb", "ariali.ttf"},
  {"Symbol",                "s050000l.pfb", nullptr},
  {"Times-Bold",            "n021004l.pfb", "timesbd.ttf"},
  {"Times-BoldItalic",      "n021024l.pfb", "timesbi.ttf"},
  {"Times-Italic",          "n021023l.pfb", "timesi.ttf"},
  {"Times-Roman",           "n021003l.pfb", "times.ttf"},
  {"ZapfDingbats",          "d050000l.pfb", nullptr},
  {nullptr}
};

static struct{
    const char *name;
    const char *relatedPath;
} ofdDefaultFonts[] = {
    {"宋体", "fonts/SimSun.ttf"},
    {"黑体", "fonts/SimHei.ttf"},
    {"楷体", "fonts/Kaiti.ttf"},
    {"仿宋", "fonts/Fangsong.ttf"},
    {"华文楷体", "fonts/华文楷体.ttf"},
    {"方正小标宋_GBK", "fonts/方正小标宋_GBK.ttf"},
    {"Calibri", "fonts/Calibri.ttf"},
    {"Times New Roman", "fonts/Times New Roman.ttf"},
    {nullptr}
};

std::vector<std::string>  SystemFontDirs;

bool FreetypeInitiator::InitializeFreetype(){
    if ( ft_lib_initialized ) return true;

    FT_Error error = FT_Init_FreeType(&ft_lib);
    if ( error ){
        LOG(ERROR) << "FT_Init_FreeType() in OFDRes::InitializeFreetype() failed.";
    } else {
        ft_lib_initialized = true;
    }

    if ( ft_lib_initialized ){
        // 自动搜索自带字休和系统字体
        for (int i = 0; ofdDefaultFonts[i].name; ++i) {
            std::string fontName = ofdDefaultFonts[i].name;
            std::string fontFilePath = ofdDefaultFonts[i].relatedPath;

            if ( Font::FontFiles.find(fontName) == Font::FontFiles.end() ){
                Font::FontFiles.insert(std::map<std::string, std::string>::value_type(fontName, fontFilePath));
            }
        }
    }

    return ft_lib_initialized;
}

void FreetypeInitiator::FinalizeFreetype(){
    if ( ft_lib_initialized ){
        FT_Done_FreeType(ft_lib);
        ft_lib = nullptr;
        ft_lib_initialized = false;
    }
}

FT_Library FreetypeInitiator::ft_lib = nullptr;
bool FreetypeInitiator::ft_lib_initialized = false;
FreetypeInitiator ftInitiator;

static cairo_user_data_key_t _ft_cairo_key;
static void _ft_done_face_uncached (void *closure) {
    FT_Face face = (FT_Face) closure;
    FT_Done_Face (face);
}

// ======== CreateCairoFontFace() ========
std::tuple<FT_Face, cairo_font_face_t*, bool> CreateCairoFontFace(char *fontData, size_t fontDataLen){
    bool ok = false;

    FT_Face face;
    cairo_font_face_t *font_face;

    if ( FT_New_Memory_Face(FreetypeInitiator::ft_lib, (unsigned char *)fontData, fontDataLen, 0, &face) != 0 ){
        LOG(ERROR) << "FT_New_Memory_Face() in OFDFont::createCairoFontFace() failed.";
    } else {

        font_face = cairo_ft_font_face_create_for_ft_face (face, FT_LOAD_NO_HINTING | FT_LOAD_NO_BITMAP);
        if ( cairo_font_face_set_user_data (font_face, &_ft_cairo_key, face, _ft_done_face_uncached) != 0 ) {
            LOG(ERROR) << "cairo_font_face_set_user_data() in OFDFont::createCairoFontFace() failed.";
            _ft_done_face_uncached(face);
            cairo_font_face_destroy(font_face);
            font_face = nullptr;
        } else {
            ok = true;
        }
    }

    return std::make_tuple(face, font_face, ok);
}

// **************** class ofd::Font ****************

Font::Font() :
    ID(0), Charset("unicode"), 
    Serif(false), Bold(false), Italic(false), FixedWidth(false),
    FontType(ofd::FontType::TrueType), FontLoc(ofd::FontLocation::Embedded),
    m_bLoaded(false), m_fontData(nullptr), m_fontDataSize(0), m_fontFace(nullptr), m_substitute(false),
    m_codeToGID(nullptr), m_codeToGIDLen(0)
{
}

Font::~Font(){
    if ( m_fontFace != nullptr ){
        cairo_font_face_destroy(m_fontFace);
        m_fontFace = nullptr;
    }
    if ( m_codeToGID != nullptr ){
        delete m_codeToGID;
        m_codeToGID = nullptr;
        m_codeToGIDLen = 0;
    }
    if ( m_fontData != nullptr ){
        delete m_fontData;
        m_fontData = nullptr;
    }
}

// ======== Font::LocateFontFile() ========
std::string Font::LocateFontFile(const std::string& fontName){
    if ( Font::FontFiles.find(fontName) != Font::FontFiles.end() ){
        return Font::FontFiles[fontName];
    } else {
        return "";
    }
}

// ======== Font::CreateFromData() ========
bool Font::CreateFromData(char *fontData, size_t fontDataSize){
    bool ok = true;

    LOG(INFO) << "@@@@@@@@ ID: " << ID << " FontName: " << FontName << " fontDataSize: " << fontDataSize;

    if ( m_fontData != nullptr ){
        delete m_fontData;
        m_fontData = nullptr;
    }
    m_fontDataSize = fontDataSize;
    m_fontData = fontData;

    FT_Face face;
    cairo_font_face_t *font_face;
    std::tie(face, font_face, ok) = CreateCairoFontFace(m_fontData, m_fontDataSize); 

    m_fontFace = font_face;
    m_bLoaded = true;

    return ok;
}

std::string Font::GenerateFontFileName(){
    return generateFontFileName(ID);
}

// ======== Font::GenerateXML() ========
void Font::GenerateXML(XMLWriter &writer) const{

    writer.StartElement("Font");{
        // -------- <Font ID="">
        writer.WriteAttribute("ID", ID);

        // -------- <Font FontName="">
        // Required.
        writer.WriteAttribute("FontName", FontName);

        // -------- <Font FamilyName="">
        // Optional.
        if ( !FamilyName.empty() ){
            writer.WriteAttribute("FamilyName", FamilyName);
        }

        // -------- <Font Charset="">
        // Optional.
        if ( !Charset.empty() ){
            writer.WriteAttribute("Charset", Charset);
        }

        // -------- <Font Serif="">
        // Optional.
        if ( Serif ){
            writer.WriteAttribute("Serif", true);
        }

        // -------- <Font Bold="">
        // Optional.
        if ( Bold ){
            writer.WriteAttribute("Bold", true);
        }

        // -------- <Font Italic="">
        // Optional.
        if ( Italic ){
            writer.WriteAttribute("Italic", true);
        }

        // -------- <Font FixedWidth="">
        // Optional.
        if ( FixedWidth ){
            writer.WriteAttribute("FixedWidth", true);
        }

        // -------- <FontFile>
        // Optional
        std::string fontFilePath = GetFontFilePath();
        writer.WriteElement("FontFile", fontFilePath);

    } writer.EndElement();

}

// ======== Font::FromXML() ========
bool Font::FromXML(XMLElementPtr fontElement){

    bool ok = false;
    FontPtr font = nullptr;

    std::string childName = fontElement->GetName();

    // -------- <Font>
    // OFD (section 11.1) P61. Res.xsd.
    if ( childName == "Font" ){
        bool exist = false;

        // -------- <Font FontName="">
        // Required.
        std::tie(ID, exist) = fontElement->GetIntAttribute("ID");
        if ( exist ){
            // -------- <Font FontName="">
            // Required.
            std::tie(FontName, exist) = fontElement->GetStringAttribute("FontName");
            if ( !exist ){
                LOG(ERROR) << "Attribute FontName is required in Font XML.";
            }
        } else {
            LOG(ERROR) << "Attribute ID is required in Font XML.";
        }

        if ( exist ) {

            // -------- <Font FamilyName="">
            // Optional
            std::tie(FamilyName, std::ignore) = fontElement->GetStringAttribute("FamilyName");

            // -------- <Font Charset="">
            // Optional
            std::tie(Charset, std::ignore) = fontElement->GetStringAttribute("Charset");

            // -------- <Font Charset="">
            // Optional
            std::tie(Charset, std::ignore) = fontElement->GetStringAttribute("Charset");

            // -------- <Font Serif="">
            // Optional
            std::tie(Serif, std::ignore) = fontElement->GetBooleanAttribute("Serif");

            // -------- <Font Bold="">
            // Optional
            std::tie(Bold, std::ignore) = fontElement->GetBooleanAttribute("Bold");

            // -------- <Font Italic="">
            // Optional
            std::tie(Italic, std::ignore) = fontElement->GetBooleanAttribute("Italic");

            // -------- <Font FixedWidth="">
            // Optional
            std::tie(FixedWidth, std::ignore) = fontElement->GetBooleanAttribute("FixedWidth");

            std::string fontFilePath;
            XMLElementPtr fontFileElement = fontElement->GetFirstChildElement();
            if ( fontFileElement != nullptr && fontFileElement->GetName() == "FontFile" ){
                std::tie(fontFilePath, std::ignore) = fontFileElement->GetStringValue();
            } else {
                fontFilePath = Font::LocateFontFile(FontName);
            }
            SetFontFilePath(fontFilePath);

            ok = true;
        } else {
            ok = false;
        }
    }

    return ok;
}

bool Font::Load(PackagePtr package, bool reload){
    if ( m_bLoaded && !reload ) return true;

        
    // FIXME
    //if ( ID > 0 ){

        //std::string fontFile = std::string("./data/embed/f") + std::to_string(ID) + ".otf";
        ////std::string fontFile = "./data/embed/f2.otf";
        //LOG(ERROR) << "Load fontFile: " << fontFile;

        //char *fontData = nullptr;
        //size_t fontDataSize = 0;
        //bool readOK = false;
        //std::tie(fontData, fontDataSize, readOK) = utils::ReadFileData(fontFile);

        //m_bLoaded = CreateFromData(fontData, fontDataSize);

    //} else {
    // --------------
        bool ok = true;

        std::string fontFilePath = m_fontFilePath;
        LOG(DEBUG) << "Load Font: " << fontFilePath;

        char *fontData = nullptr;
        size_t fontDataSize = 0;
        bool readOK = false;
        if ( package->IsZipFileExist(fontFilePath) ){
            std::tie(fontData, fontDataSize, readOK) = package->ReadZipFileRaw(fontFilePath);
        } else {
            if ( utils::FileExist(fontFilePath) ){
                std::tie(fontData, fontDataSize, readOK) = utils::ReadFileData(fontFilePath);
            } else {
                LOG(WARNING) << "Font file " << fontFilePath << " is not exist in the archive or folder.";
            }
        }
        if ( readOK ){
            if ( CreateFromData(fontData, fontDataSize) ){
                LOG(INFO) << "Font " << FontName << "(ID=" << ID << ") loaded.";
            } else {
                LOG(ERROR) << "createCairoFontFace() in OFDFont::Load() failed.";
                ok = false;
            }
        } else {
            ok = false;
            LOG(ERROR) << "Call ReadZipFileRaw() to read font file " << fontFilePath << " failed.";
        }

        m_bLoaded = ok;
    //}

    return m_bLoaded;
}

unsigned long Font::GetGlyph(unsigned int code, unsigned int *u, int uLen) const{
    FT_UInt gid;

    if ( m_codeToGID && code < m_codeToGIDLen) {
        gid = (FT_UInt)m_codeToGID[code];
    } else {
        gid = (FT_UInt)code;
    }
    return gid;
}

