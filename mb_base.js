

//' ########     ###     ######  ########
//  ##     ##   ## ##   ##    ## ##
//  ##     ##  ##   ##  ##       ##
//  ########  ##     ##  ######  ######
//  ##     ## #########       ## ##
//  ##     ## ##     ## ##    ## ##
//  ########  ##     ##  ######  ########


 const base_version=1;
 const max_windows=6;

 var logger_level=1;

 var is_restarting=false;

 var handle_base=1000000;
 var handef_array=[];

 navigator.getUserMedia=navigator.getUserMedia||navigator.webkitGetUserMedia||navigator.mozGetUserMedia||navigator.msGetUserMedia||window.RTCPeerConnection;
// var bl_constraints={audio:true,video:true};
 var bl_constraints={audio:false,video:true};
 //var bl_constraints={audio:false,video:{facingMode:'user'}};
 //var bl_constraints={audio:false,video:{facingMode:environment}};
 var pc_configurate={'iceServers':[{'urls':'stun:stun.l.google.com:19302'}]};
 var pc_constraints={'optional':  [{'DtlsSrtpKeyAgreement':true}]};


//' ##     ## ####  ######   ######
//  ###   ###  ##  ##    ## ##    ##
//  #### ####  ##  ##       ##
//  ## ### ##  ##   ######  ##
//  ##     ##  ##        ## ##
//  ##     ##  ##  ##    ## ##    ##
//  ##     ## ####  ######   ######


 function LineNumber () { var ln,bias,e,stack,frame,frameRE;
 e=new Error();
 if(!e.stack)
 try       { throw e;  }
 catch(e)  { if(!e.stack) {  return 0;  } }
 stack=e.stack.toString().split(/\r\n|\n/);
 frameRE=/:(\d+):(?:\d+)[^\d]*$/;
 do { frame=stack.shift();  } while (!frameRE.exec(frame)&&stack.length);
 ln=parseInt(frameRE.exec(stack.shift())[1]);
 return ln;
 }




 function Oof (txt) { var ln,bias,e,stack,frame,frameRE;
 e=new Error();
 if(!e.stack)
 try       { throw e;  }
 catch(e)  { if(!e.stack) {  return false;  } }
 stack=e.stack.toString().split(/\r\n|\n/);
 frameRE=/:(\d+):(?:\d+)[^\d]*$/;
 do { frame=stack.shift();  } while (!frameRE.exec(frame)&&stack.length);
 ln=parseInt(frameRE.exec(stack.shift())[1]);
 txt+="\n";
 txt+="line: ";
 txt+=ln;
 txt+="\n";
 txt+="msrunning: ";
 txt+=timer.MsRunning();
 txt+="\n";
 txt+="stage: ";
 txt+=main.vars.stage;
 alert(txt);
 return true;
 }





 function IsEmpty (val) {
 return (val==null||val.length===0||val==="");
 }



 function IsNotEmpty (val) {
 return !(val==null||val.length===0||val==="");
 }



 function ArrayRotate(arr,reverse) {
 //-------------------------------------------
 if(reverse) { arr.unshift(arr.pop()); }
 else        { arr.push(arr.shift());  }
 return arr;
 }






//' ##        #######   ######    ######   ######## ########
//  ##       ##     ## ##    ##  ##    ##  ##       ##     ##
//  ##       ##     ## ##        ##        ##       ##     ##
//  ##       ##     ## ##   #### ##   #### ######   ########
//  ##       ##     ## ##    ##  ##    ##  ##       ##   ##
//  ##       ##     ## ##    ##  ##    ##  ##       ##    ##
//  ########  #######   ######    ######   ######## ##     ##



 function LoggerLevelSet (lev) {
 logger_level=lev;
 }



 function Logger (lev,msg) { var k; var pre,sarz,darz;
 if(logger_level==0)   { return; }
 if(lev>logger_level)  { return; }
 pre=num.Pad(num.Fixed(timer.TikNow(true),2),10,'0');
 sarz=[].slice.call(arguments,1);
 darz=[];
 darz.push("%c"+pre+": %c"+sarz[0]);
 darz.push("color:green; font-weight:bold;");
 darz.push("color:black");
 for(k=1;k<sarz.length;k++)  {  darz.push(sarz[k]);  }
 //setTimeout(console.log.apply(this,darz));
 //setTimeout(console.log.bind(console,darz));
 setTimeout(console.log.bind(console,...darz));
 ///setTimeout(console.log(...darz));
 //setTimeout(console.log(msg));
// console.log.bind(console,...darz);
 }


/*
 function Loggex (colr,msg) {
 Logger(40,"%c"+msg,"font-size:120%;font-family:monospace;color:"+colr+";");
 }
// var loo=console.log.bind(console,'LOG:');


 function Loggy (...params) {
// setTimeout(console.log.bind(console,...params));
 }

*/



//' ##     ##    ###    ##    ## ########  ##       ########  ######
//  ##     ##   ## ##   ###   ## ##     ## ##       ##       ##    ##
//  ##     ##  ##   ##  ####  ## ##     ## ##       ##       ##
//  ######### ##     ## ## ## ## ##     ## ##       ######    ######
//  ##     ## ######### ##  #### ##     ## ##       ##             ##
//  ##     ## ##     ## ##   ### ##     ## ##       ##       ##    ##
//  ##     ## ##     ## ##    ## ########  ######## ########  ######




 function HandleDefine (type,slots,useall)  {  var i,obj;
 this.base=handle_base;
 this.type=type;
 this.slots=slots;
 this.count=0;
 this.use_all=useall;
 this.pf=0;
 this.array=[];
 for(i=0;i<this.slots;i++)
  {
  obj={};
  if(useall==true) { obj.in_use=true;   }
  else             { obj.in_use=false;  }
  obj.self_index=i;
  obj.self_handle=obj.self_index+this.base;
  this.array[i]=obj;
  }
 if(useall==true) { this.count=this.slots; }
 handef_array.push(this);
 handle_base+=1000000;
 }





 function HandleCheck (handef,handle)  {  var obj;
 if(handle<handef.base)   { return null; }
 handle=handle-handef.base;
 if(handle>=handef.slots) { return null; }
 obj=handef.array[handle];
 if(obj.in_use!=true) { return null; }
 return obj;
 }



 function HandleGet (handef,index) { var obj,han;
 if(index<0||index>=handef.slots) { return 0; }
 obj=handef.array[index];
 if(obj.in_use!=true) { return 0; }
 han=index+handef.base;
 return han;
 }




 function HandleSlotUnused (handef) { var i,obj;
 for(i=0;i<handef.slots;i++)
  {
  obj=handef.array[i];
  if(obj.in_use!=false) { continue; }
  return i;
  }
 return -1;
 }



 function HandleAdd (handef,index) { var obj,han;
 if(index<0||index>=handef.slots) { return 0; }
 obj=handef.array[index];
 if(obj.in_use!=false) { return 0; }
 obj.in_use=true;
 handef.array[index]=obj;
 handef.count++;
 han=index+handef.base;
 return han;
 }



 function HandleRemove (handef,handle) { var obj,idx;
 if((obj=HandleCheck(handef,handle))==null) { return false; }
 idx=obj.self_index;
 if(handef.use_all==true) { alert("handleremvove for use_all==true"); }
 obj.in_use=false;
 handef.array[idx]=obj;
 handef.count--;
 return true;
 }



 function HandleNext (handef) { var idx;
 handef.pf++;
 if(handef.pf>=handef.slots) { handef.pf=0; }
 idx=handef.pf;
 return(HandleGet(handef,idx));
 }






 function HandleText (handle) { var i,hd,str,ix,obj;
 for(i=0;i<handef_array.length;i++)
  {
  hd=handef_array[i];
  if(handle>=hd.base&&handle<(hd.base+hd.slots))
   {
   ix=handle-hd.base;
   obj=hd.array[ix];
   str=hd.type+" index="+ix+" in_use="+obj.in_use;
   return str;
   }
  }
 return null;
 }




//' ######## #### ##     ## ######## ########   ######
//     ##     ##  ###   ### ##       ##     ## ##    ##
//     ##     ##  #### #### ##       ##     ## ##
//     ##     ##  ## ### ## ######   ########   ######
//     ##     ##  ##     ## ##       ##   ##         ##
//     ##     ##  ##     ## ##       ##    ##  ##    ##
//     ##    #### ##     ## ######## ##     ##  ######



 var timer=(function() {
 var is_init=false;
 var msec_base;
 var perf_base;


 function Init () {
 if(is_init==true) { return; }
 msec_base=new Date().valueOf();
 perf_base=performance.now();
 is_init=true;
 }


 Init();



 function TikNow (perf) {
 var t;
 if(perf) { t=performance.now()-perf_base;  }
 else     { t=new Date().valueOf()-msec_base; }
 return t;
 }


 function TikElapsed (perf,tik) {
 return(TikNow(perf)-tik);
 }



 function MsRunning () {
 return(TikNow(false));
 }


 return{
 is_init:is_init,
 TikNow:TikNow,
 TikElapsed:TikElapsed,
 MsRunning:MsRunning
 }


})();







//' ##    ## ##     ## ##     ## ######## ########  ####  ######
//  ###   ## ##     ## ###   ### ##       ##     ##  ##  ##    ##
//  ####  ## ##     ## #### #### ##       ##     ##  ##  ##
//  ## ## ## ##     ## ## ### ## ######   ########   ##  ##
//  ##  #### ##     ## ##     ## ##       ##   ##    ##  ##
//  ##   ### ##     ## ##     ## ##       ##    ##   ##  ##    ##
//  ##    ##  #######  ##     ## ######## ##     ## ####  ######




 var num=(function() {
 var is_init=false;

 function Init () {
 if(is_init==true) { return; }
 is_init=true;
 }

 Init();


 function Rand (max) {
 var val=Math.floor(Math.random()*Math.floor(max));
 return val%max;
 }



 function Fixed (numb,places) {
 return numb.toFixed(places);
 }



 function PercentOf (numb,tot) {
 return(tot/100)*numb;
 }



 function PercentIs (numb,tot) {
 return(100.0/tot)*numb;
 }


 function Pad(numb,width,z) {
 z=z||'0';
 numb=numb+'';
 return numb.length>=width?numb:new Array(width-numb.length +1).join(z)+numb;
 }


 function IntToHex(intg) { var code;
 //-----------------------------------
 code=Math.round(intg).toString(16);
 (code.length>1)||(code='0'+code);
 return code;
 }

 function Round(numb,precision) {
 //--------------------------------------
 return Number.parseFloat(num).toPrecision(precision+1);
 }


 return{
 is_init:is_init,
 Rand:Rand,
 Fixed:Fixed,
 PercentOf:PercentOf,
 PercentIs:PercentIs,
 Pad:Pad,
 IntToHex:IntToHex,
 Round:Round,
 }

})();




