#pragma once

namespace Monky
{
	//Comments copied from fmod.h FMOD_REVERB_PROPERTIES struct
	struct MonkyReverbProperties
	{
		MonkyReverbProperties(){}

		MonkyReverbProperties(	int Instance,
								int Environment,
								float EnvDiffusion,
								int Room,
								int RoomHF,
								int RoomLF,
								float DecayTime,
								float DecayHFRatio,
								float DecayLFRatio,
								int Reflections,
								float ReflectionsDelay,
								int Reverb,
								float ReverbDelay,
								float ModulationTime,
								float ModulationDepth,
								float HFReference,
								float LFReference,
								float Diffusion,
								float Density,
								unsigned int Flags )
								:	Instance( Instance )
								,	Environment( Environment )
								,	EnvDiffusion( EnvDiffusion )
								,	Room( Room )
								,	RoomHF( RoomHF )
								,	RoomLF( RoomLF )
								,	DecayTime( DecayTime )
								,	DecayHFRatio( DecayHFRatio )
								,	DecayLFRatio( DecayLFRatio )
								,	Reflections( Reflections )
								,	ReflectionsDelay( ReflectionsDelay )
								,	Reverb( Reverb )
								,	ReverbDelay( ReverbDelay )
								,	ModulationTime( ModulationTime )
								,	ModulationDepth( ModulationDepth )
								,	HFReference( HFReference )
								,	LFReference( LFReference )
								,	Diffusion( Diffusion )
								,	Density( Density )
								,	Flags( Flags )
		{}
								
		int          Instance;          /* [w]   0      3       0       Environment Instance.                                                 (SUPPORTED:SFX(4 instances) and Wii (3 instances)) */
		int          Environment;       /* [r/w] -1     25      -1      Sets all listener properties.  -1 = OFF.                              (SUPPORTED:SFX(-1 only)/PSP) */
		float        EnvDiffusion;      /* [r/w] 0.0    1.0     1.0     Environment diffusion                                                 (SUPPORTED:WII) */
		int          Room;              /* [r/w] -10000 0       -1000   Room effect level (at mid frequencies)                                (SUPPORTED:SFX/WII/PSP) */
		int          RoomHF;            /* [r/w] -10000 0       -100    Relative room effect level at high frequencies                        (SUPPORTED:SFX) */
		int          RoomLF;            /* [r/w] -10000 0       0       Relative room effect level at low frequencies                         (SUPPORTED:SFX) */
		float        DecayTime;         /* [r/w] 0.1    20.0    1.49    Reverberation decay time at mid frequencies                           (SUPPORTED:SFX/WII) */
		float        DecayHFRatio;      /* [r/w] 0.1    2.0     0.83    High-frequency to mid-frequency decay time ratio                      (SUPPORTED:SFX) */
		float        DecayLFRatio;      /* [r/w] 0.1    2.0     1.0     Low-frequency to mid-frequency decay time ratio                       (SUPPORTED:---) */
		int          Reflections;       /* [r/w] -10000 1000    -2602   Early reflections level relative to room effect                       (SUPPORTED:SFX/WII) */
		float        ReflectionsDelay;  /* [r/w] 0.0    0.3     0.007   Initial reflection delay time                                         (SUPPORTED:SFX) */
		int          Reverb;            /* [r/w] -10000 2000    200     Late reverberation level relative to room effect                      (SUPPORTED:SFX) */
		float        ReverbDelay;       /* [r/w] 0.0    0.1     0.011   Late reverberation delay time relative to initial reflection          (SUPPORTED:SFX/WII) */
		float        ModulationTime;    /* [r/w] 0.04   4.0     0.25    Modulation time                                                       (SUPPORTED:---) */
		float        ModulationDepth;   /* [r/w] 0.0    1.0     0.0     Modulation depth                                                      (SUPPORTED:WII) */
		float        HFReference;       /* [r/w] 20.0   20000.0 5000.0  Reference high frequency (hz)                                         (SUPPORTED:SFX) */
		float        LFReference;       /* [r/w] 20.0   1000.0  250.0   Reference low frequency (hz)                                          (SUPPORTED:SFX) */
		float        Diffusion;         /* [r/w] 0.0    100.0   100.0   Value that controls the echo density in the late reverberation decay. (SUPPORTED:SFX) */
		float        Density;           /* [r/w] 0.0    100.0   100.0   Value that controls the modal density in the late reverberation decay (SUPPORTED:SFX) */
		unsigned int Flags;             /* [r/w] FMOD_REVERB_FLAGS - modifies the behavior of above properties                                (SUPPORTED:WII) */
	};
}


