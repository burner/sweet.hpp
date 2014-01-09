#pragma once

#include <string>
#include <regex>
#include <memory>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>

namespace sweet {

inline std::string getCurrentPath() {
	char buf[PATH_MAX];
	getcwd(buf, PATH_MAX);
	return std::string(buf);
}

struct DirDeletor {
	inline void operator()(DIR* dir) {
		closedir(dir);
	}
};

class File {
	class FileSystemIterator {
	public:
		inline FileSystemIterator() : entry(nullptr), dir(nullptr) {}
		inline FileSystemIterator(const File& f) : entry(nullptr), path(f.path+'/') {
			if(f.isDir()) {
				this->dir = std::shared_ptr<DIR>(opendir(f.path.c_str()), DirDeletor());
				this->entry = readdir(this->dir.get());
			}
		}
	
		inline File operator*() const {
			if(this->entry) {
				return File(this->path + this->entry->d_name);
			} else {
				return File("");
			}
		}

		inline File operator->() const {
			return this->operator*();
		}
	
		inline FileSystemIterator operator++() {
			auto ret(*this);
			if(this->dir) {
				this->entry = readdir(this->dir.get());	
			}
			return ret;
		}
	
		inline bool operator==(const FileSystemIterator& o) const {
			return this->entry == o.entry;
		}
	
		inline bool operator!=(const FileSystemIterator& o) const {
			return this->entry != o.entry;
		}
	
	private:
		struct dirent* entry;
		std::shared_ptr<DIR> dir;
		std::string path;
	};

public:
	typedef FileSystemIterator iterator;

	inline File() : path(getCurrentPath()) {}
	inline File(const std::string& p) : path(p) {}

	inline bool isDir() const {
		struct stat st;
		stat(path.c_str(), &st);
		return S_ISDIR(st.st_mode);
	}

	inline bool isFile() const {
		struct stat st;
		stat(path.c_str(), &st);
		return S_ISREG(st.st_mode);
	}

	inline iterator begin() {
		return FileSystemIterator(*this);
	}

	inline iterator end() {
		return FileSystemIterator();
	}

	std::string path;
};

enum class FileSystemFilterType {
	Invalid,
	File,
	Directory,
	FileFolder
};

struct FileSystemFilterIncrement {
	inline FileSystemFilterIncrement(FileSystemFilterType t = FileSystemFilterType::Invalid) : 
		type(t) {}

	inline FileSystemFilterIncrement(FileSystemFilterType t, const std::string& re) : 
			type(t), regex(re), regexStr(re) {
	}

	inline FileSystemFilterIncrement(const std::string& re) : 
			type(FileSystemFilterType::Invalid), regex(re), regexStr(re) {
	}

	inline File::iterator operator()(File::iterator cur, File::iterator end) {
		++cur;
		while(cur != end) {
			switch(this->type) {
				case FileSystemFilterType::FileFolder:
				case FileSystemFilterType::Invalid: break;
				case FileSystemFilterType::File:
					if(!(*cur).isFile()) {
						goto inc;
					}
					break;
				case FileSystemFilterType::Directory:
					if(!(*cur).isDir()) {
						goto inc;
					}
					break;
			}

			if(this->regexStr.empty() || std::regex_match((*cur).path, this->regex)) {
				break;	
			}

			inc:
			++cur;
		}
		return cur;
	}

	FileSystemFilterType type;
	std::regex regex;
	std::string regexStr;
};

}
