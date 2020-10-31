/*-----------------------------------------------------------------------*/
 #include "mb.h"
/*-----------------------------------------------------------------------*/


// Q history_skip=0;
// Q history_count=0;


 B DiscoverHistory (V)
 {
 B ret;
 Q bytes;
 H curindex;
 H left,add;
 _filestreamunit fsu;
 H chars,stage;
 B mode;
 Q foff;
 Q offset;
 B block[_128K];

 mb.history_count=0;
 mb.history_start_offset=0;
 mb.history_start_index=0;
 if(aaFileExists("the_history.txt")!=YES) { return RET_NO; }
 if((ret=aaFileSizeGet("the_history.txt",&bytes))!=YES) { oops; }
 if((ret=aaFileStreamOpenQuick(&fsu.handle,"the_history.txt"))!=YES) { oops; return ret; }
 aaFileStreamStatus(fsu.handle,&fsu.status);
 curindex=0;
 stage=0;
 while(1)
  {
  aaFileStreamStatus(fsu.handle,&fsu.status);
  if(fsu.status.left<30) { stage=F32; break; }
  if((ret=aaFileStreamStringLen(fsu.handle,&chars,&mode,&offset))!=YES) { stage=F32; oops; }
  if(chars<2) { aaNote(0,"chars=%i, around %I^4d",chars,fsu.status.offset); break; }
  if((ret=aaFileStreamRead(fsu.handle,chars,block))!=YES) { stage=F32;  oops; }
  chars-=2;
  block[chars]=NULL_CHAR;
  switch(stage)
   {
   case 0:
   if(chars!=0) { aaNote(0,"chars=%i not zero around %I64d",chars,fsu.status.offset); stage=F32; break; }
   stage=10;
   break;
   case 10: //cmd
   stage=20;
   break;
   case 20: //utc
   stage=30;
   break;
   case 30: //room
   stage=40;
   break;
   case 40: //uuid
   stage=50;
   break;
   case 50: //hancock
   stage=60;
   break;
   case 60: //ali
   stage=70;
   break;
   case 70: //img
   stage=80;
   break;
   case 80: //txt
   mb.history_count++;
   curindex++;
   stage=0;
   break;
   }
  }
 aaFileStreamDestroy(fsu.handle);
 aaDebugf("discovered %I64d",mb.history_count);

 //if(mb.history_count>=20)  {  mb.history_start_index=mb.history_count-20;  }
 return RET_YES;
 }



 B serverSendHistory (_server*server)
 {
 B ret;
 _servercalldata*cd;
 Q bytes;
 Q curindex;
 H left,add;
 _filestreamunit fsu;
 H chars,stage;
 B mode;
 Q foff;
 Q offset;
 B block[_128K];

 if(server==NULL)                    { return RET_MISSINGPARM; }
 if(server->magic!=aaHPP(serverNew)) { return RET_NOTINITIALIZED; }
 cd=(_servercalldata*)server->cd;
 if(mb.history_count==0) { return RET_YES; }
 if((ret=aaFileSizeGet("the_history.txt",&bytes))!=YES)
  {
  DiscoverHistory();
  if(mb.history_count==0) { return RET_YES; }
  if((ret=aaFileSizeGet("the_history.txt",&bytes))!=YES) { oops; }
  }
 aaDebugf("bytes=%I64d ",bytes);
 mb.history_start_offset=0;
 if((ret=aaFileStreamOpenQuick(&fsu.handle,"the_history.txt"))!=YES) { oops; return ret; }
 if((ret=aaFileStreamOffsetSet(fsu.handle,mb.history_start_offset))!=YES) { aaFileStreamDestroy(fsu.handle); oops; return ret; }
 aaFileStreamStatus(fsu.handle,&fsu.status);
 aaDebugf("left=%I64d",fsu.status.left);
 curindex=0;
 stage=0;
 while(1)
  {
  aaFileStreamStatus(fsu.handle,&fsu.status);
//  aaDebugf("%I64d left",fsu.status.left);
//  if(fsu.status.left<30) { stage=F32; break; }
  //if(chars<2) { aaDebugf("chars=%i, around %I^4d",chars,fsu.status.offset); break; }
   if((ret=aaFileStreamStringLen(fsu.handle,&chars,&mode,&offset))!=YES) { stage=F32; break; }
  if((ret=aaFileStreamRead(fsu.handle,chars,block))!=YES) { stage=F32;  oops; }
  chars-=2;
  block[chars]=NULL_CHAR;
  switch(stage)
   {
   case 0:
   if(chars!=0) { aaNote(0,"chars=%i not zero around %I64d",chars,fsu.status.offset); stage=F32; break; }
   stage=10;
   break;
   case 10: //cmd
   //if(curindex==index) { aaDebugf("********"); }
//   aaDebugf("%i %s",curindex,block);
   serverCmdBegins(server,"chatecho");
   stage=20;
   break;
   case 20: //utc
   stage=30;
   break;
   case 30: //room
   serverCmdKeyVal(server,"room","%s",cd->room_name);
   stage=40;
   break;
   case 40: //uuid
   serverCmdKeyVal(server,"uuid","%s",cd->uuid);
   stage=50;
   break;
   case 50: //hancock
   serverCmdKeyVal(server,"hancock","%s",cd->hancock);
   stage=60;
   break;
   case 60: //ali
   serverCmdKeyVal(server,"ali","%s",&block[4]);
   //aaDebugf("left=%I64d index=%i chars=%i stage=%i block=%s",fsu.status.left,curindex,chars,stage,&block[4]);
   ////serverCmdKeyVal(server,"ali","%s",mb.json_text[4]);
   stage=70;
   break;
   case 70: //img
   serverCmdKeyVal(server,"img","%s",&block[4]);
   //aaDebugf("left=%I64d index=%i chars=%i stage=%i block=%s",fsu.status.left,curindex,chars,stage,&block[4]);
   ///serverCmdKeyVal(server,"img","%s",mb.json_text[6]);
   stage=80;
   break;
   case 80: //txt
   serverCmdKeyVal(server,"txt","%s",&block[4]);
   //serverCmdEolFix(server);
   serverCmdFinish(server);
   //if(curindex>=mb.history_start_index)
    {
    aaDebugf("sending %I64d",curindex);
    if((ret=aaNetWebsocketWrite(&cd->wock,WEBSOCKET_OPCODE_TEXT,1,0,server->cmd))!=RET_YES) { oops;}
    }
   //aaDebugf("left=%I64d index=%i chars=%i stage=%i block=%s",fsu.status.left,curindex,chars,stage,&block[4]);
   ///serverCmdKeyVal(server,"txt","%s",mb.json_text[8]);
   //aaDebugf("%i %s",curindex,block);
   curindex++;
   stage=0;
   break;
   }
  }
 aaFileStreamDestroy(fsu.handle);
 //if(mb.history_count>=20)  {  mb.history_start_index=mb.history_count-20;  }
 return RET_YES;
 }


 /*
  serverCmdBegins(server,"chatecho");
  serverCmdKeyVal(server,"room","%s",cd->room_name);
  serverCmdKeyVal(server,"uuid","%s",cd->uuid);
  serverCmdKeyVal(server,"hancock","%s",cd->hancock);
  serverCmdKeyVal(server,"troom","%s",cd->room_name);
  serverCmdKeyVal(server,"tuuid","%s",cd->uuid);
  serverCmdKeyVal(server,"thancock","%s",cd->hancock);

  serverCmdKeyVal(server,"ali","%s",mb.json_text[4]);
  serverCmdKeyVal(server,"img","%s",mb.json_text[6]);
  serverCmdKeyVal(server,"txt","%s",mb.json_text[8]);


  serverCmdBegins(server,"chatecho");
  serverCmdKeyVal(server,"room","%s",cd->room_name);
  serverCmdKeyVal(server,"xfwd","%s",cd->wock.x_forwarded_for);
  serverCmdEolFix(server);
  serverCmdFinish(server);
  if((ret=aaNetWebsocketWrite(&cd->wock,WEBSOCKET_OPCODE_TEXT,1,0,server->cmd))!=RET_YES) { oops;}

*/







/*
 aaJournal("the_history.txt","");
 aaJournal("the_history.txt","cmd=chatecho");
 if((ret=aaTimeUtcExGet(&utc))!=YES) { oops; }
 aaJournal("the_history.txt","utc=%I64u",utc);
 aaJournal("the_history.txt","room=%s",cd->room_name);
 aaJournal("the_history.txt","uuid=%s",cd->uuid);
 aaJournal("the_history.txt","hancock=%s",cd->hancock);
 aaJournal("the_history.txt","ali=%s",mb.json_text[4]);
 aaJournal("the_history.txt","img=%s",mb.json_text[6]);
 aaJournal("the_history.txt","txt=%s",mb.json_text[8]);

  serverCmdBegins(server,"chatecho");
  serverCmdKeyVal(server,"room","%s",cd->room_name);
  serverCmdKeyVal(server,"uuid","%s",cd->uuid);
  serverCmdKeyVal(server,"hancock","%s",cd->hancock);
  serverCmdKeyVal(server,"troom","%s",tcd->room_name);
  serverCmdKeyVal(server,"tuuid","%s",tcd->uuid);
  serverCmdKeyVal(server,"thancock","%s",tcd->hancock);
  serverCmdKeyVal(server,"ali","%s",mb.json_text[4]);
  serverCmdKeyVal(server,"img","%s",mb.json_text[6]);
  serverCmdKeyVal(server,"txt","%s",mb.json_text[8]);
  serverCmdFinish(server);
  aaLeu;

  serverCmdBegins(server,"chatecho");
  serverCmdKeyVal(server,"room","%s",cd->room_name);
  serverCmdKeyVal(server,"uuid","%s",cd->uuid);
  serverCmdKeyVal(server,"hancock","%s",cd->hancock);
  serverCmdKeyVal(server,"troom","%s",tcd->room_name);
  serverCmdKeyVal(server,"tuuid","%s",tcd->uuid);
  serverCmdKeyVal(server,"thancock","%s",tcd->hancock);
  serverCmdKeyVal(server,"ali","%s",mb.json_text[4]);
  serverCmdKeyVal(server,"img","%s",mb.json_text[6]);
  serverCmdKeyVal(server,"txt","%s",mb.json_text[8]);

  */

  /*
  serverCmdBegins(server,"chatecho");
  serverCmdKeyVal(server,"room","%s",cd->room_name);
  serverCmdKeyVal(server,"uuid","%s",cd->uuid);
  serverCmdKeyVal(server,"hancock","%s",cd->hancock);
  serverCmdKeyVal(server,"troom","%s",tcd->room_name);
  serverCmdKeyVal(server,"tuuid","%s",tcd->uuid);
  serverCmdKeyVal(server,"thancock","%s",tcd->hancock);
  serverCmdKeyVal(server,"ali","%s",mb.json_text[4]);
  serverCmdKeyVal(server,"img","%s",mb.json_text[6]);
  serverCmdKeyVal(server,"txt","%s",mb.json_text[8]);

 serverCmdBegins(server,"chatecho");
 serverCmdKeyVal(server,"room","%s",cd->room_name);
 serverCmdKeyVal(server,"uuid","%s",cd->uuid);
 serverCmdKeyVal(server,"hancock","%s",cd->hancock);
 serverCmdKeyVal(server,"xfwd","%s",cd->wock.x_forwarded_for);
 serverCmdKeyInt(server,"peerCount",(G)room_gather.count);
 serverCmdString(server,"peerList");
 serverCmdAppend(server,":");
 serverCmdAppend(server,"[");
 for(i=0;i<room_gather.count;i++)
  {
  tcall.handle=server->ch_array[room_gather.ch_array_index[i]];
  aaNetTcpCallStatus(tcall.handle,&tcall.status);
  tcd=(_servercalldata*)tcall.status.extra_data;
  serverCmdAppend(server,"{");
  //serverCmdKeyVal(server,"uuid",server->guide[room_gather.guide_index[i]].uuid);
  serverCmdKeyVal(server,"uuid",tcd->uuid);
  serverCmdKeyVal(server,"hancock",tcd->hancock);
  serverCmdEolFix(server);
  serverCmdAppend(server,"},");
  if(aaStringICompare(cd->uuid,tcd->uuid,0)==YES)
   {
   ////aaDebugf(" we notify them of THEMSELVES peer %s",tcd->hancock);
   }
  else
   {
   ////aaDebugf(" we notify them of existing peer %s",tcd->hancock);
   }
  }
 serverCmdEolFix(server);
 serverCmdAppend(server,"]");
 serverCmdFinish(server);
 //mbLog("%s sending welcome to ch=%x uuid=%s",cd->wock.x_forwarded_for,server->call.handle,cd->uuid);
 if((ret=aaNetWebsocketWrite(&cd->wock,WEBSOCKET_OPCODE_TEXT,1,0,server->cmd))!=RET_YES) { oops;}



 }



*/





