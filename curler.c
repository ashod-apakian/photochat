/*-----------------------------------------------------------------------*/
 #include "mb.h"
/*-----------------------------------------------------------------------*/
 _curly kerly={.magic=0};
/*-----------------------------------------------------------------------*/

/*
search directories: linker
C:\libs\curl-7.65.3-win32-mingw\lib
search directories: compiler
C:\libs\curl-7.65.3-win32-mingw\include\curl
linker settings:
C:\kode\05_baba\libcurl.a
C:\kode\05_baba\libcurl.dll.a
*/




 size_t curlyProcRead                  (VP dest, size_t size, size_t nmemb,VP userp)
 {
 B ret;
 _curlycall*ccall;
 Q cando;
 Q todo;

 ccall=(_curlycall*)userp;
 //aaCustomLog("this ci = %i",ccall->this_ci);
 if(ccall->fsu.handle==0)
  {
  ret=aaFileStreamOpenQuick(&ccall->fsu.handle,"fuck.wav");//OSR_us_000_0011_8k.wav");
  if(ret!=YES) { oops; }
  ///aaFileStreamStatus(ccall->fsu.handle,&ccall->fsu.status);
  ///aaCustomLog("sending %I64d bytes",ccall->fsu.status.bytes);
  }
 aaFileStreamStatus(ccall->fsu.handle,&ccall->fsu.status);
 todo=ccall->fsu.status.left;
 cando=(Q)size*(Q)nmemb;
 if(todo>cando) { todo=cando; }
 if(todo)
  {
  ret=aaFileStreamRead(ccall->fsu.handle,(H)todo,dest);
  if(ret!=YES) { oops; }
  aaFileStreamStatus(ccall->fsu.handle,&ccall->fsu.status);
  ////aaCustomLog("readed %I64d, left=%I64d",todo,ccall->fsu.status.left);
  return (size_t)todo;
  }
 aaFileStreamDestroy(ccall->fsu.handle);
 ccall->fsu.handle=0;
//// aaCustomLog("closed file");
 return 0;






 #if 0
  struct WriteThis *wt = (struct WriteThis *)userp;
  size_t buffer_size = size*nmemb;

  if(wt->sizeleft) {
    /* copy as much as possible from the source to the destination */
    size_t copy_this_much = wt->sizeleft;
    if(copy_this_much > buffer_size)
      copy_this_much = buffer_size;
    memcpy(dest, wt->readptr, copy_this_much);

    wt->readptr += copy_this_much;
    wt->sizeleft -= copy_this_much;
    return copy_this_much; /* we copied this many bytes */
  }
 #endif
 aaCustomLog("ProcRead %i %i",size,nmemb);

  return 0; /* no more data left to deliver */
}


 Z curlyProcTrace                      (CURL*handle,curl_infotype type,CP data,Y size,VP ud)
 {
 N ci;
 H pos,off,sl;
 N code;
 H att;
 _curly*curly;
 B etc[_4K];
 B buf[_16K];

 curly=ud;
 for(ci=0;ci<curly->max_calls;ci++)
  {
  if(curly->call[ci].magic==0) { continue; }
  if(curly->call[ci].curl_handle==handle) {  break;   }
  }
 if(ci==curly->max_calls)
  {
  return 0;
  }
 curly->call[ci].el=aaMsRunning()-curly->call[ci].ms;

 UNUSE(data);
 UNUSE(size);
 switch(type)
  {
  default:
  return 0;

  case CURLINFO_TEXT:
  case CURLINFO_SSL_DATA_OUT:
  case CURLINFO_SSL_DATA_IN:
  if(aaBitGet(curly->call[ci].bug,0))
   {
   aaStringNCopy(buf,data,size,YES);
   aaStringLen(buf,&sl);
   aaStringLastCharNonVisibleRemove(buf,sl);
   //aa"!!t=%i [%s]",type,buf);
   //aaCustomLog("!!t=%i [%s]",type,buf);
   }
  break;




  case CURLINFO_HEADER_IN:
  case CURLINFO_HEADER_OUT:
  aaStringNCopy(buf,data,size,YES);
  aaStringLen(buf,&sl);
  //ezlog,"%s",buf);
  att=0;
  while(1)
   {
   if(curly->call[ci].is_cancel) {  }//ezlog,"%s is cancel ",__func__); }
   att++;
   if(att>100&&(att%10)==0) { }//ezlog,"!!%s %i = %i",__func__,__LINE__,att); }
   //if(att>=10000) { oof; }

   if(sl==0) { break; }
   if(aaStringFindCharList(buf,sl,&pos,"\r\n",YES,0,YES)!=YES)
    {
    if(type==CURLINFO_HEADER_OUT)
     {
     if(aaBitGet(curly->call[ci].bug,1))
      {
      if(curly->call[ci].hdr_o_count==0)
       {
       //aaEzyLog(&ezy,2,"!!-----");
       }
      ///aaCustomLog("!!oa %-2i %s",curly->call[ci].hdr_o_count,buf);
      }
     curly->call[ci].hdr_o_count++;
     }
    else
    if(type==CURLINFO_HEADER_IN)
     {
     if(aaBitGet(curly->call[ci].bug,2))
      {
      if(curly->call[ci].hdr_i_count==0)
       {
       //aaEzyLog(&ezy,2,"!!-----");
       }
         ///aaCustomLog("ia %-2i %s",curly->call[ci].hdr_i_count,buf);
      }
     curly->call[ci].hdr_i_count++;
     }
    break;
    }
   if(pos==0)
    {
    aaStringDeleteChars(buf,sl,0,pos+1);
    aaStringLen(buf,&sl);
    continue;
    }
   buf[pos]=NULL_CHAR;
   if(pos==0) { aaNote(0,"line=%i",__LINE__); ; }
   if(type==CURLINFO_HEADER_OUT)
    {
    if(aaBitGet(curly->call[ci].bug,1))
     {
     if(curly->call[ci].hdr_o_count==0)
      {
      //aaEzyLog(&ezy,2,"!!----- %s ",buf);
      }
///   aaCustomLog("!!ob %-2i %s",curly->call[ci].hdr_o_count,buf);
     }
    curly->call[ci].hdr_o_count++;
    }
   else
   if(type==CURLINFO_HEADER_IN)
    {
    if(aaBitGet(curly->call[ci].bug,2))
     {
     if(curly->call[ci].hdr_i_count==0)
      {
   ////   aaCustomLog("!!----- %s ",buf);
      }
///     aaCustomLog("!!ib %-2i %s",curly->call[ci].hdr_i_count,buf);
     }
    aaStringCopy(etc,buf);
    if(aaStringNICompare(etc,"location:",F32,0)==YES)
     {
     if(aaStringFindChar(etc,0,&off,':',YES,0,YES)==YES)
      {
      aaStringDeleteChars(etc,0,0,off+1);
      aaStringRemoveSpaces(etc,0,1,1);
      aaStringCopy(curly->call[ci].location,etc);
      if(aaStringFindChar(curly->call[ci].location,0,&off,';',YES,0,YES)==YES) {   curly->call[ci].location[off]=NULL_CHAR;      }
      }
     }

    if(aaStringNICompare(etc,"content-type:",F32,0)==YES)
     {
     if(aaStringFindChar(etc,0,&off,':',YES,0,YES)==YES)
      {
      aaStringDeleteChars(etc,0,0,off+1);
      aaStringRemoveSpaces(etc,0,1,1);
      aaStringCopy(curly->call[ci].res_content_type,etc);
      if(aaStringFindChar(curly->call[ci].res_content_type,0,&off,';',YES,0,YES)==YES) {   curly->call[ci].res_content_type[off]=NULL_CHAR;      }
      }
     }
    curly->call[ci].hdr_i_count++;
    }
   aaStringDeleteChars(buf,sl,0,pos+1);
   aaStringLen(buf,&sl);
   }
  //aaEzyLog(&app.ezy,2,"!!%s %i att=%i",__func__,__LINE__,att);
  if(type==CURLINFO_HEADER_IN)
   {
   if(curly->call[ci].http_code==0)
    {
    curl_easy_getinfo(curly->call[ci].curl_handle,CURLINFO_RESPONSE_CODE,&code);
    curly->call[ci].http_code=(W)code;
    ///aaCustomLog("!!%i httpcode--- %i",ci,curly->call[ci].http_code);
    if(curly->call[ci].http_code==100)
     {
     ///curl_easy_setopt(curly->call[ci].curl_handle,CURLOPT_READFUNCTION,curlyProcRead);
     }

    if(curly->call[ci].http_code!=200)
     {
    // aaCustomLog("!!HTTP CODE = %i",curly->call[ci].http_code);
     }
    }
   }
  break;



  case CURLINFO_DATA_OUT:
  case CURLINFO_DATA_IN:
  //if(curly->call[ci].is_cancel) { return -1; }
  break;
  }
 return 0;
 }





 Y curlyProcWrite                      (CP contents,Y size,Y nmemb,VP ud)
 {
 B ret;
 H left,realsize,mu;
 N ci;
 _curlycall*ccall;
 _curly*curly;

 ccall=(_curlycall*)ud;
 curly=(_curly*)ccall->curly_ptr;
 for(ci=0;ci<curly->max_calls;ci++)
  {
  if(curly->call[ci].magic==0) { continue; }
  if(curly->call[ci].thread.handle==ccall->thread.handle) {  break;   }
  }
 if(ci==curly->max_calls) {  aaNote(0,"line=%i",__LINE__);  }

 curly->call[ci].el=aaMsRunning()-curly->call[ci].ms;

 if(curly->call[ci].is_cancel)
  {
  //return -1;
  //aaLog(-555,">> ci=%i %I64d",ci,aaMsRunning());
  }

 realsize=0;

 if(!curly->call[ci].is_cancel)
  {
  realsize=size*nmemb;
  if(ccall->mun.bytes==0)
   {
   ///aaDebugf("!!alloc %i ",realsize+_8K);
   if((ret=aaMemoryUnitAllocate(&ccall->mun,realsize+_8K))!=YES) { oops; } /// xaa
   //aaMemoryUnitNameSet(&ccall->mun,"munhy");
   left=ccall->mun.bytes-ccall->mun.used;
   }
  left=ccall->mun.bytes-ccall->mun.used;
  mu=ccall->mun.used;
  if((realsize+_2K)>=left)
   {
   //aaEzyLog(&ezy,2,"!!realloc %i %i",ccall->mun.bytes,ccall->mun.bytes+realsize+_32K);
   if((ret=aaMemoryUnitReAllocate(&ccall->mun,ccall->mun.bytes+realsize+_32K))!=YES) { oops; } /// xaa
   ccall->mun.used=mu;
   }
  if(realsize>0)
   {
   aaMemoryCopy(&ccall->mun.mem[ccall->mun.used],realsize,contents);
   ccall->mun.used+=realsize;
   }
  ccall->mun.mem[ccall->mun.used]=NULL_CHAR;
  }
 return realsize;
 }








 Y curlyProcProgress                   (VP p,D dltot,D dlnow,D ultot,D ulnow)
 {
 _curlycall*ccall;
 _curly*curly;
 N ci;

 UNUSE(dltot);
 UNUSE(dlnow);
 UNUSE(ulnow);
 UNUSE(ultot);
 ccall=(_curlycall*)p;
 curly=(_curly*)ccall->curly_ptr;
 for(ci=0;ci<curly->max_calls;ci++)
  {
  if(curly->call[ci].magic==0) { continue; }
  if(curly->call[ci].thread.handle==ccall->thread.handle) {  break;   }
  }
 if(ci==curly->max_calls) { aaNote(0,"line=%i",__LINE__);  }
 curly->call[ci].el=aaMsRunning()-curly->call[ci].ms;
 if(curly->call[ci].is_cancel)
  {
  //aaLog(-555,"%I64d ci=%i is canc",aaMsRunning(),ci);
  return -1;
  }
 //if(curly->call[ci].el>aaSecs(20)) { aaEzyLog(&ezy,2,"!!kerly %i real timeout",ci);  curlyCallDelete(curly,ci); }
 if(curly->call[ci].el>aaSecs(60))
  {
//  ezlog,"call timeout");
  //aaEzyLog(&ezy,2,"!!kerly %i real timeout %.2f",ci,curly->call[ci].el/1000.0);
  //f\return -1;
  }
 return 0;
 }





