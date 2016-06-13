#pragma once
#ifndef __VCGLIB_IMPORT_NAS
#define __VCGLIB_IMPORT_NAS

#include <cctype>
#include <cstdio>
//#include "../3rd/vcg/complex/allocate.h"
#include "../Algorithm/data_type.h"
#include <string.h>
#pragma warning (push)
#pragma warning (disable:4482)	/* nonstandard extension used: enum 'enum' used in qualified name */

#define _SUPPORT_NASTRAN_FLOAT_FORMAT 1


namespace sqi{
namespace io {



//template <class OpenMeshType>
class ImporterNAS {
  
public:
//  typedef typename OpenMeshType::ScalarType			ScalarType;
//  typedef typename OpenMeshType::VertexIterator		VertexIterator;
//  typedef typename OpenMeshType::FaceIterator			FaceIterator;
  
private:
  
  enum Format {
    Anomalous = -1,
    Undetermined = 0,
    Large,
    Small,
    Free
  };
  
private:
  static const int					KeywordSize = 8;
  static const int					BufferSize = 1024;
  static const int					FieldsSize = 4096;
  static const int					FieldWidth = 32;
  static const char					Splitter = ',';
  
public:

  static int Open(const char *filename, std::vector<lq::point> &vertex_list, 
                  std::vector<lq::triangle> &tri_list)
  {
    return read(filename, vertex_list, tri_list);
  }

  
private:

  static int read(const char *name, std::vector<lq::point> &vertex_list, 
                  std::vector<lq::triangle> &tri_list) {
    
    FILE *fp = std::fopen(name, "r");
    if (!fp) return (1);
    
    vertex_list.clear();
    tri_list.clear();
    char buffer[BufferSize];
    char fields[FieldsSize];
    
    Format format = Undetermined;
    
    while (true) {
      
      // Clears buffer and fetches new line.
      memset(buffer, 0, BufferSize);
      memset(fields, 0, FieldsSize);
      fgets(buffer, BufferSize, fp);
      
      if (std::feof(fp)) break;
      
      // Skips comments.
      if (buffer[0] == '$') continue;
      
      // Parses data.
      if (!strncmp(buffer, "GRID", 4)) {
        
        switch (getFormat(buffer)) {
        case Large:
          extractLargeFields(fields, getFixedBulk(buffer, fp));
          break;
        case Small:
          extractSmallFields(fields, getFixedBulk(buffer, fp));
          break;
        case Free:
          extractFreeFields(fields, getFreeBulk(buffer, fp));
          break;
        default:
          return (1);
        }
        
        vertex_list.push_back(lq::point(toDouble(fields, 2), toDouble(fields, 3), toDouble(fields, 4)));
        
      } else if (!strncmp(buffer, "CQUAD4", 6)) {
        
        switch (getFormat(buffer)) {
        case Large:
          extractLargeFields(fields, getFixedBulk(buffer, fp));
          break;
        case Small:
          extractSmallFields(fields, getFixedBulk(buffer, fp));
          break;
        case Free:
          extractFreeFields(fields, getFreeBulk(buffer, fp));
          break;
        default:
          return (1);
        }
        tri_list.push_back(lq::triangle(toInt(fields, 2), toInt(fields, 3), toInt(fields, 4)));
        tri_list.push_back(lq::triangle(toInt(fields, 4), toInt(fields, 5), toInt(fields, 2)));
        
      } else if (!strncmp(buffer, "CTRIA3", 6)) {
        
        switch (getFormat(buffer)) {
        case Large:
          extractLargeFields(fields, getFixedBulk(buffer, fp));
          break;
        case Small:
          extractSmallFields(fields, getFixedBulk(buffer, fp));
          break;
        case Free:
          extractFreeFields(fields, getFreeBulk(buffer, fp));
          break;
        default:
          return (1);
        }
        
        tri_list.push_back(lq::triangle(toInt(fields, 2), toInt(fields, 3), toInt(fields, 4)));
        
      }
    }
    
    std::fclose(fp);
    
    return (0);
  }
  static inline int toInt(const char *fields, int index) {
    return (std::atoi(fields + FieldWidth * index));
  }
  static inline double toDouble(const char *fields, int index) {
#if _SUPPORT_NASTRAN_FLOAT_FORMAT
    return (toDouble(fields + FieldWidth * index));
#else
    return (std::atof(fields + FieldWidth * index));
#endif
  }
  static inline double toDouble(const char *str) {
    while (*str == ' ') ++str;
    char buf[FieldWidth + 1];
    *buf = *str;
    int i = 1;
    for (const char *ptr = str + 1; *ptr; ++ptr) {
      if (std::toupper(*ptr) == 'E') return (std::atof(str));
      if (std::toupper(*ptr) == 'D') {
        buf[i++] = 'E';
        while (*(ptr++)) buf[i++] = (*ptr);
        break;
      } else {
        if ((*ptr == '+') || (*ptr == '-')) buf[i++] = 'E';
        buf[i++] = *ptr;
      }
    }
    buf[i] = 0;
    return (std::atof(buf));
  }
  static int getFormat(const char *keyword) {
    for (int i = 0; i < KeywordSize; ++i) {
      if (keyword[i] == '*') return (Large);
      if (keyword[i] == ',') return (Free);
    }
    return (Small);
  }
  static char *getFixedBulk(char *buffer, FILE *fp) {
    char *head = buffer;
    bool initial = false;
    while (true) {
      if (initial && !std::fgets(buffer, BufferSize, fp)) return (head);
      if (!buffer[80]) break;
      buffer += 80;
      initial = true;
    }
    return (head);
  }
  static char *getFreeBulk(char *buffer, FILE *fp) {
    char *head = buffer;
    bool initial = false;
    if (initial && !std::fgets(buffer, BufferSize, fp)) return (head);
    return (head);
  }
  static char *extractLargeFields(char *fields, const char *buffer) {
    char *head = fields;
    while (true) {
      buffer += 8;
      for (int i = 0; i < 4; ++i) {
        strncpy(fields, buffer, 16);
        buffer += 16;
        if (!*buffer || (*buffer == '\n')) return (head);
        fields += 32;
      }
      buffer += 8;
    }
    return (head);
  }
  static char *extractSmallFields(char *fields, const char *buffer) {
    char *head = fields;
    while (true) {
      buffer += 8;
      for (int i = 0; i < 8; ++i) {
        strncpy(fields, buffer, 8);
        buffer += 8;
        if (!*buffer || (*buffer == '\n')) return (head);
        fields += 32;
      }
      buffer += 8;
    }
    return (head);
  }
  static char *extractFreeFields(char *fields, const char *buffer) {
    char *head = fields;
    int count = 0;
    while (*buffer != ',') ++buffer;
    while (true) {
      ++buffer;
      while (*buffer != ',') {
        if (!*buffer || (*buffer == '\n')) return (head);
        *(fields++) = *(buffer++);
      }
      fields = head + (++count) * 32;
    }
    return (head);
  }
};



}
}

#pragma warning (pop)

#endif /* __VCGLIB_IMPORT_NAS */