/*

 aaJournal("the_history.txt","");
 aaJournal("the_history.txt","cmd=chatecho");
 if((ret=aaTimeUtcExGet(&utc))!=YES) { oops; }
 aaJournal("the_history.txt","utc=%I64u",utc);
 aaJournal("the_history.txt","room=%s",cd->room_name);
 aaJournal("the_history.txt","uuid=%s",cd->uuid);
 aaJournal("the_history.txt","hancock=%s",cd->hancock);
 aaJournal("the_history.txt","ali=%s",mb.json_text[4]);
 aaJournal("the_history.txt","img=%s",mb.json_text[6]);
 aaJournal("the_history.txt","txt=%s",mb.json_text[8]);
 */




 B serverNew                           (_server*server,H ip,W port,H usrparm,H maxcalls,VP proc)
 {
 B ret;

 aaLeu;
 if(server==NULL)                  { return RET_MISSINGPARM; }
 aaMemoryFill(server,sizeof(_server),0);
 server->magic=aaHPP(serverNew);
 if(maxcalls==0) { maxcalls=10000; }
 server->max_calls=maxcalls;
 server->user_parm=usrparm;
 if(port==0) { port=80; }
 aaNetAdrSet(&server->adr,ip,port);
 if((ret=aaNetTcpPortCreate(&server->port.handle,server->adr.ip,server->adr.port,sizeof(_servercalldata)))!=YES)  { oops;  }
 aaNetTcpPortStatus(server->port.handle,&server->port.status);
// mbLog("local_port=%s",server->port.status.local_dot);
 server->proc=proc;

 if((ret=aaMemoryAllocate((VP)&server->ch_array,server->max_calls*sizeof(H)))!=YES) { oops; }
 ///if((ret=aaMemoryAllocate((VP)&server->guide,server->max_calls*sizeof(_serverguide)))!=YES) { oops; }
 DiscoverHistory();
 return RET_YES;
 }






 B serverDelete                        (_server*server)
 {
 aaLeu;
 if(server==NULL)                    { return RET_MISSINGPARM; }
 if(server->magic!=aaHPP(serverNew)) { return RET_NOTINITIALIZED; }
 if(server->port.handle!=0)
  {
  aaNetTcpPortStatus(server->port.handle,&server->port.status);
  ///mbLog("waiting=%i answered=%i inuse=%i total=%i",server->port.status.calls_waiting,server->port.status.calls_answered,server->port.status.calls_inuse,server->port.status.calls_total);
  aaNetTcpPortDestroy(server->port.handle);
  server->port.handle=0;
  }
 /*
 if(server->max_calls!=0&&server->guide)
  {
  aaMemoryRelease(server->guide);
  server->guide=NULL;
  }
 */
 if(server->max_calls!=0&&server->ch_array)
  {
  aaMemoryRelease(server->ch_array);
  server->ch_array=NULL;
  }

 aaMemoryFill(server,sizeof(_server),0);
 return RET_YES;
 }





/*-----------------------------------------------------------------------*/


#if 0

 B serverGuideEmptyFind                (_server*server,NP index)
 {
 H i;
 N ok;

 aaLeu;
 if(server==NULL)                    { return RET_MISSINGPARM; }
 if(server->magic!=aaHPP(serverNew)) { return RET_NOTINITIALIZED; }
 if(index==NULL)                     { return RET_MISSINGPARM; }
 ok=-1;
 for(i=0;i<server->max_calls;i++)
  {
  if(server->guide[i].state==MB_GUIDE_STATE_UNUSED) { ok=(N)i; break; }
  }
 *index=ok;
 if(ok==-1) { return RET_NOTFOUND; }
 return RET_YES;
 }




 B serverGuideFindByCallHandle         (_server*server,NP index,H callhandle,H callsesh)
 {
 H i,c;
 N ok;

 aaLeu;
 if(server==NULL)                    { return RET_MISSINGPARM; }
 if(server->magic!=aaHPP(serverNew)) { return RET_NOTINITIALIZED; }
 if(index==NULL)                     { return RET_MISSINGPARM; }
 c=0;
 ok=-1;
 for(i=0;i<server->max_calls;i++)
  {
  if(c>=server->cur_calls) { break;    }
  if(server->guide[i].state==MB_GUIDE_STATE_UNUSED) { continue; }
  c++;
  if(server->guide[i].call_handle==callhandle)
   {
   if(server->guide[i].call_session==callsesh)
    {
    ok=(N)i; break;
    }
   else
    {
    aaDebugf("ch ok session not");
    }

   }
  }
 *index=ok;
 if(ok==-1) { return RET_NOTFOUND; }
 return RET_YES;
 }




 B serverGuideFindByUuid               (_server*server,NP index,VP uuid)
 {
 H i,c;
 N ok;

 aaLeu;
 if(server==NULL)                    { return RET_MISSINGPARM; }
 if(server->magic!=aaHPP(serverNew)) { return RET_NOTINITIALIZED; }
 if(index==NULL)                     { return RET_MISSINGPARM; }
 c=0;
 ok=-1;
 for(i=0;i<server->max_calls;i++)
  {
  if(c>=server->cur_calls) { break;    }
  if(server->guide[i].state==MB_GUIDE_STATE_UNUSED) { continue; }
  c++;
  if(aaStringICompare(server->guide[i].uuid,uuid,0)==YES) { ok=(N)i; break; }
  }
 *index=ok;
 if(ok==-1) { return RET_NOTFOUND; }
 return RET_YES;
 }

#endif


