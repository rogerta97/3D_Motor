#ifndef __ModuleAudio_H__
#define __ModuleAudio_H__

#include "Module.h"
#include "SDL_mixer\include\SDL_mixer.h"
#include<list>

#define DEFAULT_MUSIC_FADE_TIME 2.0f

class ModuleAudio : public Module
{
public:

	ModuleAudio(bool start_enabled = true);
	~ModuleAudio();

	bool Init(json_file* config);
	void PrintConfigData(); 
	bool CleanUp();

	// Play a music file
	bool PlayMusic(const char* path, float fade_time = DEFAULT_MUSIC_FADE_TIME);

	// Load a WAV in memory
	unsigned int LoadFx(const char* path);

	// Play a previously loaded WAV
	bool PlayFx(unsigned int fx, int repeat = 0);
	//NEW FUNCTIONALITIES
	float GetVolume() const;
	float GetMusicVolume() const;
	float GetFXVolume() const;
	const char * GetDriver()const;
	//i don't knwo how to modify the volume, but in Ric's 
	//pannel there are this type of volumes so it's kk to have it
private:
	float volume = 1.0f;
	float music_volume = 1.0f;
	float fx_volume = 1.0f;
	
private:

	Mix_Music*			music;
	std::list<Mix_Chunk*>	fx;
};

#endif // __ModuleAudio_H__