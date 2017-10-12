#ifndef __OFD_FONT_H__
#define __OFD_FONT_H__

#include <string>
#include <map>
#include <memory>
#include "ofd/Common.h"

struct _cairo_font_face;

namespace ofd{

    std::string generateFontFileName(uint64_t fontID);

    enum class FontType {
        Unknown = -1,
        TrueType,
        CIDType2,
        Type1,
        Type3,
    };

    enum class FontLocation{
        Unknown = -1,
        Embedded,
        External,
        Resident,
    };

    class Font {

        public:
            static std::map<std::string, std::string> FontFiles;
            static std::string LocateFontFile(const std::string& fontName);

        public:
            Font();
            virtual ~Font();

            // =============== Public Attributes ================
        public:
            uint64_t          ID;
            std::string       FontName;
            std::string       FamilyName;
            std::string       Charset;
            bool              Serif;
            bool              Bold;
            bool              Italic;
            bool              FixedWidth;
            //std::string       FontFile;
            ofd::FontType          FontType;
            ofd::FontLocation      FontLoc;

            // =============== Public Methods ================
        public:
            bool IsLoaded() const {return m_bLoaded;};
            bool CreateFromData(char *fontData, size_t fontDataSize);
            _cairo_font_face* GetCairoFontFace() const {return m_fontFace;};
            void GenerateXML(utils::XMLWriter &writer) const;
            bool FromXML(utils::XMLElementPtr fontElement);
            bool Load(PackagePtr package, bool reload = false);
            unsigned long GetGlyph(unsigned int code, unsigned int *u, int uLen) const;
            std::string GenerateFontFileName();

        public:
            const char* GetFontData() const {return m_fontData;};
            size_t GetFontDataSize() const {return m_fontDataSize;};

            void SetEmbeddedFontFile(const std::string &filePath){m_embeddedFontFile = filePath;};
            std::string GetEmbeddedFontFile() const {return m_embeddedFontFile;};
            void SetExternalFontFile(const std::string &filePath){m_externalFontFile = filePath;};
            std::string GetExternalFontFile() const {return m_externalFontFile;};

            bool IsSubstitute() const {return m_substitute;};
        private:
            bool              m_bLoaded;
            char*             m_fontData;
            size_t            m_fontDataSize;
            _cairo_font_face* m_fontFace;
            std::string       m_embeddedFontFile;
            std::string       m_externalFontFile;
            bool              m_substitute;

        public:
            // FIXME
            // Set in OFDOutputDev_UpdateFont
            int*              m_codeToGID;
            size_t            m_codeToGIDLen;

    }; // class Font;

}; // namespace ofd

#endif // __OFD_FONT_H__