/*-----------------------------------------------------------------------*/


 Q global_ran=100000000000LL;
 Q global_msg=100000000000LL;




 B serverCallAnswer                    (_server*server)
 {
 B ret;
 H i;
 N ok;
 _servercalldata*cd;
 Q ran;

 if(server==NULL)                    { return RET_MISSINGPARM; }
 if(server->magic!=aaHPP(serverNew)) { return RET_NOTINITIALIZED; }
 if((ret=aaNetTcpCallAnswer(server->call.handle))!=YES) { oops; }
 aaNetTcpCallInactivityResetOn(server->call.handle,YES,YES);
 aaNetTcpCallStatus(server->call.handle,&server->call.status);
 if(server->call.status.extra_bytes!=sizeof(_servercalldata)) { oof; }
 cd=(_servercalldata*)server->call.status.extra_data;
 cd->ch_array_index=-1;
 cd->ms_ok_last_cmd=aaMsRunning();
 ok=-1;
 for(i=0;i<server->max_calls;i++)
  {
  if(server->ch_array[i]==0) { ok=(N)i; break; }
  }
 if(ok==-1) { oof; }
 cd->ch_array_index=ok;
 server->ch_array[ok]=server->call.handle;
 i=(H)ok;
 while(1)
  {
  //ran=global_ran;
  //global_ran+=aaMathRand64(1,2492);
  ran=aaMathRand64(1000000000,9999999999);
  aaStringCopyf(cd->uuid,"%I64u",ran);
  aaStringHancock(cd->hancock,(H)ran);
  break;
  }
 server->cur_calls++;
 cd=(_servercalldata*)server->call.status.extra_data;
 //cd->guide_index=(N)i;
 return RET_YES;
 }







 B serverCallDestroy                   (_server*server)
 {
 B ret;
 H i,m;
 N ok;
 B fn[_1K];
 Q fb;
 Q ran;

 aaLeu;
 if(server==NULL)                    { return RET_MISSINGPARM; }
 if(server->magic!=aaHPP(serverNew)) { return RET_NOTINITIALIZED; }

 ok=-1;
 for(i=0;i<server->max_calls;i++)
  {
  if(server->ch_array[i]==server->call.handle) { ok=(N)i; break; }
  }
 if(ok==-1) { oof; }


 //ret=serverGuideFindByCallHandle(server,&ok,server->call.handle,server->call.status.session);
 //if(ret==RET_NOTFOUND) { aaNote(0,"%s %i %s gi=%i, dc=%i stg=%i ch=%i",__func__,__LINE__,arets,server->cd->guide_index,server->cd->do_close,server->cd->stage,server->call.handle); return ret; }
 //if(ret!=RET_YES)      { oops; }
 //i=(H)ok;
 if(server->cd->is_ws)
  {
  //mbLog("%s destroyed, uuid=%s",server->cd->wock.x_forwarded_for,server->guide[i].uuid);
  }

 m=aaElementCount(server->cd->fsu);
 for(i=0;i<m;i++)
  {
  if(server->cd->fsu[i].handle)
   {
   aaFileStreamStatus(server->cd->fsu[i].handle,&server->cd->fsu[i].status);
   fb=server->cd->fsu[i].status.bytes;
   aaStringCopy(fn,server->cd->fsu[i].status.filename);
   ////aaDebugf("%s destroyed with %I64d bytes",fn,fb);
   aaFileStreamDestroy(server->cd->fsu[i].handle);
   server->cd->fsu[i].handle=0;
   if(fb<_64K)
    {
    ret=aaFileDelete("%s",fn);
    ////aaDebugf("file delete %s because too small %I64d bytes, ret=%s",fn,fb,arets);
    }
   //aaFileStreamDestroy(server->cd->fsu[i].handle);
   }
  server->cd->fsu[i].handle=0;
  }

 server->ch_array[ok]=0;
 /*
 server->guide[i].state=MB_GUIDE_STATE_UNUSED;
 server->guide[i].call_handle=0;
 server->guide[i].call_session=0;
 server->guide[i].uuid[0]=NULL_CHAR;
 server->guide[i].room_name[0]=NULL_CHAR;
 server->guide[i].type=0;
 */
 aaNetTcpCallDestroy(server->call.handle);
 server->call.handle=0;
 server->cd=NULL;
 server->cur_calls--;
 return RET_YES;
 }









 B serverYield                         (_server*server,H ita)
 {
 B ret;
 _servercalldata*cd;
 V(*proc)(_server*);

 aaLeu;
 if(server==NULL)                    { return RET_MISSINGPARM; }
 if(server->magic!=aaHPP(serverNew)) { return RET_NOTINITIALIZED; }
 if(ita==0) { ita=1; }
 while(ita--)
  {
  server->cd=NULL;
  aaNetTcpPortStatus(server->port.handle,&server->port.status);
  if(aaNetTcpPortCallNext(server->port.handle,&server->call.handle,&server->call.status,0)!=YES) { continue; }
  if(server->call.status.is_connected!=YES)
   {
   if(server->max_calls>0&&server->port.status.calls_inuse>=server->max_calls)   {  aaNetTcpCallDestroy(server->call.handle);    }
   else                                                                          {  serverCallAnswer(server);    }
   continue;
   }
  if(server->call.status.extra_bytes!=sizeof(_servercalldata)) { mbLog("eb=%i",server->call.status.extra_bytes); }
  cd=(_servercalldata*)server->call.status.extra_data;
  server->cd=cd;

  switch(cd->do_close)
   {
   case 0:
   if((aaMsRunning()-cd->ms_ok_last_cmd)<aaSecs(30)) { break; }
   if((aaMsRunning()-cd->wock.ping_last_ms)<aaSecs(30)) { break; }
   ///mbLog("%s timeout, lastcmd=%I64d, lastping=%I64d",cd->uuid,aaMsRunning()-cd->ms_ok_last_cmd,aaMsRunning()-cd->wock.ping_last_ms);
   cd->do_close=1;
   break;


   case 1:
   if(cd->room_name[0]!=NULL_CHAR)
    {
    if(cd->type==CALL_TYPE_PEER)
     {
     serverBroadcastUserLeft(server);
     }
    }
   if(cd->is_ws&&cd->wock.is_open)
    {
    if((ret=aaNetWebsocketClose(&cd->wock))!=YES)
     {
     aaDebugf(">>>> %s %i %s",__FILE__,__LINE__,arets);
     }
    }
   ///if(cd->guide_index>=0)    {    server->guide[cd->guide_index].state=MB_GUIDE_STATE_CLOSING;    }
   aaNetTcpCallClose(server->call.handle);
   aaNetTcpCallStatus(server->call.handle,&server->call.status);
   cd->do_close=2;
   ita=0;
   break;


   case 2:
   //if(cd->guide_index>=0)    {    server->guide[cd->guide_index].state=MB_GUIDE_STATE_CLOSING;    }
   serverCallDestroy(server);
   ita=0;
   break;
   }

  if(server->cd==NULL)   {   break;   }

  if(server->call.status.is_closed)
   {
   if(cd->do_close==0&&cd->stage<10)
    {
   // aaNote(0,"line=%i , do_close=%i cd->stage=%i",__LINE__,cd->do_close,cd->stage);
    ///mbLog("should be closed %i, stage=%i ",server->call.handle,cd->stage);
    cd->do_close=1;
    //if(cd->guide_index>=0)     {     server->guide[cd->guide_index].state=MB_GUIDE_STATE_CLOSING;     }
    continue;
    }
   }

  if((proc=server->proc))   {   proc(server);   }
  }
 return RET_YES;
 }





 V serverProc                          (_server*server)
 {
 B ret;
 _servercalldata*cd;
 B oc,ff;
 H bytes,hv,sl,count;
 B data[_128K];
 //B blob[_32K];

 aaLeu;
 cd=(_servercalldata*)server->cd;
 switch(cd->stage)
  {
  case 0:
  if((ret=aaNetWebsocketInit(&cd->wock,server->call.handle,0))!=YES) { oops; }
  //mbLog("new call init");
  cd->ms_ok_last_cmd=aaMsRunning();
  cd->is_ws=1;
  cd->stage=10;
  break;


  case 3:
  if(cd->do_close==0)
   {
   ///if(cd->guide_index>=0)    {    server->guide[cd->guide_index].state=MB_GUIDE_STATE_CLOSING;    }
   cd->do_close=1;
   }
  cd->stage=5;
  break;



  case 5:
  cd->do_close=2;
  break;




  case 10:
  if(server->call.status.is_closed)             { mbLog("line=%i isclosed",__LINE__); cd->stage=3; break; }
  if((ret=aaNetWebsocketYield(&cd->wock))!=YES) { oops;  }
  if(cd->wock.is_failure)                       { mbLog("line=%i failure a",__LINE__); cd->stage=3;  break; }
  if(server->call.status.ms>=aaSecs(20))        { mbLog("stage10 timeout");  cd->stage=3; break; }
  if(cd->wock.is_open!=YES)                     { break; }
  aaStringLen(cd->wock.url,&sl);
  aaStringCountChars(cd->wock.url,sl,&count,FSLASH_CHAR,NO,YES);
  if((sl<2||sl>21)||(count!=1)) // fslash + 1 room char
   {
   mbLog("bad room, fslash=%i sl=%i is=(%s)",count,sl,cd->wock.url);
   cd->stage=3;
   break;
   }
  if(strlen(cd->room_name)>64||strlen(cd->wock.url)>128)
   {
   mbLog("room name strlen=%i",strlen(cd->room_name));
   mbLog("wock url strlen=%i",strlen(cd->wock.url));
   }



  aaStringCopy(cd->room_name,cd->wock.url);
  ///server->guide[cd->guide_index].state=MB_GUIDE_STATE_READY;
  ///aaStringCopy(server->guide[cd->guide_index].room_name,cd->room_name);
  //mbLog("%s new call,room=%s,uid=%s",cd->wock.x_forwarded_for,cd->room_name,cd->uuid);
  if(aaStringICompare(cd->wock.url,"/elog",0)==YES)
   {
   cd->type=CALL_TYPE_ELOG;
   cd->stage=1100;
   break;
   }


  if(aaStringICompare(cd->wock.url,"/dashboard",0)==YES)
   {
   cd->type=CALL_TYPE_DASHBOARD;
   ///server->guide[cd->guide_index].type=cd->type;
   cd->stage=1000;
   break;
   }
  cd->type=CALL_TYPE_PEER;
  ///server->guide[cd->guide_index].type=cd->type;
 /// serverSendWelcome(server);
//  serverSendRoster(server);
  ///serverBroadcastUserJoined(server);
  ///cd->stage=1000;
  cd->stage=900;
  break;



  case 900:
  if(cd->do_close) { break; }
  if((ret=aaNetWebsocketYield(&cd->wock))!=YES) { oops; }
  if(cd->wock.is_failure) { aaNote(0,"failure c"); }
  websocketPinger(&server->call);
  if((ret=websocketReader(&server->call,&oc,&ff,&bytes,sizeof(data),data))!=YES) { break; }
  if(bytes>=_64K)
   {
   mbLog("websocket frame is %i",bytes);
   break;
   }
  if(oc==WEBSOCKET_OPCODE_CLOSE&&ff==1)
   {
   if(bytes==2)     { hv=*(WP)&data[0];  hv=aaNumSwapWord(hv);    }
   if(cd->do_close==0)    {    cd->do_close=1;    }
   break;
   }
  if(oc==WEBSOCKET_OPCODE_PING&&ff==1)   {   websocketPonger(&server->call,bytes,data);   break;   }
  if(oc==WEBSOCKET_OPCODE_PONG&&ff==1)   {   cd->wock.ping_last_ms=aaMsRunning();         break;   }
  if(oc==WEBSOCKET_OPCODE_BINARY&&ff==1) {   websocketPktDump(&server->call,oc,ff,bytes,data);   break;   }
  if(oc==WEBSOCKET_OPCODE_TEXT&&ff==1)
   {
   jsonDecode(bytes,data);
   if(mb.json_line[1].type==JSON_TYPE_KEY)
    {
    if(mb.json_line[2].type==JSON_TYPE_STRING)
     {
     if(aaStringICompare(mb.json_text[1],"cmd",0)==YES)
      {
      if(aaStringICompare(mb.json_text[2],"hello",0)==YES)
       {
       cd->stage=1000;
       //aaDebugf("%s sent hello",cd->hancock);
       serverSendWelcome(server);
       serverSendHistory(server);
       serverBroadcastUserJoined(server);
       //aaDebugf("uuid %s , hancock %s sent hello",cd->uuid,cd->hancock);
       break;
       }
      }
     }
    }
   }
  break;





  case 1000:
  if(cd->do_close) { break; }
  if((ret=aaNetWebsocketYield(&cd->wock))!=YES) { oops; }
  if(cd->wock.is_failure) { aaNote(0,"failure c"); }
  websocketPinger(&server->call);
  if((ret=websocketReader(&server->call,&oc,&ff,&bytes,sizeof(data),data))!=YES) { break; }
  if(bytes>=_64K)
   {
   mbLog("b websocket frame is %i",bytes);
   break;
   }
  if(oc==WEBSOCKET_OPCODE_CLOSE&&ff==1)
   {
   //mbLog("received websock opcode close");
   if(bytes==2)     { hv=*(WP)&data[0];  hv=aaNumSwapWord(hv);    }
   if(cd->do_close==0)
    {
    //if(cd->guide_index>=0)     {     server->guide[cd->guide_index].state=MB_GUIDE_STATE_CLOSING;     }
    cd->do_close=1;
    }
   break;
   }
  if(oc==WEBSOCKET_OPCODE_PING&&ff==1)   {   websocketPonger(&server->call,bytes,data);   break;   }
  if(oc==WEBSOCKET_OPCODE_PONG&&ff==1)   {   cd->wock.ping_last_ms=aaMsRunning();         break;   }
  ///websocketPktDump(&server->call,oc,ff,bytes,data);
  if(oc==WEBSOCKET_OPCODE_BINARY&&ff==1) {   websocketPktDump(&server->call,oc,ff,bytes,data);   break;   }
  if(oc==WEBSOCKET_OPCODE_TEXT&&ff==1)   {   jsonDecode(bytes,data);   serverOnMessage(server);   break;   }
  break;




  case 1100:
  if(cd->do_close) { break; }
  if((ret=aaNetWebsocketYield(&cd->wock))!=YES) { oops; }
  if(cd->wock.is_failure) { aaNote(0,"failure c"); }
  websocketPinger(&server->call);
  if((ret=websocketReader(&server->call,&oc,&ff,&bytes,sizeof(data),data))!=YES) { break; }
  if(bytes>=_64K)
   {
   mbLog("c websocket frame is %i",bytes);
   break;
   }

  if(oc==WEBSOCKET_OPCODE_CLOSE&&ff==1)
   {
   //mbLog("received websock opcode close");
   if(bytes==2)     { hv=*(WP)&data[0];  hv=aaNumSwapWord(hv);    }
   if(cd->do_close==0)
    {
    //if(cd->guide_index>=0)     {     server->guide[cd->guide_index].state=MB_GUIDE_STATE_CLOSING;     }
    cd->do_close=1;
    }
   break;
   }
  if(oc==WEBSOCKET_OPCODE_PING&&ff==1)   {   websocketPonger(&server->call,bytes,data);   break;   }
  if(oc==WEBSOCKET_OPCODE_PONG&&ff==1)   {   cd->wock.ping_last_ms=aaMsRunning();         break;   }
  ///websocketPktDump(&server->call,oc,ff,bytes,data);
  if(oc==WEBSOCKET_OPCODE_BINARY&&ff==1) {   websocketPktDump(&server->call,oc,ff,bytes,data);   break;   }
  if(oc==WEBSOCKET_OPCODE_TEXT&&ff==1)   {   jsonDecode(bytes,data);   serverOnElog(server);   break;   }
  break;


  }
 }



