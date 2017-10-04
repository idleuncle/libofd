#ifndef __UTILS_UNICODE_H__
#define __UTILS_UNICODE_H__

#include <vector>
#include <string>

int enc_unicode_to_utf8_one(unsigned long unic, unsigned char *pOutput, int outSize);  

int enc_utf8_to_unicode_one(const unsigned char* pInput, unsigned long *Unic);

size_t utf8_string_to_char_list(const std::string &utf8_string, std::vector<std::string> &char_list);
size_t get_utf8_string_length(const std::string &utf8_string);

#endif // __UTILS_UNICODE_H__
