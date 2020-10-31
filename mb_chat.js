

 //var audio;
 var audio_clicked=false;
 var audio = new Audio('chimein.wav');
//audio.play();

 var chat_input_text="";

 var chat_history_array=[];
 var chat_history_slots=0;
 var chat_history_count=0;

/*-----------------------------------------------------------------------*/

 var hit_counter=[];
 var hit_name=[];

/*-----------------------------------------------------------------------*/

 var keyboard_spot_array=[];
 var keyboard_spot_count=0;
 var keyboard_page=0;
 var keyboard_shift=0;
 var keyboard_painted=0;

 var global_click_count=0;
 var global_click_last="";
 var global_miss_count=0;
 var global_miss_last="";

 ///var history_from=0;
 var history_fine=0;
 var history_height=0;
 var history_change=false;

 var user_string0="";
 var user_string1="";
 var user_string2="";
 var user_string3="";
 var user_string5="";

/*-----------------------------------------------------------------------*/


 var finger=[];

 var rep_last_timestamp=0;
 var rep_last_keycode=0;
 var rep_last_type=0;

 var pre_send=0;
 var old_stage=-1;

 var is_scrolling=false;
 var scrolling_dir="none";
 var scroll_speed=26;


/*-----------------------------------------------------------------------*/



 function hitIt(linenum,name) {
 //---------------------------------------
 if(hit_counter[linenum]==0)  {  console.log("================== "+name);  }
 hit_counter[linenum]++;
 hit_name[linenum]=name;
 }



 function hitCalc() { var tot,cnt,i,avg,hat,add;
 //--------------------------------------------
 tot=0;
 cnt=0;
 for(i=0;i<hit_counter.length;i++) { if(hit_counter[i]!=0) { tot+=hit_counter[i]; cnt++; } }
 avg=parseInt(tot/cnt);
 console.log(" ");
 console.log("===HITS=== "+cnt+" counters, "+tot+" tot, "+avg+" avg =====");
 for(i=0;i<hit_counter.length;i++)
  {
  if(hit_counter[i]<=0)      { continue; }
  //if(hit_counter[i]<(avg/6)) { continue; }
  hat="";
  hat+=hit_name[i]+":"+i+"       ";
  hat+=hit_counter[i]+" hits  ";
  hat+=num.Fixed(num.PercentIs(hit_counter[i],tot),2)+"%";
  console.log(hat);
  }
 console.log(" ");
 }


/*-----------------------------------------------------------------------*/




 function ChatMain() { var i,kv,part,ch;
 //----------------------------------------
 LoggerLevelSet(100);
 main.Start(207,10,ChatMainProc,false);
 main.cfg={};
 main.app={};
 main.vars.my_info={};
 main.vars.my_info.alias=null;
 main.vars.my_info.room=null;
 for(i=0;i<env.browser_args.length;i++)
  {
  if(text.IndexOf(env.browser_args[i].key,"alias")<0) { continue; }
  main.vars.my_info.alias=env.browser_args[i].val;
  break;
  }
 if(env.browser_pathpart[0]==""&&env.browser_pathpart[1]=="chat")
  {
  main.vars.my_info.room=env.browser_pathpart[2];
  ch=text.GetLastChar(main.vars.my_info.room);
  if(ch=='/') { main.vars.my_info.room=text.TrimLastChar(main.vars.my_info.room);   }
  }
 if(IsEmpty(main.vars.my_info.alias)) { main.vars.my_info.alias="joe";       }
 if(IsEmpty(main.vars.my_info.room))  { main.vars.my_info.room="chatlobby";  }
 Logger(70,"%cALIAS="+main.vars.my_info.alias+" %cROOM="+main.vars.my_info.room,"color:blue","color:red");
 ChatHistoryInit(100);
 for(var i=0;i<10000;i++) { hit_counter[i]=0; hit_name[i]=""; }
 ChatUxBegin();
 main.Run();
 }



/*-----------------------------------------------------------------------*/


 function ChatMainProc() { var i,vi,el; var avc,vcfg; var vid,isplaying; var wobj; var mm; var kb,cnt; var loops,ok;
 //----------------------------------------------
 hitIt(LineNumber(),arguments.callee.name);
 ///if((main.vars.cycle%200)==50) { hitCalc(); }
 //------------------------------
 gui.UpdateRequest();
 ChatUxUpdate();
 remlog.Engine();


 if(main.vars.wock_status)
  {
  if(main.vars.stage!=8188)
   {
   if(main.vars.wock_status.is_closed==true) { env.BrowserReload(true,1000+num.Rand(1000)); console.log("closed");   main.StageSet(8188); }
   if(main.vars.wock_status.is_error==true) { env.BrowserReload(true,1000+num.Rand(1000));  console.log("error");  main.StageSet(8188); }
   }
  }




 switch(main.vars.stage)
  {
  case 0:
  ChatUxAnimator();
  main.StageSet(100);
  break;

  case 100:
  //remlog.Open();
  main.vars.local_av_handle=0;
  main.vars.local_av_object=null;
  main.vars.local_av_stream=null;
  Logger(40,"device detection begining");
  devices.Detect();
  main.StageSet(110);
  break;


  case 110:
  if(remlog.vars.is_open==true&&remlog.vars.is_ready!=true) { break; }
  main.StageSet(140);
  break;



  case 140:
  if(devices.state.is_detecting==true) { break; }
  if(devices.state.is_detected!=true)  { break; }
  if(devices.state.is_error==true)
   {
   Oof(devices.state.error_name+"  "+devices.state.error_message);
   main.StageSet(666);
   break;
   }
  console.log("detected in "+devices.state.elapsed+"ms, "+devices.state.audio_inp_count+" "+devices.state.video_inp_count+" "+devices.state.audio_out_count+" "+devices.state.video_out_count);
  main.StageSet(200);
  break;




  case 200:
  vi=0;
  for(i=0;i<env.browser_args.length;i++)
   {
   if(env.browser_args[i].key=="vi") { vi=env.browser_args[i].val; }
   }
  vi%=devices.state.video_inp_count;
  vcfg=devices.VideoConfig(vi);
  avc=
   {
   //video:{ deviceId:{exact:vcfg.deviceId}, width: { min: 640, ideal: 640, max: 640}, height: { min: 480, ideal: 480, max: 480 }, frameRate:30},
   video:{ deviceId:{exact:vcfg.deviceId}, width: { min: 320, ideal: 320, max: 640}, height: { min: 240, ideal: 240, max: 480 }, frameRate:30},
   audio:false
   };
  main.vars.local_av_handle=av.Create(null);
  main.vars.local_av_object=av.Get(main.vars.local_av_handle);
  av.GetMedia(main.vars.local_av_handle,avc);
  main.StageSet(250);
  break;




  case 250:
  if(main.vars.local_av_object.media_stream==null) { break; }
  if(main.vars.local_av_object.media_stream==-1)   { Logger(40,"getmedia error "+main.vars.local_av_object.media_emessage+"  "+main.vars.local_av_object.media_ename); main.StageSet(200);  Oof("getmedia error "+main.vars.local_av_object.media_emessage+"  "+main.vars.local_av_object.media_ename); break; }
  main.vars.local_av_stream=main.vars.local_av_object.media_stream;
  if(gui.Get(gui.IdFind("selfvid"))!=null)
   {
   gui.Get(gui.IdFind("selfvid")).dom.srcObject=main.vars.local_av_stream;
   gui.Get(gui.IdFind("selfvid")).dom.muted=true;
   gui.Get(gui.IdFind("selfvid")).dom.volume=0.1;
   vid=gui.Get(gui.IdFind("selfvid")).dom;
   isplaying=vid.currentTime>0&&!vid.paused&&!vid.ended&&vid.readyState>2;
   if(!isplaying)    {    gui.Get(gui.IdFind("selfvid")).dom.play();    }
   }
  user_string0=" "+main.vars.local_av_stream.getVideoTracks()[0].getSettings().width+" "+main.vars.local_av_stream.getVideoTracks()[0].getSettings().height+" "+main.vars.local_av_stream.getVideoTracks()[0].getSettings().frameRate+" "+num.Round(main.vars.local_av_stream.getVideoTracks()[0].getSettings().aspectRatio,4);
  main.StageSet(500);
  break;



  case 500:
  main.vars.room_handle=room.Create(main.vars.my_info.room,max_windows);
  main.vars.room_object=room.Get(main.vars.room_handle);
  main.StageSet(510);
  break;




  case 510:
  Logger(40,"connecting to room %c"+main.vars.room_object.name,"color:crimson");
  main.vars.wock_handle=wock.Call("wss://mebeam.com:443/wss/"+main.vars.room_object.name);
  main.vars.wock_object=wock.Get(main.vars.wock_handle);
  main.vars.wock_status=wock.Status(main.vars.wock_handle);
  wock.DirectSet(main.vars.wock_handle,true);
  main.StageSet(600);
  break;



  case 600:
  main.vars.wock_status=wock.Status(main.vars.wock_handle);
  if(main.vars.wock_status.is_open!=true) { break; }
  if(main.vars.wock_status.is_closed==true) { Oof(); }
  if(main.vars.wock_status.is_error==true) { Oof(); }
  Logger(40,"connected!");
  remlog.Write("","connected");
  wock.Write(main.vars.wock_handle,JSON.stringify({"cmd":"hello"}));
  main.StageSet(700);
  break;


  case 700:
  main.vars.wock_status=wock.Status(main.vars.wock_handle);
  ChatRoomProcess(main.vars.room_handle,main.vars.wock_handle);
  if(main.vars.room_object.my_uuid==null) { break; }
  //main.state.speed=100;
 //wock.Write(main.vars.wock_handle,JSON.stringify({"cmd":"chatMsgReplay"}));
 // msg={"cmd":"chatMsgEcho","ali":main.vars.my_info.alias,"img":data,"txt":txt};
  //remlog.Write("",main.vars.room_object.my_hancock+"> connected , sendind cmd hello");
  //remlog.AliasSet(main.vars.room_object.my_hancock);
  main.StageSet(777);
  break;


  case 777:
  main.vars.wock_status=wock.Status(main.vars.wock_handle);
  /*
  if(main.vars.wock_status.is_closed==true)
   {
   env.BrowserReload(true,1000+num.Rand(1000));
   main.StageSet(8188);
   break;
   }
  */

  ChatRoomProcess(main.vars.room_handle,main.vars.wock_handle);

  if(main.vars.room_object.peer_count==0) { pre_send=0; }

  if(main.vars.room_object.peer_count>=1)
   {
   if((env.browser_args[0].val)==1)
    {
    if((main.vars.cycle%35)==0&&pre_send<10  )
     {
    // ChatRoomSend("jpg",0.7,pre_send+"  "+timer.MsRunning());  pre_send++;
     }
    }
   //if(pre_send<3  )   {   ChatRoomSend("jpg",0.7,"ps="+pre_send+" Automation: "+timer.MsRunning());  pre_send++; }
   //if((main.vars.cycle%20)==0&&pre_send<4  )   {   ChatRoomSend("jpg",0.7,"ps="+pre_send+" Automation: "+timer.MsRunning());  pre_send++; }
   }


 loops=0;
 while(1)
  {
  if(loops>=8) { break; }
  if((kb=keyboard.Read())==null) { break; }
  loops++;
   ok=false;
   if(kb.name=="keydown")
    {
    if(kb.key=="End")
     {
     history_fine=0;
     is_scrolling=true;
     scrolling_dir="none";
     }
    else
    if(kb.key=="Home")
     {
     is_scrolling=true;
     scrolling_dir="none";
     history_fine=((history_height-1)*main.app.rcve_vgap)
     }
    else
    if(kb.key=="ArrowUp")
     {
     history_fine+=26;
     is_scrolling=true;
     scrolling_dir="none";
     if(history_fine>=((history_height-1)*main.app.rcve_vgap)) { history_fine=((history_height-1)*main.app.rcve_vgap); }
     }
    else
    if(kb.key=="ArrowDown")
     {
     history_fine-=26;
     is_scrolling=true;
     scrolling_dir="none";
     if(history_fine<=0) { history_fine=0; }
     }
    else
    if(kb.key==" ")
     {
     //ChatRoomSend("jpg",0.7,"ms="+timer.MsRunning());
     ok=true;
     }
    else
    if(kb.key=="Tab")
     {
     }
    else
     {
     ok=true;
//     console.log(kb);
          //ChatRoomSend("jpg",0.7,pre_send+"    Automation: "+timer.MsRunning());  pre_send++;
     }
    }
   //if(kb.name=="keydown")   {    ChatUxKeyEvent(kb);    }
   if(ok==true)   {    ChatUxKeyEvent(kb);    }
   }

  if((peer=room.PeerNext(main.vars.room_handle))==null) { break; }
  switch(peer.phaze)
     {
     case 0:    peer.phaze=50;      break;
     case 50:   peer.phaze=100;     break;
     }
  break;
  }



 //ChatUxUpdate();
 }





