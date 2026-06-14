#pragma once

#include "Uncopyable.h"

namespace pix
{
	// Data used to initialize the LaunchConfig singleton, provided to the GameLoop constructor
	struct LaunchConfigData
	{
		// Max number of errors with the same name logged by ErrorLogger
		int MaxLogCountPerError = 4;

		// Audio singleton settings
		int SoundChannelCount = 100;
		float MasterVolume = 0.3f;
		float ChannelVolume = 1.0f;
		float MusicVolume = 1.0f;

		// Update rate setting
		float UpdatesPerSecond = 60.0f;

		// Window / Renderer settings
		int LogicalResolutionWidth = 1920;
		int LogicalResolutionHeight = 1080;
		bool IsLinearFilter = true;
		bool IsIntegerScale = false;
		bool IsVsync = true;
		bool IsFullscreen = false;
	};

	// Immutable singleton for storing common launch settings.
	// 
	// Must be initialized with Init() to store non-default launch config data.
	// 
	// Philosophy:
	// These are common engine settings applied during GameLoop initialization.
	// Referencing launch configuration data at runtime can be useful to restore default state.
	// Therefore, this data is exposed as a global, immutable singleton.
	class LaunchConfig : private Uncopyable
	{

	public:

		// Returns the LaunchConfig instance
		static LaunchConfig& Get();

		// Initializes the LaunchConfig singleton.
		// No-op if it is already initialized.
		void Init(const LaunchConfigData& data);

		// Returns the initialized launch configuration, or the default configuration otherwise
		const LaunchConfigData& GetData() const;

		bool IsInitialized() const;

	private:

		LaunchConfig() = default;
		~LaunchConfig() = default;

		LaunchConfigData data_;
		bool isInitialized_ = false;
	};

}