#include <random>
#include <filesystem.hpp>
#include <unit.hpp>
#include <logger.hpp>

UNITTEST(test1) {
	sweet::File file;
	AS_T(file.isDir());
	auto it(file.begin());
	auto end(file.end());
	AS_T(it != end);
	for(; it != end; ++it) {
		AS_T((*it).isDir() || (*it).isFile());
	}

	size_t cnt(0);
	sweet::FileSystemFilterIncrement ff(sweet::FileSystemFilterType::File);
	for(it = ff(file.begin(), end); it != end; it = ff(it, end)) {
		AS_T_MSG((*it).isFile(), (*it).path);
		LOG("%s", (*it).path);
		++cnt;
	}
	AS_T_MSG(cnt > 0u, format("%u", cnt));

	cnt = 0;
	ff = sweet::FileSystemFilterIncrement(sweet::FileSystemFilterType::Directory);
	for(it = ff(file.begin(), end); it != end; it = ff(it, end)) {
		AS_T_MSG((*it).isDir(), (*it).path);
		LOG("%s", (*it).path);
		++cnt;
	}
	AS_T_MSG(cnt > 0u, format("%u", cnt));
}

UNITTEST(test2) {
	size_t cnt(0);

	sweet::File file;
	sweet::FileSystemFilterIncrement ff(sweet::FileSystemFilterType::File, ".*Makefile");
	for(auto it = ff(file.begin(), file.end()); it != file.end(); it = ff(it, file.end())) {
		++cnt;
	}
	AS_EQ(cnt, 1u);
}
