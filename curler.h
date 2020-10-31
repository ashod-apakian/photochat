/*-----------------------------------------------------------------------*/
 #pragma once
 #ifndef INC_CURLER_H
 #define INC_CURLER_H
 #define PUB                           extern
 #ifdef __cplusplus
 PUB "C" {
 #endif
/*-----------------------------------------------------------------------*/
 #undef UINT_MAX
 #undef SHRT_MIN
 #undef USHRT_MAX
 #undef LLONG_MIN
 #undef ULLONG_MAX
/*-----------------------------------------------------------------------*/
 //#include "aa.h"
 #include "curl.h"
/*-----------------------------------------------------------------------*/

 #define CURLY_MAX_CALLS               72




 structure
 {
 B url[_8K];
 B img[_2K];
 B txt[_4K];
 }
 _curlylink;



 structure
 {
 H magic;
 //H stage;
 B is_fin;
 B is_ret;
 B is_working;
 B is_cancel;
 B is_error;
 H bug;
 H this_ci;
 _threadunit thread;
 CURL*curl_handle;
 Z result;
 C result_str[_1K];
 VP curly_ptr;
 struct curl_slist*clist;
 _memoryunit mun;
 B cmd[_8K];
 B cookie[129];
 Q ms,el;
 W http_code;
 B location[_1K];
 H hdr_i_count;
 H hdr_o_count;
 B res_content_type[129];
 H task_type;
 _list task_list;
 B list_line_buf[_8K];
 B url[_8K];
 B data[_8K];
 B user_data[_1K];
 H s4i;
 _filestreamunit fsu;
 }
 _curlycall;



 structure
 {
 H magic;
 CURLSH*share_handle;
 _sync sync;
 N max_calls;
 N cur_calls;
 N fin_calls;
 Q tot_calls;
 Q tot_fails;
 Q tot_timeouts;
 Q tot_retries;
 _curlycall call[CURLY_MAX_CALLS];
 H pf_call;
 }
 _curly;

/*-----------------------------------------------------------------------*/


 Z curlyProcTrace                      (CURL*handle,curl_infotype type,CP data,Y size,VP ud);
 Y curlyProcWrite                      (CP contents,Y size,Y nmemb,VP ud);
 Y curlyProcProgress                   (VP p,D dltot,D dlnow,D ultot,D ulnow);


 B curlyNew                            (_curly*curly);
 B curlyDelete                         (_curly*curly);
 B curlyYield                          (_curly*curly);

 B curlyCallNew                        (_curly*curly,HP index,_curlycall**ccall,H bug,VP fmt,...);
 B curlyCallNewEx                      (_curly*curly,H index,_curlycall**ccall,H bug,VP fmt,...);
 B curlyCallDelete                     (_curly*curly,H index);
 B curlyCallCancel                     (_curly*curly,H index);
 B curlyCallCmdGet                     (_curly*curly,H index,H line,VP key,VP val);
 B curlyCallHeaderAppend               (_curly*curly,H index,VP fmt,...);

 B curlyCallWait                       (_curly*curly,H index,_curlycall**curlycallptr);
 B curlyCallRetry                      (_curly*curly,H index,HP newindex);

 B curlyCallThread                     (H handle);

 B curlyUserAgentGet                   (H ui,VP ua);

/*-----------------------------------------------------------------------*/

 V curlTextFix                         (VP txt,B donbsp);
 B curlRorgRead                        (_htmlparser*html,H rindex,H tindex,H count,H max,_htmlrorg*rorgs);


/*-----------------------------------------------------------------------*/

 structure
 {
 H magic;
 H stage;
 B arg_url[_1K];
 H call_index;
 B cookie_file[129];
 B is_redirect;
 H http_code;
 B location[_1K];
 B is_html;
 _htmlparser html;
 H extra_bytes;
 BP extra_data;
 }
 _browser;


 B browserNew                          (_browser*browser,VP cookiefile,H extrabytes,VP fmt,...);
 B browserDelete                       (_browser*browser);
 B browserYield                        (_browser*browser);
 B browserRetry                        (_browser*browser,B keepextra,VP fmt,...);
 B browserParse                        (_browser*browser);

 V browserDemo                         (V);


/*-----------------------------------------------------------------------*/

 PUB _curly kerly;

/*-----------------------------------------------------------------------*/
 #endif
/*-----------------------------------------------------------------------*/
 #ifdef __cplusplus
 }
 #endif