/*-----------------------------------------------------------------------*/






 function ChatRoomProcess(roomhandle,wockhandle) { var wobj,robj,msg,i,peer,ix; var is_me;
 //----------------------------------------------
 hitIt(LineNumber(),arguments.callee.name);
 if((wobj=wock.Get(wockhandle))==null) { Oof; }
 if((robj=room.Get(roomhandle))==null) { Oof; }
 if((msg=wock.Read(wockhandle))==null) { return false; }
 msg=JSON.parse(msg);
 Logger(40,"got "+msg.cmd);

 if(robj.my_uuid==null)
  {
  if(msg.cmd=="Full"||msg.cmd!="welcome") {  return false; }
  room.MySet(roomhandle,msg.uuid,msg.hancock);
  for(i=0;i<msg.peerCount;i++)
   {
   if(msg.peerList[i].uuid==robj.my_uuid)  { continue; }
   if((peer=room.PeerAdd(roomhandle,true,msg.peerList[i].uuid,msg.peerList[i].hancock))==null) { Oof(""); }
   }
  Logger(40,"Hi %c"+msg.hancock+" %c"+msg.uuid+" %cpc="+robj.peer_count,"color:red","color:blue","color:green");
  history_change=true;
  gui.UpdateRequest();
  return true;
  }

 if(msg.uuid==robj.my_uuid) { is_me=true;  }
 else                       { is_me=false; }

 switch(msg.cmd)
  {
  default:
  Oof(msg.cmd);
  break;

  case "userJoined":
  if(is_me==true) { return false; }
  main.state.speed=100;
  peer=room.PeerAdd(roomhandle,true,msg.uuid,msg.hancock);
  Logger(40,"peer joined "+peer.uuid+" "+peer.hancock+" peercount="+robj.peer_count);
  history_change=true;
  return true;


  case "userLeft":
  if(is_me==true) { return false; }
  if((peer=room.PeerFind(roomhandle,msg.uuid))==null) { break; }
  Logger(40,"peer left "+peer.uuid+" "+peer.hancock);
  peer=room.PeerRemove(roomhandle,msg.uuid,msg.hancock);
  Logger(40,"peercount="+robj.peer_count);
  history_change=true;
  return true;




  case "chatecho":
//  console.log(msg);
  if(msg.txt=="Nukem"||msg.txt=="nukem")
   {
   if(msg.hancock==msg.thancock&&is_me==true)
    {
    Logger(40,"received Nukem which was sent my me");
    env.BrowserReload(true,1000+num.Rand(1000));
    break;
    }
   Logger(30,"from "+msg.hancock+" to "+msg.thancock);
   env.BrowserReload(true,500+num.Rand(500));
   main.StageSet(58);
   break;
   }
  if(msg.txt.substring(0,19)=="automated.gday.ping")   {   break;   }
  if(msg.img.substring(0,5)=="data:")
   {
   //console.log(msg.gms);
      ChatHistoryAppend(msg);
      }
  history_change=true;
  break;
  }

 gui.UpdateRequest();
 return false;
 }





/*-----------------------------------------------------------------------*/




 function ChatRoomSend(type,qul,txt) { var han,obj,ctx,data; var sid,pid,msg; var cw,ch,vw,vh,th;
 //-----------------------------------------
 hitIt(LineNumber(),arguments.callee.name);
 if(main.vars.stage<777) { return; }
 han=gui.IdFind("selfvid");
 obj=gui.Get(han);
 sid=obj.dom;
 han=gui.IdFind("photcan");
 obj=gui.Get(han);
 ctx=gui.CanvasContextGet(han);
 pid=obj.dom;
 //sid=document.getElementById("selfvid");
 //pid=document.getElementById("photcan");
 cw=pid.width;
 ch=pid.height;
 vw=sid.videoWidth;
 vh=sid.videoHeight;
// console.log(vw+" "+sid.width+" "+pid.width);
 if(sid!=null&&pid!=null)
  {
  ///if(audio_clicked==true) {        audio.play(); }
  ctx.save();
  if(cw/ch<vw/vh){ th=cw*vh/vw; ctx.drawImage(sid, 0, 0, vw, vh, 0, (ch-th)/2, cw, th);  }
  else           { tw=ch*vw/vh; ctx.drawImage(sid, 0, 0, vw, vh, (cw-tw)/2, 0, tw, ch);  }
  if(type=="jpg"||type=="jpeg") { data=obj.dom.toDataURL('image/jpeg',qul);  } else
  if(type=="png")               { data=obj.dom.toDataURL('image/png');       }
  ctx.restore();
  msg={"cmd":"chatMsgEcho","ali":main.vars.my_info.alias,"img":data,"txt":txt};
  //console.log(JSON.stringify(msg).length);
  wock.Write(main.vars.wock_handle,JSON.stringify(msg));
  }
 //console.log("sent "+txt);
 audio_clicked=true;
 return true;
 }




/*-----------------------------------------------------------------------*/




 function ChatHistoryInit(slots) { var i,s,entry;
 //------------------------------
 hitIt(LineNumber(),arguments.callee.name);
 if(slots<=0) { Oof(slots); }
 chat_history_array=[];
 chat_history_slots=0;
 chat_history_count=0;
 for(i=0;i<slots;i++)
  {
  entry={};
  entry.self_index=i;
  entry.msg=null;
  entry.cha_id="cha"+i;
  entry.img=new Image();
  entry.img.width=320; // was 320,240
  entry.img.height=240;
  entry.img.id=entry.cha_id;
  entry.img.setAttribute("id",entry.img.id);
  entry.img.src=null;
  entry.img.style.position="absolute";
  entry.img.style.zIndex=8200;
  entry.img.style.width=320+"px"; // was 320,240
  entry.img.style.height=240+"px";
  entry.img.style.display="none";
  entry.txt=null;
  chat_history_array[chat_history_slots]=entry;
  document.body.appendChild(entry.img);
  chat_history_slots++;
  }
 if(chat_history_slots!=slots) { Oof(chat_history_slots+" "+slots); }
 //console.log("chat history init, "+chat_history_slots+" slots");
 return true;
 }



