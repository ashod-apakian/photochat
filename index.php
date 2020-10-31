<?php
if(1) { $vvv=412; }  else { $vvv=microtime(true); }
$req=$_SERVER['REQUEST_URI'];
if(empty($_SERVER['HTTPS'])||$_SERVER['HTTPS']==="off") {
 header('HTTP/1.1 301 Moved Permanently');
 header('Location: '.'https://'.$_SERVER['HTTP_HOST'].$req);
 exit;
 }
function echoScript ($sf) { echo "<script type='text/javascript' src='".$sf."'></script>\n"; }
function echoCode   ($cd) { echo "<script type='text/javascript'>".$cd."</script>\n";        }
?>
<!doctype html>
<html lang="en">
<head>
<title>MeBeam, video group call</title>
<meta name='theme-color' content='#ffffcc' />
<link rel='shortcut icon' href='/favicon.ico' type='image/x-icon' />
<style type="text/css">
*                           {  margin:0;    padding:0;   }
html,body                   {  width:100%;  height:100%; background: #000009; margin: 0; padding: 0; overflow: hidden; }
canvas                      {  display:block; }
-webkit-text-size-adjust:none;
-ms-text-size-adjust:none;
-moz-text-size-adjust:none;
text-size-adjust:none;
a:active,      a:focus      {  outline: 0;  border: none;  -moz-outline-style: none;  box-shadow: none;  text-decoration:none;}
button:active, button:focus {  outline: 0;  border: none;  -moz-outline-style: none;  box-shadow: none;  text-decoration:none;}
span:active,   span:focus   {  outline: 0;  border: none;  -moz-outline-style: none;  box-shadow: none;  text-decoration:none;}
div:active,    div:focus    {  outline: 0;  border: none;  -moz-outline-style: none;  box-shadow: none;  text-decoration:none;}
img:active,    img:focus    {  outline: 0;  border: none;  -moz-outline-style: none;  box-shadow: none;  text-decoration:none;}
</style>
<?php
echoScript("/mb_base.js?".$vvv);
echoScript("/fastclick3.js?".$vvv);
echoScript("/mb_chat.js?".$vvv);
//echoScript("/mbchat_a.js?".$vvv);
//echoScript("/mbchat_b.js?".$vvv);
//echoScript("/mbchat_c.js?".$vvv);
//echoScript("/mbchat_d.js?".$vvv);
///echoScript("/mbchat_e.js?".$vvv);
echoCode("window.onload=function(){ ChatMain(); };");
?>
</head>
<body id="bodid"></body>
</html>
