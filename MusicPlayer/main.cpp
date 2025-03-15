#include "FileManage.h"
#include "refer.h"
#include <windows.h>

#include "FileManage.h"
#include <mmsystem.h>
#pragma comment(lib, "winmm.lib")

void menu() {
	std::cout << "------------------------------------------\n";
	std::cout << "1.导入专辑\n";
	std::cout << "2.开始播放\n";
	std::cout << "3.停止播放\n";
	std::cout << "4.退出\n";
	std::cout << "5.下一首\n";
	std::cout << "6.上一首\n";
	std::cout << "7.存储的专辑\n";
	std::cout << "8.切换专辑\n";
	std::cout << "------------------------------------------\n";
}

std::vector<std::shared_ptr<MusicSet>> MusicSetList;

int main() {
	MusicList::GetList();
	while (true) {
		menu();
		int op; std::cin >> op;
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

		if(op == IMPORT_CD) {
			std::cout << "输入导入专辑路径\n";
			std::string Path; std::getline(std::cin, Path);
			std::cout << "Path:" << Path << '\n';
			std::shared_ptr<MusicSet> ptr = std::make_shared<MusicSet>();
			ptr->GetMusic(Path);
			if(ptr->empty()) {
				std::cout << "空专辑！！！\n";
				continue;
			}
			MusicSetList.push_back(ptr);
			MusicList::GetList()->SetList(ptr);
		}
		else if(op == PLAY_CD) {
			if(MusicSetList.empty()) {
				std::cout << "空列表！！！\n";
				continue;
			}
			MusicList::GetList()->Play();
		}
		else if(op == STOP_CD) {
			if(MusicSetList.empty()) {
				std::cout << "空列表！！！\n";
				continue;
			}
			MusicList::GetList()->Stop();
		}
		else if(op == EXIT_PLAYER) {
			exit(0);
		}
		else if(op == NEXT_MUSIC) {
			if(MusicSetList.empty()) {
				std::cout << "空列表！！！\n";
				continue;
			}
			MusicList::GetList()->NextMusic();
		}
		else if(op == PRE_MUSIC) {
			if(MusicSetList.empty()) {
				std::cout << "空列表！！！\n";
				continue;
			}
			MusicList::GetList()->PreMusic();
		}
		else if(op == SHOW_SET) {
			for(int i = 0; i < MusicSetList.size(); ++i) {
				std::cout << i << ":" << MusicSetList[i]->Path << '\n';
			}
		}
		else if(op == SWITCH_SET) {
			if(MusicSetList.empty()) {
				std::cout << "空列表！！！\n";
				continue;
			}
			std::cout << "输入切换专辑的编号\n";
			int id; std::cin >> id;
			if(id >= 0 && id < MusicSetList.size())
				MusicList::GetList()->SetList(MusicSetList[id]);
			else
				std::cout << "invalid id\n";
		}
		else {
			std::cout << "invalid operation !\n";
		}
	}
}