/*-----------------------------------------------------------------------*/
/*-----------------------------------------------------------------------*/




 B curlyNew                            (_curly*curly)
 {
 if(curly==NULL) { return RET_MISSINGPARM; }
 aaMemoryFill(curly,sizeof(_curly),0);
 curly->magic=aaHPP(curlyNew);
 curl_global_init(CURL_GLOBAL_ALL);
 curly->share_handle=curl_share_init();
 curl_share_setopt(curly->share_handle,CURLSHOPT_USERDATA,curly);
 curl_share_setopt(curly->share_handle,CURLSHOPT_SHARE,CURL_LOCK_DATA_CONNECT);
 ///aaSyncNew(&curly->sync,1,1,"connlock");
 curly->max_calls=aaElementCount(curly->call);
 curly->cur_calls=0;
 ///aaEzyLog(&ezy,2,"sanitized ok, tor.process_count=%i tor_slots=%i",curly->tor.process_count,curly->tor_slots);
 return RET_YES;
 }







 B curlyDelete                         (_curly*curly)
 {
 //B ret;
 N ci;
 _curlycall*ccall;

 if(curly==NULL) { return RET_MISSINGPARM; }
 if(curly->magic!=aaHPP(curlyNew)) { return RET_NOTINITIALIZED; }
 //appLog(0,"ending cur_calls=%i fin_calls=%i",curly->cur_calls,curly->fin_calls);
 //aaEzyLog(&app.ezy,2,"!!ending fin_calls=%i",curly->fin_calls);
 #if 1
 for(ci=0;ci<curly->max_calls;ci++)
  {
  ccall=&curly->call[ci];
  if(ccall->magic!=aaHPP(curlyCallNew)) { continue; }
  ccall->el=aaMsRunning()-ccall->ms;
  ///aaLog(-555,"?? %i isfin=%i iscn=%i iswk=%i %I64d",ci,ccall->is_fin,ccall->is_cancel,ccall->is_working,ccall->el);
  if(ccall->is_cancel!=YES) { curlyCallCancel(curly,ci); }
  }
 #endif
 //aaLog(-555,"%s %i",__func__,__LINE__); showleaks();
 #if 1
 ///aaLog(-555,"abb=%I64d",aaMsRunning());
/// aaLog(-555,"line=%i %I64d",__LINE__,aaMsRunning());
 while(1)
  {
  curlyYield(curly);
  if(curly->cur_calls==0&&curly->fin_calls==0) { break; }
  for(ci=0;ci<curly->max_calls;ci++)
   {
   ccall=&curly->call[ci];
   if(ccall->magic!=aaHPP(curlyCallNew)) { continue; }
   //aaLog(-555,"%i f=%i c=%i",ci,ccall->is_fin,ccall->is_cancel);
   if(ccall->is_fin==NO)
    {
    //aaEzyLog(&app.ezy,2,"!!%i f=%i c=%i wk=%i",ci,ccall->is_fin,ccall->is_cancel,ccall->is_working);
    if(ccall->is_cancel!=YES)
     {
     ///aaLog(-555,"cancel %i %I64d",ci,aaMsRunning());
     ///aaLog(-555,"line=%i %I64d",__LINE__,aaMsRunning());
     curlyCallCancel(curly,ci);
     }
    continue;
    }
   //if(ccall->is_cancel!=YES) { curlyCallCancel(curly,ci); continue; }
   ///aaEzyLog(&app.ezy,2,"!!del %i isfin=%i iscn=%i iswk=%i %I64d",ci,ccall->is_fin,ccall->is_cancel,ccall->is_working,aaMsRunning());
   ///aaLog(-555,"line=%i %I64d",__LINE__,aaMsRunning());
   if(curlyCallDelete(curly,ci)!=YES) oof;
   //aaLog(-555,"line=%i %I64d",__LINE__,aaMsRunning());
    //aaEzyLog(&app.ezy,2,"!!bending cur_calls=%i fin=%i",curly->cur_calls,curly->fin_calls);
   }

  /*
  if(aaMathRand32(0,100)==0)
   {
   aaLog(-555,"cur=%i fin=%i %I64d",curly->cur_calls,curly->fin_calls,aaMsRunning());
   }
  */

  aaYield(5000);
  if(curly->cur_calls!=0) { continue; }
  break;
  }
 //aaLog(-555,"line=%i %I64d",__LINE__,aaMsRunning());
 ///aaEzyLog(&app.ezy,2,"!!bbb=%I64d",aaMsRunning());
 #endif
 //aaLog(-555,"%s %i",__func__,__LINE__); showleaks();
 #if 1
 curl_share_cleanup(curly->share_handle);
 curl_global_cleanup();
 ///aaSyncDelete(&curly->sync);
 #endif
 ///aaLog(-555,"eee=%I64d",aaMsRunning());
 //aaLog(-555,"%s %i",__func__,__LINE__); showleaks();
 aaMemoryFill(curly,sizeof(_curly),0);
 ///aaEzyLog(&app.ezy,2,"!!ccc=%I64d",aaMsRunning());
 return RET_YES;
 }







 B curlyYield                          (_curly*curly)
 {
 N ci;
 _curlycall*ccall;

 if(curly==NULL) { return RET_MISSINGPARM; }
 if(curly->magic!=aaHPP(curlyNew)) { return RET_NOTINITIALIZED; }
 for(ci=0;ci<curly->max_calls;ci++)
  {
  curly->pf_call++;
  curly->pf_call%=curly->max_calls;
  ccall=&curly->call[curly->pf_call];
  if(ccall->magic==0) { continue; }
  if(ccall->thread.handle!=0)  {   aaThreadStatus(ccall->thread.handle,&ccall->thread.status);   }
  ccall->el=aaMsRunning()-ccall->ms;
  break;
  }
 return RET_YES;
 }








