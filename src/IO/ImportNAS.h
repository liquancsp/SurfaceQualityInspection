#pragma once
#ifndef __VCGLIB_IMPORT_NAS
#define __VCGLIB_IMPORT_NAS

#include <cctype>
#include <cstdio>
#include <vcg/complex/allocate.h>

#pragma warning (push)
#pragma warning (disable:4482)	/* nonstandard extension used: enum 'enum' used in qualified name */

#define _SUPPORT_NASTRAN_FLOAT_FORMAT 1



namespace vcg {
namespace tri {
namespace io {



template <class OpenMeshType>
class ImporterNAS {

public:
	typedef typename OpenMeshType::ScalarType			ScalarType;
	typedef typename OpenMeshType::VertexIterator		VertexIterator;
	typedef typename OpenMeshType::FaceIterator			FaceIterator;

private:
	struct VertexData {

	public:
		double							x;
		double							y;
		double							z;
		int								index;

	public:
		VertexData(double x, double y, double z, int index = 0) : x(x), y(y), z(z), index(index) { }

	public:
		bool operator <(const VertexData &vertex) const { return (this->index < vertex.index); }
		bool operator >(const VertexData &vertex) const { return (this->index > vertex.index); }
		bool operator ==(const VertexData &vertex) const { return (this->index == vertex.index); }
	};
  
	struct TriangleData {

	public:
		int								a;
		int								b;
		int								c;
		int								index;

	public:
		TriangleData(int a, int b, int c, int index) : a(a), b(b), c(c), index(index) { }

	public:
		bool operator <(const TriangleData &triangle) const { return (this->index < triangle.index); }
		bool operator >(const TriangleData &triangle) const { return (this->index > triangle.index); }
		bool operator ==(const TriangleData &triangle) const { return (this->index == triangle.index); }
	};

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
	static int Open(OpenMeshType &mesh, const char *filename, CallBackPos *cb = 0) {
		int dummymask = 0;
		return (Open(mesh, filename, dummymask, cb));
	}
	static int Open(OpenMeshType &mesh, const char *filename, int &loadmask, CallBackPos *cb = 0) {

		std::vector<VertexData> vertices;
		std::vector<TriangleData> triangles;

		if (int ret = read(filename, vertices, triangles)) return (ret);

		mesh.Clear();

		if (vertices.empty() || triangles.empty()) return (0);

		// Constructing indexing.
		std::sort(vertices.begin(), vertices.end());
		int base = vertices.front().index;
		std::vector<int> indices(vertices.back().index - base + 1, -1);
		for (size_t i = 0, c = 0; i < vertices.size(); ++i) {
			indices[vertices[i].index - base] = i;
		}

		VertexIterator vi = Allocator<OpenMeshType>::AddVertices(mesh, vertices.size());
    for(size_t i = 0; i < vertices.size(); ++i)
    {
      vi->P()[0] = (ScalarType)vertices[i].x;
      vi->P()[1] = (ScalarType)vertices[i].y;
      vi->P()[2] = (ScalarType)vertices[i].z;
      ++vi;
    }
		FaceIterator fi = Allocator<OpenMeshType>::AddFaces(mesh, triangles.size());
    for(size_t i = 0; i < triangles.size(); ++i)
    {
      fi->V(0) = &(mesh.vert[indices[triangles[i].a - base]]);
      fi->V(1) = &(mesh.vert[indices[triangles[i].b - base]]);
      fi->V(2) = &(mesh.vert[indices[triangles[i].c - base]]);
      ++fi;
    }
		return (0);
	}

private:
	static int read(const char *name, std::vector<VertexData> &vertices, std::vector<TriangleData> &triangles) {

		FILE *fp = std::fopen(name, "r");
		if (!fp) return (-1);

		vertices.clear();
		triangles.clear();

		char buffer[BufferSize];
		char fields[FieldsSize];

		int format = Undetermined;

		while (true) {

			// Clears buffer and fetches new line.
			std::memset(buffer, 0, BufferSize);
			std::memset(fields, 0, FieldsSize);
			std::fgets(buffer, BufferSize, fp);

			if (std::feof(fp)) break;

			// Skips comments.
			if (buffer[0] == '$') continue;

			// Parses data.
			if (!std::strncmp(buffer, "GRID", 4)) {

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
					return (-1);
				}

				vertices.push_back(VertexData(toDouble(fields, 2), toDouble(fields, 3), toDouble(fields, 4), toInt(fields, 0)));

			} else if (!std::strncmp(buffer, "CQUAD4", 6)) {

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
					return (-1);
				}

				triangles.push_back(TriangleData(toInt(fields, 2), toInt(fields, 3), toInt(fields, 4), toInt(fields, 0)));
				triangles.push_back(TriangleData(toInt(fields, 4), toInt(fields, 5), toInt(fields, 2), toInt(fields, 0)));

			} else if (!std::strncmp(buffer, "CTRIA3", 6)) {

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
					return (-1);
				}
				triangles.push_back(TriangleData(toInt(fields, 2), toInt(fields, 3), toInt(fields, 4), toInt(fields, 0)));
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
	static Format getFormat(const char *keyword) {
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
				std::strncpy(fields, buffer, 16);
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
				std::strncpy(fields, buffer, 8);
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
}



#pragma warning (pop)

#endif /* __VCGLIB_IMPORT_NAS */