/*-----------------------------------------------------------------------*/



 B serverOnMessage                     (_server*server)
 {
 B ret;
 _servercalldata*cd;
 B blob[_32K];
 B etc[_16K];
 B txt[_8K];
 _str32k str;
 D sr;
 _netstatus ns;
 H i,c;
 _tcpcallunit tcall;
 _servercalldata*tcd;

 aaLeu;
 if(server==NULL)                    { return RET_MISSINGPARM; }
 aaLeu;
 if(server->magic!=aaHPP(serverNew)) { return RET_NOTINITIALIZED; }
 aaLeu;
 cd=(_servercalldata*)server->cd;
aaLeu;
 if(mb.json_line[1].type!=JSON_TYPE_KEY)    { return RET_FAILED; }
 aaLeu;
 if(mb.json_line[2].type!=JSON_TYPE_STRING) { return RET_FAILED; }
 aaLeu;

 while(1)
  {
  aaLeu;
  if(aaStringICompare(mb.json_text[1],"cmd",0)!=YES) { break; }

  if(aaStringICompare(mb.json_text[2],"denied",0)==YES)
   {
   mbLog("********* %s received cmd=denied , uuid=%s, room=%s, replying with ackdenied",cd->wock.x_forwarded_for,cd->uuid,cd->room_name);
   cd->ms_ok_last_cmd=aaMsRunning();
   serverCmdBegins(server,"ackdenied");
   serverCmdFinish(server);
   if((ret=aaNetWebsocketWrite(&cd->wock,WEBSOCKET_OPCODE_TEXT,1,0,server->cmd))!=RET_YES) { oops;}
   ////mbLog("sending back ackdenied");
   return RET_YES;
   }
aaLeu;

//  mbLog("got %s",mb.json_text[2]);
  if(aaStringICompare(mb.json_text[2],"micLevel",0)==YES)
   {
   if(cd->type!=CALL_TYPE_PEER) { break; }
   cd->ms_ok_last_cmd=aaMsRunning();
   serverOnMicLevel(server);
   return RET_YES;
   }

  if(aaStringICompare(mb.json_text[2],"myIceOffer",0)==YES)
   {
   if(cd->type!=CALL_TYPE_PEER) { break; }
   cd->ms_ok_last_cmd=aaMsRunning();
   serverOnMyIceOffer(server);
   return RET_YES;
   }
aaLeu;
  if(aaStringICompare(mb.json_text[2],"chatMsg",0)==YES)
   {
   if(cd->type!=CALL_TYPE_PEER) { break; }
   aaLeu;
   cd->ms_ok_last_cmd=aaMsRunning();
   serverOnChatMsg(server);
   return RET_YES;
   }
aaLeu;
  if(aaStringICompare(mb.json_text[2],"chatMsgEcho",0)==YES)
   {
   if(cd->type!=CALL_TYPE_PEER) { break; }
   if(mb.json.status.mem_bytes>_50K)
    {
    aaLeu;
    mbLog("chatMsgEcho bytes=%i",mb.json.status.mem_bytes);
    jsonDump();
    }
    aaLeu;
   cd->ms_ok_last_cmd=aaMsRunning();
   serverOnChatMsgEcho(server);
   return RET_YES;
   }

aaLeu;
  if(aaStringICompare(mb.json_text[2],"chatMsgReplay",0)==YES)
   {
   if(cd->type!=CALL_TYPE_PEER) { break; }
   if(mb.json.status.mem_bytes>_50K)
    {
    aaLeu;
    mbLog("chatMsgReplay bytes=%i",mb.json.status.mem_bytes);
    jsonDump();
    }
    aaLeu;
   cd->ms_ok_last_cmd=aaMsRunning();
   serverOnChatMsgReplay(server);
   return RET_YES;
   }
aaLeu;
  if(aaStringICompare(mb.json_text[2],"myIceAnswer",0)==YES)
   {
   if(cd->type!=CALL_TYPE_PEER) { break; }
   cd->ms_ok_last_cmd=aaMsRunning();
   serverOnMyIceAnswer(server);
   return RET_YES;
   }


  if(aaStringICompare(mb.json_text[2],"myIceCandidate",0)==YES)
   {
   if(cd->type!=CALL_TYPE_PEER) { break; }
   cd->ms_ok_last_cmd=aaMsRunning();
   serverOnMyIceCandidate(server);
   return RET_YES;
   }

  if(aaStringICompare(mb.json_text[2],"videoBlob",0)==YES)
   {
   if(cd->type!=CALL_TYPE_PEER) { break; }
   cd->ms_ok_last_cmd=aaMsRunning();
   serverOnVideoBlob(server);
   return RET_YES;
   }


  if(aaStringICompare(mb.json_text[2],"reboot",0)==YES)
   {
   if(cd->type!=CALL_TYPE_DASHBOARD) { break; }
   cd->ms_ok_last_cmd=aaMsRunning();
   if(aa_is_morph_pending==0) {  aaRestart(); aaQuit();   }
   return RET_YES;
   }
aaLeu;
  if(aaStringICompare(mb.json_text[2],"dashReq",0)==YES)
   {
   if(cd->type!=CALL_TYPE_DASHBOARD) { break; }
   cd->ms_ok_last_cmd=aaMsRunning();
   str.len=0;
   str.buf[str.len]=NULL_CHAR;
   aaStringAppendfLen(&str.buf[0],&str.len,"{‘cmd‘:‘dashResponse‘,");
   //==================================================================
   aaStringNull(blob);
   aaStringAppendf(blob,"<style>");
   aaStringAppendf(blob,"html { background: #fff; } ");
   aaStringAppendf(blob,"table { font-weight: 500; font-size: 120%%; font-family: consolas;} ");
   aaStringAppendf(blob,"td { padding: 2px; } ");
   aaStringAppendf(blob,"h3 { font-size: 155%%; background: #ddd; color: #111; }");
   aaStringAppendf(blob,".kice td { border: 1px solid #00a; padding: 4px; font-size: 75%%; font-weight: 600;} ");
   aaStringAppendf(blob,".lice td { border: 1px solid #0a0; } ");
   aaStringAppendf(blob,".mice td { border: 1px solid #a00; } ");
   aaStringAppendf(blob,".nice td { border: 1px solid #088; } ");
   aaStringAppendf(blob,".oice td { border: 1px solid #a0a; padding: 4px; font-size: 90%%; } ");
   aaStringAppendf(blob,"</style>");
   aaStringAppendfLen(&str.buf[0],&str.len,"‘div0‘:‘%s‘,",blob);
   //==================================================================
   aaStringNull(blob);
   aaStringAppendf(blob,"<h3>Version %s.%s</h3>",MB_VERSION,DEV_VERSION);
   aaStringAppendf(blob,"<table class='kice'>");
   aaStringAppendf(blob,"<tr>");
   sr=aaSecsRunning();
   if(sr<60.0)     {     aaStringAppendf(blob,"<td>secs=%.2f</td>",sr);        }
   else
   if(sr<3600.0)   {     aaStringAppendf(blob,"<td>mins=%.2f</td>",sr/60.0);   }
   else            {     aaStringAppendf(blob,"<td>hrs=%.2f</td>",sr/3600.0);  }
   aaStringAppendf(blob,"<td>aa_stage=%i</td>",aa_stage);
   aaStringAppendf(blob,"<td>cpu=%.2f</td>",aa_curcpuload);
   aaStringAppendf(blob,"<td>pro=%.2f</td>",aa_curproload);
   aaStringAppendf(blob,"<td>cycle=%I64d</td>",aa_cycle);
   aaStringAppendf(blob,"<td>askhz=%.1f</td>",aa_askhz);
   aaStringAppendf(blob,"<td>curhz=%.1f</td>",aa_curhz);
   aaStringAppendf(blob,"</tr>");
   aaStringAppendf(blob,"</table>");
   aaStringAppendf(blob,"<br>");
   aaStringAppendfLen(&str.buf[0],&str.len,"‘div1‘:‘%s‘,",blob);
   //==================================================================
   aaStringNull(blob);
   aaNetStatus(&ns);
   aaStringAppendf(blob,"<table class='lice'>");
   aaStringAppendf(blob,"<tr>");
   aaStringAppendf(blob,"<td>cur_out_calls=%i</td>",ns.current_outgoing_tcp_call_count);
   aaStringAppendf(blob,"<td>cur_in_calls=%i</td>",ns.current_incoming_tcp_call_count);
   aaStringAppendf(blob,"<td>tot_out_calls=%i</td>",ns.total_outgoing_tcp_call_count);
   aaStringAppendf(blob,"<td>tot_in_calls=%i</td>",ns.total_incoming_tcp_call_count);
   aaStringAppendf(blob,"<td>iam=%i</td>",mb.who_i_am);
   aaStringAppendf(blob,"<td>iup=%i</td>",the_repo.i_upgrade);
   aaStringAppendf(blob,"</tr>");
   aaStringAppendf(blob,"</table>");
   aaStringAppendf(blob,"<br>");
   aaStringAppendfLen(&str.buf[0],&str.len,"‘div2‘:‘%s‘,",blob);
   //==================================================================
   aaStringNull(blob);
   aaNetStatus(&ns);
   aaStringAppendf(blob,"<table class='mice'>");
   aaStringAppendf(blob,"<tr>");
   aaStringCopyf(etc,"%s",mb.info.sys_info.process_filename);
   aaStringReplaceChar(etc,0,BSLASH_CHAR,FSLASH_CHAR);
   aaStringAppendf(blob,"<td>%s</td>",etc);
   aaStringAppendf(blob,"<td>cmdline=%s</td>",mb.info.sys_info.command_line);
   aaTimeToString(&mb.info.sys_info.process_filedate,txt,0,0);
   aaStringAppendf(blob,"<td>modified=%s</td>",txt);
   aaStringAppendf(blob,"<td>filebytes=%I64d</td>",mb.info.sys_info.process_filebytes);
   aaStringAppendf(blob,"</tr>");
   aaStringAppendf(blob,"</table>");
   aaStringAppendf(blob,"<br>");
   aaStringAppendfLen(&str.buf[0],&str.len,"‘div3‘:‘%s‘,",blob);
   //==================================================================
   aaStringNull(blob);
   aaStringAppendf(blob,"<table class='oice'>");
   c=0;
   for(i=0;i<server->max_calls;i++)
    {
    if(c>=server->cur_calls) { break;    }
    if(server->ch_array[i]==0) {  continue; }

    //if(server->guide[i].state==MB_GUIDE_STATE_UNUSED) { continue; }
    c++;
    //if(server->guide[i].type!=CALL_TYPE_PEER) { continue; }



    //tcall.handle=server->guide[i].call_handle;
    tcall.handle=server->ch_array[i];
    if(tcall.handle==0) { oof; continue; }
    if((ret=aaNetTcpCallStatus(tcall.handle,&tcall.status))!=YES) { oops; }
    tcd=(_servercalldata*)tcall.status.extra_data;
    if(tcd->type!=CALL_TYPE_PEER) { continue; }
    aaStringAppendf(blob,"<tr>");
    aaStringAppendf(blob,"<td>is_ws=%i</td>",tcd->is_ws);
    aaStringAppendf(blob,"<td>wockopen=%i</td>",tcd->wock.is_open);
    aaStringAppendf(blob,"<td>doclose=%i</td>",tcd->do_close);
    aaStringAppendf(blob,"<td>room=%s</td>",tcd->room_name);
    aaStringAppendf(blob,"<td>uuid=%s</td>",tcd->uuid);
    aaStringAppendf(blob,"<td>secs=%.2f</td>",tcall.status.ms/1000.0);
    aaStringAppendf(blob,"<td>icl=%i</td>",tcall.status.is_closed_by_local);
    aaStringAppendf(blob,"<td>icr=%i</td>",tcall.status.is_closed_by_remote);
    aaStringAppendf(blob,"<td>ip=%s</td>",tcd->wock.x_forwarded_for);
    aaStringAppendf(blob,"</tr>");
    }
   aaStringAppendf(blob,"</table>");
   aaStringAppendf(blob,"<br>");
   aaStringAppendfLen(&str.buf[0],&str.len,"‘div4‘:‘%s‘",blob);
   //==================================================================

   aaStringAppendfLen(&str.buf[0],&str.len,"}");
   //==================================================================
   aaStringReplaceChar(&str.buf[0],str.len,'‘',DQUOTE_CHAR);
   aaStringReplaceChar(&str.buf[0],str.len,'’',DQUOTE_CHAR);
   if((ret=aaNetWebsocketWrite(&cd->wock,WEBSOCKET_OPCODE_TEXT,1,str.len,str.buf))!=RET_YES) { oops;}
   return RET_YES;
   }
  break;
  }
aaLeu;
 mbLog("serverOnMessage (uuid=%s) unhandled",cd->uuid);
 jsonDump();
 return RET_YES;
 }


/*-----------------------------------------------------------------------*/




 B serverRoomGather                    (_server*server,_servergather*gather,VP room)
 {
 H i,c;
 _tcpcallunit tcall;
 _servercalldata*tcd;


 aaLeu;
 if(server==NULL)                    { return RET_MISSINGPARM; }
 if(server->magic!=aaHPP(serverNew)) { return RET_NOTINITIALIZED; }
 if(gather==NULL)                    { return RET_MISSINGPARM; }
 gather->count=0;
 if(room==NULL)                      { return RET_MISSINGPARM; }
 c=0;
 for(i=0;i<server->max_calls;i++)
  {
  if(c>=server->cur_calls) { break; }
  if(server->ch_array[i]==0) { continue; }
  c++;
  tcall.handle=server->ch_array[i];
  aaNetTcpCallStatus(tcall.handle,&tcall.status);
  tcd=(_servercalldata*)tcall.status.extra_data;
  if(tcd->stage<1000) { continue; }
  if(tcd->room_name[0]==NULL_CHAR) { continue; }
  if(aaStringICompare(tcd->room_name,room,0)!=YES) {  continue; }

  /*
  //if(server->guide[i].state!=MB_GUIDE_STATE_READY) { continue; }
  if(server->guide[i].type!=CALL_TYPE_PEER)        { continue; }
  c++;
  if(server->guide[i].room_name[0]==NULL_CHAR) { oof; continue; } // happened
  if(aaStringICompare(server->guide[i].room_name,room,0)!=YES) {  continue; }
  */
  gather->ch_array_index[gather->count]=i;

  //gather->guide_index[gather->count]=i;
  gather->count++;
  }
 return RET_YES;
 }