/*-----------------------------------------------------------------------*/






 B curlyCallNew                        (_curly*curly,HP index,_curlycall**ccall,H bug,VP fmt,...)
 {
 B ret;
 N ci;

 if(curly==NULL) { return RET_MISSINGPARM; }
 if(curly->magic!=aaHPP(curlyNew)) { return RET_NOTINITIALIZED; }
 if(index) { *index=0; }
 aaVargsf8K(fmt);
 if(ccall) { *ccall=NULL; }
 for(ci=0;ci<curly->max_calls;ci++)
  {
  if(curly->call[ci].magic==0)  { break; }
  }
 if(ci==curly->max_calls) { return RET_FAILED; }
 if(curly->call[ci].is_working) oof;
 if(curly->call[ci].is_fin==YES) oof;
 if(curly->call[ci].is_cancel) oof;
 aaMemoryFill(&curly->call[ci],sizeof(_curlycall),0);
 curly->call[ci].magic=aaHPP(curlyCallNew);
// curly->call[ci].stage=0;
 curly->call[ci].curly_ptr=curly;
 curly->call[ci].ms=aaMsRunning();
 curly->call[ci].bug=bug;
 curly->call[ci].http_code=0;
 aaStringNCopy(curly->call[ci].cmd,str8k.buf,str8k.len,YES);
 //aaLog(-555,"cmd=%s",str8k.buf);
 //if((ret=aaMemoryUnitAllocate(&curly->call[ci].mun,_16K))!=YES) { oops; } /// xaa

 //aaLog(-555,"curly making thread");
 if((ret=aaThreadCreateRelayed(&curly->call[ci].thread.handle,curlyCallThread,(H)curly,0,NULL,YES-1))!=YES)
  {
  aaThreadDestroy(curly->call[ci].thread.handle);
  //curly->cur_calls++;
  //curly->tot_calls++;
  //aaEzyLog(&ezy,2,"!!oops %s",arets);
  if(curlyCallDelete(curly,ci)!=YES) oof;
  return ret;
  }
 if((ret=aaThreadStatus(curly->call[ci].thread.handle,&curly->call[ci].thread.status))!=YES) { oops; }
 curly->cur_calls++;
 curly->tot_calls++;
 if(index) { *index=ci; }
 if(ccall) { *ccall=&curly->call[ci]; }
 return RET_YES;
 }







 B curlyCallNewEx                      (_curly*curly,H index,_curlycall**ccall,H bug,VP fmt,...)
 {
 B ret;
 H ci;

 oof;
 if(curly==NULL) { return RET_MISSINGPARM; }
 if(curly->magic!=aaHPP(curlyNew)) { return RET_NOTINITIALIZED; }
 if((N)index>=curly->max_calls) { return RET_BOUNDS; }
 aaVargsf8K(fmt);
 if(ccall) { *ccall=NULL; }
 ci=index;
 if(curly->call[ci].magic==0) { return RET_INUSE; }
 aaMemoryFill(&curly->call[ci],sizeof(_curlycall),0);
 curly->call[ci].magic=aaHPP(curlyCallNew);
// curly->call[ci].stage=0;
 curly->call[ci].curly_ptr=curly;
 curly->call[ci].ms=aaMsRunning();
 curly->call[ci].bug=bug;
 aaStringNCopy(curly->call[ci].cmd,str8k.buf,str8k.len,YES);

 if((ret=aaThreadCreateRelayed(&curly->call[ci].thread.handle,curlyCallThread,(H)curly,0,NULL,YES-1))!=YES)
  {
  oof;
  aaThreadDestroy(curly->call[ci].thread.handle);
  //curly->cur_calls++;
  //curly->tot_calls++;
  if(curlyCallDelete(curly,ci)!=YES) oof;
  return ret;
  }
 if((ret=aaThreadStatus(curly->call[ci].thread.handle,&curly->call[ci].thread.status))!=YES) { oops; }
 curly->cur_calls++;
 curly->tot_calls++;
 if(ccall) { *ccall=&curly->call[ci]; }
 return RET_YES;
 }









 B curlyCallDelete                     (_curly*curly,H index)
 {
 N ci;
 H go;

 if(curly==NULL)              { oof; return RET_MISSINGPARM; }
 if(curly->magic!=aaHPP(curlyNew)) { return RET_NOTINITIALIZED; }
 ci=(N)index;
 if(ci>=curly->max_calls)     { return RET_BADHANDLE; }
 if(curly->call[ci].magic==0) { return RET_BADHANDLE; }
 if(curly->call[ci].is_fin==NO)
  {
  //curlyCallCancel(curly,ci);
  }
 if(curly->call[ci].thread.handle!=0)
  {
  if(curly->call[ci].is_fin==NO)
   {
   //att=0;
   #if 0
   while(aaSyncLock(&curly->sync)!=RET_YES)
    {
    curlyYield(curly);
    att++;
    if(att>1000&&(att%100)==0) { }//ezlog,"!!%s %i = %i",__func__,__LINE__,att); }
    if(att>=10000) { oof; }
    }
    #endif
   aaThreadExit(curly->call[ci].thread.handle);
   //aaEzyLog(&app.ezy,2,"!!%s %i att=%i",__func__,__LINE__,att);
   }
  aaThreadDestroy(curly->call[ci].thread.handle);
  for(go=0;go<400;go++)
   {
   curlyYield(&kerly);
   if(curly->call[ci].is_fin) { break; }
   aaSleep(3);
   if((go%100)==99) { }//ezlog,"curly pause %i",go); }//appYield(); }
   //curlyYield(&kerly);
   //aaYield(1000);
   }
  if(go==1000) { }//ezlog,"couldnt kill hread"); }//appYield(); } //oof; } ///damon
  //aaLog(-555,"ci=%i line=%i",ci,__LINE__);
  ///curly->call[ci].thread.handle=0;// == commented out
  }

 ///if(curly->call[ci].is_fin==NO) marky
  {
  if(curly->call[ci].clist)
   {
   //curl_slist_free_all(curly->call[ci].clist);
   curly->call[ci].clist=0;
   }
  if(curly->call[ci].curl_handle!=0)
   {
   curl_easy_cleanup(curly->call[ci].curl_handle);
   }
  }

 ///curly->call[ci].curl_handle=0; === ccommented out
 if(curly->call[ci].mun.bytes)
  {
  if(aaMemoryUnitRelease(&curly->call[ci].mun)!=YES) { oof; } /// xaa
  }
 if(curly->call[ci].task_list.magic)
  {
  ///aaLog(-555,"list delete %i",ci);
  aaListDelete(&curly->call[ci].task_list);
  }
 if(curly->call[ci].is_fin==YES)
  {
  curly->fin_calls--;
  if(curly->fin_calls<0)
   {
   //aaEzyLog(&ezy,2,"!!line=%i fin_calls=%i cur_calls=%i",__LINE__,curly->fin_calls,curly->cur_calls);
   curly->fin_calls=0;
   }
  }
 aaMemoryFill(&curly->call[ci],sizeof(_curlycall),0);
 curly->cur_calls--;
 ///aaCustomLog("del");
 return RET_YES;
 }







 B curlyCallCancel                     (_curly*curly,H index)
 {
 N ci;
 _curlycall*ccall;

 if(curly==NULL) { return RET_MISSINGPARM; }
 if(curly->magic!=aaHPP(curlyNew)) { return RET_NOTINITIALIZED; }
 ci=(N)index;
 if(ci>=curly->max_calls) { return RET_BADHANDLE; }
 if(curly->call[ci].magic==0) { return RET_BADHANDLE; }
 ccall=&curly->call[ci];
 ccall->is_cancel=YES;
 return RET_YES;
 }






 B curlyCallCmdGet                     (_curly*curly,H index,H line,VP key,VP val)
 {
 B ret;
 H ci,sl,lines;
 B buf[_8K];
 H line_offs[1000];
 H line_lens[1000];

 if(key) { aaStringNull(key); }
 if(val) { aaStringNull(val); }
 ci=index;
 aaStringLen(curly->call[ci].cmd,&sl);
 if((ret=aaStringLineCountGetToBuffer(curly->call[ci].cmd,sl,&lines,1000,line_offs,line_lens))!=YES) { oops;  }
 if(line>lines) {  return RET_BOUNDS; }
 aaStringNCopy(buf,&curly->call[ci].cmd[line_offs[line]],line_lens[line],YES);
 if(line_lens[line]>0)
  {
  if((ret=aaStringSplitChar(buf,line_lens[line],'=',NO,YES,0,YES,key,val))!=YES)  { oops; }
  }
 return YES;
 }





