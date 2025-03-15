#pragma once

#include <vector>
#include "PathCoder.h"
#include <memory>
#include <dirent.h>
#include "MusicPlayer.h"

class MusicSet;

/*
播放列表
单例
饿汉 -- 马上实例化
*/

class MusicList {
public:
	static MusicList* GetList(); //获得单例对象
	void SetList(std::shared_ptr<MusicSet> List); //设置列表
	void NextMusic();
	void PreMusic();

	void Set();
	void Play();
	void Stop();

	~MusicList();

private:
	MusicList();
	MusicList(const MusicList& others) = delete;
	MusicList& operator=(const MusicList& others) = delete;
	static MusicList* ptr; //单例对象
	std::vector<std::string> list; //专辑全部歌
	int index = 0; //播放到哪一首
	int size = 0; //总共多少首
	MusicSet* ListBelong = nullptr; //属于哪个专辑
};

/*
管理专辑
*/
class MusicSet {
	friend void MusicList::SetList(std::shared_ptr<MusicSet>);
public:
	bool CheckFile(const std::string& MusicPath);
	bool empty();
	void GetMusic(std::string& MusicPath);
	std::string Path;
private:
	std::vector<std::string> list;
};