/*-----------------------------------------------------------------------*/
 #include "mb.h"
/*-----------------------------------------------------------------------*/



/*-----------------------------------------------------------------------*/

 // note: if both program.exe and program-upgrade.exe exist and neither running, ALWAYS running program-upgrade.exe first


 B repoInit                            (_repo*repo,VP fmt,...)
 {
 aaVargsf4K(fmt);
 if(repo==NULL) { return RET_MISSINGPARM; }
 aaMemoryFill(repo,sizeof(_repo),0);
 repo->magic=aaHPP(repoInit);
 aaStringCopyf(repo->exe,"%s",str4k.buf);
 aaStringCopyf(repo->upgrade_exe,"%s-upgrade.exe",repo->exe);
 repo->tik=aaMsRunning();
 if(aaStringFindFirstIString(mb.info.sys_info.process_file,0,"-upgrade.exe",0,0)==YES)  {  repo->i_upgrade=YES; }
 return RET_YES;
 }




 B repoDeleteUpgrade                   (_repo*repo)
 {
 if(repo==NULL) { return RET_MISSINGPARM; }
 if(repo->magic!=aaHPP(repoInit)) { return RET_NOTINITIALIZED; }
 return(aaFileDelete(repo->upgrade_exe));
 }





 B repoProjectUpgrade                  (_repo*repo)
 {
 if(repo==NULL) { return RET_MISSINGPARM; }
 if(repo->magic!=aaHPP(repoInit)) { return RET_NOTINITIALIZED; }
 return(aaFileCopy(repo->upgrade_exe,repo->exe,YES));
 }





 B repoLaunchUpgrade                   (_repo*repo)
 {
 B ret;

 if(repo==NULL) { return RET_MISSINGPARM; }
 if(repo->magic!=aaHPP(repoInit)) { return RET_NOTINITIALIZED; }
 ret=aaMorph(repo->upgrade_exe,mb.info.sys_info.command_line);
 aaQuit();
 return ret;
 }





 B repoLaunchProject                   (_repo*repo)
 {
 B ret;

 if(repo==NULL) { return RET_MISSINGPARM; }
 if(repo->magic!=aaHPP(repoInit)) { return RET_NOTINITIALIZED; }
 ret=aaMorph(repo->exe,mb.info.sys_info.command_line);
 aaQuit();
 return ret;
 }






 B repoUpgradeEvent                    (_repo*repo)
 {
 Q el;
 _filedetails fd;

 if(repo==NULL) { return RET_MISSINGPARM; }
 if(repo->magic!=aaHPP(repoInit)) { return RET_NOTINITIALIZED; }
 el=aa_msrunning-repo->tik;
 if(el<400) { return RET_NO; }
 if(aaFileDetailsGet(repo->upgrade_exe,&fd,0)==RET_YES)  {   return RET_YES; }
 repo->tik=aa_msrunning;
 return RET_NO;
 }





/*-----------------------------------------------------------------------*/



 V pushAndCommit                       (V)
 {
 B ret;
 _ftpclient ftpclient={.magic=0};
 _filestreamunit fsu;
 Q todo,ms;
 B buf[_8K];
 B srce_file[257];
 B dest_file[257];

 while(mbYield()==YES)
  {
  if(mb.do_quit==1) { break; }
  if(ftpclient.magic!=0)
   {
   aaNetFtpClientYield(&ftpclient,2);
   if(aa_stage>0&&(aa_msrunning-ms)>=500)
    {
    //mbLog("ftp stg=%i prpt=%i er=%i sc=%i cde=%i",ftpclient.stage,ftpclient.is_prompt,ftpclient.is_error,ftpclient.ctrl.res.is_success,ftpclient.ctrl.res.code);
    ms=aa_msrunning;
    }
   }
  switch(aa_stage)
   {
   case 0:
   if(1)
    {
    aaStringCopyf(srce_file,"%s",mb.info.sys_info.process_filename);
    aaStringCopyf(dest_file,"%s-upgrade.exe",mb.info.sys_info.process_file);
    }
   else
    {
    aaStringCopyf(srce_file,"bluefire.mp4");
    aaStringCopyf(dest_file,"bluefire.mp4");
    }
   aaStageSet(100);
   break;

   case 100:
   aaNetFtpClientNew(&ftpclient,"mebeam.com",21,"apakian","Qqqq1003!");
   aaSurfaceFillFrame(mb.canvas.handle,0,2,&col_gray[2],&col_pastelblue[24]);
   mb.is_uploading_upgrade=YES;
   aaStageSet(110);
   break;

   case 110:
   if(ftpclient.is_prompt!=YES) { break; }
   aaNetFtpClientDirChange(&ftpclient,"/www/mebeam");
   aaStageSet(120);
   break;

   case 120:
   if(ftpclient.is_prompt!=YES) { break; }
   if(ftpclient.ctrl.res.is_success!=YES) { break; }
   if(ftpclient.ctrl.res.code!=FTP_CWDOK) { break; }
   aaNetFtpClientFileUpload(&ftpclient,0,dest_file);//"program-upgrade.exe");
   aaStageSet(150);
   break;

   case 150:
   if(ftpclient.is_uploading!=YES) { break; }
   ret=aaFileStreamCreate(&fsu.handle,srce_file,aa_FILECREATE_OPEN,aa_FILEMODE_R,aa_FILESHARE_R,0,NO);
   aaFileStreamStatus(fsu.handle,&fsu.status);
   mbLog("sending upgrade, open %s = %s (%I64d bytes)",fsu.status.filename,arets,fsu.status.bytes);
   aaStageSet(160);
   break;


   case 160:
   mb.upload_complete=aaNumPercentIs(ftpclient.auxx.call.status.xmit_bytes_total,fsu.status.bytes);
   if(ftpclient.auxx.call.status.xmit_bytes>=_64K) { break; }
   aaFileStreamStatus(fsu.handle,&fsu.status);
   todo=fsu.status.bytes-fsu.status.offset;
   if(todo==0) { aaStageSet(180); break; }
   todo=aaNumRoof(todo,_4K);
   aaFileStreamRead(fsu.handle,(H)todo,buf);
   aaNetFtpClientFileWrite(&ftpclient,(H)todo,buf);
   break;

   case 180:
   if(ftpclient.auxx.call.status.xmit_bytes!=0) { break; }
   aaNetFtpClientFileFinish(&ftpclient);
   aaFileStreamDestroy(fsu.handle);
   fsu.handle=0;
   aaStageSet(190);
   break;


   case 190:
   if(ftpclient.is_prompt!=YES) { break; }
   if(ftpclient.ctrl.res.is_success!=YES) { break; }
   if(ftpclient.ctrl.res.code!=FTP_TRANSFEROK) { break; }
   mbLog("upgrade transfer complete");
   aaStageSet(200);
   break;


   case 200:
   aaNetFtpClientDelete(&ftpclient);
   aaStageSet(220);
   break;

   case 220:
   return;
   }
  }
 }