/*-----------------------------------------------------------------------*/



 B serverCmdBegins                     (_server*server,VP fmt,...)
 {
 aaLeu;
 aaVargsf128K(fmt);
 if(server==NULL)                    { return RET_MISSINGPARM; }
 if(server->magic!=aaHPP(serverNew)) { return RET_NOTINITIALIZED; }
 server->cmd[0]=NULL_CHAR;
 aaStringNull(server->cmd);
 aaStringAppendf(server->cmd,"{'cmd':'%s','gms':%I64d,",str128k.buf,global_msg);
 //aaStringAppendf(server->cmd,"{'cmd':'%s',",str128k.buf);
 global_msg++;
 return RET_YES;
 }



 B serverCmdFinish                     (_server*server)
 {
 aaLeu;
 if(server==NULL)                    { return RET_MISSINGPARM; }
 if(server->magic!=aaHPP(serverNew)) { return RET_NOTINITIALIZED; }
 serverCmdEolFix(server);
 aaStringAppendf(server->cmd,"}");
 aaStringReplaceChar(server->cmd,0,'\'',DQUOTE_CHAR);
 return RET_YES;
 }



 B serverCmdEolFix                     (_server*server)
 {
 aaLeu;
 if(server==NULL)                    { return RET_MISSINGPARM; }
 if(server->magic!=aaHPP(serverNew)) { return RET_NOTINITIALIZED; }
 aaStringLastCharDeleteIfChar(server->cmd,0,',');
 return RET_YES;
 }




 B serverCmdKeyVal                     (_server*server,VP key,VP fmt,...)
 {
 aaLeu;
 if(strlen(key)>_4K) { mbLog("cmdKeyval slen=%i",strlen(key)); }
 aaVargsf128K(fmt);
 if(str128k.len>_32K) { mbLog("vallen=%i",str128k.len); }
 if(server==NULL)                    { return RET_MISSINGPARM; }
 if(server->magic!=aaHPP(serverNew)) { return RET_NOTINITIALIZED; }
 aaStringAppendf(server->cmd,"'%s':'%s',",key,str128k.buf);
 return RET_YES;
 }



 B serverCmdKeyInt                     (_server*server,VP key,G val)
 {
 aaLeu;
 if(server==NULL)                    { return RET_MISSINGPARM; }
 if(server->magic!=aaHPP(serverNew)) { return RET_NOTINITIALIZED; }
 aaStringAppendf(server->cmd,"'%s':%I64d,",key,val);
 return RET_YES;
 }


 B serverCmdKeyDub                     (_server*server,VP key,D val)
 {
 aaLeu;
 if(server==NULL)                    { return RET_MISSINGPARM; }
 if(server->magic!=aaHPP(serverNew)) { return RET_NOTINITIALIZED; }
 aaStringAppendf(server->cmd,"'%s':%.3f,",key,val);
 return RET_YES;
 }





 B serverCmdAppend                     (_server*server,VP fmt,...)
 {
 aaLeu;
 aaVargsf128K(fmt);
 if(server==NULL)                    { return RET_MISSINGPARM; }
 if(server->magic!=aaHPP(serverNew)) { return RET_NOTINITIALIZED; }
 aaStringAppendf(server->cmd,"%s",str128k.buf);
 return RET_YES;
 }



 B serverCmdString                     (_server*server,VP fmt,...)
 {
 aaLeu;
 aaVargsf128K(fmt);
 if(server==NULL)                    { return RET_MISSINGPARM; }
 if(server->magic!=aaHPP(serverNew)) { return RET_NOTINITIALIZED; }
 aaStringAppendf(server->cmd,"'%s'",str128k.buf);
 return RET_YES;
 }



/*-----------------------------------------------------------------------*/

#if 0

 B serverSendHistory                   (_server*server)
 {
 B ret;
 H i;
 _servergather room_gather;
 _servercalldata*cd;
 _tcpcallunit tcall;
 _servercalldata*tcd;


 aaLeu;
 if(server==NULL)                    { return RET_MISSINGPARM; }
 if(server->magic!=aaHPP(serverNew)) { return RET_NOTINITIALIZED; }
 cd=(_servercalldata*)server->cd;



/// aaDebugf("we welcome %s",cd->hancock);
 if((ret=serverRoomGather(server,&room_gather,cd->room_name))!=YES) { oops; }
 if(room_gather.count==10)
  {
  serverCmdBegins(server,"Full");
  serverCmdKeyVal(server,"room","%s",cd->room_name);
  serverCmdKeyVal(server,"xfwd","%s",cd->wock.x_forwarded_for);
  serverCmdEolFix(server);
  serverCmdFinish(server);
  if((ret=aaNetWebsocketWrite(&cd->wock,WEBSOCKET_OPCODE_TEXT,1,0,server->cmd))!=RET_YES) { oops;}
  cd->do_close=1;
  return RET_YES;
  }

 serverCmdBegins(server,"welcome");
 serverCmdKeyVal(server,"room","%s",cd->room_name);
 serverCmdKeyVal(server,"uuid","%s",cd->uuid);
 serverCmdKeyVal(server,"hancock","%s",cd->hancock);
 serverCmdKeyVal(server,"xfwd","%s",cd->wock.x_forwarded_for);
 serverCmdKeyInt(server,"peerCount",(G)room_gather.count);
 serverCmdString(server,"peerList");
 serverCmdAppend(server,":");
 serverCmdAppend(server,"[");
 for(i=0;i<room_gather.count;i++)
  {
  tcall.handle=server->ch_array[room_gather.ch_array_index[i]];
  aaNetTcpCallStatus(tcall.handle,&tcall.status);
  tcd=(_servercalldata*)tcall.status.extra_data;
  serverCmdAppend(server,"{");
  //serverCmdKeyVal(server,"uuid",server->guide[room_gather.guide_index[i]].uuid);
  serverCmdKeyVal(server,"uuid",tcd->uuid);
  serverCmdKeyVal(server,"hancock",tcd->hancock);
  serverCmdEolFix(server);
  serverCmdAppend(server,"},");
  if(aaStringICompare(cd->uuid,tcd->uuid,0)==YES)
   {
   ////aaDebugf(" we notify them of THEMSELVES peer %s",tcd->hancock);
   }
  else
   {
   ////aaDebugf(" we notify them of existing peer %s",tcd->hancock);
   }
  }
 serverCmdEolFix(server);
 serverCmdAppend(server,"]");
 serverCmdFinish(server);
 //mbLog("%s sending welcome to ch=%x uuid=%s",cd->wock.x_forwarded_for,server->call.handle,cd->uuid);
 if((ret=aaNetWebsocketWrite(&cd->wock,WEBSOCKET_OPCODE_TEXT,1,0,server->cmd))!=RET_YES) { oops;}
 //aaDebugf("sending welcome room %s , uuid %s hancock %s",cd->room_name,cd->uuid,cd->hancock);

 return RET_YES;
 }


