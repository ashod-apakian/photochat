/*-----------------------------------------------------------------------*/
 #pragma once
 #ifndef INC_MB_H
 #define INC_MB_H
 #define PUB                           extern
 #ifdef __cplusplus
 PUB "C" {
 #endif
/*-----------------------------------------------------------------------*/
 #include "aa.h"
/*-----------------------------------------------------------------------*/
 #define MB_VERSION                    "2"
 #define DEV_VERSION                   "0.80"
 #define AUTO_MINIMIZE                 000
/*-----------------------------------------------------------------------*/

 #define I_AM_UNKNOWN                  0
 #define I_AM_GOOGLE_CLOUD             1
 #define I_AM_AMAZON_CLOUD             2
 #define I_AM_DESKTOP                  3
 #define I_AM_THINKPAD                 4
 #define I_AM_GIGA                     5
 #define I_AM_GODADDY                  6
 #define I_AM_DIGITALPACIFIC           7

/*-----------------------------------------------------------------------*/

/*
 structure
 {
 Q offset;
 Q bytes;
 B cmd[129];
 Q utc;
 B room[129];
 }
 _history;


 structure
 {
 H slots;
 H count;
 Q file_bytes;
 Q checked_bytes;
 _history*history;
 }
 _historylog;
 */


 structure
 {
 H magic;
 B do_quit;
 _systime launch_time;
 B launch_text[65];
 _info info;
 Q log_counter;
 Q log_ms;
 B manu[513];
 N who_i_am;
 _surfaceunit surf;
 H tray_icon_index;
 Q tray_icon_ms;
 _surfaceunit canvas;
 Q canvas_update_ms;
 _fontunit font[1];
 B is_uploading_upgrade;
 D upload_complete;
 _jsonunit json;
 _jsonline json_line[32];
 B json_text[32][_64K];
 B billboard[257];
// _historylog historylog;
 Q history_count;
 Q history_start_index;
 Q history_start_offset;
 }
 _mb;


/*-----------------------------------------------------------------------*/


 B mbStart                             (V);
 V mbStop                              (V);
 B mbYield                             (V);
 B mbLog                               (VP fmt,...);

/*-----------------------------------------------------------------------*/


 structure
 {
 H magic;
 B i_upgrade;
 B upgrade_exe[257];
 B exe[257];
 Q tik;
 }
 _repo;

/*-----------------------------------------------------------------------*/


 B repoInit                            (_repo*repo,VP fmt,...);
 B repoDeleteUpgrade                   (_repo*repo);
 B repoProjectUpgrade                  (_repo*repo);
 B repoLaunchUpgrade                   (_repo*repo);
 B repoLaunchProject                   (_repo*repo);
 B repoUpgradeEvent                    (_repo*repo);


/*-----------------------------------------------------------------------*/

 V pushAndCommit                       (V);

/*-----------------------------------------------------------------------*/

 B jsonDecode                          (H bytes,VP data);
 B jsonDump                            (V);

/*-----------------------------------------------------------------------*/

 B websocketPktDump                    (_tcpcallunit*call,B oc,B ff,H bytes,VP data);
 B websocketPinger                     (_tcpcallunit*call);
 B websocketPonger                     (_tcpcallunit*call,H bytes,VP data);
 B websocketReader                     (_tcpcallunit*call,BP oc,BP ff,HP bytes,H maxbytes,VP data);



/*-----------------------------------------------------------------------*/

 #define CALL_TYPE_PEER                1
 #define CALL_TYPE_DASHBOARD           2
 #define CALL_TYPE_ELOG                3


 #define MB_GUIDE_STATE_UNUSED         0
 #define MB_GUIDE_STATE_INUSE          1
 #define MB_GUIDE_STATE_CLOSING        3
 #define MB_GUIDE_STATE_READY          4



 structure
 {
 H count;
 //N guide_index[100];
 N ch_array_index[100];
 }
 _servergather;



 structure
 {
 H state;
 H call_handle;
 H call_session;
 B uuid[150];
 B room_name[720];
 H type;
 }
 _serverguide;


 structure
 {
 H stage;
 H type;
 N ch_array_index;
 //N guide_index;
 B is_ws;
 B do_close;
 B uuid[50];
 B hancock[65];
 B room_name[720];
 _websocket wock;
 Q ms_ok_last_cmd;
 D mic_level;
 //B is_talking;
 //B is_muted;
 //Q dash_tik;
 _filestreamunit fsu[16];
 }
 _servercalldata;



 structure
 {
 H magic;
 H user_parm;
 H max_calls;
 H cur_calls;
 _netadr adr;
 _tcpportunit port;
 _tcpcallunit call;
 _servercalldata*cd;
 //_serverguide*guide;
 HP ch_array;
 B cmd[_128K];
 VP proc;
 }
 _server;


/*-----------------------------------------------------------------------*/

 B serverNew                           (_server*server,H ip,W port,H usrparm,H maxcalls,VP proc);
 B serverDelete                        (_server*server);
 B serverGuideEmptyFind                (_server*server,NP index);
 B serverGuideFindByCallHandle         (_server*server,NP index,H callhandle,H callsesh);
 B serverGuideFindByUuid               (_server*server,NP index,VP uuid);
 B serverCallAnswer                    (_server*server);
 B serverCallDestroy                   (_server*server);
 B serverYield                         (_server*server,H ita);
 V serverProc                          (_server*server);

 B serverOnMessage                     (_server*server);

 B serverRoomGather                    (_server*server,_servergather*gather,VP room);

 B serverCmdBegins                     (_server*server,VP fmt,...);
 B serverCmdFinish                     (_server*server);
 B serverCmdEolFix                     (_server*server);
 B serverCmdKeyVal                     (_server*server,VP key,VP fmt,...);
 B serverCmdKeyInt                     (_server*server,VP key,G val);
 B serverCmdKeyDub                     (_server*server,VP key,D val);
 B serverCmdAppend                     (_server*server,VP fmt,...);
 B serverCmdString                     (_server*server,VP fmt,...);


 B serverSendWelcome                   (_server*server);
 B serverBroadcastUserJoined           (_server*server);
 B serverBroadcastUserLeft             (_server*server);

 B serverOnMyIceOffer                  (_server*server);
 B serverOnMyIceAnswer                 (_server*server);

 B serverOnMyIceCandidate              (_server*server);

 B serverOnMicLevel                    (_server*server);

 B serverOnChatMsg                     (_server*server);
 B serverOnChatMsgEcho                 (_server*server);

 B serverOnChatMsgReplay               (_server*server);

 B serverOnVideoBlob                   (_server*server);

 B serverOnElog                        (_server*server);

 //B serverSendRoster                    (_server*server);
 //B serverBroadcastUserJoined           (_server*server);
 //B serverBroadcastUserLeft             (_server*server);


/*-----------------------------------------------------------------------*/

 V bibleMaker                          (V);

/*-----------------------------------------------------------------------*/
 #include "curler.h"
/*-----------------------------------------------------------------------*/


 PUB _mb     mb;
 PUB _repo   the_repo;
 PUB _server the_server;


/*-----------------------------------------------------------------------*/
 #endif
/*-----------------------------------------------------------------------*/
 #ifdef __cplusplus
 }
 #endif