//' ######## ######## ##     ## ########
//     ##    ##        ##   ##     ##
//     ##    ##         ## ##      ##
//     ##    ######      ###       ##
//     ##    ##         ## ##      ##
//     ##    ##        ##   ##     ##
//     ##    ######## ##     ##    ##



 var text=(function() {
 var is_init=false;


 function Init () {
 if(is_init==true) { return; }
 is_init=true;
 }


 Init();


 function IndexOf (str,mat) {
 if(str==undefined) { return -1; }
 var stxt=str.toLowerCase();
 var mtxt=mat.toLowerCase();
 if(arguments.length==1)  {  return -1;  }
 if(arguments.length==2)  {  return stxt.indexOf(mtxt);  }
 if(arguments.length==3)  {  return stxt.indexOf(mtxt,argument[2]);  }
 return stxt.indexOf(mtxt,argument[2],argument[3]);
 }




 function BraceLift (str) { var len,sss,sssb,cha,stg,ii; var arr;
 ii=0;
 stg=0;
 sss="";
 len=str.length;
 arr=[];
 while(1)
  {
  if(ii>=len)
   {
   if(sss.length>0)
    {
    sssb=sss.trim();
    if(sssb.length>0) {  arr.push(sssb);  }
    }
   break;
   }
  switch(stg)
   {
   case 0:
   case 10:
   cha=str.charAt(ii);
   if(cha==" "&&sss.length==0) { ii++; break; }
   if(stg==0&&cha!="(")        { sss+=cha; ii++; break; }
   if(stg==10&&cha!=")")       { sss+=cha; ii++; break; }
   if(sss.length>0)
    {
    sssb=sss.trim();
    if(sssb.length>0) {  arr.push(sssb);  }
    }
   sss="";
   ii++;
   if(stg==0) { stg=10; }
   else       { stg=0;  }
   break;
   }
  }
 return arr;
 }


 function GetLastChar(str) { var ch;
 //-----------------------------------
 ch=str[str.length-1];
 return ch;
 }


 function TrimLastChar(str) {
 //----------------------------------
 str=str.substring(0,str.length-1);
 return str;
 }





 return{
 is_init:is_init,
 IndexOf:IndexOf,
 BraceLift:BraceLift,
 GetLastChar:GetLastChar,
 TrimLastChar:TrimLastChar,
 }

})();



//' ######## ##    ## ##     ## ####  #######  ########  ##    ## ##     ## ######## ##    ## ########
//  ##       ###   ## ##     ##  ##  ##     ## ##     ## ###   ## ###   ### ##       ###   ##    ##
//  ##       ####  ## ##     ##  ##  ##     ## ##     ## ####  ## #### #### ##       ####  ##    ##
//  ######   ## ## ## ##     ##  ##  ##     ## ########  ## ## ## ## ### ## ######   ## ## ##    ##
//  ##       ##  ####  ##   ##   ##  ##     ## ##   ##   ##  #### ##     ## ##       ##  ####    ##
//  ##       ##   ###   ## ##    ##  ##     ## ##    ##  ##   ### ##     ## ##       ##   ###    ##
//  ######## ##    ##    ###    ####  #######  ##     ## ##    ## ##     ## ######## ##    ##    ##


 var env=(function() {
 var is_init=false;
 var is_mobile;
 var browser_name;
 var browser_version;
 var browser_platform;
 var browser_url;


 function Init () {
 if(is_init==true) { return; }
 is_mobile=false;
 if(/Android|webOS|iPhone|iPad|iPod|BlackBerry|IEMobile|Opera Mini/i.test(navigator.userAgent))  {  is_mobile=true; }
 BrowserInfo();
 is_init=true;
 }


 Init();



 function BrowserVersionGet (pre) {
 var mat,off,ver;
 ver="";
 mat=pre;//" Firefox/";
 off=text.IndexOf(navigator.userAgent,mat);
 if(off>=0) { off+=mat.length; }
 ver=navigator.userAgent.substring(off);
 off=text.IndexOf(ver," ");
 if(off>=0) { ver=ver.substring(0,off); }
 return ver;
 }




 function BrowserInfo () { var isWho,mob,disp,version,name,platform; var so,i,parts; var kv;
 var isOpera=(!!window.opr&&!!opr.addons)||!!window.opera||navigator.userAgent.indexOf(' OPR/')>=0;
 var isFirefox=typeof InstallTrigger!=='undefined';
 var isSafari=/constructor/i.test(window.HTMLElement)||(function (p) { return p.toString()==="[object SafariRemoteNotification]"; })(!window['safari']||(typeof safari!=='undefined'&&safari.pushNotification));
 var isIE=false||!!document.documentMode;
 var isEdge=!isIE&&!!window.StyleMedia;
 var isChrome=(!!window.chrome&&navigator.userAgent.indexOf("Chrome")!=-1);
 var isEdgeChromium=isChrome&&(navigator.userAgent.indexOf("Edg")!=-1);
 var isSamsung=navigator.userAgent.match(/SamsungBrowser/i);
 isWho=-1;
 name="";
 if(isSamsung)      { isWho=7; name="Samsung"; }  else
 if(isEdgeChromium) { isWho=6; name="EdgeChromium"; }  else
 if(isChrome)       { isWho=5; name="Chrome"; }  else
 if(isEdge)         { isWho=4; name="Edge"; }  else
 if(isIE)           { isWho=3; name="IE"; }  else
 if(isSafari)       { isWho=2; name="Safari"; }  else
 if(isFirefox)      { isWho=1; name="Firefox"; }  else
 if(isOpera)        { isWho=0; name="Opera"; }
 platform=navigator.platform;
 ver="";
 if(isWho==1)  {  ver=BrowserVersionGet(" Firefox/");  }
 else
 if(isWho==6)  {  ver=BrowserVersionGet(" Edg/");  }
 else
 if(isWho==5)  {  ver=BrowserVersionGet(" Chrome/");  }
 else
 if(isWho==-1)
  {
  //alert(navigator.userAgent);
  }
 browser_name=name;
 browser_version=ver;
 browser_platform=platform;
 browser_url=window.location;
 browser_args=[]; // browser_args=[]  fuck
 so=browser_url.search.substring(1).split("&").reduce(function(result,value)
  {
  parts=value.split('=');
  kv={};
  if(parts[0])
   {
   kv.key=decodeURIComponent(parts[0]);
   kv.val=decodeURIComponent(parts[1]);
   }
  browser_args.push(kv);
  },{})
 browser_pathpart=browser_url.pathname.split('/');
 }




 function BrowserZoomOut() { var viewport; var win,doc,docelem,body,wid,hit;
 if(is_mobile!==true) { return false; }
 viewport=document.querySelector('meta[name="viewport"]');
 if(viewport===null)
  {
  viewport=document.createElement("meta");
  viewport.setAttribute("name","viewport");
  document.head.appendChild(viewport);
  viewport=document.querySelector('meta[name="viewport"]');
  }
 if(viewport)
  {
  wid=200;
  hit=200;
  if(browser_name=="Firefox")
   {
   win=window;
   doc=document;
   docelem=doc.documentElement;
   body=doc.getElementsByTagName('body')[0];
   wid=(win.innerWidth||docelem.clientWidth||body.clientWidth);
   hit=(win.innerHeight||docelem.clientHeight||body.clientHeight);
   }
  viewport.content="initial-scale=1";
  viewport.content="width="+(wid);
//  viewport.content="height="+(hit);
  viewport.content="maximum-scale=1"; // newly added
  viewport.content="user-scalable=0"; // was no
  return true;
  }
 return false;
 }



 function BrowserTitleSet (title) {
 document.title=title;
 }




 function BrowserReload (forced,ms) {
 if(is_restarting==true) { return true; }
 is_restarting=true;
 ms=parseInt(ms+num.Rand(500));
 setTimeout(function() { window.location.reload(forced);  return false;  }, ms);
 return true;
 }




 function DisplayGet () {  var win,doc,docelem,body,disp={};
 win=window;
 doc=document;
 docelem=doc.documentElement;
 body=doc.getElementsByTagName('body')[0];
 disp.win_wid=win.innerWidth||docelem.clientWidth||body.clientWidth;
 disp.win_hit=win.innerHeight||docelem.clientHeight||body.clientHeight;
 disp.scr_wid=screen.width;
 disp.scr_hit=screen.height;
 disp.density=1.0;
 if(win.devicePixelRatio) { disp.density=win.devicePixelRatio; }
 disp.orient=win.orientation;
 if(document.fullscreenElement) { disp.is_fse=true;  }
 else                           { disp.is_fse=false; }
 if(disp.scr_wid>disp.scr_hit) { disp.is_landscape=true;  }
 else                          { disp.is_landscape=false; }
 disp.win_ratio=disp.win_wid/disp.win_hit;
 disp.scr_ratio=disp.scr_wid/disp.scr_hit;
 return disp;
 }






 function Yield () {
 BrowserZoomOut();
 }





 return{
 is_init:is_init,
 is_mobile:is_mobile,
 browser_name:browser_name,
 browser_version:browser_version,
 browser_platform:browser_platform,
 browser_url:browser_url,
 browser_args:browser_args,
 browser_pathpart:browser_pathpart,
 BrowserZoomOut:BrowserZoomOut,
 BrowserTitleSet:BrowserTitleSet,
 BrowserReload:BrowserReload,
 DisplayGet:DisplayGet,
 Yield:Yield,
 }


})();




//'  ######   #######   #######  ##    ## #### ########  ######
//  ##    ## ##     ## ##     ## ##   ##   ##  ##       ##    ##
//  ##       ##     ## ##     ## ##  ##    ##  ##       ##
//  ##       ##     ## ##     ## #####     ##  ######    ######
//  ##       ##     ## ##     ## ##  ##    ##  ##             ##
//  ##    ## ##     ## ##     ## ##   ##   ##  ##       ##    ##
//   ######   #######   #######  ##    ## #### ########  ######



 var cookie=(function() {
 var is_init=false;


 function Init () {
 if(is_init==true) { return; }
 is_init=true;
 }


 Init();


 function Set (cname,cvalue,exsecs) { var d,expires;
 d=new Date();
 d.setTime(d.getTime()+(exsecs*1000));
 expires="expires="+d.toUTCString();
 document.cookie=cname+"="+cvalue+";"+expires+";path=/";
 return true;
 }


 function Remove (cname) {
 Set(cname,"",0);
 }


 function Purge () {  var i,kookies,eqPos,name,kookie;
 kookies=document.cookie.split(";");
 for(i=0;i<kookies.length;i++)
  {
  kookie=kookies[i];
  eqPos=kookie.indexOf("=");
  name=eqPos>-1?kookie.substr(0,eqPos):kookie;
  document.cookie=name+"=;expires=Thu, 01 Jan 1970 00:00:00 GMT";
  }
 }


 function Count () { var kookies;
 kookies=document.cookie.split("; ");
 return(kookies.length);
 }


 function GetByIndex (index) { var kookies,ca,eqPos;
 kookies=document.cookie.split("; ");
 ca=kookies[index];
 if(ca==undefined) { return ca; }
 eqPos=ca.indexOf('=');
 return(ca.substr(0,eqPos));
 }



 function Get (cname) { var i,c,name,decodedkookie,ca;
 name=cname+"=";
 decodedkookie=decodeURIComponent(document.cookie);
 ca=decodedkookie.split(';');
 for(i=0;i<ca.length;i++)
  {
  c=ca[i];
  while(c.charAt(0)==' ') { c=c.substring(1);    }
  //if(TextIndexOf(c,name)==0) { return c.substring(name.length,c.length);    }
  if(text.IndexOf(c,name)==0) { return c.substring(name.length,c.length);    }
  }
 return null;
 }


 return{
 is_init:is_init,
 Set:Set,
 Remove:Remove,
 Purge:Purge,
 Count:Count,
 GetByIndex:GetByIndex,
 Get:Get
 }


})();







