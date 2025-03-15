
#include "MusicPlayer.h"
#include <fcntl.h>
#include <mutex>
#include <windows.h>
#include <mmsystem.h>
#include <memory>
#pragma comment(lib, "winmm.lib")

std::unique_ptr<Player> Player::ptr;
std::mutex Player::mutex;

Player::Player() {

}

Player& Player::GetPlayer() {
	if (ptr == nullptr) {
		std::lock_guard<std::mutex> lock(mutex);
		if (ptr == nullptr) {
			ptr = std::unique_ptr<Player>(new Player);
		} 
	}
	return *ptr;
}

void Player::SetSong(const std::string& SongPath) {
	std::cout << "SetSong:" << SongPath << '\n';
	Path = SongPath;
}

void Player::Play() {
	std::string command = "open \"" + Path + "\"";
	std::cout << command << '\n';
	mciSendString(command.c_str(), 0, 0, 0);
	command = "play \"" + Path + "\"";
	std::cout << command << '\n';
	mciSendString(command.c_str(), 0, 0, 0);
}

void Player::Stop() {
	std::string command = "pause \"" + Path + "\"";
	mciSendString(command.c_str(), 0, 0, 0);
}

void Player::Close() {
	std::string command = "close \"" + Path + "\"";
	mciSendString(command.c_str(), 0, 0, 0);
}