#endif



 B serverSendWelcome                   (_server*server)
 {
 B ret;
 H i;
 _servergather room_gather;
 _servercalldata*cd;
 _tcpcallunit tcall;
 _servercalldata*tcd;


 aaLeu;
 if(server==NULL)                    { return RET_MISSINGPARM; }
 if(server->magic!=aaHPP(serverNew)) { return RET_NOTINITIALIZED; }
 cd=(_servercalldata*)server->cd;

/// aaDebugf("we welcome %s",cd->hancock);
 if((ret=serverRoomGather(server,&room_gather,cd->room_name))!=YES) { oops; }
 if(room_gather.count==10)
  {
  serverCmdBegins(server,"Full");
  serverCmdKeyVal(server,"room","%s",cd->room_name);
  serverCmdKeyVal(server,"xfwd","%s",cd->wock.x_forwarded_for);
  serverCmdEolFix(server);
  serverCmdFinish(server);
  if((ret=aaNetWebsocketWrite(&cd->wock,WEBSOCKET_OPCODE_TEXT,1,0,server->cmd))!=RET_YES) { oops;}
  cd->do_close=1;
  return RET_YES;
  }

 serverCmdBegins(server,"welcome");
 serverCmdKeyVal(server,"room","%s",cd->room_name);
 serverCmdKeyVal(server,"uuid","%s",cd->uuid);
 serverCmdKeyVal(server,"hancock","%s",cd->hancock);
 serverCmdKeyVal(server,"xfwd","%s",cd->wock.x_forwarded_for);
 serverCmdKeyInt(server,"peerCount",(G)room_gather.count);
 serverCmdString(server,"peerList");
 serverCmdAppend(server,":");
 serverCmdAppend(server,"[");
 for(i=0;i<room_gather.count;i++)
  {
  tcall.handle=server->ch_array[room_gather.ch_array_index[i]];
  aaNetTcpCallStatus(tcall.handle,&tcall.status);
  tcd=(_servercalldata*)tcall.status.extra_data;
  serverCmdAppend(server,"{");
  //serverCmdKeyVal(server,"uuid",server->guide[room_gather.guide_index[i]].uuid);
  serverCmdKeyVal(server,"uuid",tcd->uuid);
  serverCmdKeyVal(server,"hancock",tcd->hancock);
  serverCmdEolFix(server);
  serverCmdAppend(server,"},");
  if(aaStringICompare(cd->uuid,tcd->uuid,0)==YES)
   {
   ////aaDebugf(" we notify them of THEMSELVES peer %s",tcd->hancock);
   }
  else
   {
   ////aaDebugf(" we notify them of existing peer %s",tcd->hancock);
   }
  }
 serverCmdEolFix(server);
 serverCmdAppend(server,"]");
 serverCmdFinish(server);
 //mbLog("%s sending welcome to ch=%x uuid=%s",cd->wock.x_forwarded_for,server->call.handle,cd->uuid);
 if((ret=aaNetWebsocketWrite(&cd->wock,WEBSOCKET_OPCODE_TEXT,1,0,server->cmd))!=RET_YES) { oops;}
 //aaDebugf("sending welcome room %s , uuid %s hancock %s",cd->room_name,cd->uuid,cd->hancock);

 return RET_YES;
 }




 B serverBroadcastUserJoined           (_server*server)
 {
 B ret;
 H j;
 _servergather room_gather;
 _servercalldata*cd;
 _tcpcallunit tcall;
 _servercalldata*tcd;

 aaLeu;
 if(server==NULL)                    { return RET_MISSINGPARM; }
 if(server->magic!=aaHPP(serverNew)) { return RET_NOTINITIALIZED; }
 cd=(_servercalldata*)server->cd;
 if((ret=serverRoomGather(server,&room_gather,cd->room_name))!=YES) { oops; }
 //mbLog("chcause=%x broadcasting user joined, room=%s gathercount=%i",server->call.handle,cd->room_name,room_gather.count);

/// aaDebugf(" %s has joined room %s",cd->hancock,cd->room_name);
 for(j=0;j<room_gather.count;j++)
  {
  tcall.handle=server->ch_array[room_gather.ch_array_index[j]];
  if(tcall.handle==0) { continue; }
  if(tcall.handle==server->call.handle) { continue; }
  aaNetTcpCallStatus(tcall.handle,&tcall.status);
  tcd=(_servercalldata*)tcall.status.extra_data;

  /*
  tcall.handle=server->guide[room_gather.guide_index[j]].call_handle;
  if(tcall.handle==0) { continue; }
  if(tcall.handle==server->call.handle) { continue; }
  if((ret=aaNetTcpCallStatus(tcall.handle,&tcall.status))!=YES) { oops; }
  tcd=(_servercalldata*)tcall.status.extra_data;
  */
  //mbLog("tcd ch=%x ",tcall.handle);
  if(tcd->is_ws!=YES)           { continue; }
  if(tcd->wock.is_open!=YES)    { continue; }
  if(tcd->do_close)             { continue; }
  if(tcd->type!=CALL_TYPE_PEER) { oof; }
  if(aaStringICompare(tcd->room_name,cd->room_name,0)!=YES) { oof; }
  serverCmdBegins(server,"userJoined");
  serverCmdKeyVal(server,"room","%s",cd->room_name);
  serverCmdKeyVal(server,"uuid","%s",cd->uuid);
  serverCmdKeyVal(server,"hancock","%s",cd->hancock);
  serverCmdKeyVal(server,"troom","%s",tcd->room_name);
  serverCmdKeyVal(server,"tuuid","%s",tcd->uuid);
  serverCmdKeyVal(server,"thancock","%s",tcd->hancock);

  ////aaDebugf("sending 'userJoined' %s  to %s",cd->hancock,tcd->hancock);
  serverCmdFinish(server);
 /// mbLog("sending userjoined to ch=%x, uuid=%s",tcall.handle,cd->uuid);
  if((ret=aaNetWebsocketWrite(&tcd->wock,WEBSOCKET_OPCODE_TEXT,1,0,server->cmd))!=RET_YES) { oops;}
  }
 return RET_YES;
 }





 B serverBroadcastUserLeft             (_server*server)
 {
 B ret;
 H j;
 _servergather room_gather;
 _servercalldata*cd;
 _tcpcallunit tcall;
 _servercalldata*tcd;

 aaLeu;
 if(server==NULL)                    { return RET_MISSINGPARM; }
 if(server->magic!=aaHPP(serverNew)) { return RET_NOTINITIALIZED; }
 cd=(_servercalldata*)server->cd;
 if((ret=serverRoomGather(server,&room_gather,cd->room_name))!=YES) { oops; }
 ////aaDebugf("%s has left room %s",cd->hancock,tcd->hancock);
 for(j=0;j<room_gather.count;j++)
  {
  tcall.handle=server->ch_array[room_gather.ch_array_index[j]];
  if(tcall.handle==0) { continue; }
  if(tcall.handle==server->call.handle) { continue; }
  aaNetTcpCallStatus(tcall.handle,&tcall.status);
  tcd=(_servercalldata*)tcall.status.extra_data;
/*
  tcall.handle=server->guide[room_gather.guide_index[j]].call_handle;
  if(tcall.handle==0) { continue; }
  if(tcall.handle==server->call.handle) { continue; }
  if((ret=aaNetTcpCallStatus(tcall.handle,&tcall.status))!=YES) { oops; }
  tcd=(_servercalldata*)tcall.status.extra_data;
  */
  if(tcd->is_ws!=YES)           { continue; }
  if(tcd->wock.is_open!=YES)    { continue; }
  if(tcd->do_close)             { continue; }
  if(tcd->type!=CALL_TYPE_PEER) { oof; }
  if(aaStringICompare(tcd->room_name,cd->room_name,0)!=YES) { oof; }
  serverCmdBegins(server,"userLeft");
  serverCmdKeyVal(server,"room","%s",cd->room_name);
  serverCmdKeyVal(server,"uuid","%s",cd->uuid);
  serverCmdKeyVal(server,"hancock","%s",cd->hancock);
  serverCmdKeyVal(server,"troom","%s",tcd->room_name);
  serverCmdKeyVal(server,"tuuid","%s",tcd->uuid);
  serverCmdKeyVal(server,"thancock","%s",tcd->hancock);

  ////aaDebugf("sending 'userleft' %s to %s",cd->hancock,tcd->hancock);
  serverCmdFinish(server);
  ///mbLog("sending userleft to ch=%x, uuid=%s",tcall.handle,cd->uuid);
  if((ret=aaNetWebsocketWrite(&tcd->wock,WEBSOCKET_OPCODE_TEXT,1,0,server->cmd))!=RET_YES) { oops;}
  }
 return RET_YES;
 }



 B serverOnMyIceOffer                  (_server*server)
 {
 B ret;
 _servercalldata*cd;
 _tcpcallunit tcall;
 _servercalldata*tcd;
 N ix,c,i;

 aaLeu;
 if(server==NULL)                    { return RET_MISSINGPARM; }
 if(server->magic!=aaHPP(serverNew)) { return RET_NOTINITIALIZED; }
 cd=(_servercalldata*)server->cd;

 c=0;
 ix=-1;
 for(i=0;i<server->max_calls;i++)
  {
  if(c>=server->cur_calls) { break; }
  if(server->ch_array[i]==0) { continue; }
  c++;
  tcall.handle=server->ch_array[i];//server->ch_array[room_gather.ch_array_index[i]];
  if(tcall.handle==0) { continue; }
  if(tcall.handle==server->call.handle) { continue; }
  aaNetTcpCallStatus(tcall.handle,&tcall.status);
  tcd=(_servercalldata*)tcall.status.extra_data;
  if(tcd->uuid[0]==NULL_CHAR) { continue; }
  if(aaStringICompare(tcd->uuid,mb.json_text[4],0)!=YES) { continue; }
  ix=i;
  break;
  }
 if(ix==-1)
  {
  //aaNote(0,"iceoffer uuid %s not found",mb.json_text[4]);
  ///aaDebugf("iceoffer uuid %s not found",mb.json_text[4]);
  return RET_NOTFOUND;
  }




 //ret=serverGuideFindByUuid(server,&ix,mb.json_text[4]);
 //if(ret!=YES) { mbLog("iceoffer uuid %s not found",mb.json_text[4]); return RET_NOTFOUND; }

 //tcall.handle=server->guide[ix].call_handle;

 //if((ret=aaNetTcpCallStatus(tcall.handle,&tcall.status))!=YES) { oops; }
 //tcd=(_servercalldata*)tcall.status.extra_data;

 serverCmdBegins(server,"iceOffer");
 serverCmdKeyVal(server,"room","%s",cd->room_name);
 serverCmdKeyVal(server,"uuid","%s",cd->uuid);
 serverCmdKeyVal(server,"hancock","%s",cd->hancock);
 serverCmdKeyVal(server,"troom","%s",tcd->room_name);
 serverCmdKeyVal(server,"tuuid","%s",tcd->uuid);
 serverCmdKeyVal(server,"thancock","%s",tcd->hancock);
 serverCmdString(server,"data");
 serverCmdAppend(server,":");
 serverCmdAppend(server,"{");
 serverCmdString(server,"type");
 serverCmdAppend(server,":");
 serverCmdString(server,"offer");
 serverCmdAppend(server,",");
 serverCmdString(server,"sdp");
 serverCmdAppend(server,":");
 serverCmdAppend(server,"\"%s\"",mb.json_text[10]);
 serverCmdAppend(server,"}");
 serverCmdFinish(server);

 ////aaDebugf("relaying iceoffer from %s to %s",cd->hancock,tcd->hancock);
 ///mbLog("relaying iceoffer from %x to %x , uuid %s to %s",server->call.handle,tcall.handle,cd->uuid,tcd->uuid);
 if((ret=aaNetWebsocketWrite(&tcd->wock,WEBSOCKET_OPCODE_TEXT,1,0,server->cmd))!=RET_YES) { oops;}
 return RET_YES;
 }





 B serverOnMyIceAnswer                 (_server*server)
 {
 B ret;
 _servercalldata*cd;
 _tcpcallunit tcall;
 _servercalldata*tcd;
 N ix,c,i;

 aaLeu;
 if(server==NULL)                    { return RET_MISSINGPARM; }
 if(server->magic!=aaHPP(serverNew)) { return RET_NOTINITIALIZED; }
 cd=(_servercalldata*)server->cd;

 c=0;
 ix=-1;
 for(i=0;i<server->max_calls;i++)
  {
  if(c>=server->cur_calls) { break; }
  if(server->ch_array[i]==0) { continue; }
  c++;
  tcall.handle=server->ch_array[i];//_index[room_gather.ch_array_index[i]];
  if(tcall.handle==0) { continue; }
  if(tcall.handle==server->call.handle) { continue; }
  aaNetTcpCallStatus(tcall.handle,&tcall.status);
  tcd=(_servercalldata*)tcall.status.extra_data;
  if(tcd->uuid[0]==NULL_CHAR) { continue; }
  if(aaStringICompare(tcd->uuid,mb.json_text[4],0)!=YES) { continue; }
  ix=i;
  break;
  }
 if(ix==-1)
  {
  //aaNote(0,"iceAnser uuid %s not found",mb.json_text[4]);
  ///aaDebugf("iceAnser uuid %s not found",mb.json_text[4]);
  return RET_NOTFOUND;
  }



 //ret=serverGuideFindByUuid(server,&ix,mb.json_text[4]);
 //if(ret!=YES) { mbLog("iceanswer uuid %s not found",mb.json_text[4]); return RET_NOTFOUND; }
 //tcall.handle=server->guide[ix].call_handle;
 //if((ret=aaNetTcpCallStatus(tcall.handle,&tcall.status))!=YES) { oops; }
 //tcd=(_servercalldata*)tcall.status.extra_data;
 serverCmdBegins(server,"iceAnswer");
 serverCmdKeyVal(server,"room","%s",cd->room_name);
 serverCmdKeyVal(server,"uuid","%s",cd->uuid);
 serverCmdKeyVal(server,"hancock","%s",cd->hancock);
 serverCmdKeyVal(server,"troom","%s",tcd->room_name);
 serverCmdKeyVal(server,"tuuid","%s",tcd->uuid);
 serverCmdKeyVal(server,"thancock","%s",tcd->hancock);
 serverCmdString(server,"data");
 serverCmdAppend(server,":");
 serverCmdAppend(server,"{");
 serverCmdString(server,"type");
 serverCmdAppend(server,":");
 serverCmdString(server,"answer");
 serverCmdAppend(server,",");
 serverCmdString(server,"sdp");
 serverCmdAppend(server,":");
 serverCmdAppend(server,"\"%s\"",mb.json_text[10]);
 serverCmdAppend(server,"}");
 serverCmdFinish(server);
 ///aaDebugf("sending iceAnswer msg from %s  to %s",cd->hancock,tcd->hancock);
 if((ret=aaNetWebsocketWrite(&tcd->wock,WEBSOCKET_OPCODE_TEXT,1,0,server->cmd))!=RET_YES) { oops;}
 return RET_YES;
 }








 B serverOnMyIceCandidate              (_server*server)
 {
 B ret;
 _servercalldata*cd;
 _tcpcallunit tcall;
 _servercalldata*tcd;
 N ix,i,c;

 aaLeu;
 if(server==NULL)                    { return RET_MISSINGPARM; }
 if(server->magic!=aaHPP(serverNew)) { return RET_NOTINITIALIZED; }
 cd=(_servercalldata*)server->cd;

 c=0;
 ix=-1;
 for(i=0;i<server->max_calls;i++)
  {
  if(c>=server->cur_calls) { break; }
  if(server->ch_array[i]==0) { continue; }
  c++;
  tcall.handle=server->ch_array[i];//_index[room_gather.ch_array_index[i]];
  if(tcall.handle==0) { continue; }
  if(tcall.handle==server->call.handle) { continue; }
  aaNetTcpCallStatus(tcall.handle,&tcall.status);
  tcd=(_servercalldata*)tcall.status.extra_data;
  if(tcd->uuid[0]==NULL_CHAR) { continue; }
  if(aaStringICompare(tcd->uuid,mb.json_text[4],0)!=YES) { continue; }
  ix=i;
  break;
  }
 if(ix==-1)
  {
  //aaNote(0,"icecandi uuid %s not found",mb.json_text[4]);
  ////aaDebugf("icecandi uuid %s not found",mb.json_text[4]);
  return RET_NOTFOUND;
  }

/*

 ret=serverGuideFindByUuid(server,&ix,mb.json_text[4]);
 if(ret!=YES) { mbLog("iceCandidate uuid %s not found",mb.json_text[4]); return RET_NOTFOUND; }
 tcall.handle=server->guide[ix].call_handle;
 if((ret=aaNetTcpCallStatus(tcall.handle,&tcall.status))!=YES) { oops; }
 tcd=(_servercalldata*)tcall.status.extra_data;
 */

 serverCmdBegins(server,"iceCandidate");
 serverCmdKeyVal(server,"room","%s",cd->room_name);
 serverCmdKeyVal(server,"uuid","%s",cd->uuid);
 serverCmdKeyVal(server,"hancock","%s",cd->hancock);
 serverCmdKeyVal(server,"troom","%s",tcd->room_name);
 serverCmdKeyVal(server,"tuuid","%s",tcd->uuid);
 serverCmdKeyVal(server,"thancock","%s",tcd->hancock);
 serverCmdString(server,"data");
 serverCmdAppend(server,":");
 serverCmdAppend(server,"{");
 serverCmdString(server,"type");
 serverCmdAppend(server,":");
 serverCmdString(server,"candidate");
 serverCmdAppend(server,",");
 serverCmdString(server,"label");
 serverCmdAppend(server,":");
 serverCmdAppend(server,"%s",mb.json_text[10]);
 serverCmdAppend(server,",");
 serverCmdString(server,"id");
 serverCmdAppend(server,":");
 serverCmdString(server,"%s",mb.json_text[12]);
 serverCmdAppend(server,",");
 serverCmdString(server,"candidate");
 serverCmdAppend(server,":");
 serverCmdAppend(server,"\"%s\"",mb.json_text[14]);
 serverCmdAppend(server,"}");
 serverCmdFinish(server);

 ////aaDebugf("sending iceCandidate msg from %s  to %s",cd->hancock,tcd->hancock);
 if((ret=aaNetWebsocketWrite(&tcd->wock,WEBSOCKET_OPCODE_TEXT,1,0,server->cmd))!=RET_YES) { oops;}
 return RET_YES;
 }






 B serverOnMicLevel                    (_server*server)
 {
 B ret;
 H j;//,val;
 D dub;
 _servergather room_gather;
 _servercalldata*cd;
 _tcpcallunit tcall;
 _servercalldata*tcd;
 return RET_YES;

 #if 0
 aaLeu;
 if(server==NULL)                    { return RET_MISSINGPARM; }
 if(server->magic!=aaHPP(serverNew)) { return RET_NOTINITIALIZED; }
 cd=(_servercalldata*)server->cd;
 //if((ret=aaStringToNumber(mb.json_text[4],0,&val,0,0,0))!=YES) { oops; }
 if((ret=aaStringToDouble(mb.json_text[4],0,&dub))!=YES) { oops; }
 //cd->mic_level=val;
 cd->mic_level=dub;
 if((ret=serverRoomGather(server,&room_gather,cd->room_name))!=YES) { oops; }
 serverCmdBegins(server,"micLevels");
 serverCmdKeyVal(server,"room","%s",cd->room_name);
 serverCmdKeyInt(server,"peerCount",(G)room_gather.count);
 serverCmdString(server,"peerList");
 serverCmdAppend(server,":");
 serverCmdAppend(server,"[");
 for(j=0;j<room_gather.count;j++)
  {
  tcall.handle=server->guide[room_gather.guide_index[j]].call_handle;
  if(tcall.handle==0) { continue; }
  if((ret=aaNetTcpCallStatus(tcall.handle,&tcall.status))!=YES) { oops; }
  tcd=(_servercalldata*)tcall.status.extra_data;
  if(tcd->is_ws!=YES)           { continue; }
  if(tcd->wock.is_open!=YES)    { continue; }
  if(tcd->do_close)             { continue; }
  if(tcd->type!=CALL_TYPE_PEER) { oof; }
  if(aaStringICompare(tcd->room_name,cd->room_name,0)!=YES) { oof; }
  serverCmdAppend(server,"{");
  serverCmdKeyVal(server,"uuid",tcd->uuid);
  //serverCmdKeyInt(server,"level",(G)tcd->mic_level);
  serverCmdKeyDub(server,"level",tcd->mic_level);
  serverCmdEolFix(server);
  serverCmdAppend(server,"},");
  }
 serverCmdEolFix(server);
 serverCmdAppend(server,"]");
 serverCmdFinish(server);
 //aaDebugf("%s",server->cmd);
 if((ret=aaNetWebsocketWrite(&cd->wock,WEBSOCKET_OPCODE_TEXT,1,0,server->cmd))!=RET_YES) { oops;}
 return RET_YES;
 #endif
 }