/*-----------------------------------------------------------------------*/



 function ChatHistoryAppend(msg) { var li,entry,a,che;
 //-------------------------------------------
 hitIt(LineNumber(),arguments.callee.name);
 li=chat_history_slots-1;
 ArrayRotate(chat_history_array);
 entry=chat_history_array[li];
 entry.msg=msg;
 entry.img.src=msg.img;
 entry.txt=msg.txt;
 //entry.txt=msg.gms%1000+"  "+msg.txt;
 chat_history_array[li]=entry;
 if(audio_clicked==true)  {  audio.play();  }
 }






/*-----------------------------------------------------------------------*/






 function DispGet() {
 //---------------------------------------
 hitIt(LineNumber(),arguments.callee.name);
 return(env.DisplayGet());
 }






 function CordSet(obj,x,y) {
 //---------------------------------------
 hitIt(LineNumber(),arguments.callee.name);
 if(typeof obj!=='object'&&obj!==null)
  {
  obj={};
  obj.x=obj.y=obj.w=obj.h=obj.x1=obj.y1=obj.x2=obj.y2=0;
  }
 obj.x=x;
 obj.y=y;
 obj.x1=obj.x;
 obj.y1=obj.y;
 obj.x2=(obj.x+obj.w)-1;
 obj.y2=(obj.y+obj.h)-1;
 return obj;
 }




 function SizeSet(obj,w,h) {
 //---------------------------------------
 hitIt(LineNumber(),arguments.callee.name);
 if(typeof obj!=='object'&&obj!==null)
  {
  obj={};
  obj.x=obj.y=obj.w=obj.h=obj.x1=obj.y1=obj.x2=obj.y2=0;
  }
 obj.w=w;
 obj.h=h;
 obj.x1=obj.x;
 obj.y1=obj.y;
 obj.x2=(obj.x+obj.w)-1;
 obj.y2=(obj.y+obj.h)-1;
 return obj;
 }



 function RectSet(obj,x,y,w,h) {
 //---------------------------------------
 hitIt(LineNumber(),arguments.callee.name);
 if(typeof obj!=='object'&&obj!==null)
  {
  obj={};
  obj.x=obj.y=obj.w=obj.h=obj.x1=obj.y1=obj.x2=obj.y2=0;
  }
 obj.x=x;
 obj.y=y;
 obj.w=w;
 obj.h=h;
 obj.x1=obj.x;
 obj.y1=obj.y;
 obj.x2=(obj.x+obj.w)-1;
 obj.y2=(obj.y+obj.h)-1;
 return obj;
 }







 function ChatUxElementCreate(type,id,wid,hit,zi) { var han;
 //-----------------------------------------
 hitIt(LineNumber(),arguments.callee.name);
 han=gui.Create(type);
 gui.IdSet(han,id,0);
 gui.WidthHeightSet(han,wid,hit);
 //gui.StyleWidthHeightSet(han,wid,hit);
 if(id=="maincan")
  {
//  gui.BackgroundOutlineSet(han,"#2398aa",null);//"#000000","4px solid #00aa77","0px");
  gui.BackgroundOutlineSet(han,"#73e8ff",null);//"#000000","4px solid #00aa77","0px");
  //gui.Get(han).dom.style.zIndex=zi;
  //gui.Get(han).dom.style.borderBottom="11px solid #000";
  }
 ///gui.BackgroundOutlineSet(han,"#000000","4px solid #00aa77","0px");
 gui.Get(han).dom.style.zIndex=zi;
 return han;
 }









 function ChatUxBegin() { var disp,han,obj; var ctx; var spot,s;
 //----------------------------------------------
 hitIt(LineNumber(),arguments.callee.name);

 disp=DispGet();
 main.vars.display_stats={};
 main.vars.display_stats.tik=0;
 main.vars.display_stats.elapsed=0;
 main.vars.display_stats.frame_count=0;
 main.vars.display_stats.fps=0;
 //-------------------------


 main.app.selfvid_size=SizeSet(main.app.selfvid_size,320,240);
 main.app.selfvid_area=RectSet(main.app.selfvid_area,0,0,320,240);
 main.app.photcan_size=SizeSet(main.app.photcan_size,320,240);  // final jpeg size
 main.app.kbkey_ystride=0.8;//0.5;
 main.app.kbkey_xstride=0.8;//0.5;
 main.app.kbkey_yshift=5;//138;

 main.app.rcve_area=RectSet(main.app.rcve_area,0,0,320,240); main.app.rcve_vgap=260;
 //main.app.rcve_area=RectSet(main.app.rcve_area,0,0,160,120); main.app.rcve_vgap=140;

 main.app.text_area=RectSet(main.app.text_area,0,0,320,240);
 //main.app.text_area=RectSet(main.app.text_area,0,0,160,120);
 main.app.scroll_canvas_height=chat_history_slots*main.app.rcve_vgap;

 ///console.log("sch="+main.app.scroll_canvas_height);
 history_height=0;
 history_change=false;
 ChatUxKeyboardInit();

 han=ChatUxElementCreate("video","selfvid",main.app.selfvid_size.w,main.app.selfvid_size.h,3100);
 gui.Get(han).dom.style.display="none";
 gui.Get(han).dom.style.opacity=1.0;
 gui.Get(han).dom.style.objectFit="contain";
 if(0)  {  gui.Get(han).dom.style.objectFit="fill";  gui.Get(han).dom.style.objectFit="cover";  gui.Get(han).dom.style.objectFit="none";  gui.Get(han).dom.style.objectFit="scale-down";  }
 gui.BackgroundOutlineSet(han,"#000000","4px solid #2089a0","0px");

 han=ChatUxElementCreate("canvas","maincan",disp.win_wid/2,disp.win_hit*1,3000);
 gui.Get(han).dom.style.display="none";
 //han=ChatUxElementCreate("canvas","scrlcan",disp.win_wid,history_slots*260,7000);


 han=ChatUxElementCreate("canvas","scrlcan",disp.win_wid,main.app.scroll_canvas_height,7000);
 gui.Get(han).dom.style.display="none";

 han=ChatUxElementCreate("canvas","photcan",main.app.photcan_size.w,main.app.photcan_size.h,3500);
 gui.Get(han).dom.style.display="none";
 //gui.Get(han).dom.width=640;
 //gui.Get(han).dom.height=480;

 han=ChatUxElementCreate("div","hudsdiv",400,200,9000);
 gui.Get(han).dom.style.display="none";
 gui.Get(han).dom.style.opacity=1.0;
 gui.BackgroundOutlineSet(han,"#ddddff","4px solid #00aa77","-2px");

 han=ChatUxElementCreate("div","keybdiv",300,300,4000);
 gui.Get(han).dom.style.display="none";
 gui.Get(han).dom.style.opacity=1.0;
 gui.Get(han).dom.style.borderTop="1px solid #000";
 //gui.BackgroundOutlineSet(han,"none","1px solid #ff0000","0px");
 gui.CanvasContextGet(gui.IdFind("maincan")).font="500 32px arial";
 ChatUxClickHandler();
 keyboard.Start();
 gui.UpdateRequest();

 return true;
 }




/*-----------------------------------------------------------------------*/





 function ChatUxKeyboardInit() { var s,spot;
 //---------------------------------------
 hitIt(LineNumber(),arguments.callee.name);
 keyboard_spot_count=0;
 for(s=0;s<100;s++)
  {
  spot={};
  spot.name="";
  spot.self_index=s;
  spot.keyCode=0;
  spot.key="";
  spot.ascii=0;
  spot.x1=0;
  spot.y1=0;
  spot.x2=0;
  spot.y2=0;
  spot.alt_key=false;
  spot.ctrl_key=false;
  spot.shift_key=false;
  keyboard_spot_array[keyboard_spot_count]=spot;
  keyboard_spot_count++;
  }
 keyboard_page=0;
 keyboard_shift=0;
 keyboard_painted=0;
 }







 function ChatUxKeyFromCord(clkx,clky) { var s,spot;
 //--------------------------------------------
 hitIt(LineNumber(),arguments.callee.name);
 for(s=0;s<keyboard_spot_count;s++)
  {
  spot=keyboard_spot_array[s];
  if(clkx>=(spot.x1-10)&&clkx<=(spot.x2+5))
   {
   if(clky>=(spot.y1-10)&&clky<=(spot.y2+5))
    {
    return spot;
    }
   }
  }
 return null;
 }





 function ChatUxKeyFromKeyCode(code) { var s,spot,c;
 //--------------------------------------------------
 hitIt(LineNumber(),arguments.callee.name);
 c=code;
 //console.log(keyboard_spot_count+" "+c+"  "+keyboard_page+" "+keyboard_shift);
 for(s=0;s<keyboard_spot_count;s++)
  {
  spot=keyboard_spot_array[s];
  if(spot.keyCode==code)
   {
   return spot;
   }
  }
 return null;
 }




 function ChatUxKeyFromAsciiCode(code) { var s,spot;
 //--------------------------------------------------
 hitIt(LineNumber(),arguments.callee.name);
 for(s=0;s<keyboard_spot_count;s++)
  {
  spot=keyboard_spot_array[s];
  if(spot.ascii==code) {  return spot;   }
  }
 return null;
 }





