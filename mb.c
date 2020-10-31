/*-----------------------------------------------------------------------*/
 #include "mb.h"
/*-----------------------------------------------------------------------*/
 _mb     mb={.magic=0};
 _repo   the_repo={.magic=0};
 _server the_server={.magic=0};
/*-----------------------------------------------------------------------*/

/*
https://github.com/pion/turn/tags
C:\pionturn>turn-server-simple.exe -port 19302 -public-ip 35.189.34.181 -users ashod=fuck
*/


 B mbStart                             (V)
 {
 B ret;
 B buf[_1K];
 _size s1;
 _rect r1;

 aaMemoryFill(&mb,sizeof(_mb),0);
 mb.magic=aaHPP(mbStart);
 if((ret=aaTimeLocalGet(&mb.launch_time))!=YES) { oops; }
 //if((ret=aaTimeToString(&mb.launch_time,mb.launch_text,0,0))!=YES) { oops; }
 if((ret=aaTimeToString(&mb.launch_time,mb.launch_text,0,0))!=YES) { oops; }

 if((ret=aaInfoGet(&mb.info,F32))!=YES) { oops; }
 if((ret=aaManufacturerGet(mb.manu))!=YES) { oops; }
// aaNote(0,"%s",mb.manu);

 aaStringFindFirstIStrings(mb.manu,0,0,0,&mb.who_i_am,"$$sentinel$$","google compute","amazon","PRIME B250M-A Rev X.0x","thinkpad","gigabyte p15f","openstack","red hat kvm",0);
 if(1&&aaProcessIsRunning("dbgview.exe",0)!=YES&&(mb.who_i_am!=I_AM_DESKTOP&&mb.who_i_am!=I_AM_GIGA))
  {
  aaStringCopyf(buf,"%s\\dbgview.exe",mb.info.sys_path.desktop);
  if((ret=aaProcessLaunch(buf,NULL,NULL,SW_SHOW,0,0))!=YES) { oops; }
  aaSleep(100);
  }
 aaFocusToDbg(0);
 aaDebugf(0);
 aaDebugfLogWriteSet(NO);
 aaDebugfLogStyleSet(2);
 mb.tray_icon_index=0;
 mb.tray_icon_ms=aaMsRunning();
 aaSizeSet(&s1,80,80);
 if((ret=aaSurfaceCreate(&mb.surf.handle,&s1))!=YES) { oops; }
 if((ret=aaSurfaceVisualize(mb.surf.handle,YES,0))!=YES) { oops; }
 if((ret=aaSurfaceIconSetUsingResource(mb.surf.handle,1000,F32))!=YES) { oops; }
 aaSurfaceTitleSet(mb.surf.handle,"Mebeam");
 aaSurfaceStatus(mb.surf.handle,&mb.surf.status);
 if((ret=aaSurfaceTraySet(mb.surf.handle,1001+(mb.tray_icon_index%2),"%s %s.%s",mb.surf.status.title,MB_VERSION,DEV_VERSION))!=YES) { oops; }
 aaSurfaceStatus(mb.surf.handle,&mb.surf.status);
 mb.canvas_update_ms=aaMsRunning();
 aaSizeSet(&s1,520,280);
 aaRectSet(&r1,mb.info.display_info.tray_rect.x-s1.w-4,mb.info.display_info.tray_rect.y-s1.h-4,s1.w,s1.h);
 if((ret=aaSurfaceCreate(&mb.canvas.handle,&s1))!=YES) { oops; }
 if((ret=aaSurfaceVisualize(mb.canvas.handle,YES-1,0))!=YES) { oops; }
 if((ret=aaSurfaceIconSetUsingResource(mb.canvas.handle,1000,F32))!=YES) { oops; }
 aaSurfaceTitleSet(mb.canvas.handle,"%s %s.%s",mb.surf.status.title,MB_VERSION,DEV_VERSION);
 aaSurfaceRectSet(mb.canvas.handle,&r1);
 aaSurfaceFillFrame(mb.canvas.handle,0,2,&col_gray[2],&col_gray[24]);
 aaSurfaceShow(mb.canvas.handle,YES);
 aaSurfaceStatus(mb.canvas.handle,&mb.canvas.status);
 if((ret=aaFontCreate(&mb.font[0].handle,"consolas",0,-11,180,0,0,6,0))!=YES) { oops; }
 aaFontStatus(mb.font[0].handle,&mb.font[0].status);
 aaJsonCreate(&mb.json.handle,NO);
 aaCustomLogSet(mbLog);
 mbLog("started MeBeam %s @ %s (who_i_am=%i) dev=%s pid=%u",MB_VERSION,mb.manu,mb.who_i_am,DEV_VERSION,mb.info.sys_info.process_id);
 repoInit(&the_repo,"c:/www/mebeam/program.exe");
 return RET_YES;
 }







 V mbStop                              (V)
 {
 if(mb.magic!=aaHPP(mbStart)) { return; }
 if(mb.surf.handle)            {  aaSurfaceDestroy(mb.surf.handle);  }
 if(mb.canvas.handle)          {  aaSurfaceDestroy(mb.canvas.handle);  }
 if(mb.json.handle)            {  aaJsonDestroy(mb.json.handle); }
 if(mb.font[0].handle)         {  aaFontDestroy(mb.font[0].handle); mb.font[0].handle=0; }
 if(mb.who_i_am==I_AM_DESKTOP) {  aaFocusToCodeBlocks(); }
 mbLog(" ");
 mbLog(" ");
 mbLog("stopped MeBeam %s.%s",MB_VERSION,DEV_VERSION);
 aaMemoryFill(&mb,sizeof(_mb),0);
 }









 B mbYield                             (V)
 {
 B ret;
 B ir;
 Q el;
 _rect r1;
 _size s1;
 _netstatus ns;
 B tok[_4K];
 B etc[_4K];
 B txt[_1K];
 Q msr;
 _inputstate is;

 if(mb.magic!=aaHPP(mbStart)) { return RET_NOTINITIALIZED; }
 if((ret=aaYield(500))!=YES) { return ret; }
 aaSurfaceStatus(mb.canvas.handle,&mb.canvas.status);
 aaSurfaceStatus(mb.surf.handle,&mb.surf.status);
 if(mb.surf.status.is_systray)
  {
  el=aa_msrunning-mb.canvas_update_ms;
  if(el>=679)
   {
   aaRectSet(&r1,0,0,mb.canvas.status.size.w,mb.canvas.status.size.h);
   aaRectExpand(&r1,-4,-4);
   aaNetStatus(&ns);
   aaStringCopyf(tok,"%s.%s ",MB_VERSION,DEV_VERSION);
   aaStringNull(etc);
   aaStringAppendf(etc,"%s launched %s\n\n",mb.canvas.status.title,mb.launch_text);
   if(mb.is_uploading_upgrade)    {    aaStringAppendf(etc,"UPLOADING UPGRADE %.2f%%\n",mb.upload_complete);    }
   msr=aa_msrunning;
   if(msr<60000)     {     aaStringCopyf(txt,"secs=%.4f",msr/1000.0);    }
   else
   if(msr<3600000)   {     aaStringCopyf(txt,"mins=%.4f",msr/60000.0);   }
   else              {     aaStringCopyf(txt,"hrs=%.4f",msr/3600000.0);  }
   aaStringAppendf(etc,"%-15s ",txt);
   aaStringAppendf(etc,"cycle=%-10I64d ",aa_cycle);
   aaStringAppendf(etc,"ver=%-9s ",tok);
   aaStringAppendf(etc,"pid=%u\n",mb.info.sys_info.process_id);
   aaStringAppendf(etc,"\n");
   aaStringAppendf(etc,"TOT: ");
   aaStringAppendf(etc,"out_tcp_cc=%-4i ",ns.total_outgoing_tcp_call_count);
   aaStringAppendf(etc,"in_tcp_cc=%-5i ",ns.total_incoming_tcp_call_count);
   aaStringAppendf(etc,"tcp_cc=%-6i\n",ns.total_tcp_call_count);
   aaStringAppendf(etc,"CUR: ");
   aaStringAppendf(etc,"out_tcp_cc=%-4i ",ns.current_outgoing_tcp_call_count);
   aaStringAppendf(etc,"in_tcp_cc=%-5i ",ns.current_incoming_tcp_call_count);
   aaStringAppendf(etc,"tcp_cc=%-6i\n",ns.current_tcp_calls_connected);
   aaStringAppendf(etc,"\n");
   aaStringAppendf(etc,"CUR: ");
   aaStringAppendf(etc,"cpu_load=%-6.2f ",aa_curcpuload);
   aaStringAppendf(etc,"pro_load=%-6.2f\n",aa_curproload);
   aaStringAppendf(etc,"AVG: ");
   aaStringAppendf(etc,"cpu_load=%-6.2f ",aa_avgcpuload);
   aaStringAppendf(etc,"pro_load=%-6.2f\n",aa_avgproload);
   aaStringAppendf(etc,"\n");
   aaStringAppendf(etc,"YLD: ");
   aaStringAppendf(etc,"ask_herz=%-6.1f ",aa_askhz);
   aaStringAppendf(etc,"cur_herz=%-6.1f ",aa_curhz);
   aaStringAppendf(etc,"eve_wai=%i\n",aa_ie_events_waiting);
   aaStringAppendf(etc,"\n");
   aaStringAppendf(etc,"history=%I64d/%I64d/%I64d\n",mb.history_count,mb.history_start_index,mb.history_start_offset);
   aaStringAppendf(etc,"%s\n",mb.billboard);
   if(mb.is_uploading_upgrade)    {    aaSurfaceLabel(mb.canvas.handle,&r1,&col_pastelblue[24],mb.font[0].handle,&col_gray[2],4,4,0,0,0,"%s",etc);    }
   else    {    aaSurfaceLabel(mb.canvas.handle,&r1,&col_gray[29],mb.font[0].handle,&col_gray[2],4,4,0,0,0,"%s",etc);    }
   if(mb.canvas.status.is_shown)     {    aaSurfaceUpdateAreaAdd(mb.canvas.handle,0,YES);    }
   else                              {    aaSurfaceUpdateAreaAdd(mb.canvas.handle,0,NO);     }
   aaSurfaceStatus(mb.canvas.handle,&mb.canvas.status);
   mb.canvas_update_ms=aa_msrunning;
   }
  el=aa_msrunning-mb.tray_icon_ms;
  if(el>=700)
   {
   if(aa_display_change_counter)
    {
    if((ret=aaInfoGet(&mb.info,F32))!=YES) { oops; }
    aaSizeSet(&s1,mb.canvas.status.size.w,mb.canvas.status.size.h);
    aaRectSet(&r1,mb.info.display_info.tray_rect.x-s1.w-4,mb.info.display_info.tray_rect.y-s1.h-4,s1.w,s1.h);
    aaSurfaceRectSet(mb.canvas.handle,&r1);
    aaSurfaceStatus(mb.canvas.handle,&mb.canvas.status);
    aa_display_change_counter=0;
    mbLog("display size = %ix%i",mb.info.display_info.screen_size.w,mb.info.display_info.screen_size.h);
    }
   mb.tray_icon_index++;
   if((ret=aaSurfaceTraySet(mb.surf.handle,1001+(mb.tray_icon_index%2),"%s %s.%s",mb.surf.status.title,MB_VERSION,DEV_VERSION))!=YES) { oops; }
   mb.tray_icon_ms=aa_msrunning;
   }
  if(aaSurfaceIsTrayClicked(mb.surf.handle,0,&ir)==RET_YES)
   {
   if(ir)
    {
    if(mb.do_quit==0) { mb.do_quit=1; }
    }
   else
    {
    if(mb.canvas.status.is_shown)    {     aaSurfaceShow(mb.canvas.handle,NO);     }
    else                             {     aaSurfaceShow(mb.canvas.handle,YES);    }
    aaSurfaceStatus(mb.canvas.handle,&mb.canvas.status);
    }
   if((ret=aaSurfaceTrayClickClear(mb.surf.handle))!=YES) { oops; }
   aaSurfaceStatus(mb.surf.handle,&mb.surf.status);
   }
  }
 if(AUTO_MINIMIZE!=0&&aa_cycle==AUTO_MINIMIZE&&mb.canvas.status.is_shown)
  {
  aaSurfaceShow(mb.canvas.handle,NO);
  aaSurfaceStatus(mb.canvas.handle,&mb.canvas.status);
  }

 if(aa_ie_events_waiting)   {  aaInputStateGet(&is);  }


 return RET_YES;
 }






 B mbLog                               (VP fmt,...)
 {
 aaVargsf32K(fmt);
 if(mb.magic!=aaHPP(mbStart)) { return RET_NOTINITIALIZED; }
 #if 0
 mb.log_ms=aaMsRunning();
 aaDebugf("%-5I64d %s",mb.log_ms,str32k.buf);
 aaLog(-555,"%-5I64d %s",mb.log_ms,str32k.buf);
 #else
 aaDebugf("%s",str32k.buf);
 aaLog(-555,"%s",str32k.buf);
 #endif
 mb.log_counter++;
 return RET_YES;
 }





