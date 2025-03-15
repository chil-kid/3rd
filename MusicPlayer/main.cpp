#include "FileManage.h"
#include "refer.h"
#include <windows.h>

#include "FileManage.h"
#include <mmsystem.h>
#pragma comment(lib, "winmm.lib")

void menu() {
	std::cout << "------------------------------------------\n";
	std::cout << "1.����ר��\n";
	std::cout << "2.��ʼ����\n";
	std::cout << "3.ֹͣ����\n";
	std::cout << "4.�˳�\n";
	std::cout << "5.��һ��\n";
	std::cout << "6.��һ��\n";
	std::cout << "7.�洢��ר��\n";
	std::cout << "8.�л�ר��\n";
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
			std::cout << "���뵼��ר��·��\n";
			std::string Path; std::getline(std::cin, Path);
			std::cout << "Path:" << Path << '\n';
			std::shared_ptr<MusicSet> ptr = std::make_shared<MusicSet>();
			ptr->GetMusic(Path);
			if(ptr->empty()) {
				std::cout << "��ר��������\n";
				continue;
			}
			MusicSetList.push_back(ptr);
			MusicList::GetList()->SetList(ptr);
		}
		else if(op == PLAY_CD) {
			if(MusicSetList.empty()) {
				std::cout << "���б�����\n";
				continue;
			}
			MusicList::GetList()->Play();
		}
		else if(op == STOP_CD) {
			if(MusicSetList.empty()) {
				std::cout << "���б�����\n";
				continue;
			}
			MusicList::GetList()->Stop();
		}
		else if(op == EXIT_PLAYER) {
			exit(0);
		}
		else if(op == NEXT_MUSIC) {
			if(MusicSetList.empty()) {
				std::cout << "���б�����\n";
				continue;
			}
			MusicList::GetList()->NextMusic();
		}
		else if(op == PRE_MUSIC) {
			if(MusicSetList.empty()) {
				std::cout << "���б�����\n";
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
				std::cout << "���б�����\n";
				continue;
			}
			std::cout << "�����л�ר���ı��\n";
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