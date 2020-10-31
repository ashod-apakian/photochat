/*-----------------------------------------------------------------------*/
 #include "mb.h"
/*-----------------------------------------------------------------------*/


 B jsonDecode                          (H bytes,VP data)
 {
 B ret;
 H i,mx;

 aaJsonReset(mb.json.handle,NO);
 aaJsonAppendBytes(mb.json.handle,bytes,data);
 while(1) { if((ret=aaJsonDecode(mb.json.handle))==RET_YES) { break; } if(ret!=RET_INUSE) { oops; break; } }
 aaJsonStatus(mb.json.handle,&mb.json.status);
 if(mb.json.status.decode_success!=YES) { oof; }
 mx=aaElementCount(mb.json_line);
 for(i=0;i<mx;i++)
  {
  mb.json_line[i].type=0;
  mb.json_text[i][0]=NULL_CHAR;
  }
 if(mb.json.status.lines>mx) { return RET_NO; }//oof; }
 for(i=0;i<mb.json.status.lines;i++)
  {
  if((ret=aaJsonParserLineGet(mb.json.handle,i,&mb.json_line[i],&mb.json_text[i][0]))!=YES) {oops; break; }
  }
 return RET_YES;
 }






 B jsonDump                            (V)
 {
 H i,r,l;
 B text[_64K];

 mbLog("----------------------------------------");
 i=0;
 while(1)
  {
  r=mb.json.status.lines-i;
  if(r==0) { break; }
  if(r>=2)
   {
   if(mb.json_line[i+0].type==JSON_TYPE_KEY&&mb.json_line[i+1].type==JSON_TYPE_STRING)
    {
    l=aaNumRoof(mb.json_line[i+1].len,256);
    aaStringNCopy(text,mb.json_text[i+1],l,YES);
    mbLog("json[%-2i/%-2i]= type=KS len=%-4i [%s='%s']",i+0,mb.json.status.lines,mb.json_line[i+0].len+mb.json_line[i+1].len,mb.json_text[i+0],text);//mb.json_text[i+1]);
    i+=2;
    continue;
    }
   if(mb.json_line[i+0].type==JSON_TYPE_KEY&&mb.json_line[i+1].type==JSON_TYPE_INT)
    {
    l=aaNumRoof(mb.json_line[i+1].len,256);
    aaStringNCopy(text,mb.json_text[i+1],l,YES);
    mbLog("json[%-2i/%-2i]= type=KI len=%-4i [%s=%s]",i+0,mb.json.status.lines,mb.json_line[i+0].len+mb.json_line[i+1].len,mb.json_text[i+0],text);//mb.json_text[i+1]);
    i+=2;
    continue;
    }
   }
  mbLog("json[%-2i/%-2i]= type=%-2i len=%-4i [%s]",i,mb.json.status.lines,mb.json_line[i].type,mb.json_line[i].len,mb.json_text[i]);
  i++;
  }
 return RET_YES;
 }





/*-----------------------------------------------------------------------*/