//'  #######  ##     ## ######## ##     ## ########  ######
//  ##     ## ##     ## ##       ##     ## ##       ##    ##
//  ##     ## ##     ## ##       ##     ## ##       ##
//  ##     ## ##     ## ######   ##     ## ######    ######
//  ##  ## ## ##     ## ##       ##     ## ##             ##
//  ##    ##  ##     ## ##       ##     ## ##       ##    ##
//   ##### ##  #######  ########  #######  ########  ######



 var que=(function() {
 var is_init=false;
 var handef;


 function Init () { var i,obj;
 if(is_init==true) { return; }
 handef=new HandleDefine("que",32,false);
 is_init=true;
 }



 Init();


 function Create () { var i,h,obj;
 for(i=0;i<handef.slots;i++)
  {
  obj=handef.array[i];
  if(obj.in_use!=false) { continue; }
  obj.ms_start=timer.MsRunning();
  obj.msgs_total=0;
  obj.msgs_qued=0;
  obj.msgs_que=[];
  h=HandleAdd(handef,i)
  return h;
  }
 return 0;
 }




 function Destroy (handle) { var obj;
 if((obj=HandleCheck(handef,handle))==null) { return false; }
 obj.msgs_que=[];
 HandleRemove(handef,handle);
 return true;
 }




 function Get (handle) {
 return(HandleCheck(handef,handle));
 }




 function Write (handle,msg) { var obj;
 if((obj=HandleCheck(handef,handle))==null) { return false; }
 //console.log("que write");
 ///console.log(msg);
 obj.msgs_qued++;
 obj.msgs_que.push(msg);
 return true;
 }



 function Peek (handle,ofs) { var msg,obj;
 if((obj=HandleCheck(handef,handle))==null) { return null; }
 if(ofs<0) { return null; }
 if(ofs>=obj.msgs_qued) { return null; }
 msg=obj.msgs_que[ofs];
 return msg;
 }



 function Read (handle) { var msg,obj;
 if((obj=HandleCheck(handef,handle))==null) { return null; }
 if(obj.msgs_qued==0) {  return null; }
 msg=obj.msgs_que.shift();
 obj.msgs_qued--;
 obj.msgs_total++;
 return msg;
 }




 function Discard (handle) { var obj;
 if((obj=HandleCheck(handef,handle))==null) { return false; }
 if(obj.msgs_qued==0) {  return false; }
 obj.msgs_que.shift();
 obj.msgs_qued--;
 obj.msgs_total++;
 return true;
 }



 function Status (handle) { var obj,info;
 if((obj=HandleCheck(handef,handle))==null) { return null; }
 info={};
 info.msgs_qued=obj.msgs_qued;
 info.msgs_total=obj.msgs_total;
 return info;
 }



 return{
 is_init:is_init,
 handef:handef,
 Create:Create,
 Destroy:Destroy,
 Get:Get,
 Write:Write,
 Peek:Peek,
 Read:Read,
 Discard:Discard,
 Status:Status
 }


})();





//' ##      ## ######## ########   ######   #######   ######  ##    ## ######## ########  ######
//  ##  ##  ## ##       ##     ## ##    ## ##     ## ##    ## ##   ##  ##          ##    ##    ##
//  ##  ##  ## ##       ##     ## ##       ##     ## ##       ##  ##   ##          ##    ##
//  ##  ##  ## ######   ########   ######  ##     ## ##       #####    ######      ##     ######
//  ##  ##  ## ##       ##     ##       ## ##     ## ##       ##  ##   ##          ##          ##
//  ##  ##  ## ##       ##     ## ##    ## ##     ## ##    ## ##   ##  ##          ##    ##    ##
//   ###  ###  ######## ########   ######   #######   ######  ##    ## ########    ##     ######




 var wock=(function() {
 var is_init=false;
 var handef;


 function Init () { var i,obj;
 if(is_init==true) { return; }
 handef=new HandleDefine("wock",32,false);
 is_init=true;
 }


 Init();


 function Call (url) { var s,h,obj;
 for(s=0;s<handef.slots;s++)
  {
  obj=handef.array[s];
  if(obj.in_use!=false) { continue; }
  obj.stage=0;
  obj.ms_start=timer.MsRunning();
  obj.rcve_que_handle=que.Create();
  obj.xmit_que_handle=que.Create();
  obj.url=url;
  obj.user_val=0;
  obj.is_open=false;
  obj.is_closed=false;
  obj.is_error=false;
  obj.is_direct=false;
  obj.socket=new WebSocket(obj.url);
  obj.socket.binaryType='arraybuffer';
  obj.socket.onopen=function()  {  obj.is_open=true;   }
  obj.socket.onclose=function() {  obj.is_closed=true; }
  obj.socket.onerror=function() {  obj.is_error=true;  }
  obj.socket.onmessage=function(data)
   {
   //console.log("wock readed");
   //console.log(data.data);
   que.Write(obj.rcve_que_handle,data.data);
   }
  h=HandleAdd(handef,s)
  return h;
  }
 return 0;
 }




 function Destroy (handle) { var obj;
 if((obj=HandleCheck(handef,handle))==null) { return false; }
 que.Destroy(obj.xmit_que_handle);
 obj.xmit_que_handle=0;
 que.Destroy(obj.rcve_que_handle);
 obj.rcve_que_handle=0;
 obj.socket=null;
 HandleRemove(handef,handle);
 return true;
 }




 function Get (handle) {
 return(HandleCheck(handef,handle));
 }


 function DirectSet (handle,state) { var obj;
 if((obj=HandleCheck(handef,handle))==null) { return false; }
 if(state!=true&&state!=false) { return false; }
 obj.is_direct=state;
 return true;
 }





 function Write (handle,msg) { var obj;
 if((obj=HandleCheck(handef,handle))==null) { return false; }
 que.Write(obj.xmit_que_handle,msg);
 if(obj.is_direct==true)  { wock.Process(handle); }
 return true;
 }



 function Peek (handle,ofs) { var msg,obj;
 if((obj=HandleCheck(handef,handle))==null) { return null; }
 msg=que.Peek(obj.rcve_que_handle,ofs);
 return msg;
 }



 function Read (handle) { var msg,obj;
 if((obj=HandleCheck(handef,handle))==null) { return null; }
 msg=que.Read(obj.rcve_que_handle);
 return msg;
 }




 function Discard (handle) { var obj;
 if((obj=HandleCheck(handef,handle))==null) { return false; }
 return(que.Discard(obj.rcve_que_handle));
 }



 function Process (handle) { var obj,info;
 if((obj=HandleCheck(handef,handle))==null) { return false; }
 info=wock.Status(handle);
 if(info.xmit_que_status.msgs_qued>0&&info.is_open==true&&info.is_closed==false)
  {
  msg=que.Read(obj.xmit_que_handle);
  obj.socket.send(msg);
  wock.Status(handle);
  }
 return true;
 }




 function Status (handle) { var obj,info;
 if((obj=HandleCheck(handef,handle))==null) { return null; }
 info={};
 info.url=obj.url;
 info.is_open=obj.is_open;
 info.is_closed=obj.is_closed;
 info.ms=obj.ms_start;
 info.rcve_que_status=que.Status(obj.rcve_que_handle);
 info.xmit_que_status=que.Status(obj.xmit_que_handle);
 return info;
 }





 function Yield ()  { var go,h;
 if(handef.count==0) { return false; }
 for(go=0;go<handef.slots;go++)
  {
  if((h=HandleNext(handef))==0) { continue; }
  wock.Process(h);
  return true;
  }
 return false;
 }




 return{
 is_init:is_init,
 handef:handef,
 Call:Call,
 Destroy:Destroy,
 Get:Get,
 DirectSet:DirectSet,
 Write:Write,
 Peek:Peek,
 Read:Read,
 Discard:Discard,
 Process:Process,
 Status:Status,
 Yield:Yield
 }


})();








