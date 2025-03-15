#pragma once
#include <iostream>
#include <mutex>
#include <memory>

const std::string PLAY_MUSIC = "play music";

/*
播放一首歌
单例模式
懒汉 -- 用到再说
*/

class Player {
public:
	static Player& GetPlayer();
	void SetSong(const std::string& SongPath);
	void Play();
	void Stop();
	void Close();
	
private:
	static std::unique_ptr<Player> ptr;
	static std::mutex mutex;
	std::string Path;

	Player();
	Player(const Player& others) = delete;
	Player& operator=(const Player& others) = delete;
};