/*-----------------------------------------------------------------------*/





 function ChatUxTouchDecode(typ,eve)  { var obj; var k,i;
 //-----------------------------------------------
 hitIt(LineNumber(),arguments.callee.name);
 obj={};
 obj.type=typ;
 obj.eventName=eve.eventName;
 obj.eventPhase=eve.eventPhase;
  obj.altKey=eve.olde.altKey;
  obj.ctrlKey=eve.olde.ctrlKey;
  obj.what=eve.olde.type;
  obj.shiftKey=eve.olde.shiftKey;
  obj.timeStamp=eve.olde.timeStamp;
  obj.which=eve.olde.which;
  obj.srcElementId=eve.olde.srcElement.id;
  obj.targetId=eve.olde.target.id;
  if(eve.olde.x) { obj.x=eve.olde.x; }
  else           { obj.x=null;       }
  if(eve.olde.y) { obj.y=eve.olde.y; }
  else           { obj.y=null;       }

  obj.changedTouchesLen=0;
  obj.changedTouches=[];
  if(eve.olde.changedTouches)
   {
   obj.changedTouchesLen=eve.olde.changedTouches.length;
   k=obj.changedTouchesLen;
   for(i=0;i<3;i++)
    {
    if(i<k)
     {
     obj.changedTouches[i]={};
     obj.changedTouches[i].clientX=eve.olde.changedTouches[i].clientX;
     obj.changedTouches[i].clientY=eve.olde.changedTouches[i].clientY;
     obj.changedTouches[i].force=eve.olde.changedTouches[i].force;
     obj.changedTouches[i].identifier=eve.olde.changedTouches[i].identifier;
     obj.changedTouches[i].pageX=eve.olde.changedTouches[i].pageX;
     obj.changedTouches[i].pageY=eve.olde.changedTouches[i].pageY;
     obj.changedTouches[i].radiusX=eve.olde.changedTouches[i].radiusX;
     obj.changedTouches[i].radiusY=eve.olde.changedTouches[i].radiusY;
     obj.changedTouches[i].rotationAngle=eve.olde.changedTouches[i].rotationAngle;
     obj.changedTouches[i].screenX=eve.olde.changedTouches[i].screenX;
     obj.changedTouches[i].screenY=eve.olde.changedTouches[i].screenY;
     obj.changedTouches[i].targetId=eve.olde.changedTouches[i].targetId;
     }
    }
   }

  obj.targetTouchesLen=0;
  obj.targetTouches=[];
  if(eve.olde.targetTouches)
   {
   obj.targetTouchesLen=eve.olde.targetTouches.length;
   k=obj.targetTouchesLen;
   for(i=0;i<3;i++)
    {
    if(i<k)
     {
     obj.targetTouches[i]={};
     obj.targetTouches[i].clientX=eve.olde.targetTouches[i].clientX;
     obj.targetTouches[i].clientY=eve.olde.targetTouches[i].clientY;
     obj.targetTouches[i].force=eve.olde.targetTouches[i].force;
     obj.targetTouches[i].identifier=eve.olde.targetTouches[i].identifier;
     obj.targetTouches[i].pageX=eve.olde.targetTouches[i].pageX;
     obj.targetTouches[i].pageY=eve.olde.targetTouches[i].pageY;
     obj.targetTouches[i].radiusX=eve.olde.targetTouches[i].radiusX;
     obj.targetTouches[i].radiusY=eve.olde.targetTouches[i].radiusY;
     obj.targetTouches[i].rotationAngle=eve.olde.targetTouches[i].rotationAngle;
     obj.targetTouches[i].screenX=eve.olde.targetTouches[i].screenX;
     obj.targetTouches[i].screenY=eve.olde.targetTouches[i].screenY;
     obj.targetTouches[i].targetId=eve.olde.targetTouches[i].targetId;
     }
    }
   }

  obj.touchesLen=0;
  obj.touches=[];
  if(eve.olde.touches)
   {
   obj.touchesLen=eve.olde.touches.length;
   k=obj.touchesLen;
   for(i=0;i<3;i++)
    {
    if(i<k)
     {
     obj.touches[i]={};
     obj.touches[i].clientX=eve.olde.touches[i].clientX;
     obj.touches[i].clientY=eve.olde.touches[i].clientY;
     obj.touches[i].force=eve.olde.touches[i].force;
     obj.touches[i].identifier=eve.olde.touches[i].identifier;
     obj.touches[i].pageX=eve.olde.touches[i].pageX;
     obj.touches[i].pageY=eve.olde.touches[i].pageY;
     obj.touches[i].radiusX=eve.olde.touches[i].radiusX;
     obj.touches[i].radiusY=eve.olde.touches[i].radiusY;
     obj.touches[i].rotationAngle=eve.olde.touches[i].rotationAngle;
     obj.touches[i].screenX=eve.olde.touches[i].screenX;
     obj.touches[i].screenY=eve.olde.touches[i].screenY;
     obj.touches[i].targetId=eve.olde.touches[i].targetId;
     }
    }
   }
 return obj;
 }









 function ChatUxEventHandler(typ,eve) { var ok,clkx,clky; var spot; var i,j,k,c; var obj; var han,obj,ctx; var thing,t,delt,dx,dy,dv;
 //-----------------------------------------------
 hitIt(LineNumber(),arguments.callee.name);
 ok=false;
 if(typ=="tap"&&(eve.olde.type=="touchstart"||eve.olde.type=="touchmove"||eve.olde.type=="touchend"||eve.olde.type=="click"))
  {
  hitIt(LineNumber(),arguments.callee.name);
  obj=ChatUxTouchDecode(typ,eve);

  if(obj.what=="click")
   {
   //audio_clicked=true;
   clkx=obj.x;
   clky=obj.y;
   spot=ChatUxKeyFromCord(clkx,clky);
   if(spot)     {     ChatUxKeyEvent(spot);  ok=true;   }
   }
  else
  if(obj.what=="touchstart")
   {
   //audio_clicked=true;
   clkx=obj.changedTouches[0].clientX;
   clky=obj.changedTouches[0].clientY;
   spot=ChatUxKeyFromCord(clkx,clky);
   if(spot)    {    ChatUxKeyEvent(spot);    ok=true;    }
   finger=[];
   thing={};
   thing.stamp=obj.timeStamp;
   thing.x=parseInt(clkx);
   thing.y=parseInt(clky);
   finger.push(thing);
   }
  else
  if(obj.what=="touchmove")
   {
   clkx=obj.changedTouches[0].clientX;
   clky=obj.changedTouches[0].clientY;
   thing={};
   thing.stamp=obj.timeStamp;
   thing.x=parseInt(clkx);
   thing.y=parseInt(clky);
   finger.push(thing);
   }
  else
  if(obj.what=="touchend")
   {
   clkx=obj.changedTouches[0].clientX;
   clky=obj.changedTouches[0].clientY;
   thing={};
   thing.stamp=obj.timeStamp;
   thing.x=parseInt(clkx);
   thing.y=parseInt(clky);
   finger.push(thing);

   delt={};
   delt.x=0;
   delt.y=0;
   thing=finger[finger.length-1];
   if((thing.stamp-finger[0].stamp)<1000)
    {
    dx=(thing.x-finger[0].x);
    dy=(thing.y-finger[0].y);
    if((dy>1&&dy<70)||(dy>-70&&dy<-1))
     {
     is_scrolling=false;
     scrolling_dir="none";
     }



    if(dy>100)
     {
     dv=dy-100;
     dv=dv/50;
     dv=parseInt(dv);
     if(dv<1) { dv=1; }
     if(dv>8) { dv=8; }
     user_string1=""+(dx)+" "+(dy)+" "+(dv);
     //if(is_scrolling==true&&scrolling_dir=="up"&&scroll_speed<(13*dv)) scroll_speed+=(13*dv);
     //else

     if(is_scrolling==true&&scrolling_dir=="down")
      {
      scroll_speed=13;
      is_scrolling=true;
      scrolling_dir="up";

      }
     else
      {
      scroll_speed=13*dv;
      is_scrolling=true;
      scrolling_dir="up";
      }
     }
    else
    if(dy<-100)
     {
     dv=Math.abs(dy);
     dv=dv-100;
     dv=dv/50;
     dv=parseInt(dv);
     if(dv<1) { dv=1; }
     if(dv>8) { dv=8; }
     //if(dv<1) { dv=1; }
     user_string1=""+(dx)+" "+(dy)+" "+(dv);
     //remlog.Write("",dx+" "+dy+" "+dv);
     //if(is_scrolling==true&&scrolling_dir=="down"&&scroll_speed<(13*dv)) scroll_speed+=(13*dv);
     //else scroll_speed=13;
     if(is_scrolling==true&&scrolling_dir=="up")
      {
      scroll_speed=13;
      is_scrolling=true;
      scrolling_dir="dowm";
      }
     else
      {
      scroll_speed=13*dv;
      is_scrolling=true;
      scrolling_dir="down";
      }
     }

    /*
    for(t=0;t<finger.length;t++)
     {
     thing=finger[t];
     if(t==0) { delt.x=thing.x; delt.y=thing.y; }
     //console.log(thing.x+" "+thing.y+"   "+(thing.x-delt.x)+"  "+(thing.y-delt.y));
     remlog.Write("",thing.x+" "+thing.y+"   "+(thing.x-delt.x)+"  "+(thing.y-delt.y));
     delt.x=thing.x;
     delt.y=thing.y;
     //console.log(thing);
     }
    */

    }
   //console.log("in "+(thing.stamp-finger[0].stamp));
 //  remlog.Write("","in "+(thing.stamp-finger[0].stamp));

   }
  if(ok==true)
   {
   }
  }
 if(ok==true)
  {
  //gui.UpdateRequest();
  //ChatUxUpdate();
  }
 }













 function ChatUxClickHandler() { var clkx,clky,spot,tsd,ok;
 //--------------------------------------------
 hitIt(LineNumber(),arguments.callee.name);
 document.body.style.touchAction="none";
 document.getElementById('bodid').addEventListener('tap',function(event)
  {
  if(event.type=="click"||event.type=="tap")
   {
   ChatUxEventHandler(event.type,event);
   }
  event.preventDefault();
  event.stopPropagation();
  },false);
 }