//' ########  ######## ##     ## ####  ######  ########  ######
//  ##     ## ##       ##     ##  ##  ##    ## ##       ##    ##
//  ##     ## ##       ##     ##  ##  ##       ##       ##
//  ##     ## ######   ##     ##  ##  ##       ######    ######
//  ##     ## ##        ##   ##   ##  ##       ##             ##
//  ##     ## ##         ## ##    ##  ##    ## ##       ##    ##
//  ########  ########    ###    ####  ######  ########  ######




 var devices=(function() {
 var is_init=false;
 var state={};



 function Init () { var i,obj;
 if(is_init==true) { return; }
 state={};
 state.is_detecting=false;
 state.is_detected=false;
 state.is_updated=false;
 is_init=true;
 }



 Init();





 function Detect () {
 if(devices.state.is_detecting!=false) { return false; }
 if(devices.state.is_detected!=false)  { return false; }
 devices.state.is_good=null;
 devices.state.is_error=null;
 devices.state.error_name=null;
 devices.state.error_message=null;
 devices.state.tik=timer.TikNow(false);
 devices.state.elapsed=0;
 devices.state.audio_inp_count=0;
 devices.state.video_inp_count=0;
 devices.state.audio_out_count=0;
 devices.state.video_out_count=0;
 devices.state.audio_inp_idxoff=0;
 devices.state.video_inp_idxoff=0;
 devices.state.audio_out_idxoff=0;
 devices.state.video_out_idxoff=0;
 devices.state.device_array=[];
 devices.state.stage="device_detect_baseline";
 devices.state.is_detecting=true;
 devices.state.is_detected=false;
 devices.state.is_updated=false;

 /*
 if(navigator.mediaDevices.ondevicechange)
  {
  console.log("good a");
  navigator.mediaDevices.ondevicechange=function(event)
   {
   devices.state.is_updated=true;
   }
  }
  */
 return true;
 }







 function Yield () {  var i,j,avc,elm,dev;
 if(devices.state.is_detecting!=true) { return false; }
 switch(devices.state.stage)
  {
  case "device_detect_baseline":
  if(navigator.mediaDevices===undefined)
   {
   Oof();
   devices.state.error_name="Unsupported";
   devices.state.error_message="mediaDevices is null";
   devices.state.is_good=false;
   devices.state.is_error=true;
   devices.state.elapsed=timer.TikElapsed(false,devices.state.tik);
   devices.state.stage="device_detect_baseline_wait";
   break;
   }
  navigator.mediaDevices.getUserMedia(bl_constraints).then(function(stream)
  //navigator.getUserMedia(bl_constraints).then(function(stream)
   {
   stream.getTracks().forEach(function(track) {  track.stop();  });
   devices.state.error_name=null;
   devices.state.error_message=null;
   devices.state.is_good=true;
   devices.state.is_error=false;
   devices.state.elapsed=timer.TikElapsed(false,devices.state.tik);
   })
  .catch(function(error)
   {
   Oof(error.name+"  "+error.message);
   console.log("eee="+error.name + ": " + error.message);
   if((error.name=='NotAllowedError')||(error.name=='PermissionDismissedError'))    {      }
   devices.state.is_error=true;
   devices.state.error_name=error.name;
   devices.state.error_message=error.message;
   devices.state.elapsed=timer.TikElapsed(false,devices.state.tik);//(Date.now()-devices.state.time);
   devices.state.is_good=false;
   });



  /*
  navigator.mediaDevices.getUserMedia(bl_constraints)
  .then((stream)=>
   {
   stream.getTracks().forEach(function(track) {  track.stop();  });
   devices.state.error_name=null;
   devices.state.error_message=null;
   devices.state.is_good=true;
   devices.state.is_error=false;
   devices.state.elapsed=timer.TikElapsed(false,devices.state.tik);
   })
  .catch(function(error)
   {
   if((error.name=='NotAllowedError')||(error.name=='PermissionDismissedError'))    {      }
   devices.state.is_error=true;
   devices.state.error_name=error.name;
   devices.state.error_message=error.message;
   devices.state.elapsed=timer.TikElapsed(false,devices.state.tik);//(Date.now()-devices.state.time);
   devices.state.is_good=false;
   });
  */
  devices.state.stage="device_detect_baseline_wait";
  break;


  case "device_detect_baseline_fail":
  devices.state.stage="device_gather_fail";
  break;


  case "device_detect_baseline_wait":
  if(devices.state.is_good==true)
   {
   devices.state.stage="device_gather";
   break;
   }
  if(devices.state.is_error==true)
   {
   devices.state.stage="device_detect_baseline_fail";
   break;
   }
  break;




  case "device_gather":
  navigator.mediaDevices.enumerateDevices().then(function(devs)
   {
   devs.forEach(function(device)
    {
 //   console.log(device);
    while(1)
     {
     if(device.deviceId.length<1) { break; }
     if(device.kind=="audioinput")      {
      if(devices.state.audio_inp_count==0) { devices.state.audio_inp_idxoff=devices.state.device_array.length; }
      devices.state.audio_inp_count++;
      break;
      }
     if(device.kind=="videoinput")      {
      if(devices.state.video_inp_count==0) { devices.state.video_inp_idxoff=devices.state.device_array.length; }
      devices.state.video_inp_count++;
      break;
      }
     if(device.kind=="audiooutput")      {
      if(devices.state.audio_out_count==0) { devices.state.audio_out_idxoff=devices.state.device_array.length; }
      devices.state.audio_out_count++;
      break;
      }
     if(device.kind=="videooutput")      {
      if(devices.state.video_out_count==0) { devices.state.video_out_idxoff=devices.state.device_array.length; }
      devices.state.video_out_count++;
      break;
      }
     break;
     }
    if(device.deviceId.length>0)
     {
     devices.state.device_array.push(device);
     }
    })
   console.log("getusermedia success len="+devices.state.device_array.length);
   devices.state.stage="device_gather_success";
   })
  .catch(function(err)
   {
   console.log("eee="+err.name + ": " + err.message);
   devices.state.stage="device_gather_fail";
   });
  //.finally(function()   {   devices.state.stage="device_gather_success";   });
  devices.state.stage="device_gather_wait";
  break;


  case "device_gather_wait":
  break;



  case "device_gather_fail":
  devices.state.elapsed=timer.TikElapsed(false,devices.state.tik);
  devices.state.is_detecting=false;
  devices.state.is_detected=true;
  devices.state.is_good=false;
  devices.state.is_error=true;
  devices.state.stage="device_gather_finished";
  break;

  case "device_gather_success":
  devices.state.elapsed=timer.TikElapsed(false,devices.state.tik);
  devices.state.is_detecting=false;
  devices.state.is_detected=true;
  devices.state.is_good=true;
  devices.state.is_error=false;
  devices.state.stage="device_gather_finished";
  break;

  case "device_gather_finished":
  break;
  }
 return true;
 }






 function VideoConfig (dix) { //var conf={};
 //if(config!=null) { conf=config; }
 if(devices.state.is_detecting==true) { return null; }
 if(devices.state.is_detected!=true) { return null; }
 if(dix<0||dix>=devices.state.video_inp_count) { return null; }
 dix=devices.state.video_inp_idxoff+dix;
 /*
 ///if(config==null) { conf.type="video"; }
 conf.video={};
 conf.video.frameRate=30;
 conf.video.width={};
 conf.video.width.min=160;
 conf.video.width.ideal=320;
 conf.video.width.max=640;
 conf.video.height={};
 conf.video.height.min=160;
 conf.video.height.ideal=320;
 conf.video.height.max=640;
 conf.video.deviceId={};
 conf.video.deviceId.exact=devices.state.device_array[dix].deviceId;
 //console.log(JSON.stringify(conf,0,2));

 return conf;
 */
 return devices.state.device_array[dix];
 }




 function AudioConfig (dix) { //var conf={};
 //if(config!=null) { conf=config; }
 if(devices.state.is_detecting==true) { return null; }
 if(devices.state.is_detected!=true) { return null; }
 if(dix<0||dix>=devices.state.audio_inp_count) { return null; }
 dix=devices.state.audio_inp_idxoff+dix;
 //if(config==null) { conf.type="audio"; }
 //conf.audio={};
 //conf.audio.deviceId={};
 //conf.audio.deviceId.exact=devices.state.device_array[dix].deviceId;
 //return conf;
 return devices.state.device_array[dix];
 }









 return{
 is_init:is_init,
 state:state,
 Detect:Detect,
 Yield:Yield,
 VideoConfig:VideoConfig,
 AudioConfig:AudioConfig,
 }


})();







//' ########   #######   #######  ##     ##
//  ##     ## ##     ## ##     ## ###   ###
//  ##     ## ##     ## ##     ## #### ####
//  ########  ##     ## ##     ## ## ### ##
//  ##   ##   ##     ## ##     ## ##     ##
//  ##    ##  ##     ## ##     ## ##     ##
//  ##     ##  #######   #######  ##     ##




 var room=(function() {
 var is_init=false;
 var handef;


 function Init () { var i,obj;
 if(is_init==true) { return; }
 handef=new HandleDefine("room",32,false);
 is_init=true;
 }


 Init();


 function Create (name,maxpeers) { var s,h,obj,peer,i;
 for(s=0;s<handef.slots;s++)
  {
  obj=handef.array[s];
  if(obj.in_use!=false) { continue; }
  obj.name=name;
  obj.my_uuid=null;
  obj.my_hancock=null;
  obj.peer_slots=maxpeers;
  obj.peer_count=0;
  obj.peer_pf=0;
  obj.peer_array=[];
  for(i=0;i<obj.peer_slots;i++)
   {
   peer={};
   peer.in_use=false;
   peer.ms=0;
   peer.index=i;
   peer.phaze=0;
   peer.uuid=null;
   peer.hancock=null;
   peer.id_dif=0;
   peer.vars={};
   obj.peer_array[i]=peer;
   }
  h=HandleAdd(handef,s)
  return h;
  }
 return 0;
 }




 function Destroy (handle) { var obj;
 if((obj=HandleCheck(handef,handle))==null) { return false; }
 HandleRemove(handef,handle);
 return true;
 }



 function Get (handle) {
 return(HandleCheck(handef,handle));
 }



 function MySet (handle,uuid,hancock) { var obj;
 if((obj=HandleCheck(handef,handle))==null) { return false; }
 obj.my_uuid=uuid;
 obj.my_hancock=hancock;
 return true;
 }



 function PeerByIndexGet (handle,index) { var obj,peer;
 if((obj=HandleCheck(handef,handle))==null) { return null; }
 if(index<0||index>=obj.peer_slots) { return null; }
 peer=obj.peer_array[index];
 return peer;
 }






 function PeerFind (handle,uuid) {  var obj,i,peer;
 if((obj=HandleCheck(handef,handle))==null) { return null; }
 for(i=0;i<obj.peer_slots;i++)
  {
  peer=obj.peer_array[i];
  if(peer.in_use==false) { continue; }
  if(peer.uuid!=uuid) { continue; }
  return peer;
  }
 return null;
 }



 function PeerAdd (handle,uniq,uuid,hancock) { var obj,i,peer;
 if((obj=HandleCheck(handef,handle))==null) { return null; }
 if(uniq==true)
  {
  if(PeerFind(handle,uuid)!=null) { return null; }
  }
 for(i=0;i<obj.peer_slots;i++)
  {
  peer=obj.peer_array[i];
  if(peer.in_use!=false) { continue; }
  peer={};
  peer.in_use=true;
  peer.ms=timer.MsRunning();
  peer.index=i;
  peer.phaze=0;
  peer.uuid=uuid;
  peer.hancock=hancock;
  peer.id_dif=peer.uuid.localeCompare(obj.my_uuid);
  peer.vars={};
  obj.peer_array[i]=peer;
  obj.peer_count++;
  return peer;
  }
 return null;
 }





 function PeerRemove (handle,uuid) { var obj,i,peer;
 if((obj=HandleCheck(handef,handle))==null) { return false; }
 for(i=0;i<obj.peer_slots;i++)
  {
  peer=obj.peer_array[i];
  if(peer.in_use!=true) { continue; }
  if(peer.uuid!=uuid) { continue; }
  peer.in_use=false;
  peer.ms=0;
  peer.index=i;
  peer.phaze=0;
  peer.uuid=null;
  peer.hancock=null;
  peer.id_dif=0;
  peer.vars={};
  obj.peer_array[i]=peer;
  obj.peer_count--;
  return true;
  }
 return false;
 }




 function PeerNext (handle) { var go,obj,peer;
 if((obj=HandleCheck(handef,handle))==null) { return null; }
 if(obj.peer_count==0) { return null; }
 for(go=0;go<obj.peer_slots;go++)
  {
  obj.peer_pf++;
  if(obj.peer_pf>=obj.peer_slots) { obj.peer_pf=0; }
  peer=obj.peer_array[obj.peer_pf];
  if(peer.in_use!=true) { continue; }
  return peer;
  }
 return null;
 }





 function Process (handle) { var obj,info;
 Oof();
 if((obj=HandleCheck(handef,handle))==null) { return false; }
 return true;
 }





 function Yield ()  { var go,h;
 if(handef.count==0) { return false; }
 for(go=0;go<handef.slots;go++)
  {
  if((h=HandleNext(handef))==0) { continue; }
  return true;
  }
 return false;
 }




 return{
 is_init:is_init,
 handef:handef,
 Create:Create,
 Destroy:Destroy,
 Get:Get,
 MySet:MySet,
 PeerByIndexGet:PeerByIndexGet,
 PeerFind:PeerFind,
 PeerAdd:PeerAdd,
 PeerRemove:PeerRemove,
 PeerNext:PeerNext,
 Process:Process,
 Yield:Yield
 }


})();






