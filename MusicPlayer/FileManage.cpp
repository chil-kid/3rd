#include "FileManage.h"

MusicList* MusicList::ptr = nullptr;

MusicList::MusicList() {

}

MusicList::~MusicList() {
	delete ptr;
}

void MusicList::NextMusic() {
	if (!size) return;
	++index;
	if (index == size) index = 0;
	Set();
	Play();
}

void MusicList::PreMusic() {
	if (!size) return;
	if (index == 0) index = size;
	--index;
	Set();
	Play();
}

MusicList* MusicList::GetList() {
	if (ptr == nullptr) ptr = new MusicList;
	return ptr;
}

void MusicList::SetList(std::shared_ptr<MusicSet> List) {
	if (ListBelong != nullptr) {
		ListBelong->list = std::move(list);
		index = 0;
		size = 0;
		ListBelong = nullptr;
	}
	list = std::move(List->list);
	size = list.size();
	ListBelong = List.get();
	Set();
}

void MusicList::Set() {
	Player::GetPlayer().Close();
	Player::GetPlayer().SetSong(list[index]);
}

void MusicList::Play() {
	Player::GetPlayer().Play();
}

void MusicList::Stop() {
	Player::GetPlayer().Stop();
}

bool MusicSet::CheckFile(const std::string& MusicPath) {
	std::string tail = MusicPath.substr(MusicPath.size() - 4);
	if (tail == ".wav" || tail == ".mp3" || tail == ".mp4" || tail == ".wma") return true;
	return false;
}


void MusicSet::GetMusic(std::string& MusicPath) {
	DIR* dir = opendir(MusicPath.c_str());
	if (dir == nullptr) {
		perror("opendir failed");
		return;
	}
	list.clear();
	dirent* direntry;
	while ((direntry = readdir(dir)) != nullptr) {
		std::string Path = PathCoder::CombinePath(MusicPath, direntry->d_name).str();
		std::cout << Path << '\n';
		if (CheckFile(Path)) {
			list.push_back(Path);
		}
	}
	Path = MusicPath;
}

bool MusicSet::empty() {
	return list.empty();
}