/*-----------------------------------------------------------------------*/






 function ChatUxPaintKeyboard(disp) { var han,obj,ctx; var dww,dhh; var zt,zh,zw,zl; var sx,sy,sw; var uw,uh; var ky,difx,dify;
 //-----------------------------------------
 hitIt(LineNumber(),arguments.callee.name);
 if((han=gui.IdFind("maincan"))==0)        { Oof(); }
 if((obj=gui.Get(han))==null)              { Oof(); }
 if((ctx=gui.CanvasContextGet(han))==null) { Oof(); }
 dww=disp.win_wid;
 dhh=disp.win_hit;

 zt=gui.Get(gui.IdFind("keybdiv")).b_rect.top;
 zh=gui.Get(gui.IdFind("keybdiv")).b_rect.height;
 zw=gui.Get(gui.IdFind("keybdiv")).b_rect.width;
 zl=gui.Get(gui.IdFind("keybdiv")).b_rect.left;
 ky=gui.Get(gui.IdFind("selfvid")).b_rect.top;

 //console.log(main.app.text_area);
 ChatUxPaintTextArea(ctx,main.app.text_area.x,main.app.text_area.y,main.app.text_area.w,main.app.text_area.h,chat_input_text);
 ///ChatUxPaintTextArea(ctx,main.app.config.text_xpos,ky,main.app.config.text_width,main.app.config.text_height,chat_input_text);
 keyboard_spot_count=0;

 sy=(zh-main.app.kbkey_yshift)/4;

 uh=parseInt(sy*main.app.kbkey_ystride);
 sx=parseInt(zw/10);
 uw=parseInt(sx*main.app.kbkey_xstride);
 difx=parseInt((sx-(sx*main.app.kbkey_xstride))/2);
 dify=parseInt((sy-(sy*main.app.kbkey_ystride))/2);
 //Oof(difx+" "+dify);


 zx=zl+difx;//+((zw-((sx*9)+uw))/2);
 zy=zt+main.app.kbkey_yshift+(sy*0)+dify;
 zx=parseInt(zx);
 zy=parseInt(zy);

 if(keyboard_page==0)
  {
  if(keyboard_shift==0)
   {
   ChatUxPaintKey(ctx,zx,zy,uw,uh,"q"); zx+=sx;
   ChatUxPaintKey(ctx,zx,zy,uw,uh,"w"); zx+=sx;
   ChatUxPaintKey(ctx,zx,zy,uw,uh,"e"); zx+=sx;
   ChatUxPaintKey(ctx,zx,zy,uw,uh,"r"); zx+=sx;
   ChatUxPaintKey(ctx,zx,zy,uw,uh,"t"); zx+=sx;
   ChatUxPaintKey(ctx,zx,zy,uw,uh,"y"); zx+=sx;
   ChatUxPaintKey(ctx,zx,zy,uw,uh,"u"); zx+=sx;
   ChatUxPaintKey(ctx,zx,zy,uw,uh,"i"); zx+=sx;
   ChatUxPaintKey(ctx,zx,zy,uw,uh,"o"); zx+=sx;
   ChatUxPaintKey(ctx,zx,zy,uw,uh,"p");
   }
  else
   {
   ChatUxPaintKey(ctx,zx,zy,uw,uh,"Q"); zx+=sx;
   ChatUxPaintKey(ctx,zx,zy,uw,uh,"W"); zx+=sx;
   ChatUxPaintKey(ctx,zx,zy,uw,uh,"E"); zx+=sx;
   ChatUxPaintKey(ctx,zx,zy,uw,uh,"R"); zx+=sx;
   ChatUxPaintKey(ctx,zx,zy,uw,uh,"T"); zx+=sx;
   ChatUxPaintKey(ctx,zx,zy,uw,uh,"Y"); zx+=sx;
   ChatUxPaintKey(ctx,zx,zy,uw,uh,"U"); zx+=sx;
   ChatUxPaintKey(ctx,zx,zy,uw,uh,"I"); zx+=sx;
   ChatUxPaintKey(ctx,zx,zy,uw,uh,"O"); zx+=sx;
   ChatUxPaintKey(ctx,zx,zy,uw,uh,"P");
   }
  }
 else
 if(keyboard_page==1)
  {
  ChatUxPaintKey(ctx,zx,zy,uw,uh,"1"); zx+=sx;
  ChatUxPaintKey(ctx,zx,zy,uw,uh,"2"); zx+=sx;
  ChatUxPaintKey(ctx,zx,zy,uw,uh,"3"); zx+=sx;
  ChatUxPaintKey(ctx,zx,zy,uw,uh,"4"); zx+=sx;
  ChatUxPaintKey(ctx,zx,zy,uw,uh,"5"); zx+=sx;
  ChatUxPaintKey(ctx,zx,zy,uw,uh,"6"); zx+=sx;
  ChatUxPaintKey(ctx,zx,zy,uw,uh,"7"); zx+=sx;
  ChatUxPaintKey(ctx,zx,zy,uw,uh,"8"); zx+=sx;
  ChatUxPaintKey(ctx,zx,zy,uw,uh,"9"); zx+=sx;
  ChatUxPaintKey(ctx,zx,zy,uw,uh,"0");
  }
 else
 if(keyboard_page==2)
  {
  ChatUxPaintKey(ctx,zx,zy,uw,uh,"["); zx+=sx;
  ChatUxPaintKey(ctx,zx,zy,uw,uh,"]"); zx+=sx;
  ChatUxPaintKey(ctx,zx,zy,uw,uh,"{"); zx+=sx;
  ChatUxPaintKey(ctx,zx,zy,uw,uh,"}"); zx+=sx;
  ChatUxPaintKey(ctx,zx,zy,uw,uh,"#"); zx+=sx;
  ChatUxPaintKey(ctx,zx,zy,uw,uh,"%"); zx+=sx;
  ChatUxPaintKey(ctx,zx,zy,uw,uh,"^"); zx+=sx;
  ChatUxPaintKey(ctx,zx,zy,uw,uh,"*"); zx+=sx;
  ChatUxPaintKey(ctx,zx,zy,uw,uh,"+"); zx+=sx;
  ChatUxPaintKey(ctx,zx,zy,uw,uh,"=");
  }

 sx=parseInt(zw/10);
 uw=sx*main.app.kbkey_xstride;
 zx=zl+((zw-((sx*8)+uw))/2);
 zy=zt+main.app.kbkey_yshift+(sy*1);
 zx+=difx;
 zy+=dify;
 if(keyboard_page==0)
  {
  if(keyboard_shift==0)
   {
   ChatUxPaintKey(ctx,zx,zy,uw,uh,"a"); zx+=sx;
   ChatUxPaintKey(ctx,zx,zy,uw,uh,"s"); zx+=sx;
   ChatUxPaintKey(ctx,zx,zy,uw,uh,"d"); zx+=sx;
   ChatUxPaintKey(ctx,zx,zy,uw,uh,"f"); zx+=sx;
   ChatUxPaintKey(ctx,zx,zy,uw,uh,"g"); zx+=sx;
   ChatUxPaintKey(ctx,zx,zy,uw,uh,"h"); zx+=sx;
   ChatUxPaintKey(ctx,zx,zy,uw,uh,"j"); zx+=sx;
   ChatUxPaintKey(ctx,zx,zy,uw,uh,"k"); zx+=sx;
   ChatUxPaintKey(ctx,zx,zy,uw,uh,"l");
   }
  else
   {
   ChatUxPaintKey(ctx,zx,zy,uw,uh,"A"); zx+=sx;
   ChatUxPaintKey(ctx,zx,zy,uw,uh,"S"); zx+=sx;
   ChatUxPaintKey(ctx,zx,zy,uw,uh,"D"); zx+=sx;
   ChatUxPaintKey(ctx,zx,zy,uw,uh,"F"); zx+=sx;
   ChatUxPaintKey(ctx,zx,zy,uw,uh,"G"); zx+=sx;
   ChatUxPaintKey(ctx,zx,zy,uw,uh,"H"); zx+=sx;
   ChatUxPaintKey(ctx,zx,zy,uw,uh,"J"); zx+=sx;
   ChatUxPaintKey(ctx,zx,zy,uw,uh,"K"); zx+=sx;
   ChatUxPaintKey(ctx,zx,zy,uw,uh,"L");
   }
  }
 else
 if(keyboard_page==1)
  {
  ChatUxPaintKey(ctx,zx,zy,uw,uh,"-"); zx+=sx;
  ChatUxPaintKey(ctx,zx,zy,uw,uh,"/"); zx+=sx;
  ChatUxPaintKey(ctx,zx,zy,uw,uh,":"); zx+=sx;
  ChatUxPaintKey(ctx,zx,zy,uw,uh,";"); zx+=sx;
  ChatUxPaintKey(ctx,zx,zy,uw,uh,"("); zx+=sx;
  ChatUxPaintKey(ctx,zx,zy,uw,uh,")"); zx+=sx;
  ChatUxPaintKey(ctx,zx,zy,uw,uh,"$"); zx+=sx;
  ChatUxPaintKey(ctx,zx,zy,uw,uh,"&"); zx+=sx;
  ChatUxPaintKey(ctx,zx,zy,uw,uh,"@");
  }
 else
 if(keyboard_page==2)
  {
  ChatUxPaintKey(ctx,zx,zy,uw,uh,"_"); zx+=sx;
  ChatUxPaintKey(ctx,zx,zy,uw,uh,"\\"); zx+=sx;
  ChatUxPaintKey(ctx,zx,zy,uw,uh,"|"); zx+=sx;
  ChatUxPaintKey(ctx,zx,zy,uw,uh,"~"); zx+=sx;
  ChatUxPaintKey(ctx,zx,zy,uw,uh,"<"); zx+=sx;
  ChatUxPaintKey(ctx,zx,zy,uw,uh,">"); zx+=sx;
  ChatUxPaintKey(ctx,zx,zy,uw,uh,"!"); zx+=sx;
  ChatUxPaintKey(ctx,zx,zy,uw,uh,"!"); zx+=sx;
  ChatUxPaintKey(ctx,zx,zy,uw,uh,"!");
  }

 sx=parseInt(zw/10);
 uw=parseInt(sx*main.app.kbkey_xstride);
 zx=zl;
 zy=zt+main.app.kbkey_yshift+(sy*2);
 zx+=difx;
 zy+=dify;

 ctx.save();
 //if(env.is_mobile==true)  { gui.CanvasContextGet(gui.IdFind("maincan")).font="500 30px arial"; }
 //else                     { gui.CanvasContextGet(gui.IdFind("maincan")).font="500 20px arial"; }
 if(keyboard_page==0)  {  ChatUxPaintKey(ctx,zx,zy,uw*1.5,uh,"SHFT"); zx+=(sx*1.5); zx-=0;  zx+=0; }
 else
 if(keyboard_page==1)  {  ChatUxPaintKey(ctx,zx,zy,uw*1.5,uh,"#+="); zx+=(uw*2.0); zx-=7;  }
 else
 if(keyboard_page==2)  {  ChatUxPaintKey(ctx,zx,zy,uw*1.5,uh,"ABC"); zx+=(uw*2.0); zx-=7;  }
 //console.log("sx="+sx+" uw="+uw);
 ctx.restore();
 if(keyboard_page==0)
  {
  if(keyboard_shift==0)
   {
   ChatUxPaintKey(ctx,zx,zy,uw,uh,"z"); zx+=sx;
   ChatUxPaintKey(ctx,zx,zy,uw,uh,"x"); zx+=sx;
   ChatUxPaintKey(ctx,zx,zy,uw,uh,"c"); zx+=sx;
   ChatUxPaintKey(ctx,zx,zy,uw,uh,"v"); zx+=sx;
   ChatUxPaintKey(ctx,zx,zy,uw,uh,"b"); zx+=sx;
   ChatUxPaintKey(ctx,zx,zy,uw,uh,"n"); zx+=sx;
   ChatUxPaintKey(ctx,zx,zy,uw,uh,"m"); zx+=sx; zx+=7;
   }
  else
   {
   ChatUxPaintKey(ctx,zx,zy,uw,uh,"Z"); zx+=sx;
   ChatUxPaintKey(ctx,zx,zy,uw,uh,"X"); zx+=sx;
   ChatUxPaintKey(ctx,zx,zy,uw,uh,"C"); zx+=sx;
   ChatUxPaintKey(ctx,zx,zy,uw,uh,"V"); zx+=sx;
   ChatUxPaintKey(ctx,zx,zy,uw,uh,"B"); zx+=sx;
   ChatUxPaintKey(ctx,zx,zy,uw,uh,"N"); zx+=sx;
   ChatUxPaintKey(ctx,zx,zy,uw,uh,"M"); zx+=sx; zx+=7;
   }
  }
 else
 if(keyboard_page==1)
  {
  ChatUxPaintKey(ctx,zx,zy,uw,uh,"."); zx+=sx;
  ChatUxPaintKey(ctx,zx,zy,uw,uh,","); zx+=sx;
  ChatUxPaintKey(ctx,zx,zy,uw,uh,"?"); zx+=sx;
  ChatUxPaintKey(ctx,zx,zy,uw,uh,"!"); zx+=sx;
  ChatUxPaintKey(ctx,zx,zy,uw,uh,"'"); zx+=sx;
  ChatUxPaintKey(ctx,zx,zy,uw,uh,"#"); zx+=sx;
  ChatUxPaintKey(ctx,zx,zy,uw,uh,"*"); zx+=sx; zx+=7;
  }
 else
 if(keyboard_page==2)
  {
  ChatUxPaintKey(ctx,zx,zy,uw,uh,"."); zx+=sx;
  ChatUxPaintKey(ctx,zx,zy,uw,uh,","); zx+=sx;
  ChatUxPaintKey(ctx,zx,zy,uw,uh,"?"); zx+=sx;
  ChatUxPaintKey(ctx,zx,zy,uw,uh,"!"); zx+=sx;
  ChatUxPaintKey(ctx,zx,zy,uw,uh,"'"); zx+=sx;
  ChatUxPaintKey(ctx,zx,zy,uw,uh,"\""); zx+=sx;
  ChatUxPaintKey(ctx,zx,zy,uw,uh,"*"); zx+=sx; //zx+=7;
  }
 ctx.save();
 //gui.CanvasContextGet(gui.IdFind("maincan")).font="500 30px arial";
 if(keyboard_page==0)  {  ChatUxPaintKey(ctx,zx,zy,uw*1.5,uh,"BACK");  }
 else
 if(keyboard_page==1)  {  ChatUxPaintKey(ctx,zx,zy,uw*1.5,uh,"BACK");  }
 else
 if(keyboard_page==2)  {  ChatUxPaintKey(ctx,zx,zy,uw*1.5,uh,"BACK");  }

 ctx.restore();
 sx=parseInt(zw/10);
 uw=parseInt(sx*main.app.kbkey_xstride);
 zx=zl;
 zy=zt+main.app.kbkey_yshift+(sy*3);
 zx+=difx;
 zy+=dify;

 ctx.save();
 //gui.CanvasContextGet(gui.IdFind("maincan")).font="500 30px arial";
 if(keyboard_page==0)  {  ChatUxPaintKey(ctx,zx,zy,uw*1.5,uh,"123");         zx+=(sx*1.5); zx+=0;  }
 else
 if(keyboard_page==1)  {  ChatUxPaintKey(ctx,zx,zy,uw*1.5,uh,"ABC");         zx+=(uw*2.0); zx+=0;  }
 else
 if(keyboard_page==2)  {  ChatUxPaintKey(ctx,zx,zy,uw*1.5,uh,"#+=");         zx+=(uw*2.0); zx+=0;  }

 //ChatUxPaintKey(ctx,zx,zy,(sx*6)+uw,uh,"SPACE");    zx+=(sx*7); zx+=10;

 ChatUxPaintKey(ctx,zx,zy,(sx*6)+uw,uh,"SPACE");    zx+=(sx*7); zx+=0;
 ChatUxPaintKey(ctx,zx,zy,uw*1.5,uh,"GO");          zx+=(sx*2);
 ctx.restore();
 keyboard_painted=1;
 }




