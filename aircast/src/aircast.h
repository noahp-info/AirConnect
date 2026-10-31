/*
 *  AirCast: Chromecast to AirPlay
 *
 *  (c) Philippe, philippe_44@outlook.com
 *
 *  See LICENSE
 *
 */

#pragma once

#include <signal.h>
#include <stdarg.h>
#include <stdio.h>

#include "platform.h"
#include "pthread.h"
#include "raop_server.h"
#include "cast_util.h"

#define VERSION "v1.12.4"" ("__DATE__" @ "__TIME__")"

/*----------------------------------------------------------------------------*/
/* typedefs */
/*----------------------------------------------------------------------------*/

#define STR_LEN	256

#define MAX_PROTO		128
#define MAX_RENDERERS	32
#define	AV_TRANSPORT 	"urn:schemas-upnp-org:service:AVTransport:1"
#define	RENDERING_CTRL 	"urn:schemas-upnp-org:service:RenderingControl:1"
#define	CONNECTION_MGR 	"urn:schemas-upnp-org:service:ConnectionManager:1"
#define MAGIC			0xAABBCCDD
#define RESOURCE_LENGTH	250
#define	SCAN_TIMEOUT 	15
#define SCAN_INTERVAL	30

enum 	eMRstate { STOPPED, PLAYING, PAUSED };
enum	eVolumeSync { VOLUME_SYNC_NEVER = 0, VOLUME_SYNC_START = 1, VOLUME_SYNC_ALWAYS = 2, VOLUME_SYNC_SENDER = 3 };

typedef struct sMRConfig
{
	bool		Enabled;
	bool		StopReceiver;
	char		Name[STR_LEN];
	char		Codec[STR_LEN];
	bool		Metadata;
	bool		Flush;
	double		MediaVolume;
	uint8_t		mac[6];
	char		Latency[STR_LEN];
	bool		Drift;
	char		ArtWork[4*STR_LEN];
	int			VolumeSync;	// 0 = never take iPhone volume, 1 = only at session start, 2 = always, 3 = iPhone always, never echo cast volume back
} tMRConfig;

struct sMR {
	uint32_t Magic;
	bool  Running;
	tMRConfig Config;
	struct raopsr_s *Raop;
	raopsr_event_t	RaopState;
	char UDN	   	[RESOURCE_LENGTH];
	char Name		[STR_LEN];
	enum eMRstate 	State;
	bool			ExpectStop;
	uint32_t			Elapsed;
	unsigned		TrackPoll;
	void			*CastCtx;
	pthread_mutex_t Mutex;
	pthread_t 		Thread;
	double			Volume;
	uint32_t			VolumeStampRx, VolumeStampTx;
	uint32_t			LoadStamp;
	uint32_t			SessionStamp;
	uint32_t			PingStamp, PingFailStamp;	// removal grace when mDNS goes quiet
	bool			LoadWarned;
	bool			Group;
	struct sGroupMember {
		struct sGroupMember	*Next;
		struct in_addr		Host;
		uint16_t				Port;
   } *GroupMaster;
   bool Remove;
};

extern int32_t				glLogLimit;
extern tMRConfig			glMRConfig;
extern struct sMR			*glMRDevices;
extern int					glMaxDevices;
extern unsigned short		glPortBase, glPortRange;
extern char					glBinding[16];