//http://www.geoplugin.net/xml.gp?ip=123.103.199.114










 B serverOnChatMsg                     (_server*server)
 {
 B ret;
 H j;//,val;
 //D dub;
 H c,i;
 _servergather room_gather;
 _servercalldata*cd;
 _tcpcallunit tcall;
 _servercalldata*tcd;


 if(server==NULL)                    { return RET_MISSINGPARM; }
 if(server->magic!=aaHPP(serverNew)) { return RET_NOTINITIALIZED; }
 cd=(_servercalldata*)server->cd;
 if((ret=serverRoomGather(server,&room_gather,cd->room_name))!=YES) { oops; }
 c=0;
 for(i=0;i<server->max_calls;i++)
  {
  if(c>=server->cur_calls) { break;    }
  if(server->ch_array[i]==0) {  continue; }
  c++;
  tcall.handle=server->ch_array[i];
  if(tcall.handle==0) { oof; continue; }
  if((ret=aaNetTcpCallStatus(tcall.handle,&tcall.status))!=YES) { oops; }
  tcd=(_servercalldata*)tcall.status.extra_data;
  if(tcd->type!=CALL_TYPE_PEER) { continue; }
  if(tcd->wock.is_open!=YES)    { continue; }
  if(tcd->do_close)             { continue; }
  if(tcd->type!=CALL_TYPE_PEER) { oof; }
  if(aaStringICompare(tcd->room_name,cd->room_name,0)!=YES) { continue; } //aaNote(0,"line=%i tcd=%s cd=%s",__LINE__,tcd->room_name,cd->room_name); }
  serverCmdBegins(server,"chat");
  serverCmdKeyVal(server,"room","%s",cd->room_name);
  serverCmdKeyVal(server,"uuid","%s",cd->uuid);
  serverCmdKeyVal(server,"hancock","%s",cd->hancock);
  serverCmdKeyVal(server,"troom","%s",tcd->room_name);
  serverCmdKeyVal(server,"tuuid","%s",tcd->uuid);
  serverCmdKeyVal(server,"thancock","%s",tcd->hancock);
  serverCmdKeyVal(server,"ali","%s",mb.json_text[4]);
  serverCmdKeyVal(server,"img","%s",mb.json_text[6]);
  serverCmdKeyVal(server,"txt","%s",mb.json_text[8]);
  serverCmdFinish(server);
  if((ret=aaNetWebsocketWrite(&tcd->wock,WEBSOCKET_OPCODE_TEXT,1,0,server->cmd))!=RET_YES) { oops;}

  }


 //aaDebugf("%s",server->cmd);
             //if((ret=aaNetWebsocketWrite(&cd->wock,WEBSOCKET_OPCODE_TEXT,1,0,server->cmd))!=RET_YES) { oops;}

 return RET_YES;





 return RET_YES;

 #if 0
 aaLeu;
 if(server==NULL)                    { return RET_MISSINGPARM; }
 if(server->magic!=aaHPP(serverNew)) { return RET_NOTINITIALIZED; }
 cd=(_servercalldata*)server->cd;
 //if((ret=aaStringToNumber(mb.json_text[4],0,&val,0,0,0))!=YES) { oops; }
 //if((ret=aaStringToDouble(mb.json_text[4],0,&dub))!=YES) { oops; }
 //cd->mic_level=val;
 //cd->mic_level=dub;
 if((ret=serverRoomGather(server,&room_gather,cd->room_name))!=YES) { oops; }
 for(j=0;j<room_gather.count;j++)
  {
  if(server->guide[room_gather.guide_index[j]].type!=CALL_TYPE_PEER) { continue; }
  tcall.handle=server->guide[room_gather.guide_index[j]].call_handle;
  if(tcall.handle==0) { continue; }
  if((ret=aaNetTcpCallStatus(tcall.handle,&tcall.status))!=YES) { oops; }
  tcd=(_servercalldata*)tcall.status.extra_data;
  if(tcd->is_ws!=YES)           { continue; }
  if(tcd->wock.is_open!=YES)    { continue; }
  if(tcd->do_close)             { continue; }
  if(tcd->type!=CALL_TYPE_PEER) { oof; }
  if(aaStringICompare(tcd->room_name,cd->room_name,0)!=YES) { oof; }
  serverCmdBegins(server,"chat");
  serverCmdKeyVal(server,"room","%s",cd->room_name);
  serverCmdKeyVal(server,"uuid","%s",cd->uuid);
  serverCmdKeyVal(server,"img","%s",mb.json_text[4]);
  serverCmdKeyVal(server,"txt","%s",mb.json_text[6]);
  serverCmdFinish(server);
  if((ret=aaNetWebsocketWrite(&tcd->wock,WEBSOCKET_OPCODE_TEXT,1,0,server->cmd))!=RET_YES) { oops;}
  }
 //aaDebugf("%s",server->cmd);
             //if((ret=aaNetWebsocketWrite(&cd->wock,WEBSOCKET_OPCODE_TEXT,1,0,server->cmd))!=RET_YES) { oops;}

 return RET_YES;
 #endif
 }







 ///msg={"cmd":"chatMsgEcho","ali":main.vars.my_info.alias,"img":data,"txt":txt};





 B serverOnChatMsgEcho                 (_server*server)
 {
 B ret;
 H j;//,val;
 //D dub;
 H c,i;
 _servergather room_gather;
 _servercalldata*cd;
 _tcpcallunit tcall;
 _servercalldata*tcd;
 Q utc;

aaLeu;
 if(server==NULL)                    { return RET_MISSINGPARM; }
 if(server->magic!=aaHPP(serverNew)) { return RET_NOTINITIALIZED; }
 cd=(_servercalldata*)server->cd;
 tcd=(_servercalldata*)tcall.status.extra_data;
aaLeu;

 aaJournal("the_history.txt","");
 aaJournal("the_history.txt","cmd=chatecho");
 if((ret=aaTimeUtcExGet(&utc))!=YES) { oops; }
 aaJournal("the_history.txt","utc=%I64u",utc);
 aaJournal("the_history.txt","room=%s",cd->room_name);
 aaJournal("the_history.txt","uuid=%s",cd->uuid);
 aaJournal("the_history.txt","hancock=%s",cd->hancock);
 aaJournal("the_history.txt","ali=%s",mb.json_text[4]);
 aaJournal("the_history.txt","img=%s",mb.json_text[6]);
 aaJournal("the_history.txt","txt=%s",mb.json_text[8]);
 mb.history_count++;
 //if(mb.history_count>30)  {  mb.history_start_index=mb.history_count-30;  }

/*
  serverCmdBegins(server,"chatecho");
  serverCmdKeyVal(server,"room","%s",cd->room_name);
  serverCmdKeyVal(server,"uuid","%s",cd->uuid);
  serverCmdKeyVal(server,"hancock","%s",cd->hancock);
  serverCmdKeyVal(server,"troom","%s",tcd->room_name);
  serverCmdKeyVal(server,"tuuid","%s",tcd->uuid);
  serverCmdKeyVal(server,"thancock","%s",tcd->hancock);
  serverCmdKeyVal(server,"ali","%s",mb.json_text[4]);
  serverCmdKeyVal(server,"img","%s",mb.json_text[6]);
  serverCmdKeyVal(server,"txt","%s",mb.json_text[8]);
  serverCmdFinish(server);
  aaLeu;
  */

 if((ret=serverRoomGather(server,&room_gather,cd->room_name))!=YES) { oops; }
 c=0;
 for(i=0;i<server->max_calls;i++)
  {
  if(c>=server->cur_calls) { break;    }
  if(server->ch_array[i]==0) {  continue; }
  c++;
  tcall.handle=server->ch_array[i];
  if(tcall.handle==0) { oof; continue; }
  if((ret=aaNetTcpCallStatus(tcall.handle,&tcall.status))!=YES) { oops; }
  tcd=(_servercalldata*)tcall.status.extra_data;
  if(tcd->type!=CALL_TYPE_PEER) { continue; }
  if(tcd->wock.is_open!=YES)    { continue; }
  if(tcd->do_close)             { continue; }
  if(tcd->type!=CALL_TYPE_PEER) { oof; }
  if(aaStringICompare(tcd->room_name,cd->room_name,0)!=YES) { continue; } //aaNote(0,"line=%i tcd=%s cd=%s",__LINE__,tcd->room_name,cd->room_name); }
  serverCmdBegins(server,"chatecho");
  serverCmdKeyVal(server,"room","%s",cd->room_name);
  serverCmdKeyVal(server,"uuid","%s",cd->uuid);
  serverCmdKeyVal(server,"hancock","%s",cd->hancock);
  serverCmdKeyVal(server,"troom","%s",tcd->room_name);
  serverCmdKeyVal(server,"tuuid","%s",tcd->uuid);
  serverCmdKeyVal(server,"thancock","%s",tcd->hancock);
  serverCmdKeyVal(server,"ali","%s",mb.json_text[4]);
  serverCmdKeyVal(server,"img","%s",mb.json_text[6]);
  serverCmdKeyVal(server,"txt","%s",mb.json_text[8]);
  serverCmdFinish(server);
  if((ret=aaNetWebsocketWrite(&tcd->wock,WEBSOCKET_OPCODE_TEXT,1,0,server->cmd))!=RET_YES) { oops;}
  }
 return RET_YES;
 }











 B serverOnChatMsgReplay               (_server*server)
 {
 B ret;
 H j;//,val;
 //D dub;
 H c,i;
 _servergather room_gather;
 _servercalldata*cd;
 _tcpcallunit tcall;
 _servercalldata*tcd;
 Q utc;
 Q msgindex;
 Q msgutc;
 B msgroom[_1K];
 B msguuid[_1K];
 B msghancock[_1K];
 B msgali[_1K];
 B msgimg[_128K];
 B msgtxt[_32K];

aaLeu;
 if(server==NULL)                    { return RET_MISSINGPARM; }
 if(server->magic!=aaHPP(serverNew)) { return RET_NOTINITIALIZED; }
 cd=(_servercalldata*)server->cd;
 tcd=(_servercalldata*)tcall.status.extra_data;

/*


 aaJournal("the_history.txt","");
 aaJournal("the_history.txt","cmd=chatecho");
 if((ret=aaTimeUtcExGet(&utc))!=YES) { oops; }
 aaJournal("the_history.txt","utc=%I64u",utc);
 aaJournal("the_history.txt","room=%s",cd->room_name);
 aaJournal("the_history.txt","uuid=%s",cd->uuid);
 aaJournal("the_history.txt","hancock=%s",cd->hancock);
 aaJournal("the_history.txt","ali=%s",mb.json_text[4]);
 aaJournal("the_history.txt","img=%s",mb.json_text[6]);
 aaJournal("the_history.txt","txt=%s",mb.json_text[8]);
*/

msgindex=1234;
 if((ret=aaTimeUtcExGet(&msgutc))!=YES) { oops; }
 aaStringCopy(msgroom,"/fuck");
 aaStringCopy(msguuid,"230329432");
 aaStringCopy(msghancock,"ashod apakian");
 aaStringCopy(msgali,"adolf");
 aaStringCopy(msgimg,"/9j/4AAQSkZJRgABAQAAAQABAAD/2wBDAP+3yOnIp//p2en/////////////////////////////////////////////////////////////////////////2wBDAf//////////////////////////////////////////////////////////////////////////////////////wAARCAHgAoADASIAAhEBAxEB/8QAFwABAQEBAAAAAAAAAAAAAAAAAAECA//EABkQAQEBAQEBAAAAAAAAAAAAAAABETEhQf/EABYBAQEBAAAAAAAAAAAAAAAAAAABAv/EABsRAQEBAAMBAQAAAAAAAAAAAAABETFRYSFB/9oADAMBAAIRAxEAPwDmAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAANJVwQBABVwQBAFxVwZF1emejItiIAAAsWrgyAgAsBBUXAAQBYq4Mi1EAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAFistNRGRajKrEqot6BYikBFqFABBrfGQXdAFiCiVY14jNFqJVFiLCAUFRAIyqxQaRKi1GbyoAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAsQiwUioqFQGVFRVgVFQoAIAueIuA1Ei0naJSGmnxVZaSrURYixIpQpD9CkVKvqH1WZ1oglRajN5UAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAABYVBdABAABdVkXRcXjIb4LagINRKgugAgsVkXQWIILUAFSguhOtMhLgtQEAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAH/2Q==");
 aaStringCopy(msgtxt,"ciao");


  serverCmdBegins(server,"chatreplay");
  serverCmdKeyVal(server,"room","%s",cd->room_name);
  serverCmdKeyVal(server,"uuid","%s",cd->uuid);
  serverCmdKeyVal(server,"hancock","%s",cd->hancock);
  serverCmdKeyInt(server,"msgIndex",msgindex);
  serverCmdKeyInt(server,"msgUtc",msgutc);
  serverCmdKeyVal(server,"msgRoom","%s",msgroom);
  serverCmdKeyVal(server,"msgUuid","%s",msguuid);
  serverCmdKeyVal(server,"msgHancock","%s",msghancock);
  serverCmdKeyVal(server,"msgAli","%s",msgali);
  serverCmdKeyVal(server,"msgImg","%s",msgimg);
  serverCmdKeyVal(server,"msgTxt","%s",msgtxt);
  serverCmdFinish(server);


  if((ret=aaNetWebsocketWrite(&cd->wock,WEBSOCKET_OPCODE_TEXT,1,0,server->cmd))!=RET_YES) { oops;}
 return RET_YES;
 }