/*-----------------------------------------------------------------------*/



 function ChatUxPaintKey(ctx,xx,yy,ww,hh,key) { var rx1,ry1,rx2,ry2; var wid,xxx; var spot,s; var zx,zy; var ac,ll,subx,suby;
 //------------------------------------------
 hitIt(LineNumber(),arguments.callee.name);
 rx1=xx;
 ry1=yy;
 rx2=(rx1+ww)-1;
 ry2=(ry1+hh)-1;
 //---------------------------
 zy=gui.Get(gui.IdFind("keybdiv")).b_rect.top;
 zx=gui.Get(gui.IdFind("keybdiv")).b_rect.left;
 //---------------------------
 spot=keyboard_spot_array[keyboard_spot_count];
 spot.name="keydown";
 spot.self_index=keyboard_spot_count;
 spot.keyCode=key.charCodeAt(0);
 spot.key=key;
 spot.ascii=spot.key.charCodeAt(0);
 subx=0;
 suby=0;
 spot.x1=parseInt(rx1-subx);
 spot.y1=parseInt(ry1-suby);
 spot.x2=parseInt(rx2+subx);
 spot.y2=parseInt(ry2+suby);
 //spot.y1-=2;
 spot.x1-=5;
 spot.x2+=5;
 spot.y1-=4;
 spot.y2+=4;
 keyboard_spot_array[keyboard_spot_count]=spot;
 keyboard_spot_count++;
 //---------------------------
 ctx.save();
 if(key=="SHFT"&&keyboard_shift==1)  {  ctx.fillStyle=gui.RgbaSet(0xff,0xff,0x1f,1.0);  }
 else                                {  ctx.fillStyle=gui.RgbaSet(0xdf,0xcf,0xbf,1.0);  }
 ctx.strokeStyle=gui.RgbaSet(0x2f,0xa0,0x00,1.0);
 ctx.lineJoin="bevel";
 ctx.lineWidth=4;
 gui.CanvasShadowSet(gui.IdFind("maincan"),"transparent",0,0,0);
 //---------------------
 ctx.beginPath();
  ctx.moveTo(spot.x1,spot.y1);
  ctx.lineTo(spot.x2,spot.y1);
  ctx.lineTo(spot.x2,spot.y2);
  ctx.lineTo(spot.x1,spot.y2);
 // ctx.lineTo(spot.x1,spot.y1);
 ctx.closePath();
 //---------------------------
 ctx.stroke();
 ctx.fill();
 //-------------------
 //gui.CanvasShadowSet(gui.IdFind("maincan"),"#123451",6,4,6);
 gui.CanvasShadowSet(gui.IdFind("maincan"),"transparent",0,0,0);
 ctx.fillStyle=gui.RgbaSet(0x56,0x65,0x53,1.0);
 ctx.textAlign="center";
 ctx.textBaseline="middle";
 ctx.fillText(key,xx+(ww/2),yy+(hh/2));
 ctx.restore();
 //console.log(spot.x1+" "+spot.y1);
 }



