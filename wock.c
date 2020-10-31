/*-----------------------------------------------------------------------*/
 #include "mb.h"
/*-----------------------------------------------------------------------*/






 B websocketPktDump                    (_tcpcallunit*call,B oc,B ff,H bytes,VP data)
 {
 B out[_32K];
 B txt[_8K];
 H done,todo;
 BP bp;


 mbLog("packet dump oc = 0x%02x ff=0x%02x bytes=%i",oc,ff,bytes); return RET_YES;

 aaNetTcpCallStatus(call->handle,&call->status);
 bp=(BP)data;
 aaStringNull(out);
 aaStringAppendf(out,"ff=%i ",ff);
 switch(oc)
  {
  default:                       aaStringAppendf(out,"oc=%02x ",oc);  break;
  case WEBSOCKET_OPCODE_TEXT:    aaStringAppendf(out,"oc=TXT ");  break;
  case WEBSOCKET_OPCODE_BINARY:  aaStringAppendf(out,"oc=BIN ");  break;
  case WEBSOCKET_OPCODE_CLOSE:   aaStringAppendf(out,"oc=CLS ");  break;
  case WEBSOCKET_OPCODE_PING:    aaStringAppendf(out,"oc=PIN ");  break;
  case WEBSOCKET_OPCODE_PONG:    aaStringAppendf(out,"oc=PON ");  break;
  }
 aaStringAppendf(out,"by=%-5i ",bytes);
 done=0;
 aaStringAppendf(out,"rm: %-23s ",call->status.remote_dot);
 if(bytes<=32)
  {
  aaStringDumpFormat(txt,bytes,bp);
  aaStringAppendf(out,"%s ",txt);
  bp[bytes]=NULL_CHAR;
  mbLog("%s (%s)",out,bp);
  return RET_YES;
  }
 while(1)
  {
  todo=bytes-done;
  if(todo==0) { break; }
  todo=aaNumRoof(todo,32);
  aaStringDumpFormat(txt,todo,&bp[done]);
  aaStringAppendf(out,"%s ",txt);
  mbLog("%s",out);
  done+=todo;
  aaStringNull(out);
  aaStringAppendChars(out,SPACE_CHAR,48,YES);
  }
 return RET_YES;
 }







 B websocketPinger                     (_tcpcallunit*call)
 {
 Q ms;
 _servercalldata*cd;
 B etc[_2K];

 if(call==NULL)      { return RET_MISSINGPARM; }
 if(call->handle==0) { return RET_FAILED; }
 cd=(_servercalldata*)call->status.extra_data;;
 if(cd==NULL)              { oof; }
 if(cd->wock.is_open!=YES) { return RET_NOTREADY; }
 ms=aaMsRunning();
 if((ms-cd->wock.ping_last_ms)>=5000)
  {
  etc[0]='a';
  etc[1]=NULL_CHAR;
  aaNetWebsocketWrite(&cd->wock,WEBSOCKET_OPCODE_PING,YES,2,etc);
  cd->wock.ping_last_ms=aaMsRunning();
  }
 return RET_YES;
 }





 B websocketPonger                     (_tcpcallunit*call,H bytes,VP data)
 {
 B ret;
 _servercalldata*cd;

 if(call==NULL)      { return RET_MISSINGPARM; }
 if(call->handle==0) { return RET_FAILED; }
 cd=(_servercalldata*)call->status.extra_data;;
 if(cd==NULL)              { oof; }
 if(cd->wock.is_open!=YES) { return RET_NOTREADY; }
 if((ret=aaNetWebsocketWrite(&cd->wock,WEBSOCKET_OPCODE_PONG,YES,bytes,data))!=YES) { oops; }
 cd->wock.ping_last_ms=aaMsRunning();
 return RET_YES;
 }







 B websocketReader                     (_tcpcallunit*call,BP oc,BP ff,HP bytes,H maxbytes,VP data)
 {
 B ret;
 _servercalldata*cd;
 BP bp;

 if(call==NULL)      { return RET_MISSINGPARM; }
 if(call->handle==0) { return RET_FAILED; }
 if(oc==NULL)    { return RET_MISSINGPARM; }
 if(ff==NULL)    { return RET_MISSINGPARM; }
 if(bytes==NULL) { return RET_MISSINGPARM; }
 if(data==NULL)  { return RET_MISSINGPARM; }
 cd=(_servercalldata*)call->status.extra_data;;
 if(cd==NULL)              { oof; }
 if(cd->wock.is_open!=YES) { return RET_NOTREADY; }
 if((ret=aaNetWebsocketRead(&cd->wock,oc,ff,bytes,maxbytes,data))==RET_NOTREADY)
  {
  if(((call->status.rcve_bytes==0&&call->status.xmit_bytes==0)||(call->status.closed_ms>=aaSecs(3)))&&(call->status.is_closed))
   {
   if(cd->do_close==0)
    {
    ///mbLog("%s rb=%i xb=%i cms=%i ic=%i",__func__,call->status.rcve_bytes,call->status.xmit_bytes,call->status.closed_ms,call->status.is_closed);
    cd->do_close=1;
    }
   }
  return ret;
  }
 bp=(BP)data;
 bp[*bytes]=NULL_CHAR;
 return RET_YES;
 }





/*-----------------------------------------------------------------------*/