/*-----------------------------------------------------------------------*/




 B curlyUserAgentGet                   (H ui,VP ua)
 {
 if(ua==NULL) { return RET_MISSINGPARM; }
 aaStringNull(ua);
 while(1)
  {
  if(ui==0) { aaStringCopyf(ua,"Mozilla/5.0 (Linux; Android 6.0; Nexus 5 Build/MRA58N) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/68.0.3440.106 Mobile Safari/537.36"); break; }
  if(ui==1) { aaStringCopyf(ua,"Mozilla/5.0 (Linux; Android 6.2; Nexus 5 Build/MRA58N) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/68.0.3440.106 Mobile Safari/537.36"); break; }
  if(ui==2) { aaStringCopyf(ua,"Mozilla/5.0 (compatible; MSIE 8.0; Windows NT 6.1; Trident/5.0)"); break; }
  if(ui==3) { aaStringCopyf(ua,"Mozilla/4.0 (compatible; MSIE 6.0; Windows NT 5.1; SV1; Media Center PC)"); break; }
  if(ui==4) { aaStringCopyf(ua,"Mozilla/5.0 (Linux; U; Android 4.0.3; ko-kr; LG-L160L Build/IML74K) AppleWebkit/534.30 (KHTML, like Gecko) Version/4.0 Mobile Safari/534.30"); break; }
  if(ui==5) { aaStringCopyf(ua,"Mozilla/5.0 (Linux; U; Android 4.0.3; de-ch; HTC Sensation Build/IML74K) AppleWebKit/534.30 (KHTML, like Gecko) Version/4.0 Mobile Safari/534.30"); break; }
  if(ui==6) { aaStringCopyf(ua,"Mozilla/5.0 (Linux; U; Android 2.3; en-us) AppleWebKit/999+ (KHTML, like Gecko) Safari/999.9"); break; }
  if(ui==7) { aaStringCopyf(ua,"Mozilla/5.0 (Linux; U; Android 2.3.5; zh-cn; HTC_IncredibleS_S710e Build/GRJ90) AppleWebKit/533.1 (KHTML, like Gecko) Version/4.0 Mobile Safari/533.1"); break; }
  if(ui==8) { aaStringCopyf(ua,"Mozilla/5.0 (Linux; U; Android 2.3.5; en-us; HTC Vision Build/GRI40) AppleWebKit/533.1 (KHTML, like Gecko) Version/4.0 Mobile Safari/533.1"); break; }
  if(ui==9) { aaStringCopyf(ua,"Mozilla/5.0 (Linux; U; Android 2.3.4; fr-fr; HTC Desire Build/GRJ22) AppleWebKit/533.1 (KHTML, like Gecko) Version/4.0 Mobile Safari/533.1"); break; }
  if(ui==10) { aaStringCopyf(ua,"Mozilla/5.0 (Linux; U; Android 2.3.4; en-us; T-Mobile myTouch 3G Slide Build/GRI40) AppleWebKit/533.1 (KHTML, like Gecko) Version/4.0 Mobile Safari/533.1"); break; }
  if(ui==11) { aaStringCopyf(ua,"Mozilla/5.0 (Linux; U; Android 2.3.3; zh-tw; HTC_Pyramid Build/GRI40) AppleWebKit/533.1 (KHTML, like Gecko) Version/4.0 Mobile Safari/533.1"); break; }
  if(ui==12) { aaStringCopyf(ua,"Mozilla/5.0 (Linux; U; Android 2.3.3; zh-tw; HTC_Pyramid Build/GRI40) AppleWebKit/533.1 (KHTML, like Gecko) Version/4.0 Mobile Safari"); break; }
  if(ui==13) { aaStringCopyf(ua,"Mozilla/5.0 (Linux; U; Android 2.3.3; zh-tw; HTC Pyramid Build/GRI40) AppleWebKit/533.1 (KHTML, like Gecko) Version/4.0 Mobile Safari/533.1"); break; }
  if(ui==14) { aaStringCopyf(ua,"Mozilla/5.0 (Linux; U; Android 2.3.3; ko-kr; LG-LU3000 Build/GRI40) AppleWebKit/533.1 (KHTML, like Gecko) Version/4.0 Mobile Safari/533.1"); break; }
  if(ui==15) { aaStringCopyf(ua,"Mozilla/5.0 (Linux; U; Android 2.3.3; en-us; HTC_DesireS_S510e Build/GRI40) AppleWebKit/533.1 (KHTML, like Gecko) Version/4.0 Mobile Safari/533.1"); break; }
  if(ui==16) { aaStringCopyf(ua,"Mozilla/5.0 (Linux; U; Android 2.3.3; en-us; HTC_DesireS_S510e Build/GRI40) AppleWebKit/533.1 (KHTML, like Gecko) Version/4.0 Mobile"); break; }
  if(ui==17) { aaStringCopyf(ua,"Mozilla/5.0 (Linux; U; Android 2.3.3; de-de; HTC Desire Build/GRI40) AppleWebKit/533.1 (KHTML, like Gecko) Version/4.0 Mobile Safari/533.1"); break; }
  if(ui==18) { aaStringCopyf(ua,"Mozilla/5.0 (Linux; U; Android 2.3.3; de-ch; HTC Desire Build/FRF91) AppleWebKit/533.1 (KHTML, like Gecko) Version/4.0 Mobile Safari/533.1"); break; }
  if(ui==19) { aaStringCopyf(ua,"Mozilla/5.0 (Linux; U; Android 2.2; fr-lu; HTC Legend Build/FRF91) AppleWebKit/533.1 (KHTML, like Gecko) Version/4.0 Mobile Safari/533.1"); break; }
  if(ui==20) { aaStringCopyf(ua,"Mozilla/5.0 (Linux; U; Android 2.2; en-sa; HTC_DesireHD_A9191 Build/FRF91) AppleWebKit/533.1 (KHTML, like Gecko) Version/4.0 Mobile Safari/533.1"); break; }
  if(ui==21) { aaStringCopyf(ua,"Mozilla/5.0 (Linux; U; Android 2.2.1; fr-fr; HTC_DesireZ_A7272 Build/FRG83D) AppleWebKit/533.1 (KHTML, like Gecko) Version/4.0 Mobile Safari/533.1"); break; }
  if(ui==22) { aaStringCopyf(ua,"Mozilla/5.0 (Linux; U; Android 2.2.1; en-gb; HTC_DesireZ_A7272 Build/FRG83D) AppleWebKit/533.1 (KHTML, like Gecko) Version/4.0 Mobile Safari/533.1"); break; }
  if(ui==23) { aaStringCopyf(ua,"Mozilla/5.0 (Linux; U; Android 2.2.1; en-ca; LG-P505R Build/FRG83) AppleWebKit/533.1 (KHTML, like Gecko) Version/4.0 Mobile Safari/533.1"); break; }
  if(ui==24) { aaStringCopyf(ua,"Mozilla/5.0 (X11; Linux i586; rv:31.0) Gecko/20100101 Firefox/31.0"); break; }
  if(ui==25) { aaStringCopyf(ua,"Mozilla/5.0 (Windows NT 6.1; WOW64; rv:31.0) Gecko/20130401 Firefox/31.0"); break; }
  if(ui==26) { aaStringCopyf(ua,"Mozilla/5.0 (Windows NT 5.1; rv:31.0) Gecko/20100101 Firefox/31.0"); break; }
  if(ui==27) { aaStringCopyf(ua,"Mozilla/5.0 (Windows NT 6.1; WOW64; rv:29.0) Gecko/20120101 Firefox/29.0"); break; }
  if(ui==28) { aaStringCopyf(ua,"Mozilla/5.0 (Windows NT 6.1; Win64; x64; rv:25.0) Gecko/20100101 Firefox/29.0"); break; }
  if(ui==29) { aaStringCopyf(ua,"Mozilla/5.0 (Windows NT 6.1; rv:27.3) Gecko/20130101 Firefox/27.3"); break; }
  return RET_FAILED;
  }

 return RET_YES;
 }






 B curlyCallHeaderAppend               (_curly*curly,H index,VP fmt,...)
 {
 N ci;
 B etc[_1K];
 Z res;

 if(curly==NULL) { return RET_MISSINGPARM; }
 if(curly->magic!=aaHPP(curlyNew)) { return RET_NOTINITIALIZED; }
 aaVargsf8K(fmt);
 ci=(N)index;
 if(ci>=curly->max_calls)        { return RET_BADHANDLE; }
 if(curly->call[ci].magic==0)    { return RET_BADHANDLE; }
 if(curly->call[ci].is_working)  { return RET_INUSE;  }
 if(curly->call[ci].is_fin==YES) { return RET_ALREADYSTARTED; }
 aaStringCopyf(etc,"%s",str8k.buf);
 curly->call[ci].clist=curl_slist_append(curly->call[ci].clist,(CP)etc);
 res=curl_easy_setopt(curly->call[ci].curl_handle,CURLOPT_HTTPHEADER,curly->call[ci].clist);
 if(res!=0) { return RET_FAILED; }
 return RET_YES;
 }




 B curlyCallWait                       (_curly*curly,H index,_curlycall**curlycallptr)
 {
 H ci;
 _curlycall*ccall;

 if(curlycallptr) { *curlycallptr=NULL; }
 if(curly==NULL)              { oof; return RET_MISSINGPARM; }
 if(curly->magic!=aaHPP(curlyNew)) { return RET_NOTINITIALIZED; }
 ci=(N)index;
 if((N)ci>=curly->max_calls)     { return RET_BADHANDLE; }
 if(curlycallptr) { *curlycallptr=&curly->call[ci]; }
 if(curly->call[ci].magic==0) { return RET_BADHANDLE; }
 ccall=(_curlycall*)&curly->call[ci];
 if(ccall->is_fin!=YES)     { return RET_NOTREADY; }
 if(ccall->is_error)
  {
  aaCustomLog("ERR: %i %s",ccall->result,ccall->result_str);
  return RET_FAILED;
  } //aaEzyLog(&ezy,2,"ERR: %i %s",ccall->result,ccall->result_str);  facebookRetry(facebook);   break;    }
 if(ccall->http_code==301||ccall->http_code==302) { return RET_OTHER; } //facebookRedirect(facebook,"%s",ccall->location); break; }
 if(ccall->http_code!=200&&ccall->http_code!=100)
  {
  ///aaCustomLog("http_code=%i",ccall->http_code);
  return RET_FAILED;
  } // aaEzyLog(&ezy,2,"fin=%i",ccall->http_code);       break;    }
 if(ccall->http_code==100)
  {
  ccall->http_code=0;
  }
 return RET_YES;
 }





 B curlyCallRetry                      (_curly*curly,H index,HP newindex)
 {
 B ret;
 N ci;
 H new_ci;
 H tt,s4i;
 H bug;
 B usd[_1K];
 B cmd[_8K];
 _curlycall*ccall;

 if(curly==NULL)                   { return RET_MISSINGPARM; }
 if(curly->magic!=aaHPP(curlyNew)) { return RET_NOTINITIALIZED; }
 ci=(N)index;
 if(ci>=curly->max_calls)     { return RET_BADHANDLE; }
 if(curly->call[index].magic==0) { return RET_BADHANDLE; }
 ccall=(_curlycall*)&curly->call[index];
 bug=ccall->bug;
 tt=ccall->task_type;
 s4i=ccall->s4i;
 aaMemoryCopy(usd,_1K,ccall->user_data);
 aaMemoryCopy(cmd,_8K,ccall->cmd);
 if((ret=curlyCallDelete(curly,index))!=RET_YES)                { oops; }
 if((ret=curlyCallNew(curly,&new_ci,NULL,bug,"%s",cmd))!=RET_YES) { oops; }
 curly->call[new_ci].task_type=tt;
 curly->call[new_ci].s4i=s4i;
 aaMemoryCopy(curly->call[new_ci].user_data,_1K,usd);
 curly->tot_retries++;
 if(newindex) { *newindex=new_ci; }
 return RET_YES;
 }