//  ##    ## ######## ##    ## ########   #######     ###    ########  ########
//  ##   ##  ##        ##  ##  ##     ## ##     ##   ## ##   ##     ## ##     ##
//  ##  ##   ##         ####   ##     ## ##     ##  ##   ##  ##     ## ##     ##
//  #####    ######      ##    ########  ##     ## ##     ## ########  ##     ##
//  ##  ##   ##          ##    ##     ## ##     ## ######### ##   ##   ##     ##
//  ##   ##  ##          ##    ##     ## ##     ## ##     ## ##    ##  ##     ##
//  ##    ## ########    ##    ########   #######  ##     ## ##     ## ########





 var keyboard=(function() {
 var is_init=false;
 var state={};


 function Init () { var i;
 if(is_init==true) { return; }
 is_init=true;

 state.is_started=false;
 state.down_count=0;
 state.up_count=0;
 state.hit_map=[];
 for(i=0;i<256;i++) { state.hit_map[i]=0; }
 //state.event_counter=0;
 ///state.acked_counter=0;
 state.event_que_handle=0;
 state.event_que_object=null;
 Logger(30,"keyboard.Init()");
 }


 Init();







 function Start () { var i;
 state.is_started=true;
 state.down_count=0;
 state.up_count=0;
 state.hit_map=[];
 for(i=0;i<256;i++) { state.hit_map[i]=0; }
// state.event_counter=0;
/// state.acked_counter=0;

 state.event_que_handle=que.Create();
 state.event_que_object=que.Get(state.event_que_handle);
 //Logger(30,"keyboard.start "+state.event_que_handle);

 document.addEventListener('keyup',function(event)    { OnEvent("keyup",event); });
 document.addEventListener('keydown',function(event)  { OnEvent("keydown",event); });
 document.addEventListener('keypress',function(event) { OnEvent("keypress",event); });

 }




 function Stop () {
 }



 function Peek (idx) {  var msg;
 if(state.is_started!=true) { return null; } //Oof(); }
 if((obj=que.Peek(state.event_que_handle,idx))==null) { Oof(); }
 return msg;
 }



 function Read () { var msg;
 if(state.is_started!=true) { return null; } //Oof(); }
 if((msg=que.Read(state.event_que_handle))==null) { return null; }
 return msg;
 }




 function OnEvent (name,ev) { var key; var msg; var info; var kc; var ac,ll;
 if(ev.defaultPrevented) { return;  }
 kc=ev.keyCode||ev.key;
 if(isNaN(kc))  {  Oof();  }
 msg={};
 msg.name=name;
 msg.self_index=-1;
 msg.keyCode=kc;
 msg.key=ev.key;
 //ac=ev.key.charCodeAt(0);
 //ll=String.fromCharCode(ac);
 msg.ascii=ev.key.charCodeAt(0);
 //msg.ll=ll;

 msg.x1=0;
 msg.y1=0;
 msg.x2=0;
 msg.xy=0;
 msg.alt_key=ev.altKey;
 msg.ctrl_key=ev.ctrlKey;
 msg.shift_key=ev.shiftKey;
 //msg.ev=ev;
 que.Write(state.event_que_handle,msg);
 state.event_que_status=que.Status(state.event_que_handle);
 if(name=="keydown")
  {
  state.down_count++;
  state.hit_map[key]=1;
  //state.event_counter++;
  }
 else
 if(name=="keyup")
  {
  state.hit_map[key]=0;
  state.up_count++;
  //state.event_counter++;
  }
 //else
 //if(name=="keypress")  {  state.event_counter++;  }
 }




 return{
 is_init:is_init,
 Start:Start,
 Stop:Stop,
 state:state,
 Peek:Peek,
 Read:Read,
 }


})();








