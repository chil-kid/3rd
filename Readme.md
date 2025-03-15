懒得弄了
起码能播（）
就是对于一些文件会寄比如
```
	std::string s1 = "open \"C:\\CloudMusic\\ice - Reminiscence.mp3\"";
	std::string s2 = "play \"C:\\CloudMusic\\ice - Reminiscence.mp3\"";
	std::cout << s1 << '\n';
	std::cout << s2 << '\n';
	DWORD error = mciSendString(s1.c_str(), 0, 0, 0);
	if (error) {
		char errorBuffer[256];
		mciGetErrorString(error, errorBuffer, sizeof(errorBuffer));
		std::cerr << "Error opening file: " << errorBuffer << std::endl;
	}
	error = mciSendString(s2.c_str(), 0, 0, 0);
	if (error) {
		char errorBuffer[256];
		mciGetErrorString(error, errorBuffer, sizeof(errorBuffer));
		std::cerr << "Error opening file: " << errorBuffer << std::endl;
	}
	while(true);
```

结果会报错
```
E:\note\player\MusicPlayer\cmake-build-debug\MusicPlayer.exe
open "C:\CloudMusic\ice - Reminiscence.mp3"
play "C:\CloudMusic\ice - Reminiscence.mp3"
Error opening file: 初始化 MCI 时发生问题。
Error opening file: 指定的设备未打开，或不被 MCI 所识别。
```

或许是什么元数据的锅
长大后再学习