//if((ret=aaStringToNumber(mb.json_text[4],0,&val,0,0,0))!=YES) { oops; }












 B biggy[_1MEG];



 B serverOnVideoBlob                   (_server*server)
 {
 B ret;
 _servercalldata*cd;
 H i,bytes,pos,x,m;
 B fn[_1K];
 B file[_1K];
 B mat[_1K];
 Q utc,fb;
 _str8k match;

 aaLeu;
 if(server==NULL)                    { return RET_MISSINGPARM; }
 if(server->magic!=aaHPP(serverNew)) { return RET_NOTINITIALIZED; }
 cd=(_servercalldata*)server->cd;
 //aaDebugf("video blob from %s room %s",cd->uuid,cd->room_name);
 //aaDebugf("sl=%i",mb.json.status.lines);
 //mb.json_text[i]

 m=aaElementCount(server->cd->fsu);
 for(x=0;x<m;x++)
  {
  if(server->cd->fsu[x].handle)
   {
   aaStringCopyfLen(match.buf,&match.len,"_%s.webm",mb.json_text[4]); // for
   if(aaStringFindFirstString(server->cd->fsu[x].status.filename,0,match.buf,match.len,&pos)!=YES) { continue; }
   aaStringCopyfLen(match.buf,&match.len,"/recording%s_",cd->room_name);
   if(aaStringFindFirstString(server->cd->fsu[x].status.filename,0,match.buf,match.len,&pos)!=YES) { continue; }
   aaStringCopyfLen(match.buf,&match.len,"_%s_",cd->uuid);
   if(aaStringFindFirstString(server->cd->fsu[x].status.filename,0,match.buf,match.len,&pos)!=YES) { continue; }
   break;
   }
  }
 if(x==m)
  {
  for(x=0;x<m;x++)
   {
   if(server->cd->fsu[x].handle==0) { break; }
   ////aaDebugf("server->cd->fsu[%i].handle=%i %s",x,server->cd->fsu[x].handle,server->cd->fsu[x].status.filename);
   }
  if(x==m) { oof; }
  }

 if(mb.json.status.lines==8)
  {
  i=6;
  if(i==6)
   {
   aaStringCopyfLen(match.buf,&match.len,";base64,");
   if(aaStringFindFirstString(mb.json_text[i],mb.json_line[i].len,match.buf,match.len,&pos)==YES)
    {
    if((ret=aaBase64Decode(&mb.json_text[i][pos+match.len],mb.json_line[i].len-(pos+match.len),biggy,&bytes))!=YES) { oops; }
    if(server->cd->fsu[x].handle==0)
     {
     aaTimeUniversalClockGet(&utc);
     aaTimeEpochGet(0,&utc);
     aaStringCopyf(fn,"%s_%I64d_%s_%s.webm",cd->room_name,utc,cd->uuid,mb.json_text[4]);
     if(fn[0]=='/') { aaStringDeleteChars(fn,0,0,1); }
     aaStringCopyf(file,"%srecording/%s",mb.info.sys_path.current_dir,fn);
     aaStringReplaceChar(file,0,BSLASH_CHAR,FSLASH_CHAR);
     ////aaDebugf("created %s, initial=%i",file,bytes);
     aaFileStreamCreateQuick(&server->cd->fsu[x].handle,file);
     aaFileStreamStatus(server->cd->fsu[x].handle,&server->cd->fsu[x].status);
     }
    //aaDebugf("chunk=%i",bytes);
    if((ret=aaFileStreamWrite(server->cd->fsu[x].handle,bytes,biggy))!=YES) { oops; }
    aaFileStreamStatus(server->cd->fsu[x].handle,&server->cd->fsu[x].status);

    serverCmdBegins(server,"videoBlobStatus");
    serverCmdKeyVal(server,"room","%s",cd->room_name);
    serverCmdKeyVal(server,"uuid","%s",cd->uuid);
    serverCmdKeyVal(server,"for","%s",mb.json_text[4]);
    serverCmdKeyInt(server,"bytes",(G)server->cd->fsu[x].status.bytes);
    serverCmdFinish(server);
    if((ret=aaNetWebsocketWrite(&cd->wock,WEBSOCKET_OPCODE_TEXT,1,0,server->cmd))!=RET_YES) { oops;}


    }
   }
  }
 else
 if(mb.json.status.lines==6)
  {
  aaFileStreamStatus(server->cd->fsu[x].handle,&server->cd->fsu[x].status);
  fb=server->cd->fsu[x].status.bytes;
  aaStringCopy(fn,server->cd->fsu[x].status.filename);
  ///aaDebugf("%s closed with %I64d bytes",fn,fb);
  aaFileStreamDestroy(server->cd->fsu[x].handle);
  server->cd->fsu[x].handle=0;
  if(fb<_64K)
   {
   ret=aaFileDelete("%s",fn);
   ////aaDebugf("file delete %s because too small %I64d bytes, ret=%s",fn,fb,arets);
   }
  }
 return RET_YES;
 }





















 B serverOnElog                        (_server*server)
 {
 B ret;
 H j;//,val;
 //D dub;
 _servercalldata*cd;


 #if 1
 aaLeu;
 if(server==NULL)                    { return RET_MISSINGPARM; }
 if(server->magic!=aaHPP(serverNew)) { return RET_NOTINITIALIZED; }
 cd=(_servercalldata*)server->cd;
 //aaNote(0,"beg %i ",mb.json.status.lines);
 //for(j=0;j<mb.json.status.lines;j++) aaDebugf("ELOG : %s",mb.json_text[j]);
 if(mb.json.status.lines>=5) aaDebugf("%s %s",server->cd->wock.x_forwarded_for,mb.json_text[4]);

 return RET_YES;
 #endif
 }