//'  ######   ##     ## ####
//  ##    ##  ##     ##  ##
//  ##        ##     ##  ##
//  ##   #### ##     ##  ##
//  ##    ##  ##     ##  ##
//  ##    ##  ##     ##  ##
//   ######    #######  ####



 var gui=(function() {
 var is_init=false;
 var is_update_requested;
 var update_tik;
 var handef;

 var colour=[
 'AliceBlue','AntiqueWhite','Aqua','Aquamarine','Azure',
 'Beige','Bisque','Black','BlanchedAlmond','Blue','BlueViolet','Brown','BurlyWood',
 'CadetBlue','Chartreuse','Chocolate','Coral','CornflowerBlue','Cornsilk','Crimson','Cyan',
 'DarkBlue','DarkCyan','DarkGoldenRod','DarkGray','DarkGrey','DarkGreen','DarkKhaki','DarkMagenta',
 'DarkOliveGreen','DarkOrange','DarkOrchid','DarkRed','DarkSalmon','DarkSeaGreen','DarkSlateBlue',
 'DarkSlateGray','DarkSlateGrey','DarkTurquoise','DarkViolet','DeepPink','DeepSkyBlue','DimGray',
 'DimGrey','DodgerBlue',
 'FireBrick','FloralWhite','ForestGreen','Fuchsia',
 'Gainsboro','GhostWhite','Gold','GoldenRod','Gray','Grey','Green','GreenYellow','HoneyDew',
 'HotPink','IndianRed','Indigo','Ivory','Khaki',
 'Lavender','LavenderBlush','LawnGreen','LemonChiffon','LightBlue','LightCoral','LightCyan',
 'LightGoldenRodYellow','LightGray','LightGrey','LightGreen','LightPink','LightSalmon','LightSeaGreen',
 'LightSkyBlue','LightSlateGray','LightSlateGrey','LightSteelBlue','LightYellow','Lime',
 'LimeGreen','Linen',
 'Magenta','Maroon','MediumAquaMarine','MediumBlue','MediumOrchid','MediumPurple','MediumSeaGreen',
 'MediumSlateBlue','MediumSpringGreen','MediumTurquoise','MediumVioletRed','MidnightBlue',
 'MintCream','MistyRose','Moccasin',
 'NavajoWhite','Navy', 'OldLace','Olive','OliveDrab','Orange','OrangeRed','Orchid',
 'PaleGoldenRod','PaleGreen','PaleTurquoise','PaleVioletRed','PapayaWhip','PeachPuff',
 'Peru','Pink','Plum','PowderBlue','Purple',
 'RebeccaPurple','Red','RosyBrown','RoyalBlue',
 'SaddleBrown','Salmon','SandyBrown','SeaGreen','SeaShell','Sienna','Silver',
 'SkyBlue','SlateBlue','SlateGray','SlateGrey','Snow','SpringGreen','SteelBlue',
 'Tan','Teal','Thistle','Tomato','Turquoise',
 'Violet','Wheat','White','WhiteSmoke','Yellow','YellowGreen'];






 function Init () {
 if(is_init==true) { return; }
 handef=new HandleDefine("gui",256,false);
 is_init=true;
 UpdateRequest();
 window.addEventListener('resize',function() { UpdateRequest(); },false);
 window.addEventListener('orientationchange',function() { UpdateRequest(); },false);
 }



 Init();




 function UpdateRequest () {
 if(is_update_requested==true) { return false; }
 is_update_requested=true;
 //update_tik=timer.TikNow(false);
 return true;
 }



 function UpdateCheck () {
 if(is_update_requested==false) { return false; }
 //if(timer.TikElapsed(false,update_tik)<1000) { return false; }
 return true;
 }


 function UpdateClear () {
 is_update_requested=false;
 }




 function GetRects (obj) {
 obj.b_rect=obj.dom.getBoundingClientRect();
 obj.c_rect=obj.dom.getClientRects()[0];
 }



 function Create (type) { var s,h,obj;
 for(s=0;s<handef.slots;s++)
  {
  obj=handef.array[s];
  if(obj.in_use!=false) { continue; }
  obj.type=type;
  obj.is_animating=false;
  obj.dom=document.createElement(type);
  obj.id=null;
  obj.uvars={};
  switch(type)
   {
   case "video":
   obj.dom.muted=true;
   obj.dom.autoplay=true;//false;//true;
   obj.dom.controls=false;

   obj.dom.setAttribute('autoplay','');     // added
   obj.dom.setAttribute('muted','');        // added
   obj.dom.setAttribute('playsinline','');   // added
   //obj.dom.setAttribute("playsinline","true");
   //fill,contain,cover,none,scale-down
   //obj.dom.style.objectFit="none";
   obj.dom.srcObject=null;
   obj.dom.src=null;
   break;

   case "canvas":
   break;

   case "img":
   break;

   case "div":
   case "span":
   break;

   default: Oof(type); break;
   }
  obj.dom.style.position="absolute";
  obj.dom.style.zIndex=1000;
  obj.dom.style.display="inline";

  GetRects(obj);
  //obj.rect=obj.dom.getBoundingClientRect();
  h=HandleAdd(handef,s);
  return h;
  }
 return 0;
 }



 function Destroy (handle) { var obj;
 if((obj=HandleCheck(handef,handle))==null) { return false; }
 HandleRemove(handef,handle);
 return true;
 }





 function IdFind (id) { var i,obj;
 for(i=0;i<handef.slots;i++)
  {
  obj=handef.array[i];
  if(obj.in_use!=true)  { continue; }
  if(obj.id!=id) { continue; }
  return obj.self_handle;
  }
 return 0;
 }




 function IdSet (handle,id) { var obj;
 if((obj=HandleCheck(handef,handle))==null) { return false; }
 obj.id=id;
 obj.dom.setAttribute("id",obj.id);
 if(arguments.length==3)
  {
  gui.ParentSet(handle,arguments[2]);
  }
 return true;
 }



 function AttributeGet (handle,atr) { var obj;
 if((obj=HandleCheck(handef,handle))==null) { return  null; }
 return(obj.dom.getAttribute(atr));
 }


 function AttributeSet (handle,atr,val) { var obj;
 if((obj=HandleCheck(handef,handle))==null) { return false; }
 obj.dom.setAttribute(atr,val);
 return true;
 }


 function AttributeRemove (handle,atr) { var obj;
 if((obj=HandleCheck(handef,handle))==null) { return false; }
 obj.dom.removeAttribute(atr);
 return true;
 }



 function WidthHeightSet (handle,wid,hit) { var obj;
 if((obj=HandleCheck(handef,handle))==null) { return false; }
 ///gui.AttributeSet(handle,"width",wid);
 ///gui.AttributeSet(handle,"height",hit);
 if(wid>0) { obj.dom.width=wid; }
 if(hit>0) { obj.dom.height=hit; }
 //obj.rect=obj.dom.getBoundingClientRect();
 GetRects(obj);
 return true;
 }

 function StyleWidthHeightSet (handle,wid,hit) { var obj;
 if((obj=HandleCheck(handef,handle))==null) { return false; }
 ///gui.AttributeSet(handle,"width",wid);
 ///gui.AttributeSet(handle,"height",hit);
 if(wid>0) { obj.dom.style.width=wid; }
 if(hit>0) { obj.dom.style.height=hit; }
 //obj.rect=obj.dom.getBoundingClientRect();
 GetRects(obj);
 return true;
 }






 function BackgroundBorderSet (handle,bkg,bor) { var obj,dom;
 if((obj=HandleCheck(handef,handle))==null) { return false; }
 dom=gui.Get(handle).dom;
 if(dom==null) { return false; }
 if(bkg!=null) {  dom.style.background=bkg; }
 if(bor!=null) {  dom.style.border=bor; }
 //obj.rect=obj.dom.getBoundingClientRect();
 GetRects(obj);
 return true;
 }


 function BackgroundOutlineSet (handle,bkg,olin,olinoff) { var obj,dom;
 if((obj=HandleCheck(handef,handle))==null) { return false; }
 dom=gui.Get(handle).dom;
 if(dom==null) { return false; }
 if(bkg!=null)     { dom.style.background=bkg; }
 if(olin!=null)    { obj.dom.style.outline=olin; }
 if(olinoff!=null) { obj.dom.style.outlineOffset=olinoff; }
 //obj.rect=obj.dom.getBoundingClientRect();
 GetRects(obj);
 return true;
 }



 function Get (handle) {
 return(HandleCheck(handef,handle));
 }





 function XywhSet(x,y,w,h) { var obj={};
 obj.x=x;
 obj.y=y;
 obj.w=w;
 obj.h=h;
 obj.r=(obj.x+obj.w)-1;
 obj.b=(obj.y+obj.h)-1;
 return obj;
 }


 function XywhAdjust(obj,x,y,w,h) { //var obj={};
 obj.x+=x;
 obj.y+=y;
 obj.w+=w;
 obj.h+=h;
 obj.r=(obj.x+obj.w)-1;
 obj.b=(obj.y+obj.h)-1;
 return obj;
 }



 function XywhCopy(obj) { var nobj={};
 nobj=Object.assign({},obj);
 return nobj;
 }



 function FontHeightGet(fontstyle) { var body,dummy,dummytext,result;
 //------------------------------------------------
 body=document.getElementsByTagName("body")[0];
 dummy=document.createElement("div");
 dummytext=document.createTextNode("M");
 dummy.appendChild(dummytext);
 dummy.setAttribute("style",fontstyle);
 body.appendChild(dummy);
 result=dummy.offsetHeight;
 body.removeChild(dummy);
 return result;
 }




 function ColourGet(idx) { var len;
 //-------------------------------
 len=colour.length;
 if(idx>=len) { return null; }
 return colour[idx];
 }



 function RgbaSet(r,g,b,a) {
 var str="rgba(";
 str+=r+",";
 str+=g+",";
 str+=b+",";
 str+=a+")";
 return str;
 }





 function HslaToRgba(h,s,l,a) { var r,g,b,a,q,p;
 //------------------------------
 if(s==0) { r=g=b=l; }
 else
  {
  function hue2rgb(p,q,t)
   {
   if(t<0)   { t+=1; }
   if(t>1)   { t-=1; }
   if(t<1/6) { return p+(q-p)*6*t;       }
   if(t<1/2) { return q;                 }
   if(t<2/3) { return p+(q-p)*(2/3-t)*6; }
   return p;
   }
  q=l<0.5?l*(1+s):l+s-l*s;
  p=2*l-q;
  r=hue2rgb(p,q,h+1/3);
  g=hue2rgb(p,q,h);
  b=hue2rgb(p,q,h-1/3);
  }
 return [r*255,g*255,b*255];
 }




 function RgbaToHsla(r,g,b,a) { var max,min,h,s,l,a,d;
 //------------------------------
 r/=255;
 g/=255;
 b/=255;
 max=Math.max(r,g,b);
 min=Math.min(r,g,b);
 l=(max+min)/2;
 if(max==min) { h=s=0; }
 else
  {
  d=max-min;
  s=l>0.5?d/(2-max-min):d/(max+min);
  switch(max)
   {
   case r: h=(g-b)/d+(g<b?6:0); break;
   case g: h=(b-r)/d+2; break;
   case b: h=(r-g)/d+4; break;
   }
  h/=6;
  }
 return [h,s,l];
 }



 function RgbaBrightAdjust(code,perc) { var r,g,b,HSL,newbright,RGB;
 //------------------------------------------
 r=parseInt(code.slice(1,3),16);
 g=parseInt(code.slice(3,5),16);
 b=parseInt(code.slice(5,7),16);
 HSL=RgbaToHsla(r,g,b);
 newbright=HSL[2]+HSL[2]*(perc/100);
 RGB=HslaToRgba(HSL[0],HSL[1],newbright);
 code='#'+num.IntToHex(RGB[0])+num.IntToHex(RGB[1])+num.IntToHex(RGB[2]);
 return code;
 }



 function AreaSet (handle,astring,sufix,p1,p2,p3,p4) { var obj,len,l,ch,pa;
 if((obj=HandleCheck(handef,handle))==null) { return false; }
 len=astring.length;
 if(len<=0) { return false; }
 len=4;
 pa=[];
 pa[0]=p1;
 pa[1]=p2;
 pa[2]=p3;
 pa[3]=p4;
 if(sufix!=null&&sufix!="")
  {
  for(l=0;l<len;l++) { pa[l]+=sufix;  }
  }
 for(l=0;l<len;l++)
  {
  ch=astring.substring(l,l+1);
  if(ch=="l") { if(pa[l]!=null) obj.dom.style.left=pa[l]; } else
  if(ch=="t") { if(pa[l]!=null) obj.dom.style.top=pa[l]; } else
  if(ch=="r") { if(pa[l]!=null) obj.dom.style.right=pa[l]; } else
  if(ch=="b") { if(pa[l]!=null) obj.dom.style.bottom=pa[l]; } else
  if(ch=="w") { if(pa[l]!=null) obj.dom.style.width=pa[l]; } else
  if(ch=="h") { if(pa[l]!=null) obj.dom.style.height=pa[l]; } else { return false; }
  }
 //obj.rect=obj.dom.getBoundingClientRect();
 GetRects(obj);
 //Logger(40,obj.id+" br="+JSON.stringify(obj.b_rect));
 return true;
 }




 function FontSet (handle,fam,siz,wey) { var obj;
 if((obj=HandleCheck(handef,handle))==null) { return false; }
 if(fam!=null) { obj.dom.style.fontFamily=fam; }
 if(siz!=null) { obj.dom.style.fontSize=siz;   }
 if(wey!=null) { obj.dom.style.fontWeight=wey; }
 return true;
 }





 function ParentSet (handle,phandle) { var obj,pobj;
 if((obj=HandleCheck(handef,handle))==null) { return false; }
 if(phandle!=null&&phandle>0)
  {
  if((pobj=HandleCheck(handef,phandle))==null) { return false; }
  pobj.dom.appendChild(obj.dom);
  obj.parent_handle=pobj.self_handle;
  }
 else
  {
  document.body.appendChild(obj.dom);
  obj.parent_handle=0;
  }
 return true;
 }



 function BackgroundSet (handle,bkg) { var obj;
 if((obj=HandleCheck(handef,handle))==null) { return false; }
 obj.dom.style.background=bkg;
 GetRects(obj);
 return true;
 }


 function BorderSet (handle,bord) { var obj;
 if((obj=HandleCheck(handef,handle))==null) { return false; }
 if(bord!=null)    { obj.dom.style.border=bord; }
 ///obj.rect=obj.dom.getBoundingClientRect();
 GetRects(obj);
 return true;
 }


 function OutlineSet (handle,olin,olinoff) { var obj;
 if((obj=HandleCheck(handef,handle))==null) { return false; }
 if(olin!=null)    { obj.dom.style.outline=olin; }
 if(olinoff!=null) { obj.dom.style.outlineOffset=olinoff; }
 //obj.rect=obj.dom.getBoundingClientRect();
 GetRects(obj);
 return true;
 }




 function CanvasContextGet (handle) { var obj;
 //---------------------------------------------------------
 if((obj=HandleCheck(handef,handle))==null) { return null; }
 if(obj.type!="canvas") { oof(); return null; }
 //Logger(10,document.getElementById(obj.id));
 return(document.getElementById(obj.id).getContext("2d"));
 }



 function CanvasShadowSet(handle,shcol,shblr,shox,shoy) { var obj,ctx;
 //---------------------------------------------------------
 if((obj=HandleCheck(handef,handle))==null) { return false; }
 if(obj.type!="canvas") { oof(); return false; }
 ctx=document.getElementById(obj.id).getContext("2d");
 if(shcol!=null) { ctx.shadowColor=shcol;  }
 if(shblr!=null) { ctx.shadowBlur=shcol;   }
 if(shox!=null)  { ctx.shadowOffsetX=shox; }
 if(shoy!=null)  { ctx.shadowOffsetY=shoy; }
 ctx.shadowBlur=8;
 ctx.shadowOffsetX=4;
 ctx.shadowOffsetY=4;
 return true;
 }





 return{
 is_init:is_init,
 handef:handef,
 colour:colour,
 UpdateRequest:UpdateRequest,
 UpdateCheck:UpdateCheck,
 UpdateClear:UpdateClear,
 Create:Create,
 Destroy:Destroy,
 IdSet:IdSet,
 IdFind:IdFind,
 AttributeGet:AttributeGet,
 AttributeSet:AttributeSet,
 AttributeRemove:AttributeRemove,
 WidthHeightSet:WidthHeightSet,
 StyleWidthHeightSet:StyleWidthHeightSet,
 BackgroundBorderSet:BackgroundBorderSet,
 Get:Get,
 RgbaSet:RgbaSet,
 RgbaBrightAdjust:RgbaBrightAdjust,
 HslaToRgba:HslaToRgba,
 RgbaToHsla:RgbaToHsla,
 XywhSet:XywhSet,
 XywhAdjust:XywhAdjust,
 XywhCopy:XywhCopy,
 FontHeightGet:FontHeightGet,
 ColourGet:ColourGet,
 AreaSet:AreaSet,
 FontSet:FontSet,
 ParentSet:ParentSet,
 BackgroundSet:BackgroundSet,
 BorderSet:BorderSet,
 BackgroundOutlineSet:BackgroundOutlineSet,
 OutlineSet:OutlineSet,
 CanvasContextGet:CanvasContextGet,
 CanvasShadowSet:CanvasShadowSet,
 }


})();