//These presets were taking from fmod.h and renamed to conform to this engines naming convention
#define MONKY_PRESET_OFF              MonkyReverbProperties(  0, -1,  1.00f, -10000, -10000, 0,   1.00f,  1.00f, 1.0f,  -2602, 0.007f,   200, 0.011f, 0.25f, 0.000f, 5000.0f, 250.0f,   0.0f,   0.0f, 0x33f )
#define MONKY_PRESET_GENERIC          MonkyReverbProperties(  0,  0,  1.00f, -1000,  -100,   0,   1.49f,  0.83f, 1.0f,  -2602, 0.007f,   200, 0.011f, 0.25f, 0.000f, 5000.0f, 250.0f, 100.0f, 100.0f, 0x3f )
#define MONKY_PRESET_PADDEDCELL       MonkyReverbProperties(  0,  1,  1.00f, -1000,  -6000,  0,   0.17f,  0.10f, 1.0f,  -1204, 0.001f,   207, 0.002f, 0.25f, 0.000f, 5000.0f, 250.0f, 100.0f, 100.0f, 0x3f )
#define MONKY_PRESET_ROOM             MonkyReverbProperties(  0,  2,  1.00f, -1000,  -454,   0,   0.40f,  0.83f, 1.0f,  -1646, 0.002f,    53, 0.003f, 0.25f, 0.000f, 5000.0f, 250.0f, 100.0f, 100.0f, 0x3f )
#define MONKY_PRESET_BATHROOM         MonkyReverbProperties(  0,  3,  1.00f, -1000,  -1200,  0,   1.49f,  0.54f, 1.0f,   -370, 0.007f,  1030, 0.011f, 0.25f, 0.000f, 5000.0f, 250.0f, 100.0f,  60.0f, 0x3f )
#define MONKY_PRESET_LIVINGROOM       MonkyReverbProperties(  0,  4,  1.00f, -1000,  -6000,  0,   0.50f,  0.10f, 1.0f,  -1376, 0.003f, -1104, 0.004f, 0.25f, 0.000f, 5000.0f, 250.0f, 100.0f, 100.0f, 0x3f )
#define MONKY_PRESET_STONEROOM        MonkyReverbProperties(  0,  5,  1.00f, -1000,  -300,   0,   2.31f,  0.64f, 1.0f,   -711, 0.012f,    83, 0.017f, 0.25f, 0.000f, 5000.0f, 250.0f, 100.0f, 100.0f, 0x3f )
#define MONKY_PRESET_AUDITORIUM       MonkyReverbProperties(  0,  6,  1.00f, -1000,  -476,   0,   4.32f,  0.59f, 1.0f,   -789, 0.020f,  -289, 0.030f, 0.25f, 0.000f, 5000.0f, 250.0f, 100.0f, 100.0f, 0x3f )
#define MONKY_PRESET_CONCERTHALL      MonkyReverbProperties(  0,  7,  1.00f, -1000,  -500,   0,   3.92f,  0.70f, 1.0f,  -1230, 0.020f,    -2, 0.029f, 0.25f, 0.000f, 5000.0f, 250.0f, 100.0f, 100.0f, 0x3f )
#define MONKY_PRESET_CAVE             MonkyReverbProperties(  0,  8,  1.00f, -1000,  0,      0,   2.91f,  1.30f, 1.0f,   -602, 0.015f,  -302, 0.022f, 0.25f, 0.000f, 5000.0f, 250.0f, 100.0f, 100.0f, 0x1f )
#define MONKY_PRESET_ARENA            MonkyReverbProperties(  0,  9,  1.00f, -1000,  -698,   0,   7.24f,  0.33f, 1.0f,  -1166, 0.020f,    16, 0.030f, 0.25f, 0.000f, 5000.0f, 250.0f, 100.0f, 100.0f, 0x3f )
#define MONKY_PRESET_HANGAR           MonkyReverbProperties(  0,  10, 1.00f, -1000,  -1000,  0,   10.05f, 0.23f, 1.0f,   -602, 0.020f,   198, 0.030f, 0.25f, 0.000f, 5000.0f, 250.0f, 100.0f, 100.0f, 0x3f )
#define MONKY_PRESET_CARPETTEDHALLWAY MonkyReverbProperties(  0,  11, 1.00f, -1000,  -4000,  0,   0.30f,  0.10f, 1.0f,  -1831, 0.002f, -1630, 0.030f, 0.25f, 0.000f, 5000.0f, 250.0f, 100.0f, 100.0f, 0x3f )
#define MONKY_PRESET_HALLWAY          MonkyReverbProperties(  0,  12, 1.00f, -1000,  -300,   0,   1.49f,  0.59f, 1.0f,  -1219, 0.007f,   441, 0.011f, 0.25f, 0.000f, 5000.0f, 250.0f, 100.0f, 100.0f, 0x3f )
#define MONKY_PRESET_STONECORRIDOR    MonkyReverbProperties(  0,  13, 1.00f, -1000,  -237,   0,   2.70f,  0.79f, 1.0f,  -1214, 0.013f,   395, 0.020f, 0.25f, 0.000f, 5000.0f, 250.0f, 100.0f, 100.0f, 0x3f )
#define MONKY_PRESET_ALLEY            MonkyReverbProperties(  0,  14, 0.30f, -1000,  -270,   0,   1.49f,  0.86f, 1.0f,  -1204, 0.007f,    -4, 0.011f, 0.25f, 0.000f, 5000.0f, 250.0f, 100.0f, 100.0f, 0x3f )
#define MONKY_PRESET_FOREST           MonkyReverbProperties(  0,  15, 0.30f, -1000,  -3300,  0,   1.49f,  0.54f, 1.0f,  -2560, 0.162f,  -229, 0.088f, 0.25f, 0.000f, 5000.0f, 250.0f,  79.0f, 100.0f, 0x3f )
#define MONKY_PRESET_CITY             MonkyReverbProperties(  0,  16, 0.50f, -1000,  -800,   0,   1.49f,  0.67f, 1.0f,  -2273, 0.007f, -1691, 0.011f, 0.25f, 0.000f, 5000.0f, 250.0f,  50.0f, 100.0f, 0x3f )
#define MONKY_PRESET_MOUNTAINS        MonkyReverbProperties(  0,  17, 0.27f, -1000,  -2500,  0,   1.49f,  0.21f, 1.0f,  -2780, 0.300f, -1434, 0.100f, 0.25f, 0.000f, 5000.0f, 250.0f,  27.0f, 100.0f, 0x1f )
#define MONKY_PRESET_QUARRY           MonkyReverbProperties(  0,  18, 1.00f, -1000,  -1000,  0,   1.49f,  0.83f, 1.0f, -10000, 0.061f,   500, 0.025f, 0.25f, 0.000f, 5000.0f, 250.0f, 100.0f, 100.0f, 0x3f )
#define MONKY_PRESET_PLAIN            MonkyReverbProperties(  0,  19, 0.21f, -1000,  -2000,  0,   1.49f,  0.50f, 1.0f,  -2466, 0.179f, -1926, 0.100f, 0.25f, 0.000f, 5000.0f, 250.0f,  21.0f, 100.0f, 0x3f )
#define MONKY_PRESET_PARKINGLOT       MonkyReverbProperties(  0,  20, 1.00f, -1000,  0,      0,   1.65f,  1.50f, 1.0f,  -1363, 0.008f, -1153, 0.012f, 0.25f, 0.000f, 5000.0f, 250.0f, 100.0f, 100.0f, 0x1f )
#define MONKY_PRESET_SEWERPIPE        MonkyReverbProperties(  0,  21, 0.80f, -1000,  -1000,  0,   2.81f,  0.14f, 1.0f,    429, 0.014f,  1023, 0.021f, 0.25f, 0.000f, 5000.0f, 250.0f,  80.0f,  60.0f, 0x3f )
#define MONKY_PRESET_UNDERWATER       MonkyReverbProperties(  0,  22, 1.00f, -1000,  -4000,  0,   1.49f,  0.10f, 1.0f,   -449, 0.007f,  1700, 0.011f, 1.18f, 0.348f, 5000.0f, 250.0f, 100.0f, 100.0f, 0x3f )