//=====================================================




 B curlyCallThread                     (H handle)
 {
 B ret;
 _threadunit tu;
 _curly*curly;
 N ci;
 H li;
 B etc[_4K];
 B is_post;
 B url[_8K];
 B post_data[_8K];
 B post_len[_1K];
 B content_type[_1K];
 B refer[_8K];
 B accept[_1K];
 B key[_8K];
 B val[_8K];
 B ua[_1K];
 H socks_port;
 B use_tor;
 B use_socks;
 B use_proxy;
 B host[129];
 B cook[_1K];
 B username[_1K];
 B password[_1K];
 B xfer[_1K];
// B ua[129];
 CURLcode cres;
 //D dub;
 N response_code;

 tu.handle=handle;
 if((ret=aaThreadStatus(tu.handle,&tu.status))!=RET_YES) { oops; }
 curly=(_curly*)tu.status.data;
 for(ci=0;ci<curly->max_calls;ci++)
  {
  if(curly->call[ci].magic==0) { continue; }
  if(curly->call[ci].thread.handle==handle) {  break;   }
  }
 if(ci==curly->max_calls) { aaNote(0,"line=%i",__LINE__);  }
 is_post=NO;
 socks_port=0;
 use_tor=0;
 use_socks=0;
 use_proxy=0;
 aaStringNull(refer);
 aaStringNull(accept);
 aaStringNull(url);
 aaStringNull(post_data);
 aaStringNull(post_len);
 aaStringNull(content_type);
 aaStringNull(cook);
 aaStringNull(ua);
 aaStringNull(username);
 aaStringNull(password);
 aaStringNull(xfer);

// aaStringCopyf(ua,"Mozilla/5.0 (Windows NT 6.3; WOW64; Trident/7.0; rv:11.0) like Gecko");
// aaStringCopyf(ua,"Mozilla/5.0 (Windows NT 6.2; WOW64; rv:34.0) Gecko/20100101 Firefox/34.0");
// aaStringCopyf(ua,"Mozilla/5.0 (Linux; Android 6.0; Nexus 5 Build/MRA58N) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/68.0.3440.106 Mobile Safari/537.36");
 //aaEzyLog(&ezy,2,"UX=%s",ua);
 //aaStringCopyf(cook,"cookie.txt");
 //===================================

// aaLog(-555,"about to decode cmd");
 for(li=0;li<10;li++)
  {
  if((ret=curlyCallCmdGet(curly,ci,li,key,val))!=YES) {  break;  }
  if(aaStringICompare(key,"meth",0)==YES)
   {
   if(aaStringICompare(val,"post",0)==YES) { is_post=YES; }    else
   if(aaStringICompare(val,"get",0)==YES)  { is_post=NO;  }
   continue;
   }
  if(aaStringICompare(key,"socks5",0)==YES)   {   aaStringCopy(host,val);   use_socks=5;   continue;   }
  if(aaStringICompare(key,"socks4",0)==YES)   {   aaStringCopy(host,val);   use_socks=4;   continue;   }
  if(aaStringICompare(key,"proxy",0)==YES)    {   use_proxy=1;   aaStringCopy(host,val);   continue;   }
  if(aaStringICompare(key,"proxys",0)==YES)   {   use_proxy=2;   aaStringCopy(host,val);   continue;   }
  if(aaStringICompare(key,"url",0)==YES)            {   aaStringCopy(url,val);          continue;   }
  if(aaStringICompare(key,"contenttype",0)==YES)    {   aaStringCopy(content_type,val); continue;   }
  if(aaStringICompare(key,"accept",0)==YES)         {   aaStringCopy(accept,val);       continue;   }
  if(aaStringICompare(key,"refer",0)==YES)          {   aaStringCopy(refer,val);        continue;   }
  if(aaStringICompare(key,"postdata",0)==YES)       {   aaStringCopy(post_data,val);    continue;   }
  if(aaStringICompare(key,"postlen",0)==YES)       {   aaStringCopy(post_len,val);    continue;   }
  if(aaStringICompare(key,"ua",0)==YES)    {    aaStringCopy(ua,val);    continue;   }
  if(aaStringICompare(key,"cookie",0)==YES)             {   aaStringCopy(cook,val);    continue;   }
  if(aaStringICompare(key,"username",0)==YES)             {   aaStringCopy(username,val);    continue;   }
  if(aaStringICompare(key,"password",0)==YES)             {   aaStringCopy(password,val);    continue;   }
  if(aaStringICompare(key,"xfer",0)==YES)             {  aaStringCopy(xfer,"chunked");   continue;   }
  ///if(key[0]!=0||val[0]!=0) aaEzyLog(&ezy,2,"%i: %s=%s",li,key,val);
  }
//aaEzyLog(&ezy,2,"UA=%s",ua);
 //===================================

 //aaLog(-555,"url=%s",url);
 curly->call[ci].curl_handle=curl_easy_init();
 curl_easy_setopt(curly->call[ci].curl_handle,CURLOPT_URL,url);
 curl_easy_setopt(curly->call[ci].curl_handle,CURLOPT_DNS_CACHE_TIMEOUT,300L);//360L);

 aaStringCopy(curly->call[ci].url,url);


 curl_easy_setopt(curly->call[ci].curl_handle,CURLOPT_SSL_VERIFYPEER,0);
 curl_easy_setopt(curly->call[ci].curl_handle,CURLOPT_SSL_VERIFYHOST,0);
 curl_easy_setopt(curly->call[ci].curl_handle,CURLOPT_PROXY_SSL_VERIFYPEER,0);
 curl_easy_setopt(curly->call[ci].curl_handle,CURLOPT_PROXY_SSL_VERIFYHOST,0);

 if(cook[0])
  {
  //fsaaEzyLog(&ezy,2,"cook=%s",cook);
  curl_easy_setopt(curly->call[ci].curl_handle,CURLOPT_COOKIEJAR,cook);
  curl_easy_setopt(curly->call[ci].curl_handle,CURLOPT_COOKIEFILE,cook);
  }

 curl_easy_setopt(curly->call[ci].curl_handle,CURLOPT_DEBUGFUNCTION,curlyProcTrace);
 curl_easy_setopt(curly->call[ci].curl_handle,CURLOPT_DEBUGDATA,(VP)curly);
 curl_easy_setopt(curly->call[ci].curl_handle,CURLOPT_VERBOSE,1);
 curl_easy_setopt(curly->call[ci].curl_handle,CURLOPT_NOSIGNAL,1);
 //curl_easy_setopt(curly->call[ci].curl_handle, CURLOPT_USE_SSL, CURLUSESSL_ALL);
 //curl_easy_setopt(curly->call[ci].curl_handle, CURLOPT_SSL_OPTIONS, CURLSSLOPT_ALLOW_BEAST);
 //curl_easy_setopt(curly->call[ci].curl_handle,CURLOPT_SSLVERSION,CURL_SSLVERSION_SSLv3);


 //curl_easy_setopt(curly->call[ci].curl_handle,CURLOPT_SSLVERSION,CURL_SSLVERSION_TLSv1);
 //curl_easy_setopt(curly->call[ci].curl_handle,CURLOPT_IPRESOLVE, CURL_IPRESOLVE_V4);
 //curl_easy_setopt(curly->call[ci].curl_handle,CURLOPT_FOLLOWLOCATION,1L);
 //curl_easy_setopt(curly->call[ci].curl_handle,CURLOPT_FOLLOWLOCATION,0L);
 if(1) curl_easy_setopt(curly->call[ci].curl_handle,CURLOPT_TIMEOUT_MS,1000*60); // 30000
 if(1) curl_easy_setopt(curly->call[ci].curl_handle,CURLOPT_CONNECTTIMEOUT_MS,1000*10);//5000
 ///if(1) curl_easy_setopt(curly->call[ci].curl_handle,CURLOPT_ACCEPTTIMEOUT_MS,10000);

 curly->call[ci].this_ci=ci;

 if(xfer[0])
  {
  if((ret=curlyCallHeaderAppend(curly,ci,"Transfer-Encoding: chunked"))!=YES)             { oops; }
  ///if((ret=curlyCallHeaderAppend(curly,ci,"Content-Length: 750886"))!=YES)             { oops; }
  //curl_easy_setopt(curly->call[ci].curl_handle,CURLOPT_POSTFIELDSIZE,750886);
  //if((ret=curlyCallHeaderAppend(curly,ci,"Expect:"))!=YES) { oops; }
  //aaCustomLog("ci=%i, %i",ci,curly->call[ci].this_ci);
  //aaCustomLog("p=%i",(void *)&curly->call[ci]);
//  curl_easy_setopt(curly->call[ci].curl_handle,CURLOPT_READFUNCTION,curlyProcRead);
//  curl_easy_setopt(curly->call[ci].curl_handle,CURLOPT_READDATA, (void *)&curly->call[ci]);
  }



 if(use_tor)
  {
  aaStringCopyf(etc,"socks5://127.0.0.1:%u",socks_port);
  curl_easy_setopt(curly->call[ci].curl_handle,CURLOPT_PROXY,etc);
  //aaEzyLog(&ezy,2,"tor %s",etc);
  }
 if(use_socks)
  {
  aaStringCopyf(etc,"%s://%s",aaBoolStrings(use_socks==4,"socks4","socks5"),host);
  curl_easy_setopt(curly->call[ci].curl_handle,CURLOPT_PROXY,etc);
  //aaEzyLog(&ezy,2,"socks %s",etc);
  }
 if(use_proxy)
  {
  aaStringCopyf(etc,"%s://%s",aaBoolStrings(use_proxy==1,"http","https"),host);
  curl_easy_setopt(curly->call[ci].curl_handle,CURLOPT_PROXY,etc);
  //aaEzyLog(&ezy,2,"proxy %s",etc);
  }


 if(content_type[0])  {  if((ret=curlyCallHeaderAppend(curly,ci,"Content-Type: %s",content_type))!=YES) { oops; }  }
 if(refer[0])         {  if((ret=curlyCallHeaderAppend(curly,ci,"Referer: %s",refer))!=YES)            { oops; }  }

 if(0)  {  if((ret=curlyCallHeaderAppend(curly,ci,"Referrer-Policy: strict-origin-when-cross-origin"))!=YES) { oops; }  }
 if(0)  {  if((ret=curlyCallHeaderAppend(curly,ci,"Referrer-Policy: no-referrer-when-downgrade"))!=YES)      { oops; }  }
 if(0)  {  if((ret=curlyCallHeaderAppend(curly,ci,"upgrade-insecure-requests: 1"))!=YES) { oops; }  }

 if(accept[0])        {  if((ret=curlyCallHeaderAppend(curly,ci,"Accept: %s",accept))!=YES)             { oops; }  }
 else                 {  if((ret=curlyCallHeaderAppend(curly,ci,"Accept:"))!=YES)                       { oops; }  }

 if(0)  {  curl_easy_setopt(curly->call[ci].curl_handle,CURLOPT_ACCEPT_ENCODING,""); }//gzip, deflate, br");  }
 else   {  if((ret=curlyCallHeaderAppend(curly,ci,"Accept-Encoding:"))!=YES)  { oops; } }

 if(0)  {  if((ret=curlyCallHeaderAppend(curly,ci,"Accept-Language: en-US,en;q=0.9"))!=YES) { oops; }  }
 else   {  if((ret=curlyCallHeaderAppend(curly,ci,"Accept-Language:"))!=YES)  { oops; } }

 if(is_post)
  {
  if(post_data[0]==NULL_CHAR&&post_len[0]==NULL_CHAR&&xfer[0]!=NULL_CHAR)
   {
   curl_easy_setopt(curly->call[ci].curl_handle,CURLOPT_POST, 1L);
  /// curl_easy_setopt(curly->call[ci].curl_handle,CURLOPT_POSTFIELDSIZE,524600);
   }
  else
   {
   if(post_data[0])
    {
    curl_easy_setopt(curly->call[ci].curl_handle,CURLOPT_POSTFIELDS,post_data);
    curl_easy_setopt(curly->call[ci].curl_handle,CURLOPT_POSTFIELDSIZE,(long)strlen((CP)post_data));
    }
   if(post_len[0])
    {
    H plen;
    aaStringToNumber(post_len,0,&plen,0,0,0);
    curl_easy_setopt(curly->call[ci].curl_handle,CURLOPT_POSTFIELDS,0);
    curl_easy_setopt(curly->call[ci].curl_handle,CURLOPT_POSTFIELDSIZE,(long)plen);
    }
   }
  }




 //if((ret=curlyCallUserAgentGet(curly,ci,socks_port%10,ua))!=YES) { oops; }
 //aaStringCopyf(ua,"Mozilla/5.0 (iPhone; CPU iPhone OS 11_0 like Mac OS X) AppleWebKit/604.1.38 (KHTML, like Gecko) Version/11.0 Mobile/15A372 Safari/604.1");
 if(ua[0])
  {
  //aaEzyLog(&ezy,2,"ua=%s",ua);
  if((ret=curlyCallHeaderAppend(curly,ci,"User-Agent: %s",ua))!=YES) { oops; }
  }

 if(username[0])
  {
  curl_easy_setopt(curly->call[ci].curl_handle,CURLOPT_USERNAME,(VP)username);
  }
 if(password[0])
  {
  curl_easy_setopt(curly->call[ci].curl_handle,CURLOPT_PASSWORD,(VP)password);
  }

 if((ret=curlyCallHeaderAppend(curly,ci,"Expect: 100-continue"))!=YES)  { oops; }

 curl_easy_setopt(curly->call[ci].curl_handle,CURLOPT_READFUNCTION,curlyProcRead);


 curl_easy_setopt(curly->call[ci].curl_handle,CURLOPT_WRITEFUNCTION,curlyProcWrite);
 curl_easy_setopt(curly->call[ci].curl_handle,CURLOPT_WRITEDATA,(VP)&curly->call[ci]);
 curl_easy_setopt(curly->call[ci].curl_handle,CURLOPT_XFERINFOFUNCTION,curlyProcProgress);
 curl_easy_setopt(curly->call[ci].curl_handle,CURLOPT_XFERINFODATA,(VP)&curly->call[ci]);
 curl_easy_setopt(curly->call[ci].curl_handle,CURLOPT_NOPROGRESS,0L);

 if(xfer[0])
  {
  curl_easy_setopt(curly->call[ci].curl_handle,CURLOPT_READFUNCTION,curlyProcRead);
  curl_easy_setopt(curly->call[ci].curl_handle,CURLOPT_READDATA, (void *)&curly->call[ci]);
  }


 //aaLog(-555,"about to perform");
 curly->call[ci].is_working=YES;
 cres=curl_easy_perform(curly->call[ci].curl_handle);
 if(cres!=CURLE_OK)  { curly->call[ci].is_ret=RET_FAILED; }
 else                { curly->call[ci].is_ret=RET_YES;    }
 //curl_easy_getinfo(curly->call[ci].curl_handle,CURLINFO_NAMELOOKUP_TIME,&dub); ezlog,"took curl %.5f",dub);
 curly->call[ci].result=cres;
 curly->call[ci].is_working=NO;
 aaStringCopy(curly->call[ci].result_str,(VP)curl_easy_strerror(cres));
 if(cres!=0)
  {
  if(cres==28)  curly->tot_timeouts++;
  else          curly->tot_fails++;
  }

 ///aaCustomLog("HTTP CODE = %i",curly->call[ci].http_code);
 if(curly->call[ci].http_code==100)
  {
  curl_easy_getinfo(curly->call[ci].curl_handle, CURLINFO_RESPONSE_CODE, &response_code);
  ///aaCustomLog("RESP=%i",response_code);
  curly->call[ci].http_code=(W)response_code;
  }
/// aaCustomLog("mun=%i",curly->call[ci].mun.used);
 aaFileSaveFromMemory("fuck.json",curly->call[ci].mun.used,curly->call[ci].mun.mem);
///aaCustomLog("finito");




  //curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &response_code);

 if(curly->call[ci].clist)
  {
  curl_slist_free_all(curly->call[ci].clist);
  curly->call[ci].clist=NULL;
  }
 if(curly->call[ci].curl_handle)
  {
  curl_easy_cleanup(curly->call[ci].curl_handle);
  curly->call[ci].curl_handle=0;
  }
 if(curly->call[ci].result!=0)
  {
  curly->call[ci].is_error=YES;
  }
 if(curly->call[ci].is_fin==NO)
  {
  curly->call[ci].is_fin=YES;
  curly->fin_calls++;
  }
 else
  {
  oof;
  }
 return RET_YES;
 }