//'    ###    ##     ##
//    ## ##   ##     ##
//   ##   ##  ##     ##
//  ##     ## ##     ##
//  #########  ##   ##
//  ##     ##   ## ##
//  ##     ##    ###



 var av=(function() {
 var is_init=false;
 var state={};



 function Init () { var i,obj;
 if(is_init==true) { return; }
 state={};
 state.channel_handef=new HandleDefine("av",max_windows,false);
 is_init=true;
 }



 Init();



 function Create (onproc) { var i,obj,dev,han;
 if((i=HandleSlotUnused(state.channel_handef))<0) { Logger(40,"err line ="+LineNumber()+" ="+i); return 0; }
 if((han=HandleAdd(state.channel_handef,i))==0) { Logger(40,"err line ="+LineNumber()); return 0; }
 if((obj=HandleCheck(state.channel_handef,han))==null) { Oof(); HandleRemove(state.channel_handef,han); return 0; }
 obj.stage=0;
 obj.is_ice_safe=false;
 obj.config=pc_configurate;
 obj.constraints=pc_constraints;
 obj.loc_offer_desc=null;
 obj.loc_answer_desc=null;
 obj.rem_offer_desc=null;
 obj.rem_answer_desc=null;

 obj.loc_ice_candidate=[];
 obj.loc_ice_eoc=false;
 obj.rem_ice_candidate=[];

 obj.media_stream=null;
 obj.media_emessage=null;
 obj.media_ename=null;

 obj.user_data={};
 obj.onproc=onproc;
 obj.pc={};
 try
  {
  obj.pc=new RTCPeerConnection(obj.config,obj.constraints);
  if(obj.onproc!=null)
   {
   obj.pc.onconnectionstatechange=function(e)    { obj.onproc(obj.self_handle,"onconnectionstatechange",e);   };
   obj.pc.onicecandidate=function(e)             { obj.onproc(obj.self_handle,"onicecandidate",e);   };
   obj.pc.oniceconnectionstatechange=function(e) { obj.onproc(obj.self_handle,"oniceconnectionstatechange",e);   };
   obj.pc.onicegatheringstatechange=function(e)  { obj.onproc(obj.self_handle,"onicegatheringstatechange",e);   };
   obj.pc.onsignalingstatechange=function(e)     { obj.onproc(obj.self_handle,"onsignalingstatechange",e);   };
   obj.pc.onnegotiationneeded=function(e)        { obj.onproc(obj.self_handle,"onnegotiationneeded",e);   };
   obj.pc.ontrack=function(e)                    { obj.onproc(obj.self_handle,"ontrack",e);   };
   obj.pc.onaddtrack=function(e)                 { obj.onproc(obj.self_handle,"onaddtrack",e);   };
   obj.pc.onremovetrack=function(e)              { obj.onproc(obj.self_handle,"onremovetrack",e);   };
   obj.pc.onaddstream=function(e)                { obj.onproc(obj.self_handle,"onaddstream",e);   };
   obj.pc.onremovestream=function(e)             { obj.onproc(obj.self_handle,"onremovestream",e);   };
   }
  }
 catch(e)  {  alert('RTCPeerConnection object. exception: '+e.message);  return;   }
 h=obj.self_handle;
 return h;
 }







 function Destroy (handle) { var obj;
 if((obj=HandleCheck(state.channel_handef,handle))==null) { return false; }
 if(obj.pc)
  {
  obj.pc.close();
  obj.loc_ice_candidate=[];
  obj.rem_ice_candidate=[];
  obj.user_data={};
  delete obj.pc;
  }
 if((HandleRemove(state.channel_handef,handle))!=true) { Oof(""); return false; }
 return true;
 }





 function Get (handle) {
 return(HandleCheck(state.channel_handef,handle));
 }




 function Next () { var go,obj,han; var msg,candi;
 if(state.channel_handef.count==0) { return null; }
 for(go=0;go<state.channel_handef.slots;go++)
  {
  if((han=HandleNext(state.channel_handef))==0) { continue; } // (handef) { var idx;
  if((obj=HandleCheck(state.channel_handef,han))==null) { continue; }
  break;
  }
 if(go==state.channel_handef.slots) { return null; }
/**
 if(obj.is_ice_safe==true)
  {
  if(obj.rem_ice_candidate.length>=1)
   {
   msg=obj.rem_ice_candidate.shift();
   candi=new RTCIceCandidate({sdpMLineIndex:msg.data.label,candidate:msg.data.candidate});
   obj.pc.addIceCandidate(candi);
   Logger(40,"add ice "+obj.self_handle);
   }
  }
 */
 return obj;
 }






 function GetMedia (handle,config) { var obj;
 if((obj=HandleCheck(state.channel_handef,handle))==null) { Oof(); return false; }
 //console.log(config);
  obj.media_stream=null;
  obj.media_ename=null;
  obj.media_emessage=null;
  navigator.mediaDevices.getUserMedia(config).then(function(stream)
  //navigator.getUserMedia(config).then(function(stream)
   {
   //if((obj=HandleCheck(state.channel_handef,handle))==null) { Oof();  }
   obj.media_stream=stream;
   })
  .catch(function(error)
   {
  // Oof("video "+error.name+"  "+error.message);
   //if((obj=HandleCheck(state.channel_handef,handle))==null) { Oof();  }
   obj.media_ename=error.name;
   obj.media_emessage=error.message;
   obj.media_stream=-1;
   });
 return true;
 }

 /*
 if(config.type=="video")
  {
  obj.video_media_stream=null;
  obj.video_media_ename=null;
  obj.video_media_emessage=null;
  //obj.video_media_config=Object.assign({},config);
  navigator.mediaDevices.getUserMedia(config)
  .then(function(stream)
   {
   //if((obj=HandleCheck(state.channel_handef,handle))==null) { Oof();  }
   obj.video_media_stream=stream;
   })
  .catch(function(error)
   {
   Oof("video "+error.name+"  "+error.message);
   //if((obj=HandleCheck(state.channel_handef,handle))==null) { Oof();  }
   obj.video_media_ename=error.name;
   obj.video_media_emessage=error.message;
   obj.video_media_stream=-1;
   });
  return true;
  }
 else
 if(config.type=="audio")
  {
  obj.audio_media_stream=null;
  obj.audio_media_ename=null;
  obj.audio_media_emessage=null;
  //obj.audio_media_config= Object.assign(obj.audio_media_config,config);
  navigator.mediaDevices.getUserMedia(config)
  .then(function(stream)
   {
   //if((obj=HandleCheck(state.channel_handef,handle))==null) { Oof();  }
   obj.audio_media_stream=stream;
   })
  .catch(function(error)
   {
   Oof("audio "+error.name+"  "+error.message);
   //if((obj=HandleCheck(state.channel_handef,handle))==null) { Oof();  }
   obj.audio_media_ename=error.name;
   obj.audio_media_emessage=error.message;
   obj.audio_media_stream=-1;
   });
  return true;
  }
 else
  {
  Oof(config.type);
  }

 }
*/




 function CreateOffer (handle) { var obj;
 if((obj=HandleCheck(state.channel_handef,handle))==null) { Oof(); return false; }
 obj.loc_offer_desc=null;
 /*
 obj.pc.createOffer()
 .then((offer)=>
  {
  obj.loc_offer_desc=offer;
  obj.pc.setLocalDescription(obj.loc_offer_desc);
  })
 .catch((error)=>
  {
  obj.loc_offer_desc=-1;
  remlog.Log("create offer failure "+error.message);
  });
 */

 obj.pc.createOffer()
 .then(function(offer)
  {
  //if((obj=HandleCheck(state.channel_handef,handle))==null) { Oof();  }
//  obj.media_stream=stream;
  obj.loc_offer_desc=offer;
  obj.pc.setLocalDescription(obj.loc_offer_desc);
  })
 .catch(function(error)
  {
  obj.loc_offer_desc=-1;
  console.log("create offer failure "+error.message);
 // Oof("video "+error.name+"  "+error.message);
  //if((obj=HandleCheck(state.channel_handef,handle))==null) { Oof();  }
  //obj.media_ename=error.name;
  //obj.media_emessage=error.message;
  //obj.media_stream=-1;
  });

 return true;
 }







 function CreateAnswer (handle,data) { var obj; var ii,lines,str;
 if((obj=HandleCheck(state.channel_handef,handle))==null) { Oof(); return false; }
 obj.loc_answer_desc=null;
 obj.rem_offer_desc=data;
 obj.pc.setRemoteDescription(new RTCSessionDescription(obj.rem_offer_desc));

 obj.pc.createAnswer()
 .then(function(answer)
  {
  //if((obj=HandleCheck(state.channel_handef,handle))==null) { Oof();  }
//  obj.media_stream=stream;
  obj.loc_answer_desc=answer;
  obj.pc.setLocalDescription(answer);
  obj.is_ice_safe=true;
  })
 .catch(function(error)
  {
  obj.loc_answer_desc=-1;
  console.log("create answer failure "+error.message);
  // Oof("video "+error.name+"  "+error.message);
  //if((obj=HandleCheck(state.channel_handef,handle))==null) { Oof();  }
  //obj.media_ename=error.name;
  //obj.media_emessage=error.message;
  //obj.media_stream=-1;
  });


 /*
 obj.pc.createAnswer()
 .then((answer)=>
  {
  obj.loc_answer_desc=answer;
  obj.pc.setLocalDescription(answer);
  obj.is_ice_safe=true;
  })
 .catch((error)=>
  {
  obj.loc_answer_desc=-1;
  remlog.Log("create answer failure "+error.message);
  });
 */
 return true;
 }






 function AcceptAnswer (handle,data) { var obj;
 if((obj=HandleCheck(state.channel_handef,handle))==null) { Oof(); return false; }
 obj.rem_answer_desc=data;
 obj.pc.setRemoteDescription(new RTCSessionDescription(obj.rem_answer_desc));
 obj.is_ice_safe=true;
 return true;
 }










 return{
 is_init:is_init,
 state:state,
 Create:Create,
 Destroy:Destroy,
 Get:Get,
 Next:Next,
 GetMedia:GetMedia,
 CreateOffer:CreateOffer,
 CreateAnswer:CreateAnswer,
 AcceptAnswer:AcceptAnswer,
 }


})();