/*-----------------------------------------------------------------------*/







 function ChatUxPaintHuds() { var disp,obj,str;
 //-----------------------------------
 hitIt(LineNumber(),arguments.callee.name);
 disp=DispGet();
/// disp=env.DisplayGet();
 obj=gui.Get(gui.IdFind("hudsdiv"));
 gui.FontSet(gui.IdFind("hudsdiv"),"monospace","24px",700);
 str="";
 str+="ms  "+timer.MsRunning()+" ";
 str+="stg "+main.vars.stage+" ";
 str+="cyc "+main.vars.cycle+"  spd="+main.state.speed+"\n";
 str+="fc  "+main.vars.display_stats.frame_count+" ";
 str+="el  "+num.Fixed((main.vars.display_stats.elapsed/1000),2)+"  ";
 str+="fps "+num.Fixed(main.vars.display_stats.fps,2)+"\n";
 str+=" dww "+disp.win_wid+" "+disp.win_hit+" dsw "+disp.scr_wid+" "+disp.scr_hit+"\n";
 str+=" rat="+disp.win_ratio+"\n";
 //str+="height="+history_height+" from="+history_from+" fine="+history_fine+"\n";
 str+="height="+history_height+" fine="+history_fine+"\n";
 str+="usr0: "+user_string0+"\n";
 str+="usr1: "+user_string1+"\n";
 str+="usr5: "+user_string5+"\n";
 str+="scrtol ="+is_scrolling+"\n";
 str+="hischg ="+history_change+"\n";
 str+="ace ="+audio_clicked+"\n";
 ///str+="cit="+chat_input_text;
 obj.dom.innerText=str;
 }



/*-----------------------------------------------------------------------*/





 function ChatUxPaintTextArea(ctx,xx,yy,ww,hh,str) { var metrix;
 //------------------------------------------
 hitIt(LineNumber(),arguments.callee.name);
 //console.log(xx+" "+yy+" "+ww+" "+hh);
 ctx.save();
 ctx.beginPath();
 ctx.moveTo(xx,yy);
 ctx.lineTo((xx+ww)-1,yy);
 ctx.lineTo((xx+ww)-1,(yy+hh)-1);
 ctx.lineTo(xx,(yy+hh)-1);
 ctx.closePath();
 ctx.fillStyle="#fffaff";
 ctx.strokeStyle="#00ff00";
 ctx.lineJoin="bevel";
 ctx.lineWidth=2;
 gui.CanvasShadowSet(gui.IdFind("maincan"),"transparent",0,0,0);
 ctx.stroke();
 ctx.fill();
 ctx.font="600 42px arial";
 gui.CanvasShadowSet(gui.IdFind("maincan"),"transparent",0,0,0);
 ctx.fillStyle="#13871f";
 metrix=ctx.measureText(str);
 ///user_string1=parseInt(metrix.width)+10;
 ctx.fillText(str+"|",xx+10,yy+40);
 ctx.restore();
 }



/*-----------------------------------------------------------------------*/



 function ChatUxPaintBubble(ctx,xx,yy,ww,hh,isme,str) { var rx1,ry1,rx2,ry2;
 //-----------------------------
 hitIt(LineNumber(),arguments.callee.name);
 rx1=xx;
 ry1=yy;
 rx2=(rx1+ww)-1;
 ry2=(ry1+hh)-1;
 ctx.save();
 //---------------------------
 ctx.beginPath();
 ctx.moveTo(rx1,ry1);
 ctx.lineTo(rx2,ry1);
 ctx.lineTo(rx2,ry2);
 ctx.lineTo(rx1,ry2);
 ctx.lineTo(rx1,ry1);
 ctx.moveTo(rx1,ry1+30);
 ctx.lineTo(rx1-30,ry1+60);
 ctx.lineTo(rx1,ry1+70);
 ctx.closePath();
 //---------------------------
 ctx.fillStyle="#ffffff";
 if(isme==0)  {  ctx.strokeStyle="#ff0000";  }
 else
 if(isme==1)  {  ctx.strokeStyle="#00ff00";  }


 ctx.lineJoin="bevel";
 ctx.lineWidth=5;
 gui.CanvasShadowSet(gui.IdFind("maincan"),"transparent",0,0,0);
 //gui.CanvasShadowSet(gui.IdFind("maincan"),"#a0a0a0",4,5,6);
 ctx.stroke();
 ctx.fill();
 //---------------------------
 ctx.moveTo(rx1,ry1);
 ctx.font="600 42px arial";
 ctx.textAlign="start";
 ctx.textBaseline="middle";
 ctx.fillStyle="#705060";
 ctx.fillText(str,xx+20,yy+40);
 ctx.restore();
 }




/*-----------------------------------------------------------------------*/




 function ChatUxPaintCompleteHistory(dtx,disp) { var dww,dhh; var wid,hit,hs,done,i,che,sid,han,str,hx,rra,rrb,isme,tall; var kw,kh; var ewid,ehit; var ymiss;
 //----------------------------------------
 hitIt(LineNumber(),arguments.callee.name);
 dww=disp.win_wid;
 dhh=disp.win_hit;
 kw=gui.Get(gui.IdFind(dtx.canvas.id)).dom.width;
 kh=gui.Get(gui.IdFind(dtx.canvas.id)).dom.height;
 ewid=main.app.rcve_area.w;
 ehit=main.app.rcve_area.h;
 //console.log(ewid+" "+ehit+"  "+kw+"  "+kh);
 ymiss=20;
 rra=gui.XywhSet(20,kh-(ehit+ymiss),ewid,ehit);
 rrb=gui.XywhSet(rra.x+rra.w+40,kh-(ehit+(ymiss*2)),kw-(rra.x+rra.w+40+20),ehit);
 ///rrb.y+=10;
 /// rrb.h+=20;
 //rra.y+=40;
 //rrb.y+=40;

 hs=chat_history_slots;
 hx=hs-1;
 done=0;
 tall=main.app.rcve_vgap;
 for(i=0;i<hs;i++)
  {
  if(hx<0) { break; }
  che=chat_history_array[hx];
  sid=document.getElementById(che.cha_id);
  if(sid==null||sid.src==null)        { Oof(); }
  if(sid.src.substring(0,5)!="data:") { hx--; continue; }
  if(che.msg.uuid==main.vars.room_object.my_uuid) { isme=1;  }
  else                                            { isme=0; }
      dtx.drawImage(sid,rra.x,rra.y,rra.w,rra.h);
  ChatUxPaintBubble(dtx,rrb.x,rrb.y,rrb.w,rrb.h,isme,che.txt);
  rra.y-=tall;
  rrb.y-=tall;
  done++;
  hx--;
  }
 history_height=done;
 }