/*-----------------------------------------------------------------------*/

 V curlTextFix                         (VP txt,B donbsp)
 {
 //aaStringReplaceString(txt,0,"&#65533;",0,"\'",0,NO,txt);
 aaStringReplaceString(txt,0,"&#64256;",0,"ff",0,NO,txt);
 aaStringReplaceString(txt,0,"&#64257;",0,"fl",0,NO,txt);
 aaStringReplaceString(txt,0,"&#64258;",8,"fl",0,NO,txt);
 aaStringReplaceString(txt,0,"&#64259;",8,"ffi",0,NO,txt);
 aaStringReplaceString(txt,0,"&#64260;",8,"ffl",0,NO,txt);
 aaStringReplaceString(txt,0,"&quot;",0,"\"",0,NO,txt);
 if(donbsp) { aaStringReplaceString(txt,0,"&nbsp;",0," ",0,NO,txt); }
 aaStringReplaceString(txt,0,"&#729;",0,".",0,NO,txt);
 aaStringReplaceChar(txt,0,0xa0,0x20);
 }





 B curlRorgRead                        (_htmlparser*html,H rindex,H tindex,H count,H max,_htmlrorg*rorgs)
 {
 B ret;
 H can;
 H i,c;

 if(rindex>=html->row_count) { return RET_BOUNDS; }
 i=0;
 c=0;
 while(1)
  {
  can=html->row_count-rindex;
  can=aaNumRoof(can,max);
  if(tindex>=can) { return RET_NOMEMORY; }
  if((ret=aaHtmlParserRead(html,rindex+tindex+i,&rorgs[tindex+i]))!=YES) { oops; return ret; }
  i++;
  c++;
  if(c>=count) { break; }
  }
 if(c>1) aaDebugf("readed %i",c);
 return RET_YES;
 }