//' ########  ######## ##     ## ##        #######   ######
//  ##     ## ##       ###   ### ##       ##     ## ##    ##
//  ##     ## ##       #### #### ##       ##     ## ##
//  ########  ######   ## ### ## ##       ##     ## ##   ####
//  ##   ##   ##       ##     ## ##       ##     ## ##    ##
//  ##    ##  ##       ##     ## ##       ##     ## ##    ##
//  ##     ## ######## ##     ## ########  #######   ######


 var remlog=(function() {
 var is_init=false;
 var vars={};



 function Init () { var i;
 if(is_init==true) { return; }
 vars.stage=0;
 vars.is_open=false;
 vars.is_ready=false;
 vars.wock_handle=0;
 vars.wock_status=null;
 vars.wock_obj=null;
 vars.quick_queue=[];
 vars.iam_uuid=0;
 vars.iam_hancock=null;
 vars.iam_alias=null;
 vars.room_handle=0;
 vars.room_object=null;
 vars.ping_tik=0;
 is_init=true;
 }


 Init();



 function Open () {
 if(vars.is_open!=false) { return false; }
 vars.wock_handle=wock.Call("wss://mebeam.com:443/wss/dashtest");
 vars.wock_status=wock.Status(vars.wock_handle);
 wock.DirectSet(vars.wock_handle,true);
 if((vars.wock_obj=wock.Get(vars.wock_handle))==null) { Oof(); }
 vars.quick_queue=[];
 vars.room_handle=room.Create("dashtest",32);
 vars.room_object=room.Get(vars.room_handle);
 vars.ping_tik=timer.TikNow(false);
 vars.iam_uuid=0;
 vars.iam_hancock=null;
 vars.iam_alias=null;
 vars.is_open=true;
 vars.is_ready=false;
 main.WorkerAdd("remlog.Yield",remlog.Yield,1);
 return true;
 }




 function Close () {
 if(vars.is_open!=true) { return false; }
 vars.is_open=false;
 return true;
 }



 function AliasSet (ali) {
 if(vars.is_open!=true) { return false; }
 vars.iam_alias=ali;
 return true;
 }



 function HandleOtherMessages (msg) {
 if(msg.cmd=="userJoined")
  {
  while(1)
   {
   if(msg.uuid==vars.room_object.my_uuid)  { break; }
   if((peer=room.PeerAdd(vars.room_handle,true,msg.uuid,msg.hancock))==null) { Oof(""); }
   break;
   }
  }
 else
 if(msg.cmd=="userLeft")
  {
  while(1)
   {
   if(msg.uuid==vars.room_object.my_uuid)  { break; }
   if((peer=room.PeerFind(vars.room_handle,msg.uuid))==null) { break; }
   if((peer=room.PeerRemove(vars.room_handle,msg.uuid,msg.hancock))==null) { Oof(""); }
   break;
   }
  }
 }




 function Write (img,txt) { var msg;
 if(vars.is_open!=true) { return false; }
 if(vars.is_ready!=true)
  {
  vars.quick_queue.push({"cmd":"chatMsg","ali":remlog.vars.iam_alias,"img":img,"txt":txt});
  return true;
  }
 wock.Write(vars.wock_handle,JSON.stringify({"cmd":"chatMsg","ali":remlog.vars.iam_alias,"img":img,"txt":txt}));
 return true;
 }




 function Read () { var msg,m,peer;
 if(vars.is_open!=true)  { return null; }
 if(vars.is_ready!=true) { return null; }
 vars.wock_status=wock.Status(vars.wock_handle);
 if(vars.wock_status.is_open!=true) { return null; }
 if((msg=wock.Peek(vars.wock_handle,0))==null) { return null; }
 m=JSON.parse(msg);
 if(m.cmd!="chat")
  {
  HandleOtherMessages(m);
  }
 if(m.cmd!="chat"||m.uuid==vars.iam_uuid)
  {
  wock.Discard(vars.wock_handle);
  return null;
  }
 wock.Discard(vars.wock_handle);
 return m;
 }







 function Yield ()  { var msg,m,isme,i,peer; var obj,ordy;
 if(vars.is_open!=true) { return false; }
 while(1)
  {
  vars.wock_status=wock.Status(vars.wock_handle);
  if(vars.wock_status.is_open!=true) { break; }
  switch(vars.wock_obj.stage)
   {
   case 0:
   wock.Write(vars.wock_handle,JSON.stringify({"cmd":"hello"}));
   vars.wock_obj.stage=100;
   break;

   case 100:
   if((msg=wock.Peek(vars.wock_handle,0))==null) { break; }
   m=JSON.parse(msg);
   if(m.cmd!="welcome"||(m.uuid==vars.iam_uuid))
    {
    wock.Discard(vars.wock_handle);
    break;
    }
   vars.iam_uuid=m.uuid;
   vars.iam_hancock=m.hancock;
   room.MySet(vars.room_handle,m.uuid,m.hancock);
   for(i=0;i<m.peerCount;i++)
    {
    if(m.peerList[i].uuid==vars.room_object.my_uuid)  { continue; }
    if((peer=room.PeerAdd(vars.room_handle,true,m.peerList[i].uuid,m.peerList[i].hancock))==null) { Oof(""); }
    }
   vars.ping_tik-=35000;
   vars.is_ready=true;
   vars.wock_obj.stage=200;
   break;

   case 200:
   while(1)
    {
    if(vars.quick_queue.length<=0) { break; }
    msg=vars.quick_queue.shift();
    msg.uuid=vars.iam_uuid;
    msg.hancock=vars.iam_hancock;
    wock.Write(vars.wock_handle,JSON.stringify(msg));
    }
   vars.wock_obj.stage=1000;
   break;


   case 1000:
   if(timer.TikElapsed(false,vars.ping_tik)>=30000)
    {
    while(1)
     {
     vars.ping_tik=timer.TikNow(false);
     if(vars.is_open!=true)  { break; }
     if(vars.is_ready!=true) { break; }
     obj={};
     obj.cycle=main.vars.cycle;
     obj.stage=main.vars.stage;
     obj.speed=main.vars.speed;
     obj.ms_running=timer.MsRunning();
     obj.is_running=main.state.is_running;
     ///remlog.Write(123,JSON.stringify(obj));
     obj=null;
     break;
     }
    }
   break;
   }
  break;
  }
 return true;
 }



 function Engine () { var msg,m;
 if((msg=remlog.Read())==null) { return; }
 switch(msg.img)
  {
  case "":
  //Logger(60,"%c"+msg.txt,"font-weight:600;color:#20d590;font-size:100%");
  console.log(msg.txt);
  ///Logger(60,"%c"+msg.ali+">%c "+msg.txt,"font-weight:bold;color:#20d590;","color:gray");
  break;
  case "ENVIORNMENT":
  case "DEVICELIST":
  m=JSON.parse(msg.txt);
  Logger(10,"%c"+msg.ali+">%c "+msg.img+" %c"+JSON.stringify(m,null,4),"font-weight:bold;color:#20d590;","color:#338aca;","color:#3ac832;");
  break;
  case "NUKE":
  Logger(30,"Incoming Nuke");
  env.BrowserReload(true,1500);
  break;
  default:
  Oof(msg.img);
  break;
  }
 }

 return{
 is_init:is_init,
 vars:vars,
 Open:Open,
 Close:Close,
 AliasSet:AliasSet,
 Write:Write,
 Read:Read,
 Yield:Yield,
 Engine:Engine,
 }


})();








//' ##     ##    ###    #### ##    ##
//  ###   ###   ## ##    ##  ###   ##
//  #### ####  ##   ##   ##  ####  ##
//  ## ### ## ##     ##  ##  ## ## ##
//  ##     ## #########  ##  ##  ####
//  ##     ## ##     ##  ##  ##   ###
//  ##     ## ##     ## #### ##    ##




 var main=(function() {
 var is_init=false;
 var state={};
 var vars={};
 ///var hits=[];



 function Init () {
 if(is_init==true) { return; }
 state.is_running=false;
 state.version=0;
 state.speed=0;
 state.funcProc=null;
 state.thread_id=0;
 state.worker_array=[];
 vars={};
 is_init=true;
 }


 Init();


 function Start (ver,spd,proc,dorun) { var work;
 if(main.state.is_running!=false) { return false; }
 main.state.version=ver;
 main.vars={};
 main.vars.cycle=-1;
 main.state.thread_id=0;
 main.state.speed=spd;
 main.state.funcProc=proc;
 main.state.is_running=true;
 main.vars.stage=0;
 WorkerAdd("wock.Yield",wock.Yield,1);
 WorkerAdd("env.Yield",env.Yield,1);
 WorkerAdd("devices.Yield",devices.Yield,1);
// WorkerAdd("devices.Yield2",devices.Yield,1);
 //WorkerAdd("devices.Yield3",devices.Yield,2);
 if(dorun==true) { Run(); }
 return true;
 }



 function WorkerAdd (name,proc,step) { var work;
 if(arguments.length!=3) alert("workeradd 3 arguments, not "+arguments.length);
 work={};
 work.name=name;
 work.proc=proc;
 work.step=step;
 main.state.worker_array.push(work);
 return true;
 }


 function WorkerRemove (name) { var i,work;
 for(i=0;i<main.state.worker_array.length;i++)
  {
  work=main.state.worker_array[i];
  if(work.proc==undefined||work.proc==null) { continue; }
  if(work.name!=name) { continue; }
  work.name=null;
  work.proc=null;
  work.step=0;
  return true;
  }
 return false;
 }






 function Clear () {
 clearTimeout(main.state.thread_id);
 main.state.thread_id=0;
 return true;
 }







 function Run () { var i; var work; var oms;
 main.vars.cycle++;
 if(main.state.worker_array.length>=1)
  {
  //console.log("worker array len="+main.state.worker_array.length);
  for(i=0;i<main.state.worker_array.length;i++)
   {
   work=main.state.worker_array[i];
   if(work.proc==undefined||work.proc==null) { continue; }
   if(work.step<1) { continue; }
   if(((main.vars.cycle%work.step)==(work.step-1))||(main.vars.cycle==1))
    {
    oms=timer.MsRunning();
    work.proc();
    oms=timer.MsRunning()-oms;
    if(oms>(main.state.speed*5))
     {
     //Oof("work="+work.name+" oms = "+oms+"  "+main.state.speed+"  "+main.vars.cycle);
     }
    }
   }
  }
 main.state.thread_id=window.setTimeout(function()
  {
  main.Clear();
  oms=timer.MsRunning();
  main.state.funcProc();
  oms=timer.MsRunning()-oms;
  if(oms>(main.state.speed*5))
   {
   //Oof("<< oms = "+oms+"  "+main.state.speed+"  "+main.vars.cycle);
   }

  main.Run();
  },main.state.speed);
 return true;
 }




 function StageSet (stage) {
 main.vars.stage=stage;
 }






 return{
 is_init:is_init,
 state:state,
 vars:vars,
 ///hits:hits,
 Start:Start,
 WorkerAdd:WorkerAdd,
 WorkerRemove:WorkerRemove,
 Run:Run,
 Clear:Clear,
 StageSet:StageSet,
 };


})();