/*-----------------------------------------------------------------------*/



 function ChatUxAnimator() {
 //--------------------------------------------
 ChatUxAnimate();
 window.requestAnimationFrame(ChatUxAnimator);
 }




 function ChatUxUpdate() { var disp; var dtx,ctx,canv; var dww,dhh; var han,obj; var kb,kw,kh; var ewid,ehit,etal; var fx,fa; var dw,dh; var sy; var cando;
 //----------------------------------------------
 hitIt(LineNumber(),arguments.callee.name);
 if(1&&num.Rand(5)==0)  {   ChatUxPaintHuds();   }
 if(gui.UpdateCheck()!=true) { return; }
 if(main.vars.display_stats.frame_count==0)  {  main.vars.display_stats.tik=timer.MsRunning();  }
 main.vars.display_stats.elapsed=timer.MsRunning()-main.vars.display_stats.tik;
 main.vars.display_stats.frame_count++;
 main.vars.display_stats.fps=main.vars.display_stats.frame_count/(main.vars.display_stats.elapsed/1000);
 disp=DispGet();
 dww=disp.win_wid;
 dhh=disp.win_hit;

/*
 kh=parseInt(dhh*0.3);
 kh=221;
 kh=370;
 main.app.keyboard_area=RectSet(main.app.keyboard_area,20,dhh-(kh+10),dww-40,kh); // was dhh-380,dww-40,370
 main.app.selfvid_area=RectSet(main.app.selfvid_area,20,main.app.keyboard_area.y-260,320,240);
 main.app.text_area=RectSet(main.app.text_area,20+320+10,main.app.keyboard_area.y-260,dww-(20+320+30),240);
 main.app.historic_area=RectSet(main.app.historic_area,20,10,dww-40,main.app.text_area.y-20);
 main.app.huds_area=RectSet(main.app.huds_area,600,10,300,400);
 */
 if(env.is_mobile==true)
  {
  main.app.kbkey_ystride=0.9;//0.5;
  main.app.kbkey_xstride=0.85;//0.5;
  main.app.kbkey_yshift=5;//138;
  kh=470; kh=500;
  kh=parseInt(disp.win_hit*0.33);
  main.app.keyboard_area=RectSet(main.app.keyboard_area,20,dhh-(kh+10),dww-40,kh); // was dhh-380,dww-40,370
  user_string5="m "+main.app.keyboard_area.x+" "+main.app.keyboard_area.y+" "+main.app.keyboard_area.w+" "+main.app.keyboard_area.h;
  }
 else
  {
  main.app.kbkey_ystride=0.6;//0.5;
  main.app.kbkey_xstride=0.8;//0.5;
  main.app.kbked_yshift=5;//138;
  kh=470; kh=150;
  main.app.keyboard_area=RectSet(main.app.keyboard_area,20,dhh-(kh+10),dww-40,kh); // was dhh-380,dww-40,370
  user_string5="d "+main.app.keyboard_area.x+" "+main.app.keyboard_area.y+" "+main.app.keyboard_area.w+" "+main.app.keyboard_area.h;
  }

 kw=320;
 kh=240;
 main.app.selfvid_area=RectSet(main.app.selfvid_area,20,main.app.keyboard_area.y-(kh+20),kw,kh);
 main.app.text_area=RectSet(main.app.text_area,20+kw+10,main.app.keyboard_area.y-(kh+20),dww-(20+kw+30),kh);

 main.app.huds_area=RectSet(main.app.huds_area,500,10,400,450);

/*
 main.app.keyboard_area=RectSet(main.app.keyboard_area,20,dhh-280,dww-40,270);
 main.app.selfvid_area=RectSet(main.app.selfvid_area,20,main.app.keyboard_area.y-130,160,120);
 main.app.text_area=RectSet(main.app.text_area,20+160+10,main.app.keyboard_area.y-130,dww-(20+160+30),120);
 main.app.huds_area=RectSet(main.app.huds_area,600,10,300,400);
*/

 if(gui.Get(gui.IdFind("maincan")).dom.style.display=="none")  { gui.Get(gui.IdFind("maincan")).dom.style.display="inline";   }
 if(gui.Get(gui.IdFind("scrlcan")).dom.style.display=="none")  { gui.Get(gui.IdFind("scrlcan")).dom.style.display="inline";   }
 if(gui.Get(gui.IdFind("selfvid")).dom.style.display=="none")  { gui.Get(gui.IdFind("selfvid")).dom.style.display="inline";   }
 if(gui.Get(gui.IdFind("hudsdiv")).dom.style.display=="none")  { gui.Get(gui.IdFind("hudsdiv")).dom.style.display="inline";   }
 if(gui.Get(gui.IdFind("keybdiv")).dom.style.display=="none")  { gui.Get(gui.IdFind("keybdiv")).dom.style.display="inline";   }

 gui.AreaSet(gui.Get(gui.IdFind("hudsdiv")).self_handle,"ltwh","px",main.app.huds_area.x,main.app.huds_area.y,main.app.huds_area.w,main.app.huds_area.h);
 if(1) { gui.Get(gui.IdFind("hudsdiv")).dom.style.display="none"; }
 if(1) { gui.Get(gui.IdFind("scrlcan")).dom.style.display="none"; }

 han=gui.IdFind("scrlcan");
 gui.AreaSet(gui.Get(gui.IdFind("scrlcan")).self_handle,"ltwh","px",40,40,4,4);
 obj=gui.Get(han);
 dtx=gui.CanvasContextGet(han);
 dtx.globalAlpha=1;
 dtx.mozImageSmoothingEnabled=false;
 dtx.oImageSmoothingEnabled=false;
 dtx.webkitImageSmoothingEnabled=false;
 dtx.imageSmoothingEnabled=false;
 //dtx.save();
 //dtx.fillStyle=gui.RgbaSet(55,55,200,1.0);
 //dtx.fillRect(0,0,200,50);//obj.b_rect.width,obj.b_rect.height);
 //dtx.restore();

 han=gui.IdFind("maincan");
 obj=gui.Get(han);
 ctx=gui.CanvasContextGet(han);
 if((disp.win_wid!=obj.dom.width)||(disp.win_hit!=obj.dom.height))
  {
  if((ret=gui.WidthHeightSet(han,disp.win_wid,disp.win_hit))!=true) { Oof(); }
  ctx=gui.CanvasContextGet(han);
  if(env.is_mobile==true)   {  ctx.font="400 45px arial";   }
  else                      {   ctx.font="400 25px arial";   }
  gui.AreaSet(gui.Get(gui.IdFind("keybdiv")).self_handle,"ltwh","px",main.app.keyboard_area.x,main.app.keyboard_area.y,main.app.keyboard_area.w,main.app.keyboard_area.h);
  gui.AreaSet(gui.Get(gui.IdFind("selfvid")).self_handle,"ltwh","px",main.app.selfvid_area.x,main.app.selfvid_area.y,main.app.selfvid_area.w,main.app.selfvid_area.h);
  }
 ctx.globalAlpha=1;
 ctx.mozImageSmoothingEnabled=false;
 ctx.oImageSmoothingEnabled=false;
 ctx.webkitImageSmoothingEnabled=false;
 ctx.imageSmoothingEnabled=false;
 ChatUxPaintKeyboard(disp);
 if(history_change==true)
  {
  history_change=false;
  ChatUxPaintCompleteHistory(dtx,disp);
  if(history_height>10)
   {
///   is_scrolling=true;
///  history_fine=0;
 //  scrolling_dir="up";
   }
  ///if(is_scrolling==false) { is_scrolling=true; }//history_from=1; }
  }

 gui.UpdateClear();
 }







 function ChatUxAnimate() { var disp; var dtx,ctx,canv; var dww,dhh; var han,obj; var kb; var ewid,ehit,etal; var fx,fa; var dw,dh; var sy; var cando,todo;
 var ypos; var yup;
 //---------------------------------------------------------------------
 hitIt(LineNumber(),arguments.callee.name);
 disp=env.DisplayGet();
 dww=disp.win_wid;
 dhh=disp.win_hit;
 sy=main.app.selfvid_area.y-20;//+(main.app.kbkey_yshift*2);
 han=gui.IdFind("maincan");
 obj=gui.Get(han);
 ctx=gui.CanvasContextGet(han);
 ctx.globalAlpha=1;
 ctx.mozImageSmoothingEnabled=false;
 ctx.oImageSmoothingEnabled=false;
 ctx.webkitImageSmoothingEnabled=false;
 ctx.imageSmoothingEnabled=false;
 ctx.save();
//   ctx.fillStyle=gui.RgbaSet(200,20,150,1.0);
   ctx.fillStyle="#11889b";
   ctx.fillRect(0,0,disp.win_wid,sy);//dhh-(etal+ehit+20));//disp.win_hit/2);
 ctx.restore();
 cando=sy/main.app.rcve_vgap;
  ctx.drawImage(document.getElementById("scrlcan"),
              0,(main.app.scroll_canvas_height-sy)-history_fine,
              dww,sy,
              0,0,
              dww,sy);

 if(is_scrolling==true)
  {
  if(scrolling_dir=="down")
   {
   history_fine-=scroll_speed;
   if(history_fine<0)
    {
    history_fine=0;
    is_scrolling=false;
    }
   }
  else
  if(scrolling_dir=="up")
   {
   history_fine+=scroll_speed;
   if(history_fine>=((history_height-1)*main.app.rcve_vgap))
    {
    history_fine=((history_height-1)*main.app.rcve_vgap);
    is_scrolling=false;
    }
   }
  }



/// gui.UpdateClear();
 }





/*-----------------------------------------------------------------------*/




 function ChatUxKeyEvent(ev) { var ok; var s,spot; var ch1,ch2; var kc,si; var pot; var ac; var what,ll;
 //-----------------------------------
 hitIt(LineNumber(),arguments.callee.name);
 pot=null;
 if(ev.name!="keydown")  {  return null;  }
  if(ev.key=="ABC")  { keyboard_page=0;   gui.UpdateRequest(); return null; }
  if(ev.key=="123")  { keyboard_page=1;   gui.UpdateRequest(); return null; }
  if(ev.key=="#+=")  { keyboard_page=2;   gui.UpdateRequest(); return null; }
  if(ev.key=="SHFT") { keyboard_shift^=1; gui.UpdateRequest(); return null; }
 pot=keyboard_spot_array[ev.self_index];

 //console.log(pot);

 if(pot==null)
  {

  if(ev.keyCode==13||ev.key=="GO")
   {
   //if(chat_input_text!=""&&chat_input_text.length>0)
    {
    ChatRoomSend("jpg",0.5,chat_input_text);
    chat_input_text="";
    gui.UpdateRequest();
    }
   return null;
   }
  else
  if(ev.keyCode==8)
   {
   if(chat_input_text!=""&&chat_input_text.length>0)
    {
    chat_input_text=chat_input_text.substring(0,chat_input_text.length-1);
    gui.UpdateRequest();
    }
   return null;
   }
  else
  if(ev.keyCode>=32&&ev.keyCode<127)
   {
   //console.log("key");
   chat_input_text+=String.fromCharCode(ev.keyCode);
   //audio.play();
   gui.UpdateRequest();
   return null;
   }
  }
 else
  {

  if(ev.key=="GO")
   {
   //if(chat_input_text!=""&&chat_input_text.length>0)
    {
    if(chat_input_text=="HTOG")
     {
     if(gui.Get(gui.IdFind("hudsdiv")).dom.style.display=="none")  { gui.Get(gui.IdFind("hudsdiv")).dom.style.display="inline";   }
     else                                                          { gui.Get(gui.IdFind("hudsdiv")).dom.style.display="none";   }
     chat_input_text="";
     }
    else
     {
     ChatRoomSend("jpg",0.5,chat_input_text);
     chat_input_text="";
     }
    gui.UpdateRequest();
    }
   return pot;
   }
  else
  if(ev.key=="BACK")
   {
   if(chat_input_text!=""&&chat_input_text.length>0)
    {
    chat_input_text=chat_input_text.substring(0,chat_input_text.length-1);
    gui.UpdateRequest();
    }
   return pot;
   }
  else
  if(ev.key=="SPACE")
   {
   //console.log(LineNumber);
   if(chat_input_text!=""&&chat_input_text.length>0)
    {
    chat_input_text+=" ";
    gui.UpdateRequest();
    }
   return pot;
   }
  chat_input_text+=pot.key;
  gui.UpdateRequest();
  }
 return pot;
 }