/*-----------------------------------------------------------------------*/





 V aaMain                              (V)
 {
 B ret;

 mbStart();
 #if 0
 if((ret=serverNew(&the_server,0,8080,1,0,serverProc))!=YES) { oops; }
 while(mbYield()==YES)
  {
  if(mb.do_quit==1) { break; }
  serverYield(&the_server,25);
  }
 serverDelete(&the_server);
 mbStop();
 return;
 #endif



 if(mb.who_i_am==I_AM_GOOGLE_CLOUD||mb.who_i_am==I_AM_GODADDY||mb.who_i_am==I_AM_DIGITALPACIFIC)
  {
  if(the_repo.i_upgrade==YES)  {  repoProjectUpgrade(&the_repo);  repoLaunchProject(&the_repo);  return;  }
  repoDeleteUpgrade(&the_repo);
  if((ret=serverNew(&the_server,0,8080,1,0,serverProc))!=YES) { oops; }
  while(mbYield()==YES)
   {
   if(mb.do_quit==1) { break; }
   serverYield(&the_server,25);
   if(repoUpgradeEvent(&the_repo)==YES) {  repoLaunchUpgrade(&the_repo); break; }
   switch(aa_stage)    {    case 0:    aaStageSet(10);    break;    }
   }
  serverDelete(&the_server);
  }
 else
 if(mb.who_i_am==I_AM_DESKTOP||mb.who_i_am==I_AM_GIGA)
  {
  if(1) { pushAndCommit(); }
  else
  if(1) { bibleMaker(); }
  else
   {
   while(mbYield()==YES)
    {
    if(1) { bibleMaker(); break; }
    if(mb.do_quit==1) { break; }
    switch(aa_stage)    {    case 0:    aaStageSet(10);    break;    }
    }
   }
  }
 mbStop();
 }