/*-----------------------------------------------------------------------*/



 B browserNew                          (_browser*browser,VP cookiefile,H extrabytes,VP fmt,...)
 {
 B ret;

 if(browser==NULL)     { return RET_MISSINGPARM; }
 aaMemoryFill(browser,sizeof(_browser),0);
 browser->magic=aaHPP(browserNew);
 aaVargsf4K(fmt);
 aaStringCopyf(browser->arg_url,"%s",str4k.buf);
 if(cookiefile) { aaStringCopyf(browser->cookie_file,cookiefile); }
 browser->extra_bytes=extrabytes;
 if(browser->extra_bytes)
  {
  if((ret=aaMemoryAllocate((VP)&browser->extra_data,browser->extra_bytes))!=YES) { oops; }
  }
 browser->stage=1000;
 browser->call_index=F32;
 return RET_YES;
 }




 B browserDelete                       (_browser*browser)
 {
 if(browser==NULL) { return RET_MISSINGPARM; }
 if(browser->magic!=aaHPP(browserNew)) { return RET_NOTINITIALIZED; }
 if(browser->call_index!=F32) {  curlyCallDelete(&kerly,browser->call_index);  }
 aaHtmlParserDelete(&browser->html);
 if(browser->extra_bytes) { aaMemoryRelease(browser->extra_data); }
 aaMemoryFill(browser,sizeof(_browser),0);
 return RET_YES;
 }




 B browserYield                        (_browser*browser)
 {
 B ret;
 H go;
 B call_url[_1K];
 B call_cmd[_1K];
 H call_index;
 _curlycall*ccall;

 if(browser==NULL) { return RET_MISSINGPARM; }
 if(browser->magic!=aaHPP(browserNew)) { return RET_NOTINITIALIZED; }
 go=25;
 while(--go)
  {
  switch(browser->stage)
   {
   case 1000:
   ////aaFileDelete("cookie40.txt");
   aaStringCopyf(call_url,"%s",browser->arg_url);
   aaStringReplaceString(call_url,0," ",1,"+",1,NO,0);
   //ezlog,"%s",call_url);
   aaStringNull(call_cmd);
   aaStringAppendf(call_cmd,"meth=GET\n");
   aaStringAppendf(call_cmd,"ua=Mozilla/5.0 (Linux; Android 6.0; Nexus 5 Build/MRA58N) AppleWebKit/537.37 (KHTML, like Gecko) Chrome/68.0.3440.106 Mobile Safari/537.37\n");
   if(browser->cookie_file[0]) {   aaStringAppendf(call_cmd,"cookie=%s\n",browser->cookie_file); }
   aaStringAppendf(call_cmd,"url=%s\n\n",call_url);
   if((ret=curlyCallNew(&kerly,&call_index,&ccall,0,"%s",call_cmd))!=RET_YES) { oops; }
   browser->is_html=NO;
   browser->is_redirect=NO;
   browser->http_code=0;
   browser->call_index=call_index;
   browser->stage=1200;
   break;



   case 1200:
   call_index=browser->call_index;
   if((ret=curlyCallWait(&kerly,call_index,&ccall))==RET_NOTREADY) { break; }
   browser->http_code=(H)ccall->http_code;
   aaStringNull(browser->location);
   // aaStringCopy(browser->location,ccall->url);

   if(ccall->location[0])
    {
    aaStringCopyf(browser->location,"%s",ccall->location);
    //mbLog("loc=%s",browser->location);
    browser->is_redirect=YES;
    }
   if(ret!=RET_YES)
    {
    if(ccall->result!=28&&ccall->result!=0)
     {
     mbLog("res=%i str=%s",ccall->result,ccall->result_str);
     ///mbLog("url=%s",ccall->url);
     }
    if(browser->http_code!=0)
     {
   //  mbLog("!http=%i",browser->http_code);
     }
    //??? dont i need to delete call lhere , before going to stage 666
    browser->stage=666;
    break;
    }
   if(aaStringIsIString(ccall->res_content_type,0,"text/html",0)==YES)
    {
    if((ret=aaHtmlParserEzy(&browser->html,ccall->mun.used,ccall->mun.mem))!=RET_YES) { oops; }
    browser->is_html=YES;
    }
   else
    {
    mbLog("type=%s",ccall->res_content_type);
    }
   ///browserParse(browser);
   browser->stage=1400;
   break;



   case 1400:
   //call_index=browser->call_index;
   //curlyCallDelete(&kerly,call_index);
   //browser->call_index=F32;
   browser->stage=9999;
   break;


   case 666:
   //mbLog("err");
   return RET_FAILED;


   case 9999:
   return RET_FINISHED;
   }
  }

 return RET_YES;
 }






 B browserRetry                        (_browser*browser,B keepextra,VP fmt,...)
 {
 B ret;
 B txt[_1K];
 H eb;
 BP ed;
 B cookie_file[_1K];

 if(browser==NULL) { return RET_MISSINGPARM; }
 if(browser->magic!=aaHPP(browserNew)) { return RET_NOTINITIALIZED; }
 aaVargsf4K(fmt);
 aaStringCopy(txt,browser->arg_url);
 if(str4k.len)
  {
  aaStringCopyf(txt,"%s",str4k.buf);
  }
 ed=NULL;
 eb=browser->extra_bytes;
 if(keepextra)
  {
  if(browser->extra_bytes)
   {
   if((ret=aaMemoryAllocate((VP)&ed,eb))!=YES) { oops; }
   }
  }
 aaStringCopy(cookie_file,browser->cookie_file);
 browserDelete(browser);
 if(cookie_file[0])
  {
  if((ret=browserNew(browser,cookie_file,eb,"%s",txt))!=YES) { oops; }
  }
 else
  {
  if((ret=browserNew(browser,NULL,eb,"%s",txt))!=YES) { oops; }
  }
 if(ed)
  {
  aaMemoryCopy(browser->extra_data,eb,ed);
  aaMemoryRelease(ed);
  }
 return RET_YES;
 }






 B browserParse                        (_browser*browser)
 {
 B ret;
 H call_index;
 _curlycall*ccall;
 _htmlrorg rorg;
 H state,ii;
 B text[_16K];
 //B digstr[_1K];
 //B etc[_2K];
 //H count,val,lc;

 if(browser==NULL) { return RET_MISSINGPARM; }
 if(browser->magic!=aaHPP(browserNew)) { return RET_NOTINITIALIZED; }
 call_index=browser->call_index;
 if((ret=curlyCallWait(&kerly,call_index,&ccall))!=RET_YES) { oops; }
 state=0;
 for(ii=0;ii<browser->html.row_count;ii++)
  {
  if(aaHtmlParserRead(&browser->html,ii,&rorg)!=YES) { aaNote(0,"line=%i",__LINE__);  }
  if(rorg.row.chars>12000) { continue; }
  switch(state)
   {
   case 0:
   if(rorg.row.type==HTML_ROW_TYPE_COMMENT)  { break; }
   //if(rorg.row.type==HTML_ROW_TYPE_CSS)      { break; }
   if(rorg.row.type==HTML_ROW_TYPE_JS)       { break; }
   if(rorg.row.type==HTML_ROW_TYPE_HTML)
    {
    if(aaStringICompare(rorg.arg.tag,"script",0)==YES) { break; }
    }

   //if(rorg.row.type!=HTML_ROW_TYPE_HTML&&rorg.row.type!=HTML_ROW_TYPE_TEXT) { break; }
   aaStringNCopy(text,rorg.ptr,rorg.row.chars,YES);
   if(rorg.row.type==HTML_ROW_TYPE_TEXT)
    {
    aaStringFix(text,YES,YES);
    }
   //aaLog(-555,"%s",text);
   break;
   }
  }
 return RET_YES;
 }






 V browserDemo                         (V)
 {
 B ret;
 H i,c,li;
 //B txt[_2K];
 _browser bro[1];
 _htmlrorg rorg[10];
 B lin[_32K];
 H r,phaze;

 while(mbYield()==YES)
  {
  switch(aa_stage)
   {
   case 0:
   //ch=the_main.canvas.handle;
   aaFileDelete("cookie44.txt");
   aaStageSet(200);
   break;

   case 200:
   i=0;
   if((ret=browserNew(&bro[i],"cookie44.txt",_1K,"%s","https://www.bibliatodo.com/en/the-bible/king-james-version/job-1"))!=YES) { oops; }
 //  if((ret=browserNew(&bro[i],"cookie44.txt",_1K,"%s","https://www.bibliatodo.com/en/the-bible/new-world-translation-of-the-holy-scriptures/job-1"))!=YES) { oops; }
   aaStageSet(210);
   break;

   case 210:
   c=0;
   for(i=0;i<1;i++)
    {
    browserYield(&bro[i]);
    if(bro[i].stage==666)
     {
     ///mbLog("%i=err, http=%i loc=%s",i,bro[i].http_code,bro[i].location);
     if(bro[i].is_redirect) {   browserRetry(&bro[i],1,"%s",bro[i].location); }
     else                   {   browserRetry(&bro[i],1,0);          }
     break;
     }
    if(bro[i].stage!=9999) {  continue; }
    c++;
    }
   //ezlabel(100),"%i",c);
   if(c!=1) { break; }
   for(i=0;i<1;i++)
    {
    if(bro[i].is_html)
     {
     if((ret=browserParse(&bro[i]))!=YES) { oops; }
     }
    phaze=0;
    li=0;
    while(1)
     {
     if(li>=bro[i].html.row_count) { break; }
     if(phaze==F32)                { break; }
     switch(phaze)
      {
      case 0:
      r=0;
      if((ret=curlRorgRead(&bro[i].html,li,0,1,aaElementCount(rorg),&rorg))!=YES) { oops; }
      if(rorg[r].row.type!=HTML_ROW_TYPE_HTML) { li++; break; }
      if(rorg[r].row.flag!=HTML_ROW_FLAG_OPEN) { li++; break; }
      if(aaStringICompare(rorg[r].arg.tag,"div",0)!=YES) { li++; break; }
      if(aaStringICompare(rorg[r].arg.id,"imprimible",0)!=YES) { li++; break; }
      li++;
      phaze=10;
      break;

      case 10:
      r=0;
      if((ret=curlRorgRead(&bro[i].html,li,0,1,aaElementCount(rorg),&rorg))!=YES) { oops; }
      if(rorg[r].row.type==HTML_ROW_TYPE_HTML&&rorg[r].row.flag==HTML_ROW_FLAG_OPEN)
       {
       if(aaStringICompare(rorg[r].arg.tag,"p",0)==YES) { li++;  break; }
//       if(aaStringICompare(rorg[r].arg.tag,"strong",0)==YES) { li++;  break; }
       if(aaStringICompare(rorg[r].arg.tag,"h4",0)==YES) { li++;  break; }
       if(aaStringICompare(rorg[r].arg.tag,"h5",0)==YES) { li++;  break; }
       aaStringCopy(lin,rorg[r].ptr);
       mbLog("%s",lin);
       li+=1;
       break;
       }
      if(rorg[r].row.type==HTML_ROW_TYPE_HTML&&rorg[r].row.flag==HTML_ROW_FLAG_CLOSE)
       {
       if(aaStringICompare(rorg[r].arg.tag,"p",0)==YES) { li++;  break; }
  //     if(aaStringICompare(rorg[r].arg.tag,"strong",0)==YES) { li++;  break; }
       if(aaStringICompare(rorg[r].arg.tag,"h4",0)==YES) { li++;  break; }
       if(aaStringICompare(rorg[r].arg.tag,"h5",0)==YES) { li++;  break; }

       aaStringCopy(lin,rorg[r].ptr);
       if(aaStringICompare(rorg[r].arg.tag,"div",0)==YES) { li++; phaze=20; break; }
       mbLog("%s",lin);
       li+=1;
       break;
       }
      if(rorg[r].row.type==HTML_ROW_TYPE_HTML&&rorg[r].row.flag==HTML_ROW_FLAG_SELF)
       {
       aaStringCopy(lin,rorg[r].ptr);
       mbLog("%s",lin);
       li+=1;
       break;
       }


      if(rorg[r].row.type==HTML_ROW_TYPE_TEXT)
       {
       aaStringCopy(lin,rorg[r].ptr);
       mbLog("%s",lin);
       li+=1;
       break;
       }

      li+=1;
      break;

      case 20:
      li++;
      break;
      }
     }
    browserDelete(&bro[i]);
    }
   aaStageSet(220);
   break;
   }
  